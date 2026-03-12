/*
 * application.c
 *
 *  Created on: 2023年6月20日
 *      Author: Admin
 */
#include "application.h"
#include "base.h"
#include "GUI.h"
#include "display.h"
#include "port.h"

static uint8 switch_state_last = 0xFFU;
static GUI_DisplayModeEnum g_display_mode = GUI_MODE_ALL_OFF;

/*
 * Purpose: GUI模块初始化入口
 * Param  : 无
 * Return : 无
 * Note   : 仅在系统启动阶段调用一次
 */
void GUI_Init()//GUI初始化函数,放在while循环之前
{
    UI_init();   //UI初始化函数，获取可调参数的相关信息
    //TSUI_FlashSave(); //保存所有在调参列表上的参数，首次烧录或改变参数列表时，此时flash无数据时使用此函数，烧完之后记得注释掉这行再重新烧录一遍
    //TSUI_FlashRead();   //读取调参列表中的参数（此函数会覆盖掉之前参数初始化函数给调参列表上的参数赋的值）（即调参列表之外的参数用参数初始化函数赋的值，调参列表上的用内部flash的值）

}
//扫描函数，放在定时器中断里
/*
 * Purpose: 扫描用户输入并驱动UI状态机
 * Param  : 无
 * Return : 无
 * Note   : 建议在固定周期中断中调用
 */
void GUI_scan()
{
    key_scan(); //按键扫描函数
    ec11_scan();
    TSUI_ButtonEvent();//由扫描函数获得的数据计算当前的状态
}

/*
 * Purpose: 同步PB0/PB1开关状态到UI控制结构
 * Param  : 无
 * Return : 无
 * Note   : 状态变化时清屏，首次同步仅记录状态
 */
void GUI_UpdateSwitchState()
{
    uint8 gui_sw = (gpio_get_level(GUI_SWITCH_PIN) == GUI_SWITCH_ENABLE_LEVEL) ? 1U : 0U;
    uint8 img_sw = (gpio_get_level(IMG_SWITCH_PIN) == IMG_SWITCH_ENABLE_LEVEL) ? 1U : 0U;
    uint8 switch_state = (uint8)((gui_sw << 1) | img_sw);

    g_display_mode = (GUI_DisplayModeEnum)switch_state;

    tsui.ui_enable = (g_display_mode == GUI_MODE_PARAM_ONLY) ? 1U : 0U;
    tsui.img_enable = (g_display_mode == GUI_MODE_IMAGE_AND_MAIN_INFO) ? 1U : 0U;

    if((0xFFU != switch_state_last) && (switch_state != switch_state_last))
    {
        ips200_clear(RGB565_WHITE);
    }

    switch_state_last = switch_state;
}

GUI_DisplayModeEnum GUI_GetDisplayMode(void)
{
    return g_display_mode;
}

/*
 * Purpose: GUI显示刷新入口
 * Param  : 无
 * Return : 无
 * Note   : 建议在主循环调用，避免放在中断里
 */
void GUI_Display()//显示函数，此函数要放在main函数中的while内
{
    if(g_display_mode == GUI_MODE_PARAM_ONLY)
    {
        show_changeable();//GUI显示函数
        //ips_display_page();
    }
    else if(g_display_mode == GUI_MODE_IMAGE_AND_MAIN_INFO)
    {
        ips_show_img();
    }
    else
    {
        // GUI_MODE_ALL_OFF 和 GUI_MODE_RESERVED 均不显示
    }
}

int test=0;//临时测试UI变量，后期可全部删除
int test2=2;
int test3=3;
int test4=4;
int test5=5;
int test6=6;
int test7=7;
int test8=8;
int test9=9;
int test10=10;

struct paralist_s far paralist2[100]=   //可调参数列表
{
   //格式： {参数地址,参数名称,调节精度}, 注：1.参数只能是整形参数 2.参数名称只能是字符串且最多16个字符 
	//3.调节精度最小为1  4.受到多种原因的限制，最多只能同时调100个参数（序号0—99）
    //{&tsui.img_showmode,"img_showmode",1},            //显示屏图像显示模式
    //{&tsui.flag_page,"flag_page",1},                  //静态参数页数
	{&test,"test",1}, 			//测试变量
	{&test2,"test2",1}, 			//测试变量
	{&test3,"test3",1}, 			//测试变量
	{&test4,"test4",1}, 			//测试变量
	{&test5,"test5",1}, 			//测试变量
	{&test6,"test6",1}, 			//测试变量
	{&test7,"test7",1}, 			//测试变量
	{&test8,"test8",1}, 			//测试变量
	{&test9,"test9",1}, 			//测试变量
	{&test10,"test10",1}, 			//测试变量
    {0} //这个{0}作为结尾，不要删
};
struct paralist_s far paralist[100]=   //可调参数列表
{
    {&tsui.img_showmode,"img_showmode",1},            //显示屏图像显示模式
    {&tsui.flag_page,"flag_page",1},                  //静态参数页数
	{&setpara.steer_adjust,"steer_adjust",1}, 			//测试变量
	{&test2,"test2",1}, 			//测试变量
	{&test3,"test3",1}, 			//测试变量
	{&test4,"test4",1}, 			//测试变量
	{&test5,"test5",1}, 			//测试变量
	{&test6,"test6",1}, 			//测试变量
	{&test7,"test7",1}, 			//测试变量
	{&test8,"test8",1}, 			//测试变量
	{&test9,"test9",1}, 			//测试变量
	{&test10,"test10",1}, 			//测试变量
     {0}//这个{0}作为结尾，不要删
};
void GUI_Process(uint16 fps)
{
    GUI_UpdateSwitchState();
    GUI_Display();

    if(g_display_mode == GUI_MODE_IMAGE_AND_MAIN_INFO)
    {
        ips200_show_int32(30, 140, (int32)fps, 4);
        ips200_show_int32(40, 155, (int32)mycar.original_err, 4);
        ips200_show_int32(40, 170, (int32)mycar.steer_pwm, 4);
        ips200_show_int32(40, 185, (int32)watch.cross_state, 4);
        /*ips200_show_int32(80, 220, (int32)watch.cross, 4);
        ips200_show_int32(80, 240, (int32)watch.black_obstacle_flag, 4);
        ips200_show_int32(80, 260, (int32)watch.right_black, 4);
        ips200_show_int32(80, 280, (int32)watch.InLoop, 4);
        ips200_show_int32(160, 140, (int32)watch.InLoopAngleL, 4);
        ips200_show_int32(160, 160, (int32)watch.InLoopAngleR, 4);*/

        ips200_show_string(0, 155, "o_err");
        ips200_show_string(0, 170, "s_pwm");
        ips200_show_string(0, 185, "cro_st");
    }
}

