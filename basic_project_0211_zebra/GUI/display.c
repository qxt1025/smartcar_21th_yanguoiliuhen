/*
 * display.c
 *
 *  Created on: 2023年6月20日
 *      Author: Admin
 */
#include "display.h"
#include "port.h"
struct display_flag far page[IPS_display_max_page];
char far ips_show_page[20];


char far IPS_Display_string[40];//IPS要显示的字符串

/*
 * Purpose: 在固定宽度区域显示状态字符串
 * Param  : colum/row - 显示坐标；dat - 待显示字符串
 * Return : 无
 * Note   : 自动填充空格避免上一帧残留字符
 */
void ips_display_flag(uint16 colum, uint16 row, const char dat[])
{
    u8 i=19;
    while(i--)
	{
		ips_show_page[i]=' ';                   //清除上次显示在字符串中残留的字符
	}
    sprintf(ips_show_page,dat);
    i=19;
    while(i--)
	{
		if(ips_show_page[i]==0)
		{
			ips_show_page[i]=' ';//清除字符串中的'\0'，防止字符串断开   （sprintf函数会自动写入'\0'）
		}
	}
    ips_show_page[19]=0;                   //填入'\0'，提前截止字符串
    GUI_display_string(colum,row,ips_show_page);         //显示字符串

}

/*
 * Purpose: 显示右侧静态参数页
 * Param  : 无
 * Return : 无
 * Note   : 对页号做边界钳位，防止数组越界
 */
void ips_display_page()
{
    if(tsui.flag_page<0)
	{
		tsui.flag_page=0;
	}
    else if(tsui.flag_page>=IPS_display_max_page)
	{
		tsui.flag_page=(IPS_display_max_page-1);
	}
    //ips200_display_font=IPS200_8X16_FONT;
    //if(flag.Frame_Display_Flag == 1)
    {
        sprintf(ips_show_page,"page %d  ",tsui.flag_page);
        GUI_display_string(flag_leftmost_column,0,ips_show_page);         //显示字符串
		Static_Parameters_display();
        ips_display_flag(flag_leftmost_column, 1, page[tsui.flag_page].line1);
        ips_display_flag(flag_leftmost_column, 2, page[tsui.flag_page].line2);
        ips_display_flag(flag_leftmost_column, 3, page[tsui.flag_page].line3);
        ips_display_flag(flag_leftmost_column, 4, page[tsui.flag_page].line4);
        ips_display_flag(flag_leftmost_column, 5, page[tsui.flag_page].line5);
        ips_display_flag(flag_leftmost_column, 6, page[tsui.flag_page].line6);
        ips_display_flag(flag_leftmost_column, 7, page[tsui.flag_page].line7);
        ips_display_flag(flag_leftmost_column, 8, page[tsui.flag_page].line8);
        ips_display_flag(flag_leftmost_column, 9, page[tsui.flag_page].line9);
        ips_display_flag(flag_leftmost_column, 10, page[tsui.flag_page].line10);
        ips_display_flag(flag_leftmost_column, 11, page[tsui.flag_page].line11);
        ips_display_flag(flag_leftmost_column, 12, page[tsui.flag_page].line12);
        ips_display_flag(flag_leftmost_column, 13, page[tsui.flag_page].line13);
        ips_display_flag(flag_leftmost_column, 14, page[tsui.flag_page].line14);
    }
    
}

void GUI_display_setpara(u8 num,u16 colum,u16 row,char *display_char,int value)
{
    memset(IPS_Display_string,' ',sizeof(IPS_Display_string));//清除上次显示在字符串中残留的字符
    sprintf(IPS_Display_string,"%-3d",num);                 //前三位显示数字，数字格式：至少占3列且向左对齐
    sprintf(IPS_Display_string+3,display_char);             //从第四位开始将参数的名称写入字符串
    IPS_Display_string[strlen(IPS_Display_string)]=' ';//清除字符串中的'\0'，防止字符串断开   （sprintf函数会自动写入'\0'）
    sprintf(IPS_Display_string+4+variable_length_max,"=%-6d",value);//预留出（变量名称最大显示长度）的空间，将变量写入后面的位置 数字格式：至少占6列且向左对齐
    IPS_Display_string[strlen(IPS_Display_string)]=' ';     //清除字符串中的'\0'，防止字符串断开  （sprintf函数会自动写入'\0'）
    IPS_Display_string[display_length_max]=0;                   //填入'\0'，提前截止字符串
    GUI_display_string(colum,row,IPS_Display_string);               //显示字符串
}

