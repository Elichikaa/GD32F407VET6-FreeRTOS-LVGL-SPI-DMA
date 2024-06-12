/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：club.szlcsc.com
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-02     LCKFB-yzh    first version
 */
#include "board.h"
#include "soft_timer.h"

// 定义软件定时器数组
static software_timer timers[SOFT_TIMER_MAX];

 /**
  -  @brief  初始化单次定时器
  -  @note   None
  -  @param  timer: 要初始化的软件定时器，timeout: 超时时间
  -  @retval None
 */
void soft_timer_single_init(soft_timer_type timer, uint32_t timeout)
{
    soft_timer_interrupt_disable();
    if (timer < SOFT_TIMER_MAX)
    {
        timers[timer].counter = 0;
        timers[timer].timeout = timeout;
        timers[timer].is_timeout = 0;
        timers[timer].is_repeat = 0;
    }
    soft_timer_interrupt_enable();
}

/**
 -  @brief  初始化重复定时器
 -  @note   None
 -  @param  timer: 要初始化的软件定时器，timeout: 超时时间
 -  @retval None
*/
void soft_timer_repeat_init(soft_timer_type timer, uint32_t timeout)
{
    soft_timer_interrupt_disable();
    if (timer < SOFT_TIMER_MAX)
    {
        timers[timer].counter = 0;
        timers[timer].timeout = timeout;
        timers[timer].is_timeout = 0;
        timers[timer].is_repeat = 1;
    }
    soft_timer_interrupt_enable();
}

 /**
  -  @brief  停止一个软件定时器
  -  @note   None
  -  @param  timer：要停止的软件定时器
  -  @retval None
 */
void soft_timer_stop(soft_timer_type timer)
{
    soft_timer_interrupt_disable();
    if (timer < SOFT_TIMER_MAX)
    {
        timers[timer].is_timeout = 0;
        timers[timer].counter = 0;
        timers[timer].is_repeat = 0;
    }
    soft_timer_interrupt_enable();
}

 /**
  -  @brief  检查对应的定时器是否超时了
  -  @note   None
  -  @param  timer：要检查的定时器
  -  @retval None
 */
uint8_t soft_timer_is_timeout(soft_timer_type timer)
{
    uint8_t ret = 0;
    if (timer < SOFT_TIMER_MAX)
    {
        ret = timers[timer].is_timeout;
        if (ret)
        {
            timers[timer].is_timeout = 0;
        }
    }
    return ret;
}

/**
 -  @brief  复位对应的定时器
 -  @note   None
 -  @param  timer：要复位的定时器
 -  @retval None
*/
void soft_timer_reset(soft_timer_type timer)
{
    soft_timer_interrupt_disable();
    if (timer < SOFT_TIMER_MAX)
    {
        timers[timer].counter = 0;
        timers[timer].is_timeout = 0;
    }
    soft_timer_interrupt_enable();
}

 /**
  -  @brief  在GD32的SysTick_Handler中断服务程序中调用，每1ms调用一次，在这个工程里面他在
  board.c文件中被定时调用。
  -  @note   None
  -  @param  None
  -  @retval None
 */
void soft_timer_tick(void)
{
    for (soft_timer_type timer = SOFT_TIMER_0; timer < SOFT_TIMER_MAX; ++timer)
    {
        if (++timers[timer].counter >= timers[timer].timeout)
        {
            timers[timer].is_timeout = 1;
            if (timers[timer].is_repeat)
            {
                timers[timer].counter = 0;
            }
        }
    }
}

 /**
  -  @brief  在进行软件定时器数值修改时关闭全局中断来保护变量
  -  @note   None
  -  @param  None
  -  @retval None
 */
static void soft_timer_interrupt_disable(void)
{
    __disable_irq(); // 关闭全局中断
}

/**
 -  @brief  修改完对应软件定时器变量后恢复全局中段
 -  @note   None
 -  @param  None
 -  @retval None
*/
static void soft_timer_interrupt_enable(void)
{
    __enable_irq(); // 开启全局中断
}
