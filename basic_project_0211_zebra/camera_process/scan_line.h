/*
 * scan_line.h
 *
 *  Created on: 2023年6月21日
 *      Author: Admin
 */

#ifndef CODE_CAMERA_PROCESS_SCAN_LINE_H_
#define CODE_CAMERA_PROCESS_SCAN_LINE_H_

#include "type_def.h"
#include "zf_common_headfile.h"

#define base_line 				5   	//基础行
#define _EDGE_STORE_SIZE 		22 		//扫描时可容纳的赛道跳变沿数量最大值,必须为偶数
#define _MIN_EDGE_WIDTH 		20  	//绝对最小赛道宽度
#define forward_near 			0      	//最近端
#define forward_far 			115     //最远端
struct lineinfo_s
{
        uint8 y;          //0~119  行数
        uint8 left;       //0~160  本行车道左线所在列数
        uint8 right;      //0~160  本行车道右线所在列数
        uint8 edge_count; //0~160  本行中跳变沿总数
        uint8 edge_store[_EDGE_STORE_SIZE];
	
        uint8 left_adjust; //补线后的左线
        uint8 right_adjust;//补线后的右线
        uint8 left_lost;            //左线丢失
        uint8 right_lost;           //右线丢失
        uint8 left_black;            //左线黑
        uint8 right_black;           //右线黑
        uint8 zebra_flag; //斑马线标志

        int16 persp_lx; //0~255  //本行左线逆透视后x轴坐标
        int16 persp_ly; //0~255  //本行左线逆透视后y轴坐标
        int16 persp_rx; //0~255  //本行右线逆透视后x轴坐标
        int16 persp_ry; //0~255  //本行右线逆透视后y轴坐标

        int16 angel_left;
        int16 angel_right;

        //整行线全部丢失标记变量 1：本行线全部丢失  0：本行线没有丢失
        uint8 whole_lost;
        uint8 whole_black;
};

extern struct lineinfo_s far lineinfo[120];
int zebra_detect(struct lineinfo_s *lineinfo, uint8 *edge_store, uint8 *inputimg);

#endif /* CODE_CAMERA_PROCESS_SCAN_LINE_H_ */
