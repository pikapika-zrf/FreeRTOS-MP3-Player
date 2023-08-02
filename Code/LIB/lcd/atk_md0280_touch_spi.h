#ifndef __ATK_MD0280_TOUCH_SPI_H
#define __ATK_MD0280_TOUCH_SPI_H

//#include "./BSP/ATK_MD0280/atk_md0280.h"
#include "st7789_drv.h"

/* ���Ŷ��� */
#define ATK_MD0280_TOUCH_SPI_MI_GPIO_PORT           GPIOB
#define ATK_MD0280_TOUCH_SPI_MI_GPIO_PIN            GPIO_PIN_2
#define ATK_MD0280_TOUCH_SPI_MI_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define ATK_MD0280_TOUCH_SPI_MO_GPIO_PORT           GPIOF
#define ATK_MD0280_TOUCH_SPI_MO_GPIO_PIN            GPIO_PIN_11
#define ATK_MD0280_TOUCH_SPI_MO_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)
#define ATK_MD0280_TOUCH_SPI_TCS_GPIO_PORT          GPIOC
#define ATK_MD0280_TOUCH_SPI_TCS_GPIO_PIN           GPIO_PIN_13
#define ATK_MD0280_TOUCH_SPI_TCS_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define ATK_MD0280_TOUCH_SPI_CLK_GPIO_PORT          GPIOB
#define ATK_MD0280_TOUCH_SPI_CLK_GPIO_PIN           GPIO_PIN_0
#define ATK_MD0280_TOUCH_SPI_CLK_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

/* IO���� */
#define ATK_MD0280_TOUCH_SPI_READ_MI()              HAL_GPIO_ReadPin(ATK_MD0280_TOUCH_SPI_MI_GPIO_PORT, ATK_MD0280_TOUCH_SPI_MI_GPIO_PIN)
#define ATK_MD0280_TOUCH_SPI_MO(x)                  do{ x ?                                                                                                         \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_MO_GPIO_PORT, ATK_MD0280_TOUCH_SPI_MO_GPIO_PIN, GPIO_PIN_SET) :      \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_MO_GPIO_PORT, ATK_MD0280_TOUCH_SPI_MO_GPIO_PIN, GPIO_PIN_RESET);     \
                                                    }while(0)
#define ATK_MD0280_TOUCH_SPI_TCS(x)                 do{ x ?                                                                                                         \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_TCS_GPIO_PORT, ATK_MD0280_TOUCH_SPI_TCS_GPIO_PIN, GPIO_PIN_SET) :    \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_TCS_GPIO_PORT, ATK_MD0280_TOUCH_SPI_TCS_GPIO_PIN, GPIO_PIN_RESET);   \
                                                    }while(0)
#define ATK_MD0280_TOUCH_SPI_CLK(x)                 do{ x ?                                                                                                         \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_CLK_GPIO_PORT, ATK_MD0280_TOUCH_SPI_CLK_GPIO_PIN, GPIO_PIN_SET) :    \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_CLK_GPIO_PORT, ATK_MD0280_TOUCH_SPI_CLK_GPIO_PIN, GPIO_PIN_RESET);   \
                                                    }while(0)

/* �������� */
void atk_md0280_touch_spi_init(void);               /* ATK-MD0280ģ�鴥��SPI�ӿڳ�ʼ�� */
uint16_t atk_md0280_touch_spi_read(uint8_t cmd);    /* ATK-MD0280ģ�鴥��SPI�ӿڶ����� */

#endif
