#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"

#define USART_CMD_LEN	50

int main(void)
{
	u8 lcd_id[12];				//存放LCD ID字符串
	
	HAL_Init();								//初始化HAL库
	Stm32_Clock_Init(336,8,2,7);			//设置时钟,168Mhz
	delay_init(168);
	uart_init(115200);
	LED_Init();

	LCD_Init();           			//初始化LCD FSMC接口
	POINT_COLOR = RED;     			//画笔颜色：红色
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);		//将LCD ID打印到lcd_id数组。
	
	LCD_Clear(WHITE);
	POINT_COLOR=RED;	  
	LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID		
	
	while(1)
	{	
		LED0=!LED0;	 
		delay_ms(1000);	

//		HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9); /* 闪烁 LED,提示系统正在运行. */
//		delay_ms(5000);
	}
}
