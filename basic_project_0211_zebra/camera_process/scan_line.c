/*
 * scan_line.c
 *
 *  Created on: 2023年6月21日
 *      Author: Admin
 */

#include "scan_line.h"
#include "Element_recognition.h"

struct lineinfo_s far lineinfo[120];
#define LINE_WIDTH 188

void scan_line()
{/*
    //watch.base_line = 0;
    int y=0;
    memset(lineinfo, 0, 120 * sizeof(struct lineinfo_s));
    line_single(&lineinfo[base_line], Grayscale[119-base_line]);   // 寻找基准行

    for (y = base_line-1; y >= forward_near; y--) // 向下搜线到near行
    {
        line_findnext(&lineinfo[y], Grayscale[119-y], &lineinfo[y + 1]);
    }

    for (y = base_line+1; y < forward_far; y++) // 向上搜线到far行
    {
        lineinfo[y].y = y;
        line_findnext(&lineinfo[y], Grayscale[119-y], &lineinfo[y - 1]);
        watch.watch_line = y;//摄像头视野的赛道最远端
    }
    //line_findnext(&lineinfo[base_line], Grayscale[119-base_line], &lineinfo[base_line - 1]);*/
}

//斑马线识别函数
int zebra_detect(struct lineinfo_s *lineinfo, uint8_t *edge_store, uint8_t *inputimg)
{/*
    uint8_t white_width, zobra_white_count, edge_now;
    uint8_t k;
    zobra_white_count = 0;
    if (lineinfo->edge_count > 12)
    {
		for (k = 0; k < lineinfo->edge_count; k += 2)
		{
			edge_now = edge_store[k];
			if (edge_now < LINE_WIDTH)
			{
				if ((inputimg[edge_now] & 0x80) == 0x80)
				{
					white_width = edge_store[k + 1] - edge_store[k];
					if (white_width > 1 && white_width < 14)
					{
						zobra_white_count++;
						watch.ZebraInLine=lineinfo->y;//取满足条件时y最大值作为ZebraLine
					}                 //watch.ZebraInLine为看到的斑马线最远端
				}
			}
		}
		if (zobra_white_count > 6)
		{
			lineinfo->zebra_flag = 1;
			//beep(20);
			// if(!mycar.status)
			//ips200_draw_horizon(0, 119 - lineinfo->y, 188, 119 - lineinfo->y, BLUE);
		}
	}
    else
    {
        lineinfo->zebra_flag = 0;
    }*/
    return 0;
}





