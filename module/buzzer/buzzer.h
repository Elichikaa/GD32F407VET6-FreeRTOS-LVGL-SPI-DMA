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

/**
 * @file buzzer.h
 * @brief 为无源蜂鸣器实现蜂鸣器驱动程序.
 */

#ifndef __BUZZER_H
#define __BUZZER_H

#include "board.h"


typedef struct
{
    uint16_t tone_freq; // 鸣叫频率
    uint8_t volume;     // 音量百分比
    uint32_t on_time;   // 鸣叫开始时间
    uint32_t off_time;  // 鸣叫停止时间
} buzzer_data_t;

void buzzer_init(void);
void buzzer_poll(void);
void buzzer_beep_tone(uint16_t _tone_freq, uint8_t _volume, uint32_t _on_time,
                      uint32_t _off_time);
void buzzer_beep_immediate(uint32_t _on_time);
uint8_t is_buzzer_beeping(void);

#endif // __BUZZER_H

