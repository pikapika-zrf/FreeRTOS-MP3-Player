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

#define START_TASK_PRIO 	1 			//�������ȼ�
#define START_STK_SIZE 		128 		//�����ջ��С
TaskHandle_t StartTask_Handler; 		//������
void start_task(void *pvParameters); 	//������

#define LED0_TASK_PRIO 		2 			//�������ȼ�
#define LED0_STK_SIZE 		50 			//�����ջ��С
TaskHandle_t LED0Task_Handler; 			//������
void led0_task(void *p_arg); 			//������

#define LED1_TASK_PRIO 		3 			//�������ȼ�
#define LED1_STK_SIZE 		50 			//�����ջ��С
TaskHandle_t LED1Task_Handler; 			//������
void led1_task(void *p_arg); 			//������

#define FLOAT_TASK_PRIO 	4 			//�������ȼ�
#define FLOAT_STK_SIZE 		128 		//�����ջ��С
TaskHandle_t FLOATTask_Handler; 		//������
void float_task(void *p_arg); 			//������

#endif /* __MAIN_H */
