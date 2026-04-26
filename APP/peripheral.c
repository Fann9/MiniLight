/********************************** (C) COPYRIGHT *******************************
 * File Name          : peripheral.C
 * Author             : zhangxiyi @WCH
 * Version            : v0.1
 * Date               : 2020/11/26
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "CONFIG.h"
#include "devinfoservice.h"
#include "peripheral.h"

#include "ble_uart_service.h"
#include "app_drv_fifo.h"
#include "ws2812.h"
#include "CH57x_pwm.h"
#include <stdlib.h>

uint8 Peripheral_TaskID = INVALID_TASK_ID; // Task ID for internal task/event processing

// LED控制标志位
typedef enum {
    LED_CMD_NONE = 0,
    LED_CMD_RED,
    LED_CMD_GREEN,
    LED_CMD_BLUE,
    LED_CMD_WHITE,
    LED_CMD_OFF,
    LED_CMD_YELLOW,
    // 新协议模式
    LED_CMD_NEW_PROTOCOL
} led_cmd_t;

static led_cmd_t led_cmd = LED_CMD_NONE;

// 新协议数据结构
typedef struct {
    uint8_t led_num;
    uint8_t mode;
    uint8_t param;
    uint8_t reserved;
} new_protocol_data_t;

static new_protocol_data_t new_protocol_data;

// PWM控制数据结构
typedef struct {
    uint8_t channel;
    uint8_t duty;
    uint8_t reserved;
} pwm_protocol_data_t;

static pwm_protocol_data_t pwm_protocol_data;

// 流星灯状态结构
typedef struct {
    uint8_t led_num;     // LED数量
    uint8_t head_len;    // 头部长度
    uint8_t tail_len;    // 拖尾长度
    uint8_t delay;       // 步进延时（单位：10ms）
    uint8_t r;           // 红色分量
    uint8_t g;           // 绿色分量
    uint8_t b;           // 蓝色分量
    uint8_t direction;   // 方向（0=从左到右，1=从右到左）
    uint16_t position;   // 当前位置
    bool active;         // 是否激活
} meteor_led_state_t;

static meteor_led_state_t meteor_state;


//
static uint8_t to_test_buffer[BLE_BUFF_MAX_LEN - 4 - 3];

//The buffer length should be a power of 2
#define APP_UART_TX_BUFFER_LENGTH    512U
#define APP_UART_RX_BUFFER_LENGTH    2048U

//The tx buffer and rx buffer for app_drv_fifo
//length should be a power of 2
static uint8_t app_uart_tx_buffer[APP_UART_TX_BUFFER_LENGTH] = {0};
static uint8_t app_uart_rx_buffer[APP_UART_RX_BUFFER_LENGTH] = {0};

static app_drv_fifo_t app_uart_tx_fifo;
static app_drv_fifo_t app_uart_rx_fifo;

//interupt uart rx flag ,clear at main loop
bool uart_rx_flag = false;

//for interrupt rx blcak hole ,when uart rx fifo full
uint8_t for_uart_rx_black_hole = 0;

//fifo length less that MTU-3, retry times
uint32_t uart_to_ble_send_evt_cnt = 0;

void PERSON_BLE_UART_DATA_HANDLE(uint8_t *data, uint16_t length);



// 处理LED控制命令的函数
void process_led_commands(void)
{
    // 触发LED控制事件，由事件处理函数处理
    tmos_start_task(Peripheral_TaskID, LED_CONTROL_EVT, 0);
}

void app_uart_process(void)
{
    UINT32 irq_status;
    SYS_DisableAllIrq(&irq_status);
    if(uart_rx_flag)
    {
        tmos_start_task(Peripheral_TaskID, UART_TO_BLE_SEND_EVT, 2);
        uart_rx_flag = false;
    }
    SYS_RecoverIrq(irq_status);

    //tx process
    if(R8_UART3_TFC < UART_FIFO_SIZE)
    {
        app_drv_fifo_read_to_same_addr(&app_uart_tx_fifo, (uint8_t *)&R8_UART3_THR, UART_FIFO_SIZE - R8_UART3_TFC);
    }
}

void app_uart_init()
{
    //tx fifo and tx fifo
    //The buffer length should be a power of 2
    app_drv_fifo_init(&app_uart_tx_fifo, app_uart_tx_buffer, APP_UART_TX_BUFFER_LENGTH);
    app_drv_fifo_init(&app_uart_rx_fifo, app_uart_rx_buffer, APP_UART_RX_BUFFER_LENGTH);

    //uart tx io
    GPIOA_SetBits(bTXD3);
    GPIOA_ModeCfg(bTXD3, GPIO_ModeOut_PP_5mA);

    //uart rx io
    GPIOA_SetBits(bRXD3);
    GPIOA_ModeCfg(bRXD3, GPIO_ModeIN_PU);

    //uart3 init
    UART3_DefInit();

    //enable interupt
    UART3_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART3_IRQn);
}

void app_uart_tx_data(uint8_t *data, uint16_t length)
{
    uint16_t write_length = length;
    app_drv_fifo_write(&app_uart_tx_fifo, data, &write_length);
}
//
//Not every uart reception will end with a UART_II_RECV_TOUT
//UART_II_RECV_TOUT can only be triggered when R8_UARTx_RFC is not 0
//Here we cannot rely UART_II_RECV_TOUT as the end of a uart reception

__attribute__((interrupt("WCH-Interrupt-fast")))
__attribute__((section(".highcode")))
void UART3_IRQHandler(void)
{
    uint16_t error;
    switch(UART3_GetITFlag())
    {
        case UART_II_LINE_STAT:
            UART3_GetLinSTA();
            break;

        case UART_II_RECV_RDY:
        case UART_II_RECV_TOUT:
            error = app_drv_fifo_write_from_same_addr(&app_uart_rx_fifo, (uint8_t *)&R8_UART3_RBR, R8_UART3_RFC);
            if(error != APP_DRV_FIFO_RESULT_SUCCESS)
            {
                for(uint8_t i = 0; i < R8_UART3_RFC; i++)
                {
                    //fifo full,put to fifo black hole
                    for_uart_rx_black_hole = R8_UART3_RBR;
                }
            }
            uart_rx_flag = true;
            break;

        case UART_II_THR_EMPTY:
            break;
        case UART_II_MODEM_CHG:
            break;
        default:
            break;
    }
}

//ble uart service callback handler
void on_bleuartServiceEvt(uint16_t connection_handle, ble_uart_evt_t *p_evt)
{
    switch(p_evt->type)
    {
        case BLE_UART_EVT_TX_NOTI_DISABLED:
            PRINT("%02x:bleuart_EVT_TX_NOTI_DISABLED\r\n", connection_handle);
            break;
        case BLE_UART_EVT_TX_NOTI_ENABLED:
            PRINT("%02x:bleuart_EVT_TX_NOTI_ENABLED\r\n", connection_handle);
            break;
        case BLE_UART_EVT_BLE_DATA_RECIEVED:
            PRINT("BLE RX DATA len:%d\r\n", p_evt->data.length);

            //for notify back test
            //to ble
            uint16_t to_write_length = p_evt->data.length;
            app_drv_fifo_write(&app_uart_rx_fifo, (uint8_t *)p_evt->data.p_data, &to_write_length);
            tmos_start_task(Peripheral_TaskID, UART_TO_BLE_SEND_EVT, 2);
            //end of nofify back test

            //ble to uart
            app_uart_tx_data((uint8_t *)p_evt->data.p_data, p_evt->data.length);

             //data analyze
            PERSON_BLE_UART_DATA_HANDLE((uint8_t *)p_evt->data.p_data, p_evt->data.length);

            break;
        default:
            break;
    }
}

/******************************************************************
 * 功能：蓝牙串口数据处理函数
 * 参数:uint8_t *data,接收的数据； uint16_t length，接收的数据长度
 * 返回值：无
 ******************************************************************/
