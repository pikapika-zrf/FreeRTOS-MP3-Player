#include "sys.h"
#include "delay.h"
#include "usart.h"

#define USART_CMD_LEN	50

int main(void)
{
	GPIO_InitTypeDef GPIO_Initure;

	u8 len;
	u8 USART_MUSIC_PLAY_TX[] = {0xAA, 0x02, 0x00, 0xAC};
	u8 USART_MUSIC_STOP_TX[] = {0xAA, 0x04, 0x00, 0xAE}; 
	u8 USART_NEXT_MUSIC_TX[] = {0xAA, 0x06, 0x00, 0xB0};
	u8 USART_MUSIC_VOLUP_TX[] = {0xAA, 0x14, 0x00, 0xBE};
	
	HAL_Init();								//初始化HAL库
	Stm32_Clock_Init(336,8,2,7);			//设置时钟,168Mhz
	delay_init(168);
	uart_init(9600);

	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_Initure.Pin = GPIO_PIN_9;    //PF9,10
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;				//上拉
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;			//高速
	HAL_GPIO_Init(GPIOF, &GPIO_Initure);
	
	GPIO_Initure.Pin = GPIO_PIN_3 | GPIO_PIN_4;    	
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_Initure.Pull = GPIO_PULLDOWN;				
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;			//高速
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	
	GPIO_Initure.Pin = GPIO_PIN_5;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;				
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;			//高速
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	len = 4;
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//	HAL_UART_Transmit(&UART1_Handler, (uint8_t *)USART_MUSIC_STOP_TX, len, 1000);
//	while(__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_TC) != SET);
	
	while(1)
	{	
		HAL_UART_Transmit(&UART1_Handler, (uint8_t*)USART_NEXT_MUSIC_TX, len, 1000);
		//HAL_UART_Transmit(&UART1_Handler, (uint8_t *)USART_MUSIC_VOLUP_TX, len, 1000);
		
		HAL_UART_Transmit(&UART1_Handler, (uint8_t *)USART_MUSIC_PLAY_TX, len, 1000);
		//while(__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_TC) != SET);
		
		HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9); /* 闪烁 LED,提示系统正在运行. */
		delay_ms(5000);
	}
}
