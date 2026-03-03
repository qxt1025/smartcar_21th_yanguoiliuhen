/*
 * display.h
 *
 *  Created on: 2023年6月20日
 *      Author: Admin
 */

#ifndef CODE_GUI_DISPLAY_H_
#define CODE_GUI_DISPLAY_H_
#include "zf_common_headfile.h"
#include "GUI.h"
#include "application.h"

//UI界面显示设置
#define IPS_display_max_page 6
#define variable_length_max 19
#define display_length_max 31
#define display_top_row 11
#define display_bottom_row 18
#define display_adjust_row 14
#define display_leftmost_column 0
#define flag_leftmost_column 189


//标记变量显示
struct display_flag
{
    char line1[30];
    char line2[30];
    char line3[30];
    char line4[30];
    char line5[30];
    char line6[30];
    char line7[30];
    char line8[30];
    char line9[30];
    char line10[30];
    char line11[30];
    char line12[30];
    char line13[30];
    char line14[30];
	char line15[30];
};


void show_changeable();
void ips_show_img();
void ips200_draw_horizon(int16 xs, int16 ys, int16 xe, int16 ye, int16 color);
void ips200_draw_vertical(int16 xs, int16 ys, int16 xe, int16 ye, int16 color);
void IPS_ShowEdge2(uint8 *p,uint8 *q, uint16 width, uint16 height);
extern struct display_flag far page[IPS_display_max_page];
void ips_display_page();

#endif /* CODE_GUI_DISPLAY_H_ */