void PERSON_BLE_UART_DATA_HANDLE(uint8_t *data, uint16_t length)
{
//    UART1_SendString(data,length);
//    UART1_SendString("\r\n",2);
    PRINT("data[0]=%02X",data[0]);
    
    // 保持原有0和1的处理
    if(data[0]=='1')
    {
        GPIOB_ModeCfg(GPIO_Pin_15, GPIO_ModeOut_PP_5mA);//推挽输出最大5mA
        GPIOB_ResetBits(GPIO_Pin_15);
        UART1_SendString("LED turn on\r\n",sizeof("LED turn on\r\n"));
    }
    else if(data[0]=='0'){
        GPIOB_ModeCfg(GPIO_Pin_15, GPIO_ModeOut_PP_5mA);//推挽输出最大5mA
        GPIOB_SetBits(GPIO_Pin_15);
        UART1_SendString("LED turn off\r\n",sizeof("LED turn off\r\n"));
    }
    // 新协议处理
    else if(length >= 6 && data[0] == 0xEE && data[5] == 0xFF)
    {
        // 解析新协议数据
        new_protocol_data.led_num = data[1];
        new_protocol_data.mode = data[2];
        new_protocol_data.param = data[3];
        new_protocol_data.reserved = data[4];

        // 设置标志位，在主循环中处理
        led_cmd = LED_CMD_NEW_PROTOCOL;

        // 发送响应
        UART1_SendString("New protocol command received\r\n", sizeof("New protocol command received\r\n"));
    }
    // PWM控制协议处理
    else if(length >= 5 && data[0] == 0xEE && data[4] == 0xAF) // PWM控制协议：EE CHANNEL DUTY RESERVED AF
    {
        // 解析PWM协议数据
        pwm_protocol_data.channel = data[1];
        pwm_protocol_data.duty = data[2];
        pwm_protocol_data.reserved = data[3];

        // 验证通道号
        if(pwm_protocol_data.channel == 0x01 || pwm_protocol_data.channel == 0x04 || pwm_protocol_data.channel == 0x05)
        {
            // 触发PWM控制事件
            tmos_start_task(Peripheral_TaskID, PWM_CONTROL_EVT, 0);

            // 发送响应
            UART1_SendString("PWM control command received\r\n", sizeof("PWM control command received\r\n"));
        }
    }
    // 流星灯协议处理
    else if(length >= 10 && data[0] == 0xEE && data[9] == 0xBF) // 流星灯协议：EE XX HH TT DD RR GG BB DR BF
    {
        // 解析流星灯协议数据
        uint8_t led_num = data[1];
        uint8_t head_len = data[2];
        uint8_t tail_len = data[3];
        uint8_t delay = data[4];
        uint8_t r = data[5];
        uint8_t g = data[6];
        uint8_t b = data[7];
        uint8_t direction = data[8]; // 方向（0=从左到右，1=从右到左）

        // 检查是否为停止指令（LED数量为0）
        if(led_num == 0)
        {
            // 停止流星灯
            meteor_state.active = FALSE;
            // 关闭所有LED
            ws2812_off_all();
            UART1_SendString("Meteor LED stopped\r\n", sizeof("Meteor LED stopped\r\n"));
        }
        else
        {
            // 存储流星灯参数
            meteor_state.led_num = led_num;
            meteor_state.head_len = head_len;
            meteor_state.tail_len = tail_len;
            meteor_state.delay = delay;
            meteor_state.r = r;
            meteor_state.g = g;
            meteor_state.b = b;
            meteor_state.direction = direction;
            meteor_state.position = 0;
            meteor_state.active = TRUE;

            // 触发流星灯控制事件
            tmos_start_task(Peripheral_TaskID, METEOR_LED_EVT, 0);

            // 发送响应
            UART1_SendString("Meteor LED command received\r\n", sizeof("Meteor LED command received\r\n"));
        }
    }

}


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// How often to perform periodic event
#define SBP_PERIODIC_EVT_PERIOD              1600

