#include "atk_md0280_touch.h"


/* ��ȡX/Y����ADCֵ������ */
#define ATK_MD0280_TOUCH_CMD_X  0xD0
#define ATK_MD0280_TOUCH_CMD_Y  0x90

/* ATK-MD0280ģ�鴥��״̬���ݽṹ�� */
static struct
{
    struct
    {
        float x;
        float y;
    } fac;          /* У׼�������� */
    struct
    {
        uint16_t x;
        uint16_t y;
    } center;       /* ���������ADCֵ */
} g_atk_md0280_touch_sta = {0};

/**
 * @brief       ATK-MD0280ģ�鴥��Ӳ����ʼ��
 * @param       ��
 * @retval      ��
 */
static void atk_md0280_touch_hw_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* ʹ��ʱ�� */
    ATK_MD0280_TOUCH_PEN_GPIO_CLK_ENABLE();
    
    /* ��ʼ��PEN���� */
    gpio_init_struct.Pin    = ATK_MD0280_TOUCH_PEN_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_INPUT;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_TOUCH_PEN_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief       ��ȡATK-MD0280ģ�鴥����X/Y��ADCֵ
 * @note        ������ȡATK_MD0280_TOUCH_READ_TIMES�Σ�
 *              �����������Сֵ��ATK_MD0280_TOUCH_READ_DISCARD����
 *              ��󷵻�ʣ��ľ�ֵ
 * @param       cmd: ATK_MD0280_TOUCH_CMD_X: ��ȡX���ADCֵ��LCD��ʾ����δ��ת��
 *                   ATK_MD0280_TOUCH_CMD_Y: ��ȡY���ADCֵ��LCD��ʾ����δ��ת��
 * @retval      ��ȡ���Ĵ���ADCֵ
 */
static uint16_t atk_md0280_touch_get_adc(uint8_t cmd)
{
    uint16_t dat[ATK_MD0280_TOUCH_READ_TIMES];
    uint8_t dat_index;
    uint8_t dat_index2;
    uint16_t dat_tmp;
    uint16_t dat_sum = 0;
    
    for (dat_index=0; dat_index < ATK_MD0280_TOUCH_READ_TIMES; dat_index++)
    {
        dat[dat_index] = atk_md0280_touch_spi_read(cmd);
    }
    
    for (dat_index=0; dat_index < (ATK_MD0280_TOUCH_READ_TIMES - 1); dat_index++)
    {
        for (dat_index2=(dat_index + 1); dat_index2 < ATK_MD0280_TOUCH_READ_TIMES; dat_index2++)
        {
            if (dat[dat_index] > dat[dat_index2])
            {
                dat_tmp = dat[dat_index];
                dat[dat_index] = dat[dat_index2];
                dat[dat_index2] = dat_tmp;
            }
        }
    }
    
    for (dat_index=ATK_MD0280_TOUCH_READ_DISCARD; dat_index<(ATK_MD0280_TOUCH_READ_TIMES-ATK_MD0280_TOUCH_READ_DISCARD); dat_index++)
    {
        dat_sum += dat[dat_index];
    }
    
    return (dat_sum / (ATK_MD0280_TOUCH_READ_TIMES - (ATK_MD0280_TOUCH_READ_DISCARD << 1)));
}

/**
 * @brief       ��ȡATK-MD0280ģ�鴥����X/Y��ADCֵ
 * @note        ������ȡ���Σ�ֱ�����ζ�ȡ�Ĳ�ֵС��ATK_MD0280_TOUCH_READ_RANGE��
 *              �󷵻ؾ�ֵ
 * @param       cmd: ATK_MD0280_TOUCH_CMD_X: ��ȡX���ADCֵ��LCD��ʾ����δ��ת��
 *                   ATK_MD0280_TOUCH_CMD_Y: ��ȡY���ADCֵ��LCD��ʾ����δ��ת��
 * @retval      ��ȡ���Ĵ���ADCֵ
 */
