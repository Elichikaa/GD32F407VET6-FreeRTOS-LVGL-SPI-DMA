#include "spi.h"
#include "lcd.h"

#define LOG_TAG          "spi"
#include "elog.h"

/*****************************************************************************
 * @name       :void  SPIv_WriteData(u8 Data)
 * @date       :2018-08-09 
 * @function   :Write a byte of data using STM32's Software SPI
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/
void  SPIv_WriteData(u8 Data)
{
#if USE_HARDWARE_SPI 
	
    while(RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_TBE));
    spi_i2s_data_transmit(PORT_SPI, Data);
    while(RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_RBNE));
    spi_i2s_data_receive(PORT_SPI);  

#else    
    unsigned char i=0;
        for(i=8;i>0;i--)
        {
          if(Data&0x80)        
          LCD_SDA_SET; //�������
      else LCD_SDA_CLR;
           
      LCD_SCL_CLR;       
      LCD_SCL_SET;
      Data<<=1; 
        }
#endif
}
