#include "GUI.h"
#include "atk_md0280_touch.h"
//#include "touch.h"

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
	return atk_md0280_touch_get_adc(0XD0);
}

int  GUI_TOUCH_X_MeasureY(void)
{
    return atk_md0280_touch_get_adc(0X90);
}
