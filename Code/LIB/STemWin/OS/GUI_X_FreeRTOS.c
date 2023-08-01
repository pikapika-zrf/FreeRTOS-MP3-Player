/*********************************************************************
*          Portions COPYRIGHT 2016 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.32 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_X.C
Purpose     : This file provides emWin Interface with FreeRTOS
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "GUI.h"
    
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
    
/*********************************************************************
*
* Global data
*/
    static xSemaphoreHandle xQueueMutex;
static xSemaphoreHandle xSemaTxDone;

/*********************************************************************
*
* Timing:
* GUI_X_GetTime()
* GUI_X_Delay(int)

Some timing dependent routines require a GetTime
and delay function. Default time unit (tick), normally is
1 ms.
*/
//获取当前系统时间（单位ms）
int GUI_X_GetTime(void)
{
  return ((int) xTaskGetTickCount());
}
//延时函数（单位ms）
void GUI_X_Delay(int ms)
{
  vTaskDelay( ms );
}

/*********************************************************************
*
* GUI_X_Init()
*
* Note:
* GUI_X_Init() is called from GUI_Init is a possibility to init
* some hardware which needs to be up and running before the GUI.
* If not required, leave this routine blank.
*/

void GUI_X_Init(void) {
}


/*********************************************************************
*
* GUI_X_ExecIdle
*
* Note:
* Called if WM is in idle state
*/
//窗口管理器空闲时调用
void GUI_X_ExecIdle(void) 
{
  vTaskDelay(30/portTICK_RATE_MS);
}

/*********************************************************************
*
* Multitasking:
*
* GUI_X_InitOS()
* GUI_X_GetTaskId()
* GUI_X_Lock()
* GUI_X_Unlock()
*
* Note:
* The following routines are required only if emWin is used in a
* true multi task environment, which means you have more than one
* thread using the emWin API.
* In this case the
* #define GUI_OS 1
* needs to be in GUIConf.h
*/

/* Init OS */
//创建互斥信号量和二值信号量
void GUI_X_InitOS(void)
{ 
	/* 创建互斥信号量 用于资源共享 */
	xQueueMutex = xSemaphoreCreateMutex();
	configASSERT (xQueueMutex != NULL);
  
	/* 创建二值信号量 用于事件触发 */
	vSemaphoreCreateBinary( xSemaTxDone );
	configASSERT ( xSemaTxDone != NULL );
}

//互斥解锁
void GUI_X_Unlock(void)
{ 
	/* 给出互斥量 */
	xSemaphoreGive( xQueueMutex ); 
}

//互斥锁定
void GUI_X_Lock(void)
{
  if(xQueueMutex == NULL)
  {
    GUI_X_InitOS();
  }
  /* 获取互斥量 */
  xSemaphoreTake( xQueueMutex, portMAX_DELAY );	//互斥量句柄，阻塞等待
}

/* Get Task handle */
//获取当前任务句柄
U32 GUI_X_GetTaskId(void) 
{ 
  return ((U32) xTaskGetCurrentTaskHandle());
}

//等待二值信号量
void GUI_X_WaitEvent (void) 
{
  /* 获取信号量 */
  while( xSemaphoreTake(xSemaTxDone, portMAX_DELAY ) != pdTRUE );
}

//发送二值信号量
void GUI_X_SignalEvent (void) 
{
  /* 给出信号量 */
  xSemaphoreGive( xSemaTxDone );
}

/*********************************************************************
*
* Logging: OS dependent

Note:
Logging is used in higher debug levels only. The typical target
build does not use logging and does therefor not require any of
the logging routines below. For a release build without logging
the routines below may be eliminated to save some space.
(If the linker is not function aware and eliminates unreferenced
functions automatically)

*/

void GUI_X_Log (const char *s) { }
void GUI_X_Warn (const char *s) { }
void GUI_X_ErrorOut(const char *s) { }

/*************************** End of file ****************************/
