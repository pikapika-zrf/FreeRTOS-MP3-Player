#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "WM.h"
#include "GUI.h"

#define USART_CMD_LEN	50

int main(void)
{
//	u8 lcd_id[12];					//存放LCD ID字符串
	int xPos,yPos;

	HAL_Init();						//初始化HAL库
	Stm32_Clock_Init(336,8,2,7);	//设置时钟,168Mhz
	delay_init(168);
	uart_init(115200);
	SysTick_Config(SystemCoreClock / 1000);
	LED_Init();
	
	__HAL_RCC_CRC_CLK_ENABLE();
	
	WM_SetCreateFlags(WM_CF_MEMDEV);//要加上这句话，允许存储器
	GUI_Init();
	GUI_Clear();
//	GUI_SetBkColor(GUI_LIGHTBLUE);
//	GUI_SetPenSize(30);
//	GUI_SetColor(GUI_RED);
//	GUI_DispStringAt("Hello World!",30,120);

	xPos = LCD_GetXSize()/2;
	yPos = LCD_GetYSize()/3;
	GUI_SetFont(GUI_FONT_COMIC24B_ASCII);
	GUI_DispStringHCenterAt("Hello world!", xPos, yPos);	

	while(1)
	{	
		LED0=!LED0;	 
		delay_ms(1000);	

//		HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9); /* 闪烁 LED,提示系统正在运行. */
//		delay_ms(5000);
	}
}
