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
 * @file buzzer.c
 * @brief 实现无源蜂鸣器的非阻塞蜂鸣器驱动程序.
 */

#include "buzzer.h"
#include "bsp_beep.h"

#define MAX_BUZZER_DATA_SIZE 20
/**
 * @brief 定义蜂鸣器鸣叫数据，等待buzzer_poll获取鸣叫信息，它就是一个数组，乞丐版FIFO
 */
static buzzer_data_t buzzer_beep_data[MAX_BUZZER_DATA_SIZE];
/**
 * @brief 蜂鸣器鸣叫数据写入位置
 */
static uint8_t buzzer_beep_data_write_pos = 0;
/**
 * @brief 蜂鸣器鸣叫数据读出位置
 */
static uint8_t buzzer_beep_data_read_pos = 0;

/**
 * @brief 蜂鸣器现在有没有在叫啊，0是没在叫，1是在叫
 */
static uint8_t _is_buzzer_beeping = 0;
/**
 * @brief 蜂鸣器立马开始鸣叫的结束时间，和系统时间(sysytick)做对比
 */
static uint32_t immediate_buzzer_end_time = 0;

/**
 * @brief 在这里加入实际控制蜂鸣器发声的函数
 *
 * @param _tone_freq 音频频率
 * @param _volume 音量
 */
static void buzzer_beep_port(uint16_t _tone_freq, uint8_t _volume)
{
    // 对无源蜂鸣器来说，占空比为50%时音量最大
    //_tone_freq或者_volume为0时蜂鸣器就是静音状态
    // TODO 实际要调用的蜂鸣器发声函数
    if ((_tone_freq == 0) || (_volume == 0))
    {
        // 停止发声
        buzzer_beep_set(_tone_freq, _volume);
    }
    else
    {
        // 赋值并开始发出声音
        buzzer_beep_set(_tone_freq, _volume);
    }
}

/**
 * @brief 初始化蜂鸣器
 *
 */
void buzzer_init(void)
{
    _is_buzzer_beeping = 0;
    immediate_buzzer_end_time = 0;
    beep_init();
}

/**
 * @brief 需要被定时10ms执行，非阻塞
 */
void buzzer_poll(void)
{
    static uint32_t buzzer_beep_on_time = 0;
    static uint32_t buzzer_beep_off_time = 0;
    static uint32_t current_tick = 0;
    static uint8_t inter_beeping_flag = 0;
    current_tick = get_system_tick();
    if (immediate_buzzer_end_time != 0)
    {
        // 处理立即鸣叫的变量，先判断是不是需要现在立马鸣叫
        if (immediate_buzzer_end_time >= current_tick)
        {
            // 没在鸣叫就开始鸣叫
            if (_is_buzzer_beeping == 0)
            {
                buzzer_beep_port(4000, 50);
                _is_buzzer_beeping = 1;
            }
        }
        else
        {
            immediate_buzzer_end_time = 0;
            buzzer_beep_port(0, 0);
            _is_buzzer_beeping = 0;
        }
    }
    // 如果现在不需要立即鸣叫
    if (immediate_buzzer_end_time < current_tick)
    {
        // 如果蜂鸣器鸣叫数据还没开始写入数组，就直接返回
        if (buzzer_beep_data_write_pos == 0)
        {
            return;
        }
        // 如果没在鸣叫
        if (inter_beeping_flag == 0)
        {
            buzzer_beep_on_time =
                buzzer_beep_data[buzzer_beep_data_read_pos].on_time
                + current_tick;
            buzzer_beep_off_time =
                buzzer_beep_data[buzzer_beep_data_read_pos].off_time
                + buzzer_beep_on_time;
            inter_beeping_flag = 1;
        }
        if (current_tick < buzzer_beep_on_time)
        {
            // TODO:可以在这里加入静音功能
            if (_is_buzzer_beeping == 0)
            {
                buzzer_beep_port(
                    buzzer_beep_data[buzzer_beep_data_read_pos].tone_freq,
                    buzzer_beep_data[buzzer_beep_data_read_pos].volume);
            }
            _is_buzzer_beeping = 1;
        }
        else
        {
            // 停止鸣叫
            buzzer_beep_port(0, 0);
            _is_buzzer_beeping = 0;
        }
        if (current_tick <= buzzer_beep_off_time)
        {
            return;
        }
        inter_beeping_flag = 0;
        buzzer_beep_data_read_pos++;
        if (buzzer_beep_data_read_pos >= MAX_BUZZER_DATA_SIZE)
        {
            buzzer_beep_data_read_pos = 0;
            buzzer_beep_data_write_pos = 0;
        }
    }
}


/**
 * @brief 控制蜂鸣器鸣叫的函数，非阻塞，写入数组后里面出来
 *
 * @param _tone_freq 蜂鸣器频率
 * @param _volume 蜂鸣器音量
 * @param _on_time 蜂鸣器一个周期内的鸣叫时间
 * @param _off_time 蜂鸣器一个周期内不鸣叫时间
 */
void buzzer_beep_tone(uint16_t _tone_freq, uint8_t _volume, uint32_t _on_time,
                      uint32_t _off_time)
{
    if (buzzer_beep_data_write_pos < MAX_BUZZER_DATA_SIZE)
    {
        buzzer_beep_data[buzzer_beep_data_write_pos].tone_freq = _tone_freq;
        buzzer_beep_data[buzzer_beep_data_write_pos].volume = _volume;
        buzzer_beep_data[buzzer_beep_data_write_pos].on_time = _on_time;
        buzzer_beep_data[buzzer_beep_data_write_pos].off_time = _off_time;
        buzzer_beep_data_write_pos++;
    }
}

/**
 * @brief 立即发出蜂鸣音，不进入数组，用于紧急情况
 *
 * @param _on_time 持续时间时间
 */
void buzzer_beep_immediate(uint32_t _on_time)
{
    immediate_buzzer_end_time = _on_time + get_system_tick();
}

/**
 * @brief 检查蜂鸣器是否在鸣叫
 *
 * @return uint8_t 0表示没有在鸣叫，1表示在鸣叫
 */
uint8_t is_buzzer_beeping(void) { return _is_buzzer_beeping; }
