#ifndef __WS2812_H__
#define __WS2812_H__

#include "CH57x_common.h"
#include "CH57x_spi.h"

// 默认灯带数量
#define WS2812_DEFAULT_LED_NUM    100

typedef struct {
    uint32_t g;
    uint32_t r;
    uint32_t b;
} ws2812_in_spi_buffer_t;

// 外部声明
extern uint16_t ws2812_led_num;
extern ws2812_in_spi_buffer_t ws2812_buf[WS2812_DEFAULT_LED_NUM];

// 函数声明
void ws2812_set_rgb(ws2812_in_spi_buffer_t *buf, uint8_t r, uint8_t g, uint8_t b);
void ws2812_off_all(void);
void ws2812_test(char argc, char *argv);
void ws2812_init(void);
void delay_ms(uint32_t ms);
void ws2812_set_led_num(uint16_t num);

#endif
