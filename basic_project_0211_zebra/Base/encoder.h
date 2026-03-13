/*
 * ecoder.h
 *
 *  Created on: 2023年2月10日
 *      Author: Admin
 */
#include "zf_common_headfile.h"
#ifndef CODE_BASE_ENCODER_H_
#define CODE_BASE_ENCODER_H_
#include "base.h"
typedef struct
{
    int left_speed;    //左轮原始速度
    int right_speed;    //右轮原始速度
    float left_arrange_speed; //左轮滑动平均滤波后的速度
    float right_arrange_speed;//右轮滑动平均滤波后的速度

}encoder_STRUCT;
extern encoder_STRUCT encoder;
void encoder_init();
void encoder_quad_force_recover(void);
float encoder_get_speed_Left();
float encoder_get_speed_Right();
float moving_average_filtre_right_speed(float xn);
float moving_average_filtre_left_speed(float xn);
float speed_Kalman_Filter(float speed_measure,float accel_x);
float speed_Kalman_Filter2(float present_speed);
float left_speed_Kalman_Filter(float speed_measure,float accel_x,float gyroz);
float right_speed_Kalman_Filter(float speed_measure,float accel_x,float gyroz);


#endif /* CODE_BASE_ENCODER_H_ */
