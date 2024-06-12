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

#include "bsp_uart.h"
#include <stdio.h>

#include "stdarg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define BSP_DMA_RCU               RCU_DMA1  // DMA时钟
#define BSP_DMA                   DMA1         // DMA
#define BSP_DMA_RX_CH             DMA_CH5     // DMA通道 
#define BSP_DMA_TX_CH             DMA_CH7     // DMA通道 

#define BSP_DMA_CH_IRQ               DMA1_Channel5_IRQn  // DMA中断
#define BSP_DMA_CH_IRQHandler       DMA1_Channel5_IRQHandler  // DMA中断服务函数


#define BSP_USART                            USART0
#define BSP_USART_RATE                        115200U    // 波特率

#define  ARRAYNUM(arr_name)   (uint32_t)(sizeof(arr_name) / sizeof(*(arr_name)))  //求传进来的数组的长度


#define BUFFER_SIZE 1024  //Receive datas size
static uint8_t rx_buffer[BUFFER_SIZE]={0};
static uint8_t tx_buffer[BUFFER_SIZE]={0};

//对应.c文件的变量
//extern DMA_HandleTypeDef hdma_usart1_rx;
//extern osThreadId_t usartRxTaskHandle;//usart任务句柄
//extern osSemaphoreId_t usartRxBinarySemHandle;//二值信号量

UART_RxData_t rx_datas = {  //define receive data
   .rx_len = 0,
   .rx_buf = rx_buffer
};


//若在中断__get_IPSR()返回1，否则返回0
static int inHandlerMode(void)
{
	return __get_IPSR();
}
 
//串口1DMA发送函数
void uart1_printf(const char *format,...)
{
	
	uint32_t length;
	
	if(inHandlerMode() != 0) {
		taskDISABLE_INTERRUPTS();//如果在中断中则禁止所有中断
	}
	else {
		while(usart_flag_get(USART0, USART_FLAG_BSY) == SET) {
			taskYIELD();//如果串口忙则挂起该任务
		}
	}

	va_list args;
	va_start(args, format);
	length = vsnprintf((char*)tx_buffer, sizeof(tx_buffer)+1, (char*)format, args);
	va_end(args);
	dma_transfer_number_config(BSP_DMA, BSP_DMA_TX_CH, length);
	
	if(inHandlerMode() != 0) {
		taskENABLE_INTERRUPTS();//使能中断
	}	
}

//osStatus osSemaphoreRelease (osSemaphoreId semaphore_id)
//{
//  osStatus result = osOK;
//  portBASE_TYPE taskWoken = pdFALSE;


//  if (inHandlerMode()) {
//    if (xSemaphoreGiveFromISR(semaphore_id, &taskWoken) != pdTRUE) {
//      return osErrorOS;
//    }
//    portEND_SWITCHING_ISR(taskWoken);
//  }
//  else {
//    if (xSemaphoreGive(semaphore_id) != pdTRUE) {
//      result = osErrorOS;
//    }
//  }

//  return result;
//}

   #define __HAL_DMA_GET_COUNTER(__HANDLE__) ((__HANDLE__)->Instance->CNDTR);

//中断处理函数
//void USART1_ReceiveIDLE(void)
//{
//	if(RESET != usart_flag_get(USART0, USART_FLAG_IDLE)) {
//		//usart_flag_clear(USART0, );//清除标志位
//		//HAL_UART_DMAStop(&huart1);//停止DMA
//		usart_dma_receive_config(BSP_USART, USART_RECEIVE_DMA_DISABLE);
//		rx_datas.rx_len = BUFFER_SIZE - dma_transfer_number_get(BSP_DMA, BSP_DMA_RX_CH);//数据长度
//		usart_dma_receive_config(BSP_USART, USART_RECEIVE_DMA_ENABLE);//开启DMA
//		xSemaphoreGiveFromISR(usartRxBinarySemHandle);//释放二值信号量
//		
//	}
//}




