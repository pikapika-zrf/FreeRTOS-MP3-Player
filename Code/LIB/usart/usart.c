#include "usart.h"
#include "delay.h"
#include "FreeRTOS.h"

#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE 
{
	int handle; 
}; 

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{
	x = x; 
}
//重定义fputc函数 
int fputc(int ch, FILE *f)
{
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;
	return ch;
}
#endif 

#if EN_USART1_RX					//如果使能了接收
u8 USART_RX_BUF[USART_REC_LEN];		//接收缓冲,最大USART_REC_LEN个字节
u8 aRxBuffer[RXBUFFERSIZE];			//HAL库使用的串口接收缓冲
//接收状态
//bit15，		接收完成标志
//bit14，		接收到0x0d
//bit13~0，		接收到的有效字节数目
u16 USART_RX_STA = 0;		//接收状态标记

UART_HandleTypeDef UART1_Handler;	//UART句柄
#endif 
//初始化IO 串口1 
//baud:波特率
void uart_init(u32 baud)
{
	//UART 初始化设置
	UART1_Handler.Instance = USART1;						//USART1
	UART1_Handler.Init.BaudRate = baud;						//波特率
	UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B;		//字长为8位数据格式
	UART1_Handler.Init.StopBits = UART_STOPBITS_1;			//一个停止位
	UART1_Handler.Init.Parity = UART_PARITY_NONE;			//无奇偶校验位
	UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;		//无硬件流控
	UART1_Handler.Init.Mode = UART_MODE_TX_RX;				//收发模式
	//UART1_Handler.Init.OverSampling = UART_OVERSAMPLING_16; //16过采样
	HAL_UART_Init(&UART1_Handler);							//HAL_UART_Init()会使能UART1
	
	//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);
	
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//uart_handle:串口句柄
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	if (huart->Instance == USART1)					//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();				//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();				//使能USART1时钟

		GPIO_Initure.Pin = GPIO_PIN_9;				//PA9
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed = GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate = GPIO_AF7_USART1;	//复用为USART1
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);		//初始化PA9

		GPIO_Initure.Pin = GPIO_PIN_10;				//PA10
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);		//初始化PA10

#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);		//抢占优先级3，子优先级3
#endif
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)					//如果是串口1
	{
		if ((USART_RX_STA & 0x8000) == 0)			//接收未完成
		{
			if(USART_RX_STA & 0x4000)				//接收到了0x0d
			{
				if (aRxBuffer[0] != 0x0a) USART_RX_STA = 0;		//接收错误,重新开始
				else USART_RX_STA |= 0x8000;		//接收完成了 
			}
			else	//还没收到0x0d
			{
				if (aRxBuffer[0] == 0x0d) USART_RX_STA |= 0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = aRxBuffer[0] ;
					USART_RX_STA++;
					//接收数据错误,重新开始接收
					if (USART_RX_STA > (USART_REC_LEN - 1)) USART_RX_STA = 0;
				}
			}
		}
	}
}
 
//串口1中断服务程序
void USART1_IRQHandler(void)
{
	
	HAL_UART_IRQHandler(&UART1_Handler);
	while (HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
	{
		/* 如果出错就会卡死在这里 */
	}
}
