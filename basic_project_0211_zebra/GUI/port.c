/*
 * port.c
 *
 *  Created on: 2023年6月20日
 *      Author: Admin
 */
#include "port.h"
#include "GUI.h"

extern uint16 ips200_x_max;
extern uint16 ips200_y_max;


/*******与flash相关的接口函数**********************************************************/
//改成自己的工程中对应的显示函数
//----------------------------------------------------------------------
//flash 擦除一页的全部内容的函数
// 参数说明    页码数
//----------------------------------------------------------------------
//如果实在不会移植flash可以把下面这三个函数的内容全注释掉，即不使用flash存储功能（注意不是注释整个函数，只注释其内容）
void GUI_flash_erase_page(u32 page_num)
{
   // flash_erase_page(0,page_num);
}

//----------------------------------------------------------------------
//flash 读取一页内容的函数
// 参数说明   page_num：页码数
// 参数说明   buf：读取后数据所存放的地址（数组地址）
// 参数说明   len：数据数量
//----------------------------------------------------------------------
void GUI_flash_read_page( u32 page_num, u32 *buf, u16 len)
{
   // flash_read_page(0,page_num,buf,len);
}

//----------------------------------------------------------------------
//flash 写入一页内容的函数
// 参数说明   page_num：页码数
// 参数说明   buf：要写入的数据所存放的地址（数组地址）
// 参数说明   len：数据数量
//----------------------------------------------------------------------
void GUI_flash_write_page(u32 page_num, const u32 *buf, u16 len)
{
    //flash_write_page (0,page_num,buf,len);
}

///***********************************************************************************/

/*******显示函数相关的接口函数***********************************************************/
//改成自己的工程中对应的显示函数
//----------------------------------------------------------------------
// 字符串显示函数
// 参数说明     x               坐标x方向的起点，单位为一像素
// 参数说明     y               坐标y方向的起点，单位为一行
// 参数说明     dat             需要显示的字符串
//----------------------------------------------------------------------
void GUI_display_string(u16 x,u16 y,char dat[])//x的单位为像素，y的单位为一行，千万不要弄错
{
    u16 px_x = x;
    u16 px_y = (u16)(y * 12);
    u16 i = 0;
    if(px_x >= ips200_x_max || px_y >= ips200_y_max)
    {
        return;
    }
    while(dat[i] != 0)
    {
        if((u16)(px_x + 7) >= ips200_x_max)
        {
            break;
        }
        ips200_show_char(px_x, px_y, dat[i]);
        px_x = (u16)(px_x + 8);
        i++;
    }
}

/***********************************************************************************/
//按键扫描函数
/*
 * Purpose: 扫描按键并生成GUI事件
 * Param  : 无
 * Return : 无
 * Note   : 本函数应放在定时中断中周期调用
 */
void key_scan()
{
    static uint8 key_press;            // 按键状态：0松开 1按下
    static uint16 key_press_time;      // 当前按键持续按下计数
    static uint16 key_interval_time;  // 双击判定窗口计数（单位：扫描周期）
    if(key_press==0&&(KEY1_PIN==0||KEY2_PIN==0||KEY3_PIN==0||PRESS_PIN==0))
	{
       //GUI_delay_ms(1);
		key_press=1;//检测到有按钮压下
		if(KEY1_PIN==0)
		{
			//tsui.buttonevent=UP;          //发送指令
			tsui.buttonevent=CCW;
        }
		else if(KEY2_PIN==0)
        {
            //tsui.buttonevent=DOWN;        //发送指令
            tsui.buttonevent=CW;
        }
		else if(KEY3_PIN==0)
        {
            //tsui.buttonevent=OK;          //发送指令
            tsui.buttonevent=PRESS;
        }
		else if(PRESS_PIN==0)
		{
           tsui.buttonevent=PRESS;   //发送指令
//         if(mycar.car_running==1)car_stop();
//         adjust.take_picture=1;
           if(key_interval_time>0)
           {
			   tsui.buttonevent=Press2;
               key_interval_time=0;
           }
           else if(key_interval_time==0)
		   {
			   key_interval_time=500;
		   }
		}
	}
    else if(key_press==1)
	{
       if(KEY1_PIN==0)//如果key1处于按下状态
       {
           key_press_time++;            //不断累加
           if(key_press_time>long_press_time)//按下超过一定时间
           {
               TSUI_FlashSave();                    //存储参数
               tsui.buttonevent=PRESS;              //发送指令，表示储存成功（这是方便我们看是否存储成功，对单片机来说没什么意义）
           }
       }
       else if(KEY2_PIN==0)//如果key2处于按下状态
       {
           key_press_time++;            //不断累加
           if(key_press_time>long_press_time)//按下超过一定时间
           {
               TSUI_FlashSave();                    //存储参数
               tsui.buttonevent=PRESS;              //发送指令，表示储存成功（这是方便我们看是否存储成功，对单片机来说没什么意义）
           }
       }
       else if(KEY3_PIN==0)//如果key3处于按下状态
       {
           key_press_time++;            //不断累加
           if(key_press_time==long_press_time)//按下超过一定时间
           {
               TSUI_FlashSave();                    //存储参数
               tsui.buttonevent=PRESS;              //发送指令，表示储存成功（这是方便我们看是否存储成功，对单片机来说没什么意义）
           }
       }
       else if(PRESS_PIN==0)//如果处于按下状态
       {
           key_press_time++;            //不断累加
           if(key_press_time==long_press_time)//按下超过一定时间
           {
                //flag.Frame_Display_Flag ^= 1;
                //flag.Reinit_flag = 1;
                //Reinit();
                tsui.paraStatus = 0;
            //    tsui.buttonevent=PRESS;    //发送指令，表示储存成功（这是方便我们看是否存储成功，对单片机来说没什么意义）
               //car_start_warning();     //先蜂鸣器警告再发车
               //car_begin();         //发车指令
           }
       }
       else                                 //按键松开
       {
           key_press=0;                     //清除检测按键是否按下的变量
           key_press_time=0;                //清除计时器
       }
   }
    if(key_interval_time!=0)
	{
		key_interval_time--;
	}
}
//ec11扫描函数，放在定时器中断里
/*
 * Purpose: 扫描EC11相位并生成旋转事件
 * Param  : 无
 * Return : 无
 * Note   : 仅做电平采样和事件置位，避免中断中重计算
 */
void ec11_scan()
{
    static uint8 CW_PIN_level,CCW_PIN_level,CW_PIN_level_last,CCW_PIN_level_last; // A/B相当前电平与上一周期电平
    CW_PIN_level=CW_PIN;
    CCW_PIN_level=CCW_PIN;
    if(CW_PIN_level==0&&CW_PIN_level_last==1)
    {
        if(CCW_PIN_level==1)
		{
			tsui.buttonevent=CCW;
		}
        else if(CCW_PIN_level==0)
		{
			tsui.buttonevent=CW;
		}
    }
	else if(CW_PIN_level==1&&CW_PIN_level_last==0)
    {
        if(CCW_PIN_level==0)
		{
			tsui.buttonevent=CCW;
		}
        else if(CCW_PIN_level==1)
		{
			tsui.buttonevent=CW;
		}
    }
    CW_PIN_level_last=CW_PIN_level;
    CCW_PIN_level_last=CCW_PIN_level;
}



