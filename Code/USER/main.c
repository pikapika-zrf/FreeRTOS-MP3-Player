#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"

#define USART_CMD_LEN	50

int main(void)
{
	u8 lcd_id[12];				//���LCD ID�ַ���
	
	HAL_Init();								//��ʼ��HAL��
	Stm32_Clock_Init(336,8,2,7);			//����ʱ��,168Mhz
	delay_init(168);
	uart_init(115200);
	LED_Init();

	LCD_Init();           			//��ʼ��LCD FSMC�ӿ�
	POINT_COLOR = RED;     			//������ɫ����ɫ
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);		//��LCD ID��ӡ��lcd_id���顣
	
	LCD_Clear(WHITE);
	POINT_COLOR=RED;	  
	LCD_ShowString(30,110,200,16,16,lcd_id);		//��ʾLCD ID		
	
	while(1)
	{	
		LED0=!LED0;	 
		delay_ms(1000);	

//		HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9); /* ��˸ LED,��ʾϵͳ��������. */
//		delay_ms(5000);
	}
}