// How often to perform read rssi event
#define SBP_READ_RSSI_EVT_PERIOD             3200

// Parameter update delay
#define SBP_PARAM_UPDATE_DELAY               6400

// What is the advertising interval when device is discoverable (units of 625us, 80=50ms)
#define DEFAULT_ADVERTISING_INTERVAL         160

// Limited discoverable mode advertises for 30.72s, and then stops
// General discoverable mode advertises indefinitely
#define DEFAULT_DISCOVERABLE_MODE            GAP_ADTYPE_FLAGS_GENERAL

// Minimum connection interval (units of 1.25ms, 10=12.5ms)
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL    8

// Maximum connection interval (units of 1.25ms, 100=125ms)
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL    20

// Slave latency to use parameter update
#define DEFAULT_DESIRED_SLAVE_LATENCY        0

// Supervision timeout value (units of 10ms, 100=1s)
#define DEFAULT_DESIRED_CONN_TIMEOUT         100

// Company Identifier: WCH
#define WCH_COMPANY_ID                       0x07D7

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

//for send to ble
typedef enum
{
    SEND_TO_BLE_TO_SEND = 1,
    SEND_TO_BLE_ALLOC_FAILED,
    SEND_TO_BLE_SEND_FAILED,
} send_to_ble_state_t;
send_to_ble_state_t send_to_ble_state = SEND_TO_BLE_TO_SEND;

blePaControlConfig_t pa_lna_ctl;

//static uint8 Peripheral_TaskID = INVALID_TASK_ID;   // Task ID for internal task/event processing

// GAP - SCAN RSP data (max size = 31 bytes)
static uint8 scanRspData[] =
{
    // complete name
    15, // length of this data
    GAP_ADTYPE_LOCAL_NAME_COMPLETE,
    'x', 'i', 'x', 'i', '_', 'k', 'a', 'n', 'b', 'u', 'd', 'a', 'o', 'w', 'o',
    // connection interval range
    0x05, // length of this data
    GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE,
    LO_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL), // 100ms
    HI_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL),
    LO_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL), // 1s
    HI_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL),

    // Tx power level
    0x02, // length of this data
    GAP_ADTYPE_POWER_LEVEL,
    0 // 0dBm
};

// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting)
static uint8 advertData[] =
{
    // Flags; this sets the device to use limited discoverable
    // mode (advertises for 30 seconds at a time) instead of general
    // discoverable mode (advertises indefinitely)
    0x02, // length of this data
    GAP_ADTYPE_FLAGS,
    DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

    // service UUID, to notify central devices what services are included
    // in this peripheral
    0x03,                  // length of this data
    GAP_ADTYPE_16BIT_MORE, // some of the UUID's, but not all
    LO_UINT16(SIMPLEPROFILE_SERV_UUID),
    HI_UINT16(SIMPLEPROFILE_SERV_UUID)};

// GAP GATT Attributes
static uint8 attDeviceName[GAP_DEVICE_NAME_LEN] = "xixi_kanbudaowo";

// Connection item list
static peripheralConnItem_t peripheralConnList;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void Peripheral_ProcessTMOSMsg(tmos_event_hdr_t *pMsg);
static void peripheralStateNotificationCB(gapRole_States_t newState, gapRoleEvent_t *pEvent);

static void peripheralParamUpdateCB(uint16 connHandle, uint16 connInterval,
                                    uint16 connSlaveLatency, uint16 connTimeout);
static void peripheralInitConnItem(peripheralConnItem_t *peripheralConnList);
static void peripheralRssiCB(uint16 connHandle, int8 rssi);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t Peripheral_PeripheralCBs =
{
    peripheralStateNotificationCB, // Profile State Change Callbacks
    peripheralRssiCB,              // When a valid RSSI is read from controller (not used by application)
    peripheralParamUpdateCB
};

