#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "st7789_drv.h"

#include "WM.h"
#include "GUI.h"
#include "arm_math.h"
#include "atk_md0280_touch.h"
#include "FramewinDLG.h"

#include "FreeRTOS.h"
#include "task.h"

#define START_TASK_PRIO 	1 			//任务优先级
#define START_STK_SIZE 		128 		//任务堆栈大小
TaskHandle_t StartTask_Handler; 		//任务句柄
void start_task(void *pvParameters); 	//任务函数

#define LED0_TASK_PRIO 		2 			//任务优先级
#define LED0_STK_SIZE 		50 			//任务堆栈大小
TaskHandle_t LED0Task_Handler; 			//任务句柄
void led0_task(void *p_arg); 			//任务函数

#define LED1_TASK_PRIO 		3 			//任务优先级
#define LED1_STK_SIZE 		50 			//任务堆栈大小
TaskHandle_t LED1Task_Handler; 			//任务句柄
void led1_task(void *p_arg); 			//任务函数

#define FLOAT_TASK_PRIO 	4 			//任务优先级
#define FLOAT_STK_SIZE 		128 		//任务堆栈大小
TaskHandle_t FLOATTask_Handler; 		//任务句柄
void float_task(void *p_arg); 			//任务函数

#endif /* __MAIN_H */