/*
 * Purpose: 清空一行参数显示区域
 * Param  : colum/row - 显示坐标
 * Return : 无
 * Note   : 用空格覆盖旧内容，避免滚动残影
 */
void GUI_display_blank_roll(u16 colum,u16 row)
{
    memset(IPS_Display_string,' ',sizeof(IPS_Display_string)); //清除上次显示在字符串中残留的字符
    IPS_Display_string[display_length_max]=0;          //填入'\0'，提前截止字符串
    GUI_display_string(colum,row,IPS_Display_string);               //显示字符串
}

void GUI_display_adjusting(u8 num,u16 colum,u16 row,char *display_char,int value,u8 mode)
{
    memset(IPS_Display_string,' ',sizeof(IPS_Display_string)); //清除上次显示在字符串中残留的字符
    sprintf(IPS_Display_string,"%-3d",num);                 //前三位显示数字，数字格式：至少占3列且向左对齐
    sprintf(IPS_Display_string+3,display_char);             //从第四位开始将参数的名称写入字符串
    IPS_Display_string[strlen(IPS_Display_string)]=' ';//清除字符串中的'\0'，防止字符串断开  （sprintf函数会自动写入'\0'）
    sprintf(IPS_Display_string+4+variable_length_max,"=%-6d",value);//预留出（变量名称最大显示长度）的空间，将变量写入后面的位置数字格式：至少占6列且向左对齐

    IPS_Display_string[strlen(IPS_Display_string)]=' ';//清除字符串中的'\0'，防止字符串断开  （sprintf函数会自动写入'\0'）
    if(!mode)   //参数选择 模式
	{
		IPS_Display_string[2]='[';                          //在相应的位置加入'[' 与 ']'来表示要调的是什么
		IPS_Display_string[3+variable_length_max]=']';
	}
    else       //参数改变 模式
	{
		IPS_Display_string[3+variable_length_max]='[';    //在相应的位置加入'[' 与 ']'来表示要调的是什么
		IPS_Display_string[display_length_max-1]=']';
	}
    IPS_Display_string[display_length_max]=0;                   //填入'\0'，提前截止字符串
    GUI_display_string(colum,row,IPS_Display_string);         //显示字符串
}

/*
 * Purpose: 刷新可调参数窗口
 * Param  : 无
 * Return : 无
 * Note   : 固定中间行为当前选中项，上下行为邻近参数
 */
//UI界面显示函数（显示的是整个界面）
void show_changeable()
{
    int row = 0, num = 0;             // row: 显示行号; num: 参数索引
    int temp_para_select = 0;         // 当前选中参数索引
    u8 para_max = 0;                  // 当前页最大参数索引
    struct paralist_s* adjust_paralist; // 当前页参数列表首地址
    temp_para_select = tsui.paraSelect;
	
    if(tsui.para_page==1)
    {
        adjust_paralist= paralist2;
        para_max=tsui.paraMax2;
    }
    else
    {
        adjust_paralist= paralist;
        para_max=tsui.paraMax;
    }
    for(row=display_top_row;row<=display_bottom_row;row++)  //从顶行到底行，一行行地显示
    {
        num=temp_para_select-(display_adjust_row-row);      //计算出这行显示的参数的序号
        if(row==display_adjust_row)                         //如果这一行正好是要调整的参数
        {
            GUI_display_adjusting(num,display_leftmost_column,row,adjust_paralist[num].label,*adjust_paralist[num].para,tsui.paraStatus);
        }
        else                                                //如果这一行不是要调整的参数
        {
            if(num>para_max||num<0)
			{
				GUI_display_blank_roll(display_leftmost_column,row);
			}
            //如何这行的参数序号小于0或大于参数总数，说明这行没有参数要显示，因此显示一堆空格清除之前显示的内容
            else
			{
				GUI_display_setpara(num,display_leftmost_column,row,adjust_paralist[num].label,*adjust_paralist[num].para);//显示参数
			}
        }
    }

}

//画水平线，相比连续画点更省时间
void ips200_draw_horizon(int16 xs, int16 ys, int16 xe, int16 ye, int16 color)
{
/*    if(ys>=0&&xs>=0&&xe>=0&&ye>=0)
    {
        ips200_set_region(xs, ys, xe, ye);
        for (uint16 i = 0; i < (xe - xs +1); i++)
        {
            ips200_write_16bit_data(color);
        }
    }*/
}

