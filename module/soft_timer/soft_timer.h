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
#ifndef __SOFT_TIMER_H__
#define __SOFT_TIMER_H__

#include "board.h"

// 定义软件定时器的枚举类型
typedef enum
{
    SOFT_TIMER_0,
    SOFT_TIMER_1,
    SOFT_TIMER_2,
    SOFT_TIMER_3,
    SOFT_TIMER_4,
    SOFT_TIMER_5,
    SOFT_TIMER_6,
    SOFT_TIMER_7,
    SOFT_TIMER_8,
    SOFT_TIMER_9,
    SOFT_TIMER_MAX // 定时器的最大数量
} soft_timer_type;

// 定义软件定时器的结构体
typedef struct
{
    volatile uint32_t counter;   // 计数器
    volatile uint32_t timeout;   // 超时时间
    volatile uint8_t is_timeout; // 超时标志
    volatile uint8_t is_repeat;  // 是否重复计数标志(为1是重复定时器)
} software_timer;


void soft_timer_single_init(soft_timer_type timer, uint32_t timeout);
void soft_timer_repeat_init(soft_timer_type timer, uint32_t timeout);
void soft_timer_start(soft_timer_type timer);
void soft_timer_stop(soft_timer_type timer);
uint8_t soft_timer_is_timeout(soft_timer_type timer);
void soft_timer_reset(soft_timer_type timer);
void soft_timer_tick(void);
static void soft_timer_interrupt_disable(void);
static void soft_timer_interrupt_enable(void);

#endif // __SOFT_TIMER_H__
