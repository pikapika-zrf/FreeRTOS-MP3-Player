#include "music.h"
#include "sys.h"

u8 len;
	
void music_init()
{
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();

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
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	
}

void music_play()
{
	//u8 USART_MUSIC_PLAY_TX[] = {0xAA, 0x02, 0x00, 0xAC};
	//len = 4;
	//HAL_UART_Transmit(&UART1_Handler, (uint8_t *)USART_MUSIC_PLAY_TX, len, 1000);
	//while(__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_TC) != SET);
}
