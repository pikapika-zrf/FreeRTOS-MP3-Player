#include "sys.h"
#include "delay.h"
#include "usart.h"

void Delay(__IO uint32_t nCount);

void Delay(__IO uint32_t nCount)
{
  while(nCount--){}
}

int main(void)
{

	GPIO_InitTypeDef GPIO_Initure;
     
    HAL_Init();                    	 			//��ʼ��HAL��    
    Stm32_Clock_Init(336,8,2,7);   				//����ʱ��,168Mhz

    __HAL_RCC_GPIOF_CLK_ENABLE();           	//����GPIOFʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10; 	//PF9,10
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//����
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);

	while(1)
	{
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);		//PF9��1 
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET);		//PF10��1  			
		Delay(0x7FFFFF);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_RESET);		//PF9��0
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET);	//PF10��0  
		Delay(0x7FFFFF);
	}
}
