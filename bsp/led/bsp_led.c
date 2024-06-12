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

#include "bsp_led.h"


static const uint32_t LED_PORT[LED_MAX_NUM] = {LED1_GPIO_PORT};

static const uint32_t LED_PIN[LED_MAX_NUM] = {LED1_PIN};
static const rcu_periph_enum LED_CLK[LED_MAX_NUM] = {LED1_GPIO_CLK};
 /**
  -  @brief  初始化LED灯
  -  @note   None
  -  @param  led：LED1,LED2,LED3,LED4
  -  @retval None
 */
static void bsp_led_gpio_init(led_type_def led)
{
    /* enable the led clock */
    rcu_periph_clock_enable(LED_CLK[led]);
    /* configure led GPIO port */
    gpio_mode_set(LED_PORT[led], GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                  LED_PIN[led]);
    gpio_output_options_set(LED_PORT[led], GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,
                            LED_PIN[led]);
}

 /**
  -  @brief  初始化所有LED灯，并设置LED为关闭状态
  -  @note   None
  -  @param  None
  -  @retval None
 */
void bsp_led_init(void)
{
    for (led_type_def _led = LED1; _led < LED_MAX_NUM; _led++)
    {
        bsp_led_gpio_init(_led);
        bsp_led_off(_led);
    }
}
 /**
  -  @brief  点亮led
  -  @note   None
  -  @param  led：LED1,LED2,LED3,LED4
  -  @retval None
 */
void bsp_led_on(led_type_def led)
{
    gpio_bit_set(LED_PORT[led], LED_PIN[led]);
}

/**
 -  @brief  熄灭led
 -  @note   None
 -  @param  led：LED1,LED2,LED3,LED4
 -  @retval None
*/
void bsp_led_off(led_type_def led)
{
    gpio_bit_reset(LED_PORT[led], LED_PIN[led]);
}

/**
 -  @brief  翻转led
 -  @note   None
 -  @param  led：LED1,LED2,LED3,LED4
 -  @retval None
*/
void bsp_led_toggle(led_type_def led)
{
    gpio_bit_toggle(LED_PORT[led], LED_PIN[led]);
}

/**
 -  @brief  获取LED状态
 -  @note   None
 -  @param  None
 -  @retval 0：LED灯熄灭，1：LED灯亮了
*/
uint8_t bsp_led_get_status(led_type_def led)
{
    FlagStatus led_status = RESET;
    led_status = gpio_input_bit_get(LED_PORT[led], LED_PIN[led]);
    if (led_status == RESET)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
