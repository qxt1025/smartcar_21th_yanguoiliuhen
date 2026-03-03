/*
 * Binarization.c
 *
 *  Created on: 2023年6月21日
 *      Author: Admin
 */
#include "Binarization.h"
#include "Element_recognition.h"
#include "scan_line.h"


//uint8 Grayscale[120][188];	//灰度图
//uint8 imo[120][188]; 		//处理后的图像
//uint8 img_threshold[12];

//struct caminfo_s caminfo;
//struct dir_control_struct cardir;
//struct watch_o watch;
//struct FLAG_STRUCT flag;



//图形二值化
void Binarization()
{
    /*char row;
	uint8_t	colum;
    for(row=0;row<120;row++)
    {
        for(colum=0;colum<MT9V03X_W;colum++)
        {
            if(mt9v03x_image[row][colum]<(watch.threshold<<8))
			{
                Grayscale[row][colum]=0;
			}
            else
			{
                Grayscale[row][colum]=255;
			}
        }
    }*/
}

//绘制边界线
void draw_edge()
{
	/*char row;
	uint8_t colum;
    for(row=0;row<120;row++)
    {
        for(colum=0;colum<MT9V03X_W;colum++)
        {
            if(Grayscale[row][colum]==0)
			{
                imo[row][colum]=0;
			}
            else
			{
                imo[row][colum]=255;
			}
        }

    }
	for(row=0;row<120;row++)
    {
		//imo[119-row][lineinfo[row].left]=1;
		//imo[119-row][lineinfo[row].right]=2;
	}*/
}
