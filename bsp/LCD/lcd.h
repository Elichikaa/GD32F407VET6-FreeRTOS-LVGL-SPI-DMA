#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h"

#include "gd32f4xx.h"
#ifndef u8
#define u8 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u32
#define u32 uint32_t
#endif

#define USE_HARDWARE_SPI	1	// 使用软件SPI还是硬件SPI 1为硬件，0为软件 
#define USE_SPI_DMA			1	// 是否使用DMA 

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
#define USE_HORIZONTAL  	 1//定义液晶屏顺时针旋转方向 	0->0度旋转，1->90度旋转，2->180度旋转，3->270度旋转

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD_W 320
#define LCD_H 480

//TFTLCD部分外要调用的函数		   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 

//#define LED  13        //背光控制引脚
//#define CS   15       //片选引脚
//#define RS   14       //寄存器/数据选择引脚  
//#define RST  12       //复位引脚

//QDtech全系列模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度
//#define	LCD_LED PBout(LED) //LCD背光    		 
//#define LCD_CS  PBout(CS)
//#define LCD_RS  PBout(RS)
//#define LCD_RST PBout(RST)
//如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法
//以下IO定义直接操作寄存器，快速IO操作，刷屏速率可以达到28帧每秒！ 

//#define	LCD_CS_SET  LCD_CS=1 //GPIO_TYPE->BSRRL=1<<LCD_CS    //片选端口  	PB11
//#define	LCD_RS_SET	LCD_RS=1 //GPIO_TYPE->BSRRL=1<<LCD_RS    //数据/命令  PB10	  
//#define	LCD_RST_SET	LCD_RST=1 //GPIO_TYPE->BSRRL=1<<LCD_RST    //复位			PB12

// 							    
//#define	LCD_CS_CLR  LCD_CS=0 //GPIO_TYPE->BSRRH=1<<LCD_CS     //片选端口  	PB11
//#define	LCD_RS_CLR	LCD_RS=0 //GPIO_TYPE->BSRRH=1<<LCD_RS     //数据/命令  PB10	 
//#define	LCD_RST_CLR	LCD_RST=0 //GPIO_TYPE->BSRRH=1<<LCD_RST    //复位			  PB12


//液晶控制口置1操作语句宏定义
#define        LCD_CS_SET          gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET)
#define        LCD_RS_SET          gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET) 
#define        LCD_SDA_SET          gpio_bit_write(PORT_LCD_SDA, GPIO_LCD_SDA, SET)
#define        LCD_SCL_SET          gpio_bit_write(PORT_LCD_SCL, GPIO_LCD_SCL, SET)
#define        LCD_RST_SET          gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET)
#define        LCD_LED_SET          gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, SET)

//液晶控制口置0操作语句宏定义
#define        LCD_CS_CLR          gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, RESET)//CS 
#define        LCD_RS_CLR          gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, RESET)//DC
#define        LCD_SDA_CLR          gpio_bit_write(PORT_LCD_SDA, GPIO_LCD_SDA, RESET)//SDA=MOSI    
#define        LCD_SCL_CLR          gpio_bit_write(PORT_LCD_SCL, GPIO_LCD_SCL, RESET)//SCL=SCLK
#define        LCD_RST_CLR          gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, RESET)//RES
#define        LCD_LED_CLR          gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, RESET)


//-----------------LCD端口移植---------------- 
//VCC - 3.3V
//SCL - PA5 	SPI0_SCK
//SDA - PA7 	SPI0_MOSI
//RES - PD12	(可以接入复位)
//DC  - PD11
//CS  - PB12    SPI1_NSS
//BLK - PD10
#define RCU_LCD_SCL     RCU_GPIOA//SCK
#define PORT_LCD_SCL    GPIOA
#define GPIO_LCD_SCL    GPIO_PIN_5

#define RCU_LCD_SDA     RCU_GPIOA//MOSI
#define PORT_LCD_SDA    GPIOA
#define GPIO_LCD_SDA    GPIO_PIN_7

#define RCU_LCD_MIS     RCU_GPIOA//MIS0
#define PORT_LCD_MIS    GPIOA
#define GPIO_LCD_MIS    GPIO_PIN_6

#define RCU_LCD_CS      RCU_GPIOB//NSS
#define PORT_LCD_CS     GPIOB
#define GPIO_LCD_CS     GPIO_PIN_12

#define RCU_LCD_DC      RCU_GPIOD //DC
#define PORT_LCD_DC     GPIOD
#define GPIO_LCD_DC     GPIO_PIN_11

#define RCU_LCD_RES     RCU_GPIOD//RES
#define PORT_LCD_RES    GPIOD
#define GPIO_LCD_RES    GPIO_PIN_12

#define RCU_LCD_BLK     RCU_GPIOD//BLK/LED
#define PORT_LCD_BLK    GPIOD
#define GPIO_LCD_BLK    GPIO_PIN_10

#define RCU_LCD_CS2      RCU_GPIOB//CS2/T_CS
#define PORT_LCD_CS2     GPIOB
#define GPIO_LCD_CS2     GPIO_PIN_7

#define RCU_LCD_PEN      RCU_GPIOB //PEN/T_IRQ
#define PORT_LCD_PEN     GPIOB
#define GPIO_LCD_PEN     GPIO_PIN_4

#define RCU_LCD_TCLK      RCU_GPIOB //T_CLK
#define PORT_LCD_TCLK     GPIOB
#define GPIO_LCD_TCLK     GPIO_PIN_8

#define RCU_LCD_TDIN      RCU_GPIOB //T_DIN
#define PORT_LCD_TDIN     GPIOB
#define GPIO_LCD_TDIN     GPIO_PIN_6

#define RCU_LCD_TD0       RCU_GPIOB //T_DO
#define PORT_LCD_TD0      GPIOB
#define GPIO_LCD_TD0      GPIO_PIN_5

#define RCU_SPI_HARDWARE RCU_SPI0
#define PORT_SPI         SPI0
#define LINE_AF_SPI      GPIO_AF_5
			
#define RCU_DMA_SPI				RCU_DMA1
#define RCU_SPI_DMA				DMA1
#define RCU_SPI_DMA_TX_CH		DMA_CH3
#define RCU_SPI_DMA_RX_CH		DMA_CH2


#define	SPI_DMA_IRQHandler		DMA1_Channel3_IRQHandler


//画笔颜色
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //棕色
#define BRRED 			0XFC07 //棕红色
#define GRAY  			0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X841F //浅绿色
#define LIGHTGRAY     0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(u16 Color);	 
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);//画点
u16  LCD_ReadPoint(u16 x,u16 y); //读点
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);

u16 LCD_RD_DATA(void);//读取LCD数据									    
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_DATA(u8 data);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
u16 LCD_ReadRAM(void);		   
u16 LCD_BGR2RGB(u16 c);
void LCD_SetParam(void);
void Lcd_WriteData_16Bit(u16 Data);
void LCD_direction(u8 direction );
void GUI_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16* color);
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16* color);
void Lcd_Refrsh_DMA(void);
void LVGL_SPI1_DMAInit(u32* buf,u16 buf_size);
void lvgl_lcd_init(void);
void LVGL_LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,uint16_t* color);
//如果仍然觉得速度不够快，可以使用下面的宏定义,提高速度.
//注意要去掉lcd.c中void LCD_WR_DATA(u16 data)函数定义哦
/*
#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	DATAOUT(data<<8);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
	#else//使用16位并行数据总线模式
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	} 	
#endif
*/
				  		 
#endif  
	 
	 