// clang-format off
static const uint32_t COM_UART[COM_UART_MAX_NUM] =
{
COM_UART0,
COM_UART1,
COM_UART2
};
static const IRQn_Type COM_UART_IRQn[COM_UART_MAX_NUM] =
{
COM_UART0_IRQn,
COM_UART1_IRQn,
COM_UART2_IRQn
};
static const rcu_periph_enum COM_UART_CLK[COM_UART_MAX_NUM] =
{
COM_UART0_CLK,
COM_UART1_CLK,
COM_UART2_CLK,
};
static const rcu_periph_enum COM_UART_TX_GPIO_CLK[COM_UART_MAX_NUM] =
{
COM_UART0_TX_GPIO_CLK,
COM_UART1_TX_GPIO_CLK,
COM_UART2_TX_GPIO_CLK
};
static const uint32_t COM_UART_TX_GPIO_AF[COM_UART_MAX_NUM] =
{
COM_UART0_TX_GPIO_AF,
COM_UART1_TX_GPIO_AF,
COM_UART2_TX_GPIO_AF
};
static const uint32_t COM_UART_TX_PORT[COM_UART_MAX_NUM] =
{
COM_UART0_TX_PORT,
COM_UART1_TX_PORT,
COM_UART2_TX_PORT
};
static const uint32_t COM_UART_TX_PIN[COM_UART_MAX_NUM] =
{
COM_UART0_TX_PIN,
COM_UART1_TX_PIN,
COM_UART2_TX_PIN
};
static const rcu_periph_enum COM_UART_RX_GPIO_CLK[COM_UART_MAX_NUM] =
{
COM_UART0_RX_GPIO_CLK,
COM_UART1_RX_GPIO_CLK,
COM_UART2_RX_GPIO_CLK
};
static const uint32_t COM_UART_RX_GPIO_AF[COM_UART_MAX_NUM] =
{
COM_UART0_RX_GPIO_AF,
COM_UART1_RX_GPIO_AF,
COM_UART2_RX_GPIO_AF
};
static const uint32_t COM_UART_RX_PORT[COM_UART_MAX_NUM] =
{
COM_UART0_RX_PORT,
COM_UART1_RX_PORT,
COM_UART2_RX_PORT
};
static const uint32_t COM_UART_RX_PIN[COM_UART_MAX_NUM] =
{
COM_UART0_RX_PIN,
COM_UART1_RX_PIN,
COM_UART2_RX_PIN
};

static const uint32_t COM_UART_BAUD[COM_UART_MAX_NUM] =
{
COM_UART0_BAUD,
COM_UART1_BAUD,
COM_UART2_BAUD
};

// clang-format on

static void bsp_uart_periph_init(uart_type_def _com_uart)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable( COM_UART_TX_GPIO_CLK[_com_uart]);
    rcu_periph_clock_enable( COM_UART_RX_GPIO_CLK[_com_uart]);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_UART_CLK[_com_uart]);

    /* connect port to USARTx_Tx */
    gpio_af_set(COM_UART_TX_PORT[_com_uart], COM_UART_TX_GPIO_AF[_com_uart], COM_UART_TX_PIN[_com_uart]);

    /* connect port to USARTx_Rx */
    gpio_af_set(COM_UART_RX_PORT[_com_uart], COM_UART_RX_GPIO_AF[_com_uart], COM_UART_RX_PIN[_com_uart]);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(COM_UART_TX_PORT[_com_uart], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_UART_TX_PIN[_com_uart]);
    gpio_output_options_set(COM_UART_TX_PORT[_com_uart], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_UART_TX_PIN[_com_uart]);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(COM_UART_RX_PORT[_com_uart], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_UART_RX_PIN[_com_uart]);
    gpio_output_options_set(COM_UART_RX_PORT[_com_uart], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_UART_RX_PIN[_com_uart]);

    /* USART configure */
    usart_deinit(COM_UART[_com_uart]);
    usart_baudrate_set(COM_UART[_com_uart],COM_UART_BAUD[_com_uart]);
    usart_receive_config(COM_UART[_com_uart], USART_RECEIVE_ENABLE);
    usart_transmit_config(COM_UART[_com_uart], USART_TRANSMIT_ENABLE);
    usart_enable(COM_UART[_com_uart]);

    /* USART interrupt configuration */
    if (_com_uart == COM_UART_NUM0)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 2, 0);
    }
    else if (_com_uart == COM_UART_NUM1)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 2, 0);
    }
    else if (_com_uart == COM_UART_NUM2)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 0, 0);
    }
    else if (_com_uart == COM_UART_NUM5)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 1, 0);
    }
    else
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 2, 0);
    }
}

