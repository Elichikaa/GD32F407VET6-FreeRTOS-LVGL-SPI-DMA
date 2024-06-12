#ifndef __OLED_H
#define __OLED_H 

#include "gd32f4xx.h"
#include "stdlib.h"	

#define USE_SOFT_HARD_SPI 1 // 设置使用软件SPI还是硬件SPI，1为软件SPI 0为硬件SPI


#ifndef u8
#define u8 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u32
#define u32 uint32_t
#endif

//-----------------测试LED端口定义---------------- 

//#define LED_ON GPIO_ResetBits(GPIOC,GPIO_Pin_12)
//#define LED_OFF GPIO_SetBits(GPIOC,GPIO_Pin_12)


//-----------------OLED端口移植---------------- 
//VCC - 3.3V
//SCL - PB10 SPI1_SCK
//SDA - PC3 SPI1_MOSI
//RES - PB0(可以接入复位)
//DC  - PB1
//CS  - PB12        SPI1_NSS
#define RCU_LCD_D0     RCU_GPIOB//SCK
#define PORT_LCD_D0    GPIOB
#define GPIO_LCD_D0    GPIO_PIN_10

#define RCU_LCD_D1     RCU_GPIOC//MOSI
#define PORT_LCD_D1    GPIOC
#define GPIO_LCD_D1    GPIO_PIN_3

#define RCU_LCD_CS      RCU_GPIOB//NSS
#define PORT_LCD_CS     GPIOB
#define GPIO_LCD_CS     GPIO_PIN_12

#define RCU_LCD_DC      RCU_GPIOB //DC
#define PORT_LCD_DC     GPIOB
#define GPIO_LCD_DC     GPIO_PIN_1

#define RCU_LCD_RES     RCU_GPIOB//RES
#define PORT_LCD_RES    GPIOB
#define GPIO_LCD_RES    GPIO_PIN_0

#define RCU_SPI_HARDWARE RCU_SPI1
#define PORT_SPI         SPI1
#define LINE_AF_SPI      GPIO_AF_5

//-----------------OLED端口定义---------------- 

#define OLED_SCL_Clr() gpio_bit_write(PORT_LCD_D0, GPIO_LCD_D0, RESET)//SCL=SCLK
#define OLED_SCL_Set() gpio_bit_write(PORT_LCD_D0, GPIO_LCD_D0, SET)

#define OLED_SDA_Clr() gpio_bit_write(PORT_LCD_D1, GPIO_LCD_D1, RESET)//SDA=MOSI
#define OLED_SDA_Set()  gpio_bit_write(PORT_LCD_D1, GPIO_LCD_D1, SET)

#define OLED_RES_Clr()  gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, RESET)//RES
#define OLED_RES_Set()  gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET)

#define OLED_DC_Clr()   gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, RESET)//DC
#define OLED_DC_Set()   gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET)
                       
#define OLED_CS_Clr()   gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, RESET)//CS
#define OLED_CS_Set()   gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);

#endif