static uint16_t atk_md0280_touch_get_adc2(uint8_t cmd)
{
    uint16_t dat1;
    uint16_t dat2;
    uint16_t delta;
    
    do {
        dat1 = atk_md0280_touch_get_adc(cmd);
        dat2 = atk_md0280_touch_get_adc(cmd);
        if (dat1 > dat2)
        {
            delta = dat1 - dat2;
        }
        else
        {
            delta = dat2 - dat1;
        }
    } while (delta > ATK_MD0280_TOUCH_READ_RANGE);
    
    return ((dat1 + dat2) >> 1);
}

/**
 * @brief       �滭ATK-MD0280ģ�鴥��У׼������
 * @param       x    : ����У׼�������X����
 *              y    : ����У׼�������Y����
 *              color: ����У׼���������ɫ
 * @retval      ��
 */
static void atk_md0280_touch_draw_touch_point(uint16_t x, uint16_t y, uint16_t color)
{
	POINT_COLOR = color;
    LCD_DrawLine(x - 12, y, x + 13, y);
    LCD_DrawLine(x, y - 12, x, y + 13);
    LCD_DrawPoint(x + 1, y + 1, color);
    LCD_DrawPoint(x - 1, y + 1, color);
    LCD_DrawPoint(x + 1, y - 1, color);
    LCD_DrawPoint(x - 1, y - 1, color);
    LCD_Draw_Circle(x, y, 6, color);
}

/**
 * @brief       ATK-MD0280ģ�鴥��У׼
 * @param       ��
 * @retval      ��
 */
static void atk_md0280_touch_calibration(void)
{
    struct
    {
        uint16_t x;
        uint16_t y;
    } point[5];
    uint8_t point_index;
    int16_t d1, d2, d3, d4;
    double x_fac, y_fac;
    char *str = "Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";
    LCD_Clear(WHITE);
    LCD_ShowString( 40,
                            40,
                            lcddev.width - 80,
                            lcddev.height - 80,
                            16,
                            (u8 *)str);
    
    while (1)
    {
        for (point_index=0; point_index<5 + 1; point_index++)
        {
            switch (point_index)
            {
                case 0:
                {
                    atk_md0280_touch_draw_touch_point(lcddev.width >> 1, lcddev.height >> 1, WHITE);
                    atk_md0280_touch_draw_touch_point(20, 20, RED);
                    break;
                }
                case 1:
                {
                    atk_md0280_touch_draw_touch_point(20, 20, WHITE);
                    atk_md0280_touch_draw_touch_point(lcddev.width - 20, 20, RED);
                    break;
                }
                case 2:
                {
                    atk_md0280_touch_draw_touch_point(lcddev.width - 20, 20, WHITE);
                    atk_md0280_touch_draw_touch_point(20, lcddev.height - 20, RED);
                    break;
                }
                case 3:
                {
                    atk_md0280_touch_draw_touch_point(20, lcddev.height - 20, WHITE);
                    atk_md0280_touch_draw_touch_point(lcddev.width - 20, lcddev.height - 20, RED);
                    break;
                }
                case 4:
                {
                    atk_md0280_touch_draw_touch_point(lcddev.width - 20, lcddev.height - 20, WHITE);
                    atk_md0280_touch_draw_touch_point(lcddev.width >> 1, lcddev.height >> 1, RED);
                    break;
                }
                case 5:
                {
                    d1 = point[1].x - point[0].x;
                    d3 = point[3].x - point[2].x;
                    d2 = point[3].y - point[1].y;
                    d4 = point[2].y - point[0].y;
                    
                    x_fac = (double)d1 / d3;
                    y_fac = (double)d2 / d4;
                    
                    if (x_fac < 0)
                    {
                        x_fac = -x_fac;
                    }
                    if (y_fac < 0)
                    {
                        y_fac = -y_fac;
                    }
                    
                    if (    x_fac < 0.95 || x_fac > 1.05 || y_fac < 0.95 || y_fac > 1.05 ||
                            abs(d1) > 4095 || abs(d2) > 4095 || abs(d3) > 4095 || abs(d4) > 4095 ||
                            abs(d1) == 0 || abs(d2) == 0 || abs(d3) == 0 || abs(d4) == 0)
                    {
                        break;
                    }
                    
                    g_atk_md0280_touch_sta.fac.x = (float)(d1 + d3) / ((lcddev.width - 40) << 1);
                    g_atk_md0280_touch_sta.fac.y = (float)(d2 + d4) / ((lcddev.height - 40) << 1);
                    
                    g_atk_md0280_touch_sta.center.x = point[4].x;
                    g_atk_md0280_touch_sta.center.y = point[4].y;
					char *str = "Touch Screen Adjust OK!";
                            
                    LCD_Clear(WHITE);
                    LCD_ShowString( 30,
                                            100,
                                            lcddev.width,
                                            lcddev.height,
                                            16,
                                            (u8 *)str);
                    delay_ms(1000);
                    LCD_Clear(WHITE);
                    
                    return;
                }
            }
            delay_ms(200);
            while (ATK_MD0280_TOUCH_READ_PEN() != 0);
            point[point_index].x = atk_md0280_touch_get_adc2(ATK_MD0280_TOUCH_CMD_X);
            point[point_index].y = atk_md0280_touch_get_adc2(ATK_MD0280_TOUCH_CMD_Y);
            while (ATK_MD0280_TOUCH_READ_PEN() == 0);
        }
    }
}