// Broadcast Callbacks
static gapRolesBroadcasterCBs_t Broadcaster_BroadcasterCBs =
{
    NULL, // Not used in peripheral role
    NULL  // Receive scan request callback
};

// GAP Bond Manager Callbacks
static gapBondCBs_t Peripheral_BondMgrCBs =
{
    NULL, // Passcode callback (not used by application)
    NULL  // Pairing / Bonding state Callback (not used by application)
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Peripheral_Init
 *
 * @brief   Initialization function for the Peripheral App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by TMOS.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void Peripheral_Init()
{
    Peripheral_TaskID = TMOS_ProcessEventRegister(Peripheral_ProcessEvent);

    // Setup the GAP Peripheral Role Profile
    {
        uint8  initial_advertising_enable = TRUE;
        uint16 desired_min_interval = 6;
        uint16 desired_max_interval = 1000;

        // Set the GAP Role Parameters
        GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8), &initial_advertising_enable);
        GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof(scanRspData), scanRspData);
        GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData), advertData);
        GAPRole_SetParameter(GAPROLE_MIN_CONN_INTERVAL, sizeof(uint16), &desired_min_interval);
        GAPRole_SetParameter(GAPROLE_MAX_CONN_INTERVAL, sizeof(uint16), &desired_max_interval);
    }

    // Set advertising interval
    {
        uint16 advInt = DEFAULT_ADVERTISING_INTERVAL;

        GAP_SetParamValue(TGAP_DISC_ADV_INT_MIN, advInt);
        GAP_SetParamValue(TGAP_DISC_ADV_INT_MAX, advInt);
    }

    // Setup the GAP Bond Manager
    {
        uint32 passkey = 0; // passkey "000000"
        uint8  pairMode = GAPBOND_PAIRING_MODE_WAIT_FOR_REQ;
        uint8  mitm = TRUE;
        uint8  bonding = TRUE;
        uint8  ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;
        GAPBondMgr_SetParameter(GAPBOND_PERI_DEFAULT_PASSCODE, sizeof(uint32), &passkey);
        GAPBondMgr_SetParameter(GAPBOND_PERI_PAIRING_MODE, sizeof(uint8), &pairMode);
        GAPBondMgr_SetParameter(GAPBOND_PERI_MITM_PROTECTION, sizeof(uint8), &mitm);
        GAPBondMgr_SetParameter(GAPBOND_PERI_IO_CAPABILITIES, sizeof(uint8), &ioCap);
        GAPBondMgr_SetParameter(GAPBOND_PERI_BONDING_ENABLED, sizeof(uint8), &bonding);
    }

    // Initialize GATT attributes
    GGS_AddService(GATT_ALL_SERVICES);         // GAP
    GATTServApp_AddService(GATT_ALL_SERVICES); // GATT attributes
    DevInfo_AddService();                      // Device Information Service
    ble_uart_add_service(on_bleuartServiceEvt);

    // Set the GAP Characteristics
    GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName);

    // Init Connection Item
    peripheralInitConnItem(&peripheralConnList);

    // Register receive scan request callback
    GAPRole_BroadcasterSetCB(&Broadcaster_BroadcasterCBs);

    // 初始化流星灯默认参数（上电启动效果）
    // TODO: 修复蓝牙发送无法点亮的问题
    // EE20020308FF66FF00BF蓝牙发送无法点亮 (测试发现，当红色为0xff时，无法点亮)
    // meteor_state.led_num = 10;      // 默认10个LED
    // meteor_state.head_len = 2;       // 头部长度
    // meteor_state.tail_len = 5;       // 拖尾长度
    // meteor_state.delay = 10;         // 延时100ms
    // meteor_state.r = 0xff;           // 红色
    // meteor_state.g = 0x66;           // 绿色
    // meteor_state.b = 0xff;           // 蓝色
    // meteor_state.direction = 0;      // 从左到右
    // meteor_state.position = 0;       // 起始位置
    // meteor_state.active = TRUE;      // 激活

    // 触发流星灯控制事件
    tmos_start_task(Peripheral_TaskID, METEOR_LED_EVT, 1000); // 延时1秒启动，确保系统初始化完成

    // Setup a delayed profile startup
    tmos_set_event(Peripheral_TaskID, SBP_START_DEVICE_EVT);
}

/*********************************************************************
 * @fn      peripheralInitConnItem
 *
 * @brief   Init Connection Item
 *
 * @param   peripheralConnList -
 *
 * @return  NULL
 */
static void peripheralInitConnItem(peripheralConnItem_t *peripheralConnList)
{
    peripheralConnList->connHandle = GAP_CONNHANDLE_INIT;
    peripheralConnList->connInterval = 0;
    peripheralConnList->connSlaveLatency = 0;
    peripheralConnList->connTimeout = 0;
}

uint32_t get_fattime(void)
{
    return 0;
}

/*********************************************************************
 * @fn      Peripheral_ProcessEvent
 *
 * @brief   Peripheral Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id - The TMOS assigned task ID.
 * @param   events  - events to process.  This is a bit map and can
 *                    contain more than one event.
 *
 * @return  events not processed
 */
