/*
 * ADRC.h
 *
 *  Created on: 2024年3月29日
 *      Author: hwj
 */

#ifndef CODE_CONTROL_ADRC_H_
#define CODE_CONTROL_ADRC_H_

#include "global.h"

typedef struct ADRC_STRUCT
{
        // TD para
        float r_ratio;
        float h_ratio;

        // ESO para
        //float b;//系统系数
        float delta_ratio;//delta为fal（e，alpha，delta）函数的线性区间宽度

        float belta01_ratio;//扩张状态观测器反馈增益1
        float belta02_ratio;//扩张状态观测器反馈增益2
        float belta03_ratio;//扩张状态观测器反馈增益3
        float w0_ratio;//扩张状态观测器带宽
        float b_ratio;
//        float w=0.7;
//        float belta01;//扩张状态观测器反馈增益1
//        float belta02;//扩张状态观测器反馈增益2
//        float belta03;//扩张状态观测器反馈增益3

        // NLSEF para
        float a1_ratio;
        float a2_ratio;
        float kp_ratio;//跟踪输入信号增益
        float kd_ratio;//跟踪微分信号增益


    ADRC_para_STRUCT* ADRC_para;

}ADRC_STRUCT;

extern ADRC_STRUCT  cam_adrc;     //电磁转向PID

void ADRC_init();
float sign(float x);
float fsg(float x,float d);
float fhan(float x1,float x2,float r,float h);
float fal(float e,float alpha,float delta);
float ADRC(ADRC_STRUCT* ADRC,float y,float v);
void ADRC_TD(ADRC_STRUCT* ADRC,float target,float*err,float*d_err);


#endif /* CODE_CONTROL_ADRC_H_ */
