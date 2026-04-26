#include "ws2812.h"

// 全局变量：当前灯带数量
uint16_t ws2812_led_num = WS2812_DEFAULT_LED_NUM;

__attribute__((aligned(4))) ws2812_in_spi_buffer_t ws2812_buf[WS2812_DEFAULT_LED_NUM];

void delay_ms(uint32_t ms)
{
    mDelaymS(ms);
}

uint32_t map_table[8] = {
    0x8e888888,
    0xe8888888,

    0x888e8888,
    0x88e88888,

    0x88888e88,
    0x8888e888,

    0x8888888e,
    0x888888e8,
};

void ws2812_set_rgb(ws2812_in_spi_buffer_t *buf, uint8_t r, uint8_t g, uint8_t b){

    buf->r = 0x88888888;
    for(uint8_t i=0;i<8;i++){
        if(r&0x1) {
            buf->r |= map_table[i];
        }
        r>>=1;
    }

    buf->g = 0x88888888;
    for(uint8_t i=0;i<8;i++){
        if(g&0x1) {
            buf->g |= map_table[i];
        }
        g>>=1;
    }

    buf->b = 0x88888888;
    for(uint8_t i=0;i<8;i++){
        if(b&0x1) {
            buf->b |= map_table[i];
        }
        b>>=1;
    }
}

void ws2812_test(char argc, char *argv) {
    SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12*ws2812_led_num);
}

void ws2812_off_all(void) {
    for(uint32_t i=0;i<ws2812_led_num;i++) {
        ws2812_set_rgb(&ws2812_buf[i],0,0,0);

    }
    SPI0_MasterDMATrans((uint8_t *)ws2812_buf, 12*ws2812_led_num);
}

void ws2812_init(void) {
    // 配置GPIO
    GPIOA_ModeCfg(GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14, GPIO_ModeOut_PP_5mA);
    // 初始化SPI0
    SPI0_MasterDefInit();
    // 配置SPI0时钟频率为3.75MHz (60MHz/16)
    SPI0_CLKCfg(15);
}

/**
 * @brief 设置WS2812灯带数量
 * @param num 灯带数量，范围1-WS2812_DEFAULT_LED_NUM
 */
void ws2812_set_led_num(uint16_t num) {
    if(num > 0 && num <= WS2812_DEFAULT_LED_NUM) {
        ws2812_led_num = num;
    }
}


