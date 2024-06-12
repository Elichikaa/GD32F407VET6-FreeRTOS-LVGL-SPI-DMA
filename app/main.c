/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：club.szlcsc.com
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-02     LCKFB-yzh    first version
 */
#include "board.h"

#include "lv_demo_widgets.h"

#define LOG_TAG          "main"
#include "elog.h"

TaskHandle_t led_task_handle;// 定义任务句柄变量
void led_task(void);

TaskHandle_t lcd_task_handle;
void lcd_task(void);

int main(void)
{
    board_init();
	bsp_uart_init();
	
	elog_init();
	elog_set_text_color_enabled(false);

    /* 设置每个级别的日志输出格式 */
    //输出所有内容
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    //输出日志级别信息和日志TAG
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG);
    //除了时间、进程信息、线程信息之外，其余全部输出
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_TIME | ELOG_FMT_P_INFO | ELOG_FMT_T_INFO));
    //输出所有内容
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL);

    /* 启动elog */
    elog_start();
	
//	log_a("Hello EasyLogger!");
//	log_e("Hello EasyLogger!");
//	log_w("Hello EasyLogger!");
//	log_i("Hello EasyLogger!");
	log_d("Hello EasyLogger!\r\n");
//	log_v("Hello EasyLogger!");
		
	
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_2);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_2);
	
	
	xTaskCreate((TaskFunction_t)led_task, "led_task", 128, NULL, 1, &led_task_handle);
	xTaskCreate((TaskFunction_t)lcd_task, "lcd_task", 512, NULL, 4, &lcd_task_handle);
	
	vTaskStartScheduler();// 启动RTOS调度器	
	
    while (1)
    {
    }
}


void led_task(void)
{
	while(1)
	{
		// 配置PB2引脚输出高电平
		gpio_bit_write(GPIOB,GPIO_PIN_2,SET);
		vTaskDelay(pdMS_TO_TICKS(500)); // 延时 500 毫秒
		// 配置PB2引脚输出低电平
		gpio_bit_write(GPIOB,GPIO_PIN_2,RESET);
		vTaskDelay(pdMS_TO_TICKS(500)); 
	}
	
	vTaskDelete(NULL);
}	


void vApplicationTickHook()
{
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
		lv_tick_inc(1);
	}
}

void lcd_task(void)
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS( 5 );
	
	// 使用当前时间初始化变量xLastWakeTime ,注意这和vTaskDelay()函数不同 
	xLastWakeTime = xTaskGetTickCount();  
	
	lv_init();
	lv_port_disp_init(); 
	lv_port_indev_init();	
	
	lv_demo_widgets();
	
	while(1)
	{
		/* 调用系统延时函数,周期性阻塞5ms */    
		vTaskDelayUntil( &xLastWakeTime,xPeriod );
		lv_task_handler();
	}
	
	vTaskDelete(NULL);
}	

