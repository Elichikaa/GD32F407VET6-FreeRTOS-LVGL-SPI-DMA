#include "lcd.h"
#include "stdlib.h"
#include "board.h"	 
#include "spi.h"
#include <string.h>

#define LOG_TAG          "lcd"
#include "elog.h"
	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 

#define X_MAX_PIXEL	        480
#define Y_MAX_PIXEL	        320   

uint8_t SPI_TX_BUFF[X_MAX_PIXEL * 10 * 3];  //宽480，10行
uint8_t SPI_RX_BUFF[10] = {0};

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 data)
{ 
	LCD_CS_CLR;     
	LCD_RS_CLR;	  
	SPIv_WriteData(data);
	LCD_CS_SET;	
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
	LCD_CS_CLR;
	LCD_RS_SET;
	SPIv_WriteData(data);
	LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{	
  	//18Bit	
	LCD_WR_DATA((Data>>8)&0xF8);//RED
	LCD_WR_DATA((Data>>3)&0xFC);//GREEN
	LCD_WR_DATA(Data<<3);//BLUE
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
  unsigned int i,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<lcddev.height;i++)
	{
		for(m=0;m<lcddev.width;m++)
		{	
			Lcd_WriteData_16Bit(Color);
		}
	}
	 LCD_CS_SET;
} 



