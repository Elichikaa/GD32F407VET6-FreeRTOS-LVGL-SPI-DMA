#include <stdint.h>
#include <board.h>

#define  BSP_TIMER_RCU    RCU_TIMER5  // 定时器时钟
#define  BSP_TIMER   TIMER5   // 定时器
#define  BSP_TIMER_IRQ  TIMER5_DAC_IRQn   // 定时器中断

static __IO uint32_t g_system_tick = 0;
static uint8_t tim5_flag = 1;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
    {
    }
    /* USER CODE END Error_Handler */
}

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if(SysTick_Config(SystemCoreClock / 1000U)) {
        /* capture error */
        while(1) {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

void TIM5_Init(uint16_t pre, uint16_t per) 
{
	
	/* 开启时钟 */
	rcu_periph_clock_enable(BSP_TIMER_RCU); // 开启定时器时钟
	
	/* CK_TIMERx = 4 x CK_APB1  = 4x42M = 168MHZ */    
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4); // 配置定时器时钟  
	
	timer_parameter_struct timer_initpara; // 定义定时器结构体
	/* 配置定时器参数 */
	timer_initpara.prescaler = pre -1;     //  时钟预分频值 0-65535  psc_clk = CK_TIMER / pre
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE;        // 边缘对齐
	timer_initpara.counterdirection = TIMER_COUNTER_UP;     // 向上计数
	timer_initpara.period = per  - 1;                       // 周期
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;        // 分频因子
	timer_initpara.repetitioncounter = 0;                   // 重复计数器 0-255  
	timer_init(BSP_TIMER,&timer_initpara);                  // 初始化定时器
	
	/* 配置中断优先级 */
	nvic_irq_enable(BSP_TIMER_IRQ,3,2); // 设置中断优先级为 3,2断
	
	/* 使能中断 */
	timer_interrupt_enable(BSP_TIMER,TIMER_INT_UP); // 使能更新事件中断 
	
	/* 使能定时器 */
	timer_enable(BSP_TIMER);
	
}



///*!
//    \brief      this function handles SysTick exception
//    \param[in]  none
//    \param[out] none
//    \retval     none
//*/
//void SysTick_Handler(void)
//{
//	g_system_tick ++;
//    soft_timer_tick();
//}

void TIMER5_DAC_IRQHandler(void)
{
     /* 这里是定时器中断 */ 
    if(timer_interrupt_flag_get(BSP_TIMER, TIMER_INT_FLAG_UP) == SET)    
    {        
        timer_interrupt_flag_clear(BSP_TIMER, TIMER_INT_FLAG_UP); // 清除中断标志位

		tim5_flag = 1;
		
		//lv_tick_inc(1);
    }
}

uint32_t get_system_tick(void)
{
    return g_system_tick;
}

/**
 * This function will initial GD32 board.
 */
void board_init(void)
{
    /* NVIC Configuration */
#define NVIC_VTOR_MASK              0x3FFFFF80
#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x10000000 */
    SCB->VTOR  = (0x10000000 & NVIC_VTOR_MASK);
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    SCB->VTOR  = (0x08000000 & NVIC_VTOR_MASK);
#endif

//    systick_config();
	TIM5_Init(16800,10);  // 定时时间为：time = pre / 168 000 000 * per  = 16800 / 168 000 000 * 10 = 1ms 

}

/**
 -  @brief  用内核的 systick 实现的微妙延时
 -  @note   None
 -  @param  _us:要延时的us数
 -  @retval None
*/
void delay_us(uint32_t _us)
{
    // uint32_t ticks;
    // uint32_t told, tnow, tcnt = 0;

    // // 计算需要的时钟数 = 延迟微秒数 * 每微秒的时钟数
    // ticks = _us * (SystemCoreClock / 1000000);

    // // 获取当前的SysTick值
    // told = SysTick->VAL;

    // while (1)
    // {
    //     // 重复刷新获取当前的SysTick值
    //     tnow = SysTick->VAL;

    //     if (tnow != told)
    //     {
    //         if (tnow < told)
    //             tcnt += told - tnow;
    //         else
    //             tcnt += SysTick->LOAD - tnow + told;

    //         told = tnow;

    //         // 如果达到了需要的时钟数，就退出循环
    //         if (tcnt >= ticks)
    //             break;
    //     }
    // }

    // 计算循环次数
//    uint32_t loops = _us * (SystemCoreClock / 1000000) / 10;

//    // 简单的循环延时
//    for (uint32_t i = 0; i < loops; i++)
//    {
//        __NOP(); // 空操作指令，用于增加循环的延时
//    }

	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;						//LOAD的值	    	 
	ticks=_us*(SystemCoreClock/1000000); 	//需要的节拍数	  		 
	tcnt=0;
	vTaskSuspendAll();									//阻止OS调度，防止打断us延时
	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;						//时间超过/等于要延迟的时间,则退出.
		}  
	};
	xTaskResumeAll();										//恢复OS调度	
						    
}

/**
 -  @brief  调用用内核的 systick 实现的毫秒延时
 -  @note   None
 -  @param  _ms:要延时的ms数
 -  @retval None
*/
void delay_ms(uint32_t _ms) { 

//delay_us(_ms * 1000); 
	tim5_flag = 0;
	while(_ms)
	{
		if( tim5_flag )
		{
			_ms--;
			tim5_flag = 0;
		}
	}
}
