/*
* 梁山派软硬件资料与相关扩展板软硬件资料官网全部开源
* 开发板官网：www.lckfb.com
* 技术支持常驻论坛，任何技术问题欢迎随时交流学习
* 立创论坛：club.szlcsc.com
* 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
* 不靠卖板赚钱，以培养中国工程师为己任
* Change Logs:
* Date           Author       Notes
* 2023-11-02     LCKFB-yzh    first version
*/

#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "board.h"

typedef enum
{
    LED1 = 0,
    LED_MAX_NUM
} led_type_def;

#define LED1_PIN                         GPIO_PIN_2
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCU_GPIOB


void bsp_led_init(void);
void bsp_led_on(led_type_def led);
void bsp_led_off(led_type_def led);
void bsp_led_toggle(led_type_def led);
uint8_t bsp_led_get_status(led_type_def led);

#endif /* __BSP_LED_H__ */