/**
 * @brief       ATK-MD0280ģ�鴥����ʼ��
 * @param       ��
 * @retval      ��
 */
void atk_md0280_touch_init(void)
{
    atk_md0280_touch_hw_init();
    atk_md0280_touch_spi_init();
    atk_md0280_touch_calibration();
}

/**
 * @brief       ATK-MD0280ģ�鴥��ɨ��
 * @param       x: ɨ�赽������X����
 *              y: ɨ�赽������Y����
 * @retval      ATK_MD0280_TOUCH_EOK  : ɨ�赽��Ч�Ĵ���
 *              ATK_MD0280_TOUCH_ERROR: ����������Ч
 *              ATK_MD0280_TOUCH_EMPTY: δɨ�赽��Ч�Ĵ���
 */
uint8_t atk_md0280_touch_scan(uint16_t *x, uint16_t *y)
{
    uint16_t x_adc;
    uint16_t y_adc;
    //atk_md0280_lcd_disp_dir_t 
	int dir;
    uint16_t x_raw;
    uint16_t y_raw;
    
    if (ATK_MD0280_TOUCH_READ_PEN() == 0)
    {
        x_adc = atk_md0280_touch_get_adc2(ATK_MD0280_TOUCH_CMD_X);
        y_adc = atk_md0280_touch_get_adc2(ATK_MD0280_TOUCH_CMD_Y);
        
        x_raw = (int16_t)(x_adc - g_atk_md0280_touch_sta.center.x) / g_atk_md0280_touch_sta.fac.x + lcddev.width / 2;
        y_raw = (int16_t)(y_adc - g_atk_md0280_touch_sta.center.y) / g_atk_md0280_touch_sta.fac.y + lcddev.height / 2;
        
        if((x_raw >= lcddev.width) || (y_raw >= lcddev.height))
        {
            return ATK_MD0280_TOUCH_ERROR;
        }
        
        //dir = atk_md0280_get_disp_dir();
		dir = 0;
        switch (dir)
        {
            case 0:		//ATK_MD0280_LCD_DISP_DIR_0
            {
                *x = x_raw;
                *y = y_raw;
                break;
            }
            case 1:		//ATK_MD0280_LCD_DISP_DIR_90:
            {
                *x = y_raw;
                *y = lcddev.height - x_raw;
                break;
            }
            case 2:		//ATK_MD0280_LCD_DISP_DIR_180:
            {
                *x = lcddev.width - x_raw;
                *y = lcddev.height - y_raw;
                break;
            }
            case 3:		//ATK_MD0280_LCD_DISP_DIR_270:
            {
                *x = lcddev.width - y_raw;
                *y = x_raw;
                break;
            }
        }
        
        return ATK_MD0280_TOUCH_EOK;
    }
    
    return ATK_MD0280_TOUCH_EMPTY;
}
