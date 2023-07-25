#include "sys.h"
#include "delay.h"
#include "usart.h"

#define USART_CMD_LEN	50

int main(void)
{
	GPIO_InitTypeDef GPIO_Initure_LED;

	u8 len;
	u16 times = 0;

	HAL_Init();								//��ʼ��HAL��
	Stm32_Clock_Init(336,8,2,7);			//����ʱ��,168Mhz
	delay_init(168);
	uart_init(9600);

	__HAL_RCC_GPIOF_CLK_ENABLE();			//����GPIOFʱ��
	GPIO_Initure_LED.Pin = GPIO_PIN_9;    //PF9,10
	GPIO_Initure_LED.Mode = GPIO_MODE_OUTPUT_PP;		//�������
	GPIO_Initure_LED.Pull = GPIO_PULLUP;				//����
	GPIO_Initure_LED.Speed = GPIO_SPEED_HIGH;			//����
	HAL_GPIO_Init(GPIOF, &GPIO_Initure_LED);

	printf("\r\n���ֲ�����ʵ��\r\n");
	
	u8 USART_MUSIC_PLAY_TX[] = {0xAA, 0x02, 0x00, 0xAC}; 
	
	while(1)
	{
		if (USART_RX_STA & 0x8000) /* ���յ�������? */
		{
			len = USART_RX_STA & 0x3fff; /* �õ��˴ν��յ������ݳ��� */
			printf("\r\n �����͵���ϢΪ:\r\n");
			/*���ͽ��յ�������*/
			HAL_UART_Transmit(&UART1_Handler, (uint8_t*)USART_RX_BUF, len, 1000);
			/*�ȴ����ͽ���*/
			while(__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_TC) != SET);
			printf("\r\n\r\n"); /* ���뻻�� */
			USART_RX_STA = 0;
		}
		else
		{
			times++;
			if (times % 200 == 0) printf("����������,�Իس�������\r\n");
			if (times % 30 == 0) HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9); /* ��˸ LED,��ʾϵͳ��������. */
			delay_ms(10);
		}
	}
}
