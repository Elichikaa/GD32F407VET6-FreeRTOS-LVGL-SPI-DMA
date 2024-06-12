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
 * @file ring_buffer.c
 * @brief 环形缓冲区实现
 */

#include "ringbuffer.h"

/**
 * @brief 初始化环形缓冲区
 * @param rb 指向环形缓冲区的指针
 * @param buffer 缓冲区
 * @param size 缓冲区大小
 */
void ring_buffer_init(ring_buffer_t *rb, uint8_t *buffer, uint16_t size)
{
    rb->buffer = buffer;
    rb->size = size;
    rb->write_index = 0;
    rb->read_index = 0;
    rb->mirror = 0;
}

/**
 * @brief 从环形缓冲区中读取数据
 * @param rb 指向环形缓冲区的指针
 * @param buffer 用于存储读取数据的缓冲区
 * @param size 要读取的字节数
 * @return 已读取的字节数
 */
uint16_t ring_buffer_read(ring_buffer_t *rb, uint8_t *buffer, uint16_t size)
{
    uint16_t i;
    for (i = 0; i < size && rb->read_index != rb->write_index; i++)
    {
        buffer[i] = rb->buffer[rb->read_index];
        rb->read_index = (rb->read_index + 1) % rb->size;
        if (rb->read_index == 0)
        {
            rb->mirror = ~rb->mirror;
        }
    }
    return i;
}

/**
 * @brief 向环形缓冲区中写入数据，如果缓冲区满，则新数据覆盖最旧的数据
 * @param rb 指向环形缓冲区的指针
 * @param data 要写入的数据
 * @param size 要写入的字节数
 * @return 已写入的字节数
 */
uint16_t ring_buffer_write(ring_buffer_t *rb, const uint8_t *data,
                           uint16_t size)
{
    uint16_t i;
    uint16_t next_write_index;
    for (i = 0; i < size; i++)
    {
        next_write_index = (rb->write_index + 1) % rb->size;
        if ((next_write_index == rb->read_index)
            && ((rb->mirror & 0x8000) == (rb->write_index & 0x8000)))
        {
            rb->read_index = (rb->read_index + 1) % rb->size;
            if (rb->read_index == 0)
            {
                rb->mirror = ~rb->mirror;
            }
        }
        rb->buffer[rb->write_index] = data[i];
        rb->write_index = next_write_index;
        if (rb->write_index == 0)
        {
            rb->mirror = ~rb->mirror;
        }
    }
    return i;
}

/**
 * @brief 获取环形缓冲区中当前待读取的数据大小
 * @param rb 指向环形缓冲区的指针
 * @return 待读取的数据大小（字节数）
 */
uint16_t ring_buffer_available_read(const ring_buffer_t *rb)
{
    uint16_t temp = rb->write_index;
    if (rb->read_index > temp)
    {
        temp += rb->size;
    }
    return (temp - rb->read_index);
}

/**
 * @brief 重置环形缓冲区
 * @param rb 指向环形缓冲区的指针
 */
void ring_buffer_reset(ring_buffer_t *rb)
{
    rb->write_index = 0;
    rb->read_index = 0;
    rb->mirror = 0;
}