uint16 Peripheral_ProcessEvent(uint8 task_id, uint16 events)
{
    static attHandleValueNoti_t noti;
    //  VOID task_id; // TMOS required parameter that isn't used in this function

    if(events & SYS_EVENT_MSG)
    {
        uint8 *pMsg;

        if((pMsg = tmos_msg_receive(Peripheral_TaskID)) != NULL)
        {
            Peripheral_ProcessTMOSMsg((tmos_event_hdr_t *)pMsg);
            // Release the TMOS message
            tmos_msg_deallocate(pMsg);
        }
        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if(events & SBP_START_DEVICE_EVT)
    {
        // Start the Device
        GAPRole_PeripheralStartDevice(Peripheral_TaskID, &Peripheral_BondMgrCBs, &Peripheral_PeripheralCBs);
        return (events ^ SBP_START_DEVICE_EVT);
    }
    if(events & SBP_PARAM_UPDATE_EVT)
    {
        // Send connect param update request
        GAPRole_PeripheralConnParamUpdateReq(peripheralConnList.connHandle,
                                             DEFAULT_DESIRED_MIN_CONN_INTERVAL,
                                             DEFAULT_DESIRED_MAX_CONN_INTERVAL,
                                             DEFAULT_DESIRED_SLAVE_LATENCY,
                                             DEFAULT_DESIRED_CONN_TIMEOUT,
                                             Peripheral_TaskID);

//        GAPRole_PeripheralConnParamUpdateReq( peripheralConnList.connHandle,
//                                              10,
//                                              20,
//                                              0,
//                                              400,
//                                              Peripheral_TaskID);

        return (events ^ SBP_PARAM_UPDATE_EVT);
    }

    if(events & UART_TO_BLE_SEND_EVT)
    {
        static uint16_t read_length = 0;;
        uint8_t result = 0xff;
        switch(send_to_ble_state)
        {
            case SEND_TO_BLE_TO_SEND:

                //notify is not enabled
                if(!ble_uart_notify_is_ready(peripheralConnList.connHandle))
                {
                    if(peripheralConnList.connHandle == GAP_CONNHANDLE_INIT)
                    {
                        //connection lost, flush rx fifo here
                        app_drv_fifo_flush(&app_uart_rx_fifo);
                    }
                    break;
                }
                read_length = ATT_GetMTU(peripheralConnList.connHandle) - 3;

                if(app_drv_fifo_length(&app_uart_rx_fifo) >= read_length)
                {
                    PRINT("FIFO_LEN:%d\r\n", app_drv_fifo_length(&app_uart_rx_fifo));
                    result = app_drv_fifo_read(&app_uart_rx_fifo, to_test_buffer, &read_length);
                    uart_to_ble_send_evt_cnt = 0;
                }
                else
                {
                    if(uart_to_ble_send_evt_cnt > 10)
                    {
                        result = app_drv_fifo_read(&app_uart_rx_fifo, to_test_buffer, &read_length);
                        uart_to_ble_send_evt_cnt = 0;
                    }
                    else
                    {
                        tmos_start_task(Peripheral_TaskID, UART_TO_BLE_SEND_EVT, 4);
                        uart_to_ble_send_evt_cnt++;
                        PRINT("NO TIME OUT\r\n");
                    }
                }

                if(APP_DRV_FIFO_RESULT_SUCCESS == result)
                {
                    noti.len = read_length;
                    noti.pValue = GATT_bm_alloc(peripheralConnList.connHandle, ATT_HANDLE_VALUE_NOTI, noti.len, NULL, 0);
                    if(noti.pValue != NULL)
                    {
                        tmos_memcpy(noti.pValue, to_test_buffer, noti.len);
                        result = ble_uart_notify(peripheralConnList.connHandle, &noti, 0);
                        if(result != SUCCESS)
                        {
                            PRINT("R1:%02x\r\n", result);
                            send_to_ble_state = SEND_TO_BLE_SEND_FAILED;
                            GATT_bm_free((gattMsg_t *)&noti, ATT_HANDLE_VALUE_NOTI);
                            tmos_start_task(Peripheral_TaskID, UART_TO_BLE_SEND_EVT, 2);
                        }
                        else
                        {
                            send_to_ble_state = SEND_TO_BLE_TO_SEND;
                            //app_fifo_write(&app_uart_tx_fifo,to_test_buffer,&read_length);
                            //app_drv_fifo_write(&app_uart_tx_fifo,to_test_buffer,&read_length);
                            read_length = 0;
                            tmos_start_task(Peripheral_TaskID, UART_TO_BLE_SEND_EVT, 2);
                        }
                    }
                    else
                    {
                        send_to_ble_state = SEND_TO_BLE_ALLOC_FAILED;
                        tmos_start_task(Peripheral_TaskID, UART_TO_BLE_SEND_EVT, 2);
                    }
                }
                else
                {
                    //send_to_ble_state = SEND_TO_BLE_FIFO_EMPTY;
                }
                break;
            case SEND_TO_BLE_ALLOC_FAILED:
            case SEND_TO_BLE_SEND_FAILED:

                noti.len = read_length;
                noti.pValue = GATT_bm_alloc(peripheralConnList.connHandle, ATT_HANDLE_VALUE_NOTI, noti.len, NULL, 0);
                if(noti.pValue != NULL)
                {
                    tmos_memcpy(noti.pValue, to_test_buffer, noti.len);
                    result = ble_uart_notify(peripheralConnList.connHandle, &noti, 0);
                    if(result != SUCCESS)
                    {
                        PRINT("R2:%02x\r\n", result);
                        send_to_ble_state = SEND_TO_BLE_SEND_FAILED;
                        GATT_bm_free((gattMsg_t *)&noti, ATT_HANDLE_VALUE_NOTI);
                        tmos_start_task(Peripheral_TaskID, UART_TO_BLE_SEND_EVT, 2);
                    }
                    else
                    {
                        send_to_ble_state = SEND_TO_BLE_TO_SEND;
                        //app_drv_fifo_write(&app_uart_tx_fifo,to_test_buffer,&read_length);
                        read_length = 0;
                        tmos_start_task(Peripheral_TaskID, UART_TO_BLE_SEND_EVT, 2);
                    }
                }
                else
                {
                    send_to_ble_state = SEND_TO_BLE_ALLOC_FAILED;
                    tmos_start_task(Peripheral_TaskID, UART_TO_BLE_SEND_EVT, 2);
                }
                break;
            default:
                break;
        }
        return (events ^ UART_TO_BLE_SEND_EVT);
    }

    if(events & LED_CONTROL_EVT)
    {
        UINT32 irq_status;
        SYS_DisableAllIrq(&irq_status);
        led_cmd_t current_cmd = led_cmd;
        if(current_cmd != LED_CMD_NONE)
        {
            led_cmd = LED_CMD_NONE;
        }
        SYS_RecoverIrq(irq_status);

        switch(current_cmd)
        {
            case LED_CMD_RED:
                // 设置所有LED为红色
                for(uint32_t i=0; i<ws2812_led_num; i++) {
                    ws2812_set_rgb(&ws2812_buf[i], 0xff, 0, 0);
                }
                SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12*ws2812_led_num);
                break;
            case LED_CMD_GREEN:
                // 设置所有LED为绿色
                for(uint32_t i=0; i<ws2812_led_num; i++) {
                    ws2812_set_rgb(&ws2812_buf[i], 0, 0xff, 0);
                }
                SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12*ws2812_led_num);
                break;
            case LED_CMD_BLUE:
                // 设置所有LED为蓝色
                for(uint32_t i=0; i<ws2812_led_num; i++) {
                    ws2812_set_rgb(&ws2812_buf[i], 0, 0, 0xff);
                }
                SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12*ws2812_led_num);
                break;
            case LED_CMD_WHITE:
                // 设置所有LED为白色
                for(uint32_t i=0; i<ws2812_led_num; i++) {
                    ws2812_set_rgb(&ws2812_buf[i], 0xff, 0xff, 0xff);
                }
                SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12*ws2812_led_num);
                break;
            case LED_CMD_OFF:
                // 关闭所有LED
                ws2812_off_all();
                break;
            case LED_CMD_YELLOW:
                // 设置所有LED为黄色
                for(uint32_t i=0; i<ws2812_led_num; i++) {
                    ws2812_set_rgb(&ws2812_buf[i], 0xff, 0xff, 0);
                }
                SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12*ws2812_led_num);
                break;
            case LED_CMD_NEW_PROTOCOL:
                // 处理新协议
                {
                    // 设置灯的数量
                    if(new_protocol_data.led_num > 0) {
                        ws2812_set_led_num(new_protocol_data.led_num);
                    }

                    // 处理不同模式
                    switch(new_protocol_data.mode)
                    {
                        case 0x00: // 关闭所有灯
                            ws2812_off_all();
                            break;
                        case 0x01: // 单色模式
                            {
                                // 参数高4位为R，低4位为G，备用字节为B
                                uint8_t r = (new_protocol_data.param >> 4) * 17; // 0-15 -> 0-255
                                uint8_t g = (new_protocol_data.param & 0x0F) * 17;
                                uint8_t b = (new_protocol_data.reserved & 0x0F) * 17;

                                for(uint32_t i=0; i<ws2812_led_num; i++) {
                                    ws2812_set_rgb(&ws2812_buf[i], r, g, b);
                                }
                                SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12*ws2812_led_num);
                            }
                            break;
                        // TODO: 流水灯模式
                        // case 0x02: // 流水灯模式
                        // {
                        //     // 参数为速度
                        //     uint8_t speed = new_protocol_data.param;
                        //     // 简单实现：依次点亮LED
                        //     for(uint32_t i=0; i<ws2812_led_num; i++) {
                        //         // 关闭所有LED
                        //         ws2812_off_all();
                        //         // 点亮当前LED
                        //         ws2812_set_rgb(&ws2812_buf[i], 0xff, 0xff, 0);
                        //         SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12*ws2812_led_num);
                        //         // 延时
                        //         for(uint32_t j=0; j<speed*1000; j++);
                        //     }
                        //     // 最后关闭所有LED
                        //     ws2812_off_all();
                        // }
                        // break;
                        // TODO: 呼吸灯模式
                        // case 0x03: // 呼吸灯模式
                        // {
                        //     // 参数为呼吸频率
                        //     uint8_t freq = new_protocol_data.param;
                        //     // 简单实现：亮度渐变
                        //     for(uint8_t brightness=0; brightness<255; brightness+=10) {
                        //         for(uint32_t i=0; i<ws2812_led_num; i++) {
                        //             ws2812_set_rgb(&ws2812_buf[i], brightness, brightness, brightness);
                        //         }
                        //         SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12*ws2812_led_num);
                        //         for(uint32_t j=0; j<freq*100; j++);
                        //     }
                        //     for(uint8_t brightness=255; brightness>0; brightness-=10) {
                        //         for(uint32_t i=0; i<ws2812_led_num; i++) {
                        //             ws2812_set_rgb(&ws2812_buf[i], brightness, brightness, brightness);
                        //         }
                        //         SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12*ws2812_led_num);
                        //         for(uint32_t j=0; j<freq*100; j++);
                        //     }
                        // }
                        // break;
                        default:
                            // 未知模式
                            break;
                    }
                }
                break;
            default:
                break;
        }
        return (events ^ LED_CONTROL_EVT);
    }
    if(events & PWM_CONTROL_EVT)
    {
        // 处理PWM控制事件
        UINT32 irq_status;
        SYS_DisableAllIrq(&irq_status);
        uint8_t channel = pwm_protocol_data.channel;
        uint8_t duty = pwm_protocol_data.duty;
        SYS_RecoverIrq(irq_status);

        // TODO: 实现PWM具体控制逻辑
        // 1. 计算占空比（0-1000）
        // 2. 配置GPIO
        // 3. 初始化并设置PWM

        // 发送调试信息
        char debug_msg[50];
        sprintf(debug_msg, "PWM Control: Channel=%02X, Duty=%02X\r\n", channel, duty);
        UART1_SendString(debug_msg, strlen(debug_msg));

        return (events ^ PWM_CONTROL_EVT);
    }

    if(events & METEOR_LED_EVT)
    {
        // 处理流星灯控制事件
        UINT32 irq_status;
        SYS_DisableAllIrq(&irq_status);
        meteor_led_state_t current_state = meteor_state;
        SYS_RecoverIrq(irq_status);

        // 测试用例1：点亮一颗WS2812
        // 协议：EE 01 01 01 01 01 01 01 BF
        if(current_state.led_num == 0x01 && current_state.head_len == 0x01 &&
           current_state.tail_len == 0x01 && current_state.delay == 0x01 &&
           current_state.r == 0x01 && current_state.g == 0x01 && current_state.b == 0x01)
        {
            // 点亮第一颗LED（白色高亮）
            ws2812_set_led_num(1);
            ws2812_set_rgb(&ws2812_buf[0], 0xff, 0xff, 0xff);
            SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12);
            UART1_SendString("LED ON\r\n", sizeof("LED ON\r\n"));
        }
        // 测试用例2：熄灭一颗WS2812
        // 协议：EE 01 00 00 00 00 00 00 BF
        else if(current_state.led_num == 0x01 && current_state.head_len == 0x00 &&
                current_state.tail_len == 0x00 && current_state.delay == 0x00 &&
                current_state.r == 0x00 && current_state.g == 0x00 && current_state.b == 0x00)
        {
            // 熄灭第一颗LED
            ws2812_set_led_num(1);
            ws2812_set_rgb(&ws2812_buf[0], 0, 0, 0);
            SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12);
            UART1_SendString("LED OFF\r\n", sizeof("LED OFF\r\n"));
        }
        // 流星灯效果
        else if(current_state.active && current_state.led_num > 0)
        {
            // 1. 更新流星位置
            if(current_state.direction == 0) // 从左到右
            {
                meteor_state.position++;
                if(meteor_state.position >= current_state.led_num)
                    meteor_state.position = 0;
            }
            else // 从右到左
            {
                if(meteor_state.position == 0)
                    meteor_state.position = current_state.led_num - 1;
                else
                    meteor_state.position--;
            }
            
            // 2. 计算并填充颜色数据
            for(uint32_t i=0; i<current_state.led_num; i++)
            {
                int32_t distance = abs((int32_t)i - (int32_t)meteor_state.position);
                if(distance < current_state.head_len)
                {
                    // 头部：全亮度
                    ws2812_set_rgb(&ws2812_buf[i], current_state.r, current_state.g, current_state.b);
                }
                else if(distance < current_state.head_len + current_state.tail_len)
                {
                    // 拖尾：亮度衰减
                    uint8_t attenuation = 255 - ((distance - current_state.head_len) * 255 / current_state.tail_len);
                    ws2812_set_rgb(&ws2812_buf[i], 
                        (current_state.r * attenuation) / 255,
                        (current_state.g * attenuation) / 255,
                        (current_state.b * attenuation) / 255);
                }
                else
                {
                    // 其他位置：关闭
                    ws2812_set_rgb(&ws2812_buf[i], 0, 0, 0);
                }
            }
            
            // 3. 发送数据
            SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12 * current_state.led_num);
            
            // 4. 设置下一次事件
            if(meteor_state.active)
            {
                tmos_start_task(Peripheral_TaskID, METEOR_LED_EVT, current_state.delay * 10); // 单位：10ms
            }
        }

        return (events ^ METEOR_LED_EVT);
    }

    // Discard unknown events
    return 0;
}

