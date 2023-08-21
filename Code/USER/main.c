#include "main.h"

#define USART_CMD_LEN	50

int main(void)
{
	int xPos,yPos;
	
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);		//设置系统中断优先级分组 4
	
	HAL_Init();						//初始化HAL库
	Stm32_Clock_Init(336,8,2,7);	//设置时钟,168Mhz
	delay_init(168);
	uart_init(115200);
//	SysTick_Config(SystemCoreClock / 1000);
	LED_Init();
	
	__HAL_RCC_CRC_CLK_ENABLE();

	WM_SetCreateFlags(WM_CF_MEMDEV);//要加上这句话，允许存储器
	GUI_Init();
	GUI_Clear();
	atk_md0280_touch_init();
		
//	GUI_SetPenSize(30);
//	GUI_DispStringAt("Hello World!",10,10);
	xPos = LCD_GetXSize()/2;
	yPos = LCD_GetYSize()/3;
	GUI_SetFont(GUI_FONT_COMIC24B_ASCII);
	GUI_DispStringHCenterAt("Hello world!", xPos, yPos);
//	GUI_PID_STATE GUI_Touch;
	
	//创建开始任务
	xTaskCreate((TaskFunction_t )start_task, 		//任务函数
				(const char* )"start_task", 		//任务名称
				(uint16_t )START_STK_SIZE, 			//任务堆栈大小
				(void* )NULL, 						//传递给任务函数的参数
				(UBaseType_t )START_TASK_PRIO, 		//任务优先级
				(TaskHandle_t* )&StartTask_Handler); //任务句柄
	vTaskStartScheduler(); 							//开启任务调度
//	while(1)
//	{	
//		LED0=!LED0;	 
//		delay_ms(1000);	
//		GUI_Delay(100);
		

		
//		uint16_t x,y;
//		uint16_t x_adc,y_adc;
//		atk_md0280_touch_scan_tmp(&x, &y, &x_adc, &y_adc);
//		//atk_md0280_touch_scan(&x, &y);
//		GUI_DispDecAt(x,10,50,5);
//		GUI_DispDecAt(y,10,100,5);
//		GUI_DispDecAt(x_adc,50,50,5);
//		GUI_DispDecAt(y_adc,50,100,5);
//				
//		GUI_TOUCH_GetState(&GUI_Touch);
//		GUI_DispDecAt(GUI_Touch.x,20,180,5);
//		GUI_DispDecAt(GUI_Touch.y,150,180,5);
//		GUI_TOUCH_Exec();
//		delay_ms(10);
//	}
}

//开始任务任务函数
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL(); //进入临界区
	//创建 LED0 任务
	xTaskCreate((TaskFunction_t )led0_task,
				(const char* )"led0_task",
				(uint16_t )LED0_STK_SIZE,
				(void* )NULL,
				(UBaseType_t )LED0_TASK_PRIO,
				(TaskHandle_t* )&LED0Task_Handler);
	
	//创建 LED1 任务
	xTaskCreate((TaskFunction_t )led1_task,
				(const char* )"led1_task",
				(uint16_t )LED1_STK_SIZE,
				(void* )NULL,
				(UBaseType_t )LED1_TASK_PRIO,
				(TaskHandle_t* )&LED1Task_Handler);
	
	//浮点测试任务
	xTaskCreate((TaskFunction_t )float_task,
				(const char* )"float_task",
				(uint16_t )FLOAT_STK_SIZE,
				(void* )NULL,
				(UBaseType_t )FLOAT_TASK_PRIO,
				(TaskHandle_t* )&FLOATTask_Handler);
	vTaskDelete(StartTask_Handler); //删除开始任务
	taskEXIT_CRITICAL(); //退出临界区
}

//LED0 任务函数
void led0_task(void *pvParameters)
{
	while(1)
	{
		LED0=~LED0;
		vTaskDelay(500);
	}
}
//LED1 任务函数
void led1_task(void *pvParameters)
{
	while(1)
	{
		LED1=0;
		vTaskDelay(200);
		LED1=1;
		vTaskDelay(800);
	}
}
//浮点测试任务
void float_task(void *p_arg)
{
	static float float_num=0.00;
	int num;
	while(1)
	{
		float_num += 0.01f;
		printf("float_num 的值为: %.4f\r\n",float_num);
		//num = float_num * 100;
		//GUI_DispDecAt(num,50,150,5);
		vTaskDelay(100);
	}
}