void bsp_uart_init(void)
{
//    for (uart_type_def _uart = COM_UART_NUM0; _uart < COM_UART_MAX_NUM; _uart++)
//    {
//        bsp_uart_periph_init(_uart);
//    }
	
	bsp_uart_periph_init(COM_UART_NUM0); //初始化串口0
	
	/* 开启时钟 */
	rcu_periph_clock_enable(BSP_DMA_RCU);
	
	dma_single_data_parameter_struct dma_init_struct;
	
	/* 初始化RX-DMA通道  */    
	dma_deinit(BSP_DMA, BSP_DMA_RX_CH);    // 配置DMA初始化参数     
	dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;  // 外设到内存     
	dma_init_struct.memory0_addr = (uint32_t)rx_buffer; // 内存地址    
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE; // 增量模式    
	dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT; // 一次传输长度8bit    
	dma_init_struct.number = ARRAYNUM(rx_buffer);  // 要传输的数据量    
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(BSP_USART); // 外设地址     
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE; // 不使用增量模式，为固定模式    
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH; // 超高优先级    
	/* 初始化DMA结构体 */    
	dma_single_data_mode_init(BSP_DMA, BSP_DMA_RX_CH, &dma_init_struct);
	
	/* 初始化TX-DMA通道  */    
	dma_deinit(BSP_DMA, BSP_DMA_TX_CH);    // 配置DMA初始化参数     
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;  // 内存到外设     
	dma_init_struct.memory0_addr = (uint32_t)tx_buffer; // 内存地址    
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE; // 增量模式    
	dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT; // 一次传输长度8bit    
	dma_init_struct.number = ARRAYNUM(tx_buffer);  // 要传输的数据量    
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(BSP_USART); // 外设地址     
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE; // 不使用增量模式，为固定模式    
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH; // 超高优先级    
	/* 初始化DMA结构体 */    
	dma_single_data_mode_init(BSP_DMA, BSP_DMA_TX_CH, &dma_init_struct);
	
	/* 使能通道外设 */ 
	dma_channel_subperipheral_select(BSP_DMA, BSP_DMA_TX_CH | BSP_DMA_RX_CH, DMA_SUBPERI4);
	
	/* 使能DMA通道 */
	dma_channel_enable(BSP_DMA, BSP_DMA_TX_CH | BSP_DMA_RX_CH);
	
	/* 使能DMA通道中断 */
	nvic_irq_enable(BSP_DMA_CH_IRQ,2,1); 
	/* 使能DMA中断 */
	dma_interrupt_enable(BSP_DMA,BSP_DMA_RX_CH,DMA_CHXCTL_FTFIE);  // 使能传输完成中断

	/* 使能串口DMA接收 */    
	usart_dma_receive_config(BSP_USART, USART_RECEIVE_DMA_ENABLE);
	
	/* 使能串口DMA发送 */ 
	usart_dma_transmit_config(BSP_USART , USART_TRANSMIT_DMA_ENABLE);

}

#if !defined(__MICROLIB)
//不使用微库的话就需要添加下面的函数
#if (__ARMCLIB_VERSION <= 6000000)
//如果编译器是AC5  就定义下面这个结构体
struct __FILE
{
	int handle;
};
#endif

FILE __stdout;

//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
#endif

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(COM_UART[0], (uint8_t)ch);
    while(RESET == usart_flag_get(COM_UART[0], USART_FLAG_TBE));
    return ch;
}

