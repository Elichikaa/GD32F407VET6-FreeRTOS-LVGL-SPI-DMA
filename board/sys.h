#ifndef _SYS_H
#define _SYS_H

#include "gd32f4xx.h"


#define BIT_ADDR(byte_offset,bitnum)  (volatile unsigned long*)(0x42000000 + (byte_offset * 32) + (bitnum * 4))

#define GPIOA_OCTL_OFFSET ((GPIOA + 0x14) - 0x40000000)
#define GPIOB_OCTL_OFFSET ((GPIOB + 0x14) - 0x40000000)
#define GPIOC_OCTL_OFFSET ((GPIOC + 0x14) - 0x40000000)
#define GPIOD_OCTL_OFFSET ((GPIOD + 0x14) - 0x40000000)
#define GPIOE_OCTL_OFFSET ((GPIOE + 0x14) - 0x40000000)
#define GPIOF_OCTL_OFFSET ((GPIOF + 0x14) - 0x40000000)
#define GPIOG_OCTL_OFFSET ((GPIOG + 0x14) - 0x40000000)


#define GPIOA_ISTAT_OFFSET ((GPIOA + 0x10) - 0x40000000)
#define GPIOB_ISTAT_OFFSET ((GPIOB + 0x10) - 0x40000000)
#define GPIOC_ISTAT_OFFSET ((GPIOC + 0x10) - 0x40000000)
#define GPIOD_ISTAT_OFFSET ((GPIOD + 0x10) - 0x40000000)
#define GPIOE_ISTAT_OFFSET ((GPIOE + 0x10) - 0x40000000)
#define GPIOF_ISTAT_OFFSET ((GPIOF + 0x10) - 0x40000000)
#define GPIOG_ISTAT_OFFSET ((GPIOG + 0x10) - 0x40000000)

#define PAin(n)     *(BIT_ADDR(GPIOA_ISTAT_OFFSET,n))   // 输入
#define PBin(n)     *(BIT_ADDR(GPIOB_ISTAT_OFFSET,n))   // 输入
#define PCin(n)     *(BIT_ADDR(GPIOC_ISTAT_OFFSET,n))   // 输入
#define PDin(n)     *(BIT_ADDR(GPIOD_ISTAT_OFFSET,n))   // 输入
#define PEin(n)     *(BIT_ADDR(GPIOE_ISTAT_OFFSET,n))   // 输入
#define PFin(n)     *(BIT_ADDR(GPIOF_ISTAT_OFFSET,n))   // 输入
#define PGin(n)     *(BIT_ADDR(GPIOG_ISTAT_OFFSET,n))   // 输入

#define PAout(n)    *(BIT_ADDR(GPIOA_OCTL_OFFSET,n))    // 输出
#define PBout(n)    *(BIT_ADDR(GPIOB_OCTL_OFFSET,n))    // 输出
#define PCout(n)    *(BIT_ADDR(GPIOC_OCTL_OFFSET,n))    // 输出
#define PDout(n)    *(BIT_ADDR(GPIOD_OCTL_OFFSET,n))    // 输出
#define PEout(n)    *(BIT_ADDR(GPIOE_OCTL_OFFSET,n))    // 输出
#define PFout(n)    *(BIT_ADDR(GPIOF_OCTL_OFFSET,n))    // 输出
#define PGout(n)    *(BIT_ADDR(GPIOG_OCTL_OFFSET,n))    // 输出

#endif

