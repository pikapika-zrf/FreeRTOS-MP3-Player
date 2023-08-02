#ifndef __ATK_MD0280_TOUCH_H
#define __ATK_MD0280_TOUCH_H


#include "st7789_drv.h"
#include "atk_md0280_touch_spi.h"
#include <stdlib.h>
#include "delay.h"

#define ATK_MD0280_TOUCH_READ_TIMES             5   /* ���嵥�λ�ȡ�����ȡ�Ĵ��� */
#define ATK_MD0280_TOUCH_READ_DISCARD           1   /* ���嵥�λ�ȡ���궪���������Сֵ������ */
#define ATK_MD0280_TOUCH_READ_RANGE             50  /* ���嵥�λ�ȡ�����ȡ������ֵ֮���������ֵ */

/* ���Ŷ��� */
#define ATK_MD0280_TOUCH_PEN_GPIO_PORT          GPIOB
#define ATK_MD0280_TOUCH_PEN_GPIO_PIN           GPIO_PIN_1
#define ATK_MD0280_TOUCH_PEN_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

/* IO���� */
#define ATK_MD0280_TOUCH_READ_PEN()              HAL_GPIO_ReadPin(ATK_MD0280_TOUCH_PEN_GPIO_PORT, ATK_MD0280_TOUCH_PEN_GPIO_PIN)

/* ������� */
#define ATK_MD0280_TOUCH_EOK                    0   /* û�д��� */
#define ATK_MD0280_TOUCH_ERROR                  1
#define ATK_MD0280_TOUCH_EMPTY                  2   /* �� */

/* �������� */
void atk_md0280_touch_init(void);                        /* ATK-MD0280ģ�鴥����ʼ�� */
uint8_t atk_md0280_touch_scan(uint16_t *x, uint16_t *y);    /* ATK-MD0280ģ�鴥��ɨ�� */

#endif