/*********************************************************************
 * @fn      Peripheral_ProcessTMOSMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void Peripheral_ProcessTMOSMsg(tmos_event_hdr_t *pMsg)
{
    switch(pMsg->event)
    {
        default:
            break;
    }
}

/*********************************************************************
 * @fn      Peripheral_LinkEstablished
 *
 * @brief   Process link established.
 *
 * @param   pEvent - event to process
 *
 * @return  none
 */
static void Peripheral_LinkEstablished(gapRoleEvent_t *pEvent)
{
    gapEstLinkReqEvent_t *event = (gapEstLinkReqEvent_t *)pEvent;

    // See if already connected
    if(peripheralConnList.connHandle != GAP_CONNHANDLE_INIT)
    {
        GAPRole_TerminateLink(pEvent->linkCmpl.connectionHandle);
        PRINT("Connection max...\n");
    }
    else
    {
        peripheralConnList.connHandle = event->connectionHandle;
        peripheralConnList.connInterval = event->connInterval;
        peripheralConnList.connSlaveLatency = event->connLatency;
        peripheralConnList.connTimeout = event->connTimeout;

        // Set timer for param update event
        tmos_start_task(Peripheral_TaskID, SBP_PARAM_UPDATE_EVT, SBP_PARAM_UPDATE_DELAY);

        PRINT("Conn %x - Int %x \n", event->connectionHandle, event->connInterval);
    }
}

