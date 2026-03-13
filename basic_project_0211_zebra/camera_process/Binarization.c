/*
 * Binarization.c
 *
 *  Created on: 2023年6月21日
 *      Author: Admin
 */
#include "Binarization.h"
#include "Element_recognition.h"
#include "scan_line.h"
#include <string.h>

//uint8 Grayscale[120][188];	//灰度图
//uint8 imo[120][188]; 		//处理后的图像
//uint8 img_threshold[12];

//struct caminfo_s caminfo;
//struct dir_control_struct cardir;
//struct watch_o watch;
//struct FLAG_STRUCT flag;

//大津法
#define _GRAY_SCALE 64
#define _SHIFT_FOR_GRAYSCALE 10
#define _Thresh_Mult 2 //1/2*(256/_GRAY_SCALE)

//大津法，计算二值化阈值
int img_otsu(uint16 *img, uint8 img_v, uint8 img_h, uint8 step)
{
    uint8 grayhist[_GRAY_SCALE]; //灰度直方图
    uint16 px_sum_all;           //像素点总数
    uint32 gray_sum_all;         //总灰度积分
    uint16 px_sum;               //像素点数量
    uint32 gray_sum;             //灰度积分

    float fTemp_maxvar;
    uint8 temp_best_th;
    uint8 temp_best_th2;
    uint8 temp_this_pixel;
    float fCal_var;
    float u0;
    float u1;
    float w0;
    float w1;
    uint8 i;
    uint8 j;
    uint8 k;

    memset(grayhist, 0, sizeof(grayhist));
    px_sum_all = 0;
    gray_sum_all = 0;
    px_sum = 0;
    gray_sum = 0;

    fTemp_maxvar = 0;
    temp_best_th = 0;
    temp_best_th2 = 0;
    temp_this_pixel = 0;

    //生成：1. 灰度直方图 2. 像素点总数 3. 总灰度积分
    for (i = 0; i < img_v; i += step)
    {
        for (j = 0; j < img_h; j += step)
        {
            temp_this_pixel = (uint8)(img[i * img_h + j] >> _SHIFT_FOR_GRAYSCALE);
            if (temp_this_pixel < _GRAY_SCALE)
            {
                grayhist[temp_this_pixel]++;
            }
            gray_sum_all += temp_this_pixel;
            px_sum_all++;
        }
    }

    //迭代求得最大类间方差的阈值
    for (k = 0; k < _GRAY_SCALE; k++)
    {
        px_sum += grayhist[k];       //该灰度及以下的像素点数量
        gray_sum += k * grayhist[k]; //该灰度及以下的像素点的灰度和
        w0 = 1.0f * px_sum / px_sum_all;
        w1 = 1.0f - w0;
        if (px_sum > 0)
        {
            u0 = 1.0f * gray_sum / px_sum;
        }
        else
        {
            u0 = 0.0f;
        }
        if (px_sum_all - px_sum > 0)
        {
            u1 = 1.0f * (gray_sum_all - gray_sum) / (px_sum_all - px_sum);
        }
        else
        {
            u1 = 0.0f;
        }

        fCal_var = w0 * w1 * (u0 - u1) * (u0 - u1);
        if (fCal_var > fTemp_maxvar)
        {
            fTemp_maxvar = fCal_var;
            temp_best_th = k;
            temp_best_th2 = k;
        }
        else if (fCal_var == fTemp_maxvar)
        {
            temp_best_th2 = k;
        }
    }

    return (temp_best_th + temp_best_th2) * _Thresh_Mult;
}

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