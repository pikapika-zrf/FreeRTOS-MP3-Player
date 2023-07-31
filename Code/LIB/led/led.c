#include "led.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOF_CLK_ENABLE();           //����GPIOFʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10; //PF9,10
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);	//PF9��1��Ĭ�ϳ�ʼ�������
    HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET);	//PF10��1��Ĭ�ϳ�ʼ�������
}
