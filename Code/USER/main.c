#include "sys.h"
#include "delay.h"
#include "usart.h"

#define USART_CMD_LEN	50

int main(void)
{
	GPIO_InitTypeDef GPIO_Initure_LED;

	u8 len;
	u16 times = 0;

	HAL_Init();								//初始化HAL库
	Stm32_Clock_Init(336,8,2,7);			//设置时钟,168Mhz
	delay_init(168);
	uart_init(9600);

	__HAL_RCC_GPIOF_CLK_ENABLE();			//开启GPIOF时钟
	GPIO_Initure_LED.Pin = GPIO_PIN_9;    //PF9,10
	GPIO_Initure_LED.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_Initure_LED.Pull = GPIO_PULLUP;				//上拉
	GPIO_Initure_LED.Speed = GPIO_SPEED_HIGH;			//高速
	HAL_GPIO_Init(GPIOF, &GPIO_Initure_LED);

	printf("\r\n音乐播放器实验\r\n");
	
	u8 USART_MUSIC_PLAY_TX[] = {0xAA, 0x02, 0x00, 0xAC}; 
	
	while(1)
	{
		if (USART_RX_STA & 0x8000) /* 接收到了数据? */
		{
			len = USART_RX_STA & 0x3fff; /* 得到此次接收到的数据长度 */
			printf("\r\n 您发送的消息为:\r\n");
			/*发送接收到的数据*/
			HAL_UART_Transmit(&UART1_Handler, (uint8_t*)USART_RX_BUF, len, 1000);
			/*等待发送结束*/
			while(__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_TC) != SET);
			printf("\r\n\r\n"); /* 插入换行 */
			USART_RX_STA = 0;
		}
		else
		{
			times++;
			if (times % 200 == 0) printf("请输入数据,以回车键结束\r\n");
			if (times % 30 == 0) HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9); /* 闪烁 LED,提示系统正在运行. */
			delay_ms(10);
		}
	}
}