void SPI_DMAInit(void)
{
	//DMA初始化
	rcu_periph_clock_enable(RCU_DMA_SPI);  
    
	//DMA发送
    dma_single_data_parameter_struct dma_init_SPI_TX={0};

	dma_deinit(RCU_SPI_DMA, RCU_SPI_DMA_TX_CH);      /* SPI0_Tx --> DMA1 Channel3 */
    dma_init_SPI_TX.periph_addr = (uint32_t)(&SPI_DATA(PORT_SPI));  	// 配置外设地址为SPI数据寄存器地址
    dma_init_SPI_TX.periph_inc  = DMA_PERIPH_INCREASE_DISABLE;   	// 禁止外设地址增量
    dma_init_SPI_TX.memory0_addr= NULL;           					// 配置存储器地址
    dma_init_SPI_TX.memory_inc  = DMA_MEMORY_INCREASE_ENABLE;    	// 允许存储器地址增量
    dma_init_SPI_TX.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;  	// 配置外设宽度为8位
    dma_init_SPI_TX.direction   = DMA_MEMORY_TO_PERIPH;            // 配置传输方向为存储器到外设	
    dma_init_SPI_TX.number      = 0;                  	 			// 配置传输数据个数
    dma_init_SPI_TX.priority    = DMA_PRIORITY_ULTRA_HIGH;         // 配置DMA优先级
    dma_init_SPI_TX.circular_mode = DMA_CIRCULAR_MODE_DISABLE; 	// 禁止循环模式
	
    dma_single_data_mode_init(RCU_SPI_DMA, RCU_SPI_DMA_TX_CH, &dma_init_SPI_TX);	// 初始化DMA0的DMA_CH4通道
    dma_channel_subperipheral_select(RCU_SPI_DMA, RCU_SPI_DMA_TX_CH, DMA_SUBPERI3);
	dma_channel_enable(RCU_SPI_DMA, RCU_SPI_DMA_TX_CH);	

	spi_dma_enable(PORT_SPI,SPI_DMA_TRANSMIT);	
	
//	//DMA接收
//	dma_single_data_parameter_struct dma_init_SPI_RX={0};

//	dma_deinit(RCU_SPI_DMA, RCU_SPI_DMA_RX_CH);      /* SPI0_Rx --> DMA1 Channel2 */
//    dma_init_SPI_RX.periph_addr = (uint32_t)(&SPI_DATA(PORT_SPI));  	// 配置外设地址为SPI数据寄存器地址
//    dma_init_SPI_RX.periph_inc  = DMA_PERIPH_INCREASE_DISABLE;   	// 禁止外设地址增量
//    dma_init_SPI_RX.memory0_addr= (uint32_t)SPI_RX_BUFF;       // 配置存储器地址
//    dma_init_SPI_RX.memory_inc  = DMA_MEMORY_INCREASE_DISABLE;    	// 不允许存储器地址增量
//    dma_init_SPI_RX.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;  	// 配置外设宽度为8位
//    dma_init_SPI_RX.direction   = DMA_PERIPH_TO_MEMORY;            // 配置传输方向为外设到存储器	
//    dma_init_SPI_RX.number      = 0;                  	 			// 配置传输数据个数 
//    dma_init_SPI_RX.priority    = DMA_PRIORITY_ULTRA_HIGH;         // 配置DMA优先级
//    dma_init_SPI_RX.circular_mode = DMA_CIRCULAR_MODE_DISABLE; 	// 禁止循环模式
//	
//    dma_single_data_mode_init(RCU_SPI_DMA, RCU_SPI_DMA_RX_CH, &dma_init_SPI_RX);	// 初始化DMA0的DMA_CH4通道
//	dma_channel_subperipheral_select(RCU_SPI_DMA, RCU_SPI_DMA_RX_CH, DMA_SUBPERI3);
//	dma_channel_enable(RCU_SPI_DMA, RCU_SPI_DMA_RX_CH);

//	spi_dma_enable(SPI1,SPI_DMA_RECEIVE);	

	#if USE_SPI_DMA
	//DMA中断
	/* 使能DMA通道中断 */
	nvic_irq_enable(DMA1_Channel3_IRQn, 0, 0); 
	/* 使能DMA中断 */
	dma_interrupt_enable(RCU_SPI_DMA, RCU_SPI_DMA_TX_CH, DMA_CHXCTL_FTFIE);  // 使能传输完成中断
	#endif 
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
    #if USE_HARDWARE_SPI 

	spi_parameter_struct spi_init_struct;
	
      /* 开启各引脚时钟 */
    rcu_periph_clock_enable(RCU_LCD_SCL);
    rcu_periph_clock_enable(RCU_LCD_SDA);
    rcu_periph_clock_enable(RCU_LCD_CS);
    rcu_periph_clock_enable(RCU_LCD_DC);
    rcu_periph_clock_enable(RCU_LCD_RES);
    rcu_periph_clock_enable(RCU_LCD_BLK);
	
      /* 使能SPI */      
    rcu_periph_clock_enable(RCU_SPI_HARDWARE); 

      /* 配置 SPI的SCK GPIO */
    gpio_af_set(PORT_LCD_SCL, LINE_AF_SPI, GPIO_LCD_SCL);
    gpio_mode_set(PORT_LCD_SCL, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_SCL);
    gpio_output_options_set(PORT_LCD_SCL, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_LCD_SCL);
    gpio_bit_set(PORT_LCD_SCL,GPIO_LCD_SCL);
	
      /* 配置 SPI的MOSI  GPIO */
    gpio_af_set(PORT_LCD_SDA, LINE_AF_SPI, GPIO_LCD_SDA);
    gpio_mode_set(PORT_LCD_SDA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_SDA);
    gpio_output_options_set(PORT_LCD_SDA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_LCD_SDA);
    gpio_bit_set(PORT_LCD_SDA, GPIO_LCD_SDA);         

       /* 配置DC */
    gpio_mode_set(PORT_LCD_DC,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_DC);
    gpio_output_options_set(PORT_LCD_DC,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_LCD_DC);
    gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET);
	
       /*  配置RES */
    gpio_mode_set(PORT_LCD_RES,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_RES);
    gpio_output_options_set(PORT_LCD_RES,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_LCD_RES);
    gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET);
	
       /* 配置BLK */
    gpio_mode_set(PORT_LCD_BLK, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_BLK);
    gpio_output_options_set(PORT_LCD_BLK, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_LCD_BLK);
    gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, SET);
	
       /*配置CS */
    gpio_mode_set(PORT_LCD_CS,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_LCD_CS);
    gpio_output_options_set(PORT_LCD_CS,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_LCD_CS);
    gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET);

    /* 配置 SPI 参数 */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;  	// 传输模式全双工
    spi_init_struct.device_mode          = SPI_MASTER;   				// 配置为主机
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;			// 8位数据
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;		// 极性高相位2
    spi_init_struct.nss                  = SPI_NSS_SOFT;  				// 软件cs
    spi_init_struct.prescale             = SPI_PSC_2;					// 2分频
    spi_init_struct.endian               = SPI_ENDIAN_MSB;				// 高位在前	
    spi_init(PORT_SPI, &spi_init_struct);


    /* 使能 SPI */
    spi_enable(PORT_SPI);
      
	#if USE_SPI_DMA  
	SPI_DMAInit();
	#endif