//画竖直线，相比连续画点更省时间
void ips200_draw_vertical(int16 xs, int16 ys, int16 xe, int16 ye, int16 color)
{
/*    if(ys>=0&&xs>=0&&xe>=0&&ye>=0)
    {
    ips200_set_region(xs, ys, xe, ye);
    for (uint16 i = 0; i < (ye - ys + 1); i++)
        ips200_write_16bit_data(color);
    }*/
}

//显示边框
void IPS_ShowEdge(uint8 *p, uint16 width, uint16 height)
{
    int16 i, j;
    uint16 color;

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            if (*(p + j * width + i) == 1)
                color = RGB565_RED;
            else if (*(p + j * width + i) == 2)
                color = RGB565_GREEN;
            else if (*(p + j * width + i) == 3)
                color = RGB565_YELLOW;
            else if (*(p + j * width + i) == 4)
                color = RGB565_GRAY;
            else if (*(p + j * width + i) == 5)
                color = RGB565_BLUE;
            else if (*(p + j * width + i) == 6)
                color = RGB565_WHITE;
            else
                color = RGB565_BLACK;

            ips200_draw_point((uint16)i, (uint16)(height - 1 - j), color);
        }
    }
}

void IPS_ShowEdge2(uint8 *p,uint8 *q, uint16 width, uint16 height)
{
    int16 i, j;
    uint16 color;

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            if (*(p + j * width + i) == 1)
                color = RGB565_RED;
            else if (*(p + j * width + i) == 2)
                color = RGB565_GREEN;
            else if (*(p + j * width + i) == 3)
                color = RGB565_YELLOW;
            else if (*(p + j * width + i) == 4)
                color = RGB565_GRAY;
            else if (*(p + j * width + i) == 5)
                color = RGB565_BLUE;
            else if (*(p + j * width + i) == 6)
                color = RGB565_WHITE;
            else if (*(q + (height - 1 - j) * width + i) > 128)
                color = RGB565_GRAY;
            else
                color = RGB565_BLACK;

            ips200_draw_point((uint16)i, (uint16)(height - 1 - j), color);
        }
    }
}

//图像显示函数
/*
 * Purpose: 根据显示模式输出图像
 * Param  : 无
 * Return : 无
 * Note   : 模式值范围限制为0~4，超界会被钳位
 */

    uint8 y;                                               // 行索引（调试绘图循环）
void ips_show_img()
{
    if(tsui.img_showmode<0)
	{
		tsui.img_showmode=0;
	}
    else if(tsui.img_showmode>4)
	{
		tsui.img_showmode=4;
	}
    //sprintf(page[5].line11,"page=%d",tsui.img_showmode);
    if(1)//显示黑白图
    {
		ips200_displayimage03x(g_frame_img[0], MT9V03X_W, MT9V03X_H);
		for(y = 0; y <= 119; y++)
            {
                //middle_line[y] = (lineinfo[119-y].left_adjust + lineinfo[119-y].right_adjust) / 2;
                ips200_draw_point(lineinfo[119-y].left_adjust, y, RGB565_RED);
                ips200_draw_point(lineinfo[119-y].right_adjust, y, RGB565_GREEN);
                //ips200_draw_point(lineinfo[119-y].persp_lx, 119-lineinfo[119-y].persp_ly, RGB565_RED);
                //ips200_draw_point(lineinfo[119-y].persp_rx, 119-lineinfo[119-y].persp_ry, RGB565_GREEN);
                //ips200_draw_point(reference_col_line[y], y, RGB565_YELLOW);
                //ips200_draw_point(middle_line[y], y, RGB565_BLUE);
            }
       //ips200_show_gray_image(0, 0, (const uint8 *)g_frame_img, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
//       paint_patch_line(0);
    }
    else if(tsui.img_showmode==0)//显示原图
    {
       // ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, 188, 120, 188, 120, 0);
    }
    else if(tsui.img_showmode==2)//显示扫线图
    {
//        ips200_show_gray_image(0, 0, (const uint8 *)Grayscale, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
        //IPS_ShowEdge2((uint8_t *)imo,(uint8_t *)g_frame_img, 188, 120);
       // memset(imo, 0, 120 * 188);
    }
    else if(tsui.img_showmode==3)//显示补线图
    {
//        ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, 188, 120, 188, 120, 0);
        //IPS_ShowEdge2((uint8_t *)imo,(uint8_t *)g_frame_img, 188, 120);
        //memset(imo, 0, 120 * 188);        
    }
    else if(tsui.img_showmode==4)//显示逆透视边线图
    {
        //IPS_ShowEdge((uint8_t *)imo, 188, 120);
        //memset(imo, 0, 120 * 188);
    }
}

