#include "GUI.h"
#include "atk_md0280_touch.h"
//#include "touch.h"

//���ļ��ĺ���Ϊ������������emWin�ϲ����

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