/*********************************************************************
 * @fn      Peripheral_LinkTerminated
 *
 * @brief   Process link terminated.
 *
 * @param   pEvent - event to process
 *
 * @return  none
 */
static void Peripheral_LinkTerminated(gapRoleEvent_t *pEvent)
{
    gapTerminateLinkEvent_t *event = (gapTerminateLinkEvent_t *)pEvent;

    if(event->connectionHandle == peripheralConnList.connHandle)
    {
        peripheralConnList.connHandle = GAP_CONNHANDLE_INIT;
        peripheralConnList.connInterval = 0;
        peripheralConnList.connSlaveLatency = 0;
        peripheralConnList.connTimeout = 0;

        // Restart advertising
        {
            uint8 advertising_enable = TRUE;
            GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8), &advertising_enable);
        }
    }
    else
    {
        PRINT("ERR..\n");
    }
}

/*********************************************************************
 * @fn      peripheralRssiCB
 *
 * @brief   RSSI callback.
 *
 * @param   connHandle - connection handle
 * @param   rssi - RSSI
 *
 * @return  none
 */
static void peripheralRssiCB(uint16 connHandle, int8 rssi)
{
    PRINT("RSSI -%d dB Conn  %x \n", -rssi, connHandle);
}

/*********************************************************************
 * @fn      peripheralParamUpdateCB
 *
 * @brief   Parameter update complete callback
 *
 * @param   connHandle - connect handle
 *          connInterval - connect interval
 *          connSlaveLatency - connect slave latency
 *          connTimeout - connect timeout
 *
 * @return  none
 */
