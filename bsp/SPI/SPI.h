#include "sys.h"

#ifndef _SPI_H_
#define _SPI_H_

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


//#define SCLK        3	//PB13--->>TFT --SCL/SCK
//#define MISO        4	
//#define MOSI        5	//PB15 MOSI--->>TFT --SDA/DIN

//#define SPI_MOSI  PBout(MOSI)
//#define SPI_SCLK  PBout(SCLK)
//#define SPI_MISO  PBin(MISO)

//////Һ�����ƿ���1�������궨��

//#define	SPI_MOSI_SET  	SPI_MOSI=1 //LCD_CTRL->BSRR=SPI_MOSI    
//#define	SPI_SCLK_SET  	SPI_SCLK=1 //LCD_CTRL->BSRR=SPI_SCLK    


//////Һ�����ƿ���0�������궨��

//#define	SPI_MOSI_CLR  	SPI_MOSI=0 //LCD_CTRL->BRR=SPI_MOSI    
//#define	SPI_SCLK_CLR  	SPI_SCLK=0 //LCD_CTRL->BRR=SPI_SCLK    

void  SPIv_WriteData(u8 Data);


#endif
