#include "GUI.h"
#include "atk_md0280_touch.h"
#include "usart.h"
#include "st7789_drv.h"

//本文件的函数为触摸驱动，由emWin上层调用

void GUI_TOUCH_X_ActivateX(void)
{
    // XPT2046_WriteCMD(0x90);
}


void GUI_TOUCH_X_ActivateY(void)
{
    //XPT2046_WriteCMD(0xd0);
}


int  GUI_TOUCH_X_MeasureX(void)
{
	uint16_t x;
    uint16_t y;
	atk_md0280_touch_scan(&x, &y);
    return x;

}

int  GUI_TOUCH_X_MeasureY(void)
{
    uint16_t x;
    uint16_t y;
	atk_md0280_touch_scan(&x, &y);
    return y;
}