#else
		  /* 使能时钟 */      
	rcu_periph_clock_enable(RCU_LCD_SCL);
	rcu_periph_clock_enable(RCU_LCD_SDA);
	rcu_periph_clock_enable(RCU_LCD_CS);
	rcu_periph_clock_enable(RCU_LCD_DC);
	rcu_periph_clock_enable(RCU_LCD_RES);
	rcu_periph_clock_enable(RCU_LCD_BLK);
	
	
			 /* 配置SCL */
	gpio_mode_set(PORT_LCD_SCL,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_SCL);
	gpio_output_options_set(PORT_LCD_SCL,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_SCL);
	gpio_bit_write(PORT_LCD_SCL, GPIO_LCD_SCL, SET);
	
			 /* 配置SDA */
	gpio_mode_set(PORT_LCD_SDA,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_SDA);
	gpio_output_options_set(PORT_LCD_SDA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_SDA);
	gpio_bit_write(PORT_LCD_SDA, GPIO_LCD_SDA, SET);
	
			 /* 配置DC */
	gpio_mode_set(PORT_LCD_DC,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_DC);
	gpio_output_options_set(PORT_LCD_DC,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_DC);
	gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET);

			 /* 配置CS */
	gpio_mode_set(PORT_LCD_CS,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_CS);
	gpio_output_options_set(PORT_LCD_CS,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_CS);
	gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET);
	
			 /* 配置RES */
	gpio_mode_set(PORT_LCD_RES,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_RES);
	gpio_output_options_set(PORT_LCD_RES,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_RES);
	gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET);

			 /* 配置BLK */
	gpio_mode_set(PORT_LCD_BLK, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_BLK);
	gpio_output_options_set(PORT_LCD_BLK, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_BLK);
	gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, SET);

#endif
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{  
	LCD_GPIOInit();//LCD GPIO初始化	
 	LCD_RESET(); //LCD 复位
//************* ILI9488初始化**********//	
	LCD_WR_REG(0XF7);
	LCD_WR_DATA(0xA9);
	LCD_WR_DATA(0x51);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x82);

	/* Power Control 1 (C0h)  */
	LCD_WR_REG(0xC0);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x09);

	/* Power Control 2 (C1h) */
	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x41);

	/* VCOM Control (C5h)  */
	LCD_WR_REG(0XC5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x80);

	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0xB0);
	LCD_WR_DATA(0x11);

	/* Display Inversion Control (B4h) */
	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x02); 

	/* Display Function Control (B6h)  */
	LCD_WR_REG(0xB6);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x42); //0x42 0X22

	/* Entry Mode Set (B7h)  */
	LCD_WR_REG(0xB7);
	LCD_WR_DATA(0xc6);

	/* HS Lanes Control (BEh) */
	LCD_WR_REG(0xBE);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x04);

	/* Set Image Function (E9h)  */
	LCD_WR_REG(0xE9);
	LCD_WR_DATA(0x00);


	LCD_WR_REG(0x36);
	//LCD_WR_DATA((1<<3)|(0<<7)|(1<<6)|(1<<5));
	LCD_WR_DATA( 0x08 );

	/* Interface Pixel Format (3Ah) */
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x66); //0X55 16BIT/PIXEL

	/* PGAMCTRL (Positive Gamma Control) (E0h) */
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x41);
	LCD_WR_DATA(0x89);
	LCD_WR_DATA(0x4B);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x0F);

	/* NGAMCTRL (Negative Gamma Control) (E1h)  */
	LCD_WR_REG(0XE1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x1A);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x0F);

	LCD_WR_REG(0x11);	//SLEEP OUT
	delay_ms(120);
	LCD_WR_REG(0x29);	//DISPLAY ON
	
	LCD_direction(USE_HORIZONTAL);//设置LCD显示方向
	LCD_LED_SET; //点亮背光
	LCD_Clear(WHITE);//清全屏白色
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//开始写入GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
	}		
	
}	 

/*******************************************************************
 * @name       :void GUI_DrawPoint(u16 x,u16 y,u16 color)
 * @date       :2018-08-09 
 * @function   :draw a point in LCD screen
 * @parameters :x:the x coordinate of the point
                y:the y coordinate of the point
								color:the color value of the point
 * @retvalue   :None
********************************************************************/
void GUI_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(color); 
}