static void peripheralParamUpdateCB(uint16 connHandle, uint16 connInterval,
                                    uint16 connSlaveLatency, uint16 connTimeout)
{
    if(connHandle == peripheralConnList.connHandle)
    {
        peripheralConnList.connInterval = connInterval;
        peripheralConnList.connSlaveLatency = connSlaveLatency;
        peripheralConnList.connTimeout = connTimeout;

        PRINT("Update %x - Int %x \n", connHandle, connInterval);
    }
    else
    {
        PRINT("peripheralParamUpdateCB err..\n");
    }
}

/*********************************************************************
 * @fn      peripheralStateNotificationCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void peripheralStateNotificationCB(gapRole_States_t newState, gapRoleEvent_t *pEvent)
{
    switch(newState)
    {
        case GAPROLE_STARTED:
            PRINT("Initialized..\n");
            break;

        case GAPROLE_ADVERTISING:
            if(pEvent->gap.opcode == GAP_LINK_TERMINATED_EVENT)
            {
                Peripheral_LinkTerminated(pEvent);
            }
            PRINT("Advertising..\n");
            break;

        case GAPROLE_CONNECTED:
            if(pEvent->gap.opcode == GAP_LINK_ESTABLISHED_EVENT)
            {
                Peripheral_LinkEstablished(pEvent);
                PRINT("Connected..\n");
            }
            break;

        case GAPROLE_CONNECTED_ADV:
            PRINT("Connected Advertising..\n");
            break;

        case GAPROLE_WAITING:
            if(pEvent->gap.opcode == GAP_END_DISCOVERABLE_DONE_EVENT)
            {
                PRINT("Waiting for advertising..\n");
            }
            else if(pEvent->gap.opcode == GAP_LINK_TERMINATED_EVENT)
            {
                Peripheral_LinkTerminated(pEvent);
                PRINT("Disconnected.. Reason:%x\n", pEvent->linkTerminate.reason);
            }
            else if(pEvent->gap.opcode == GAP_LINK_ESTABLISHED_EVENT)
            {
                if(pEvent->gap.hdr.status != SUCCESS)
                {
                    PRINT("Waiting for advertising..\n");
                }
                else
                {
                    PRINT("Error..\n");
                }
            }
            else
            {
                PRINT("Error..%x\n", pEvent->gap.opcode);
            }
            break;

        case GAPROLE_ERROR:
            PRINT("Error..\n");
            break;

        default:
            break;
    }
}

/*********************************************************************
*********************************************************************/
