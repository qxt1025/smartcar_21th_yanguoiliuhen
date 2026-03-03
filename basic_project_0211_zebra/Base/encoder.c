/*
 * ecoder.c
 *
 *  Created on: 2023年2月10日
 *      Author: Admin
 */
#include "encoder.h"
//测量得23532个脉冲走了2m
//折合每一千个脉冲8.5厘米
//每个脉冲约 8.6x10^-5 米
//可以配合定时器中断计算出实际速度
//编码器初始化
encoder_STRUCT encoder;
void encoder_init()
{
      encoder_quad_init(PWMA_ENCODER, PWMA_ENCODER_CH1P_P60, PWMA_ENCODER_CH2P_P62);   // 初始化编码器模块与引脚 带方向增量编码器模式
    encoder_quad_init(PWMC_ENCODER, PWMC_ENCODER_CH1P_P40, PWMC_ENCODER_CH2P_P42);   // 初始化编码器模块与引脚 带方向增量编码器模式
}
//读取编码器的数据（包含滑动平均滤波）单位：m/s
float encoder_get_speed_Left()
{
    int16 speed;
    static float speed_least;
    speed=encoder_get_count(PWMA_ENCODER);
    encoder_clear_count(PWMA_ENCODER);
    speed_least=moving_average_filtre_left_speed(speed);
//    speed_least=left_speed_Kalman_Filter(speed,-0.10*imu.accx,imu.gyroz);
//    speed_least=speed;
    return speed_least;
}

//读取编码器的数据（包含滑动平均滤波）
float encoder_get_speed_Right()
{
    static float speed_least;
    int16 speed;
    speed=-encoder_get_count(PWMC_ENCODER);
    encoder_clear_count(PWMC_ENCODER);
    speed_least=moving_average_filtre_right_speed(speed);
//    speed_least=right_speed_Kalman_Filter(speed,-0.10*imu.accx,imu.gyroz);
//    speed_least=speed;
    return speed_least;
}

//速度滑动平均滤波函数
#define MVF_LENGTH_present_speed 10 /*滑动平均滤波器长度*/
float moving_average_filtre_left_speed(float xn)
{
  static int index = -1;
  static float buffer[MVF_LENGTH_present_speed];
  static float sum = 0;
  float yn = 0;
  int i = 0;
  if(index == -1)
  {
    //初始化
    for(i = 0; i <MVF_LENGTH_present_speed; i++)
    {
      buffer[i] = xn;
    }
    sum = xn*MVF_LENGTH_present_speed;
    index = 0;
  }
  else
  {
    sum -= buffer[index];
    buffer[index] = xn;
    sum += xn;
    index++;
    if(index >= MVF_LENGTH_present_speed)
    {
      index = 0;
    }
  }
  yn = sum/MVF_LENGTH_present_speed;
  return yn;
}

float moving_average_filtre_right_speed(float xn)
{
  static int index = -1;
  static float buffer[MVF_LENGTH_present_speed];
  static float sum = 0;
  float yn = 0;
  int i = 0;
  if(index == -1)
  {
    //初始化
    for(i = 0; i <MVF_LENGTH_present_speed; i++)
    {
      buffer[i] = xn;
    }
    sum = xn*MVF_LENGTH_present_speed;
    index = 0;
  }
  else
  {
    sum -= buffer[index];
    buffer[index] = xn;
    sum += xn;
    index++;
    if(index >= MVF_LENGTH_present_speed)
    {
      index = 0;
    }
  }
  yn = sum/MVF_LENGTH_present_speed;
  return yn;
}
//速度融合
void speed_fusion(float speed_measure,float accel_x)
{

}
/************************卡尔曼滤波**************************************/
float best_estimate_value=10;//最优估计值
float best_estimate_err=2;  //最优估计偏差
float Kalman_K;           //K值
#define encoder_err 2    //编码器误差
float speed_Kalman_Filter(float speed_measure,float accel_x)
{
    Kalman_K=best_estimate_err/(best_estimate_err+encoder_err);
    best_estimate_value=best_estimate_err+Kalman_K*(speed_measure-best_estimate_value);
    best_estimate_err=(1-Kalman_K)*best_estimate_err;
    return best_estimate_value;
}

float X_pre,P_pre,X_kalman_last,P_kalman_last,X_kalman=10,P_kalman=2 , speed_ec;   //赋初值
float H , Q = 0.001 , R = 0.5;
#define INTEGRAL_RANGE 1000
float speed_Kalman_Filter2(float present_speed)
{
    P_kalman_last = P_kalman;
    X_kalman_last = X_kalman;      //获取上一个X
    X_pre = X_kalman_last;
    P_pre = P_kalman_last + Q;
    H = P_pre / (P_pre + R);
    P_kalman = P_pre - H * P_pre;
    X_kalman = X_pre + H * (present_speed - X_pre);
    speed_ec = X_kalman - X_kalman_last;
    return X_kalman;
    //indata.ad_pid_out = setpara.ad_kp * X_kalman * 0.1f + setpara.ad_kd * speed_ec * 0.1f ;
}

/************************卡尔曼滤波**************************************/
#define speed_var  6    //速度测量方差
#define acc_var 0.4       //加速度测量方差
#define car_wide 2*3.1415926/360*10/2/1000      //车宽
float left_speed_predict=0;//预测速度
float left_predict_var=0;//预测速度方差
float left_speed_Kalman_Filter(float speed_measure,float accel_x,float gyroz)
{
    left_speed_predict=left_speed_predict+accel_x-car_wide*gyroz;
    left_predict_var+=acc_var*acc_var;//更新预测数据的方差
    left_speed_predict = left_speed_predict * speed_var / (left_predict_var + speed_var) + speed_measure * left_predict_var / (left_predict_var + speed_var);
    left_predict_var = (left_predict_var * speed_var) /((left_predict_var + speed_var) *(left_predict_var + speed_var));
    return left_speed_predict;
}
float right_speed_predict=0;//预测速度
float right_predict_var=0;//预测速度方差
float right_speed_Kalman_Filter(float speed_measure,float accel_x,float gyroz)
{
    right_speed_predict=right_speed_predict+accel_x+car_wide*gyroz;
    right_predict_var+=acc_var*acc_var;//更新预测数据的方差
    right_speed_predict = right_speed_predict * speed_var / (right_predict_var + speed_var) + speed_measure * right_predict_var / (right_predict_var + speed_var);
    right_predict_var = (right_predict_var * speed_var) /((right_predict_var + speed_var) *(right_predict_var + speed_var));
    return right_speed_predict;
}