/*******************************************************************
 * @name       :void LVGL_LCD_Fill((u16 sx,u16 sy,u16 ex,u16 ey,u16* color)
 * @date       :2024-06-06
 * @function   :use LVGL fill the specified area
 * @parameters :sx:the bebinning x coordinate of the specified area
 *                sy:the bebinning y coordinate of the specified area
 *                ex:the ending x coordinate of the specified area
 *                ey:the ending y coordinate of the specified area
 *                color:the filled color value
 * @retvalue   :None				
********************************************************************/
void LVGL_LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,uint16_t* color)
{
	u16 width=ex-sx+1; 	
	u16 height=ey-sy+1;		
	LCD_SetWindows(sx,sy,ex,ey);

	u32 pixelNum = width*height;

	uint32_t j = 0;
	for(uint32_t i = 0; i < pixelNum; i++) 
	{
		SPI_TX_BUFF[j++] = ((*color>>8)&0xF8);//RED
		SPI_TX_BUFF[j++] = ((*color>>3)&0xFC);//GREEN
		SPI_TX_BUFF[j++] = (*color<<3);//BLUE

		color++;
	}

	LCD_CS_CLR; 
	LCD_RS_SET; 

	#if !USE_SPI_DMA
	
	for(int i = 0;i < j;i++)
	{
		SPIv_WriteData(SPI_TX_BUFF[i]);
	}	
	
	LCD_CS_SET;
	#else
 
	// 配置发送DMA通道 
	dma_channel_disable(RCU_SPI_DMA, RCU_SPI_DMA_TX_CH);
	dma_flag_clear(RCU_SPI_DMA, RCU_SPI_DMA_TX_CH, DMA_FLAG_FTF);	
	dma_memory_address_config(RCU_SPI_DMA, RCU_SPI_DMA_TX_CH, DMA_MEMORY_0,(uint32_t)SPI_TX_BUFF);
	dma_transfer_number_config(RCU_SPI_DMA, RCU_SPI_DMA_TX_CH, j);	
	dma_channel_enable(RCU_SPI_DMA, RCU_SPI_DMA_TX_CH);	

//	// 配置接收DMA通道
//    dma_channel_disable(RCU_SPI_DMA, RCU_SPI_DMA_RX_CH);  
//    dma_flag_clear(RCU_SPI_DMA, RCU_SPI_DMA_RX_CH, DMA_FLAG_FTF); 
//    dma_memory_address_config(RCU_SPI_DMA, RCU_SPI_DMA_RX_CH, DMA_MEMORY_0, (uint32_t)SPI_RX_BUFF);
//    dma_transfer_number_config(RCU_SPI_DMA, RCU_SPI_DMA_RX_CH, j);
//    dma_channel_enable(RCU_SPI_DMA, RCU_SPI_DMA_RX_CH); 
		
	//spi_dma_enable(SPI1,SPI_DMA_TRANSMIT);	
	// while(!dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF) || !dma_flag_get(DMA0, DMA_CH3, DMA_FLAG_FTF));	
	// dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_FTF); 	
	// dma_flag_clear(DMA0, DMA_CH3, DMA_FLAG_FTF); 			
	
	// dma_channel_disable(DMA0, DMA_CH4);	
	// dma_channel_disable(DMA0, DMA_CH3);	
	// spi_dma_disable(SPI1,SPI_DMA_TRANSMIT);
	
	#endif
}



/*******************************************************************
 * @name       :void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16* color)
 * @date       :2018-08-09 
 * @function   :fill the specified area
 * @parameters :sx:the bebinning x coordinate of the specified area
                sy:the bebinning y coordinate of the specified area
								ex:the ending x coordinate of the specified area
								ey:the ending y coordinate of the specified area
								color:the filled color value
 * @retvalue   :None
********************************************************************/
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16* color)
{  	
	u16 height, width;
	width = ex - sx + 1;            
    height = ey - sy + 1;           
	LCD_SetWindows(sx,sy,ex,ey);
	for(uint32_t y = 0; y <width*height; y++) 
	{
		Lcd_WriteData_16Bit(*color);
		color++;
	}	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
	
	 
}

