#include "usart.h"
#include "delay.h"
#include "FreeRTOS.h"

#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE 
{
	int handle; 
}; 

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{
	x = x; 
}
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;
	return ch;
}
#endif 

#if EN_USART1_RX					//���ʹ���˽���
u8 USART_RX_BUF[USART_REC_LEN];		//���ջ���,���USART_REC_LEN���ֽ�
u8 aRxBuffer[RXBUFFERSIZE];			//HAL��ʹ�õĴ��ڽ��ջ���
//����״̬
//bit15��		������ɱ�־
//bit14��		���յ�0x0d
//bit13~0��		���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA = 0;		//����״̬���

UART_HandleTypeDef UART1_Handler;	//UART���
#endif 
//��ʼ��IO ����1 
//baud:������
void uart_init(u32 baud)
{
	//UART ��ʼ������
	UART1_Handler.Instance = USART1;						//USART1
	UART1_Handler.Init.BaudRate = baud;						//������
	UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B;		//�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits = UART_STOPBITS_1;			//һ��ֹͣλ
	UART1_Handler.Init.Parity = UART_PARITY_NONE;			//����żУ��λ
	UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;		//��Ӳ������
	UART1_Handler.Init.Mode = UART_MODE_TX_RX;				//�շ�ģʽ
	//UART1_Handler.Init.OverSampling = UART_OVERSAMPLING_16; //16������
	HAL_UART_Init(&UART1_Handler);							//HAL_UART_Init()��ʹ��UART1
	
	//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);
	
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//uart_handle:���ھ��
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	if (huart->Instance == USART1)					//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();				//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();				//ʹ��USART1ʱ��

		GPIO_Initure.Pin = GPIO_PIN_9;				//PA9
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull = GPIO_PULLUP;			//����
		GPIO_Initure.Speed = GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate = GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);		//��ʼ��PA9

		GPIO_Initure.Pin = GPIO_PIN_10;				//PA10
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);		//��ʼ��PA10

#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);		//��ռ���ȼ�3�������ȼ�3
#endif
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)					//����Ǵ���1
	{
		if ((USART_RX_STA & 0x8000) == 0)			//����δ���
		{
			if(USART_RX_STA & 0x4000)				//���յ���0x0d
			{
				if (aRxBuffer[0] != 0x0a) USART_RX_STA = 0;		//���մ���,���¿�ʼ
				else USART_RX_STA |= 0x8000;		//��������� 
			}
			else	//��û�յ�0x0d
			{
				if (aRxBuffer[0] == 0x0d) USART_RX_STA |= 0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = aRxBuffer[0] ;
					USART_RX_STA++;
					//�������ݴ���,���¿�ʼ����
					if (USART_RX_STA > (USART_REC_LEN - 1)) USART_RX_STA = 0;
				}
			}
		}
	}
}
 
//����1�жϷ������
void USART1_IRQHandler(void)
{
	
	HAL_UART_IRQHandler(&UART1_Handler);
	while (HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
	{
		/* �������ͻῨ�������� */
	}
}
