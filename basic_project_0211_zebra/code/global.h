/*
 * global.h
 *
 *  Created on: 2023年6月20日
 *      Author: Admin
 */

#ifndef CODE_GLOBAL_H_
#define CODE_GLOBAL_H_
#include "zf_common_headfile.h"
#define MYCAR   1
typedef unsigned char         u8;                                               // 无符号  8 bits
typedef unsigned short        u16;                                              // 无符号 16 bits
typedef unsigned long         u32;                                              // 无符号长整形 32bits
#define _LIMIT(data, min, max) (((int)data) < (min) ? (min) : ((data) > (max) ? (max) : (data)))
#define _MIN(x, y) (x) < (y) ? (x) : (y)
#define abs_m(a,b) (a>=b ? a-b:b-a)
#define abs_f(a) (a>0?a:-a)
//typedef struct PID_para_STRUCT
//{
//   int kp;
//   int ki;
//   int kd;
//}PID_para_STRUCT;

//typedef struct ADRC_para_STRUCT
//{
//// TD para
//   int r;
//   int h;

//// ESO para
//   int b;//系统系数
//   int delta;//delta为fal（e，alpha，delta）函数的线性区间宽度

//   int belta01;//扩张状态观测器反馈增益1
//   int belta02;//扩张状态观测器反馈增益2
//   int belta03;//扩张状态观测器反馈增益3
//   int w0;//扩张状态观测器带宽

//// int w=0.7;
//// int belta01;//扩张状态观测器反馈增益1
//// int belta02;//扩张状态观测器反馈增益2
//// int belta03;//扩张状态观测器反馈增益3

//// NLSEF para
//   int a1;
//   int a2;
//   int kp;//跟踪输入信号增益
//   int kd;//跟踪微分信号增益
//}ADRC_para_STRUCT;

extern float tim;
extern float cam_original_err;//摄像头原始偏差
#include "mycar.h"
#include "Element_recognition.h"
//#include "adjust.h"
#include "base.h"


#endif /* CODE_GLOBAL_H_ */