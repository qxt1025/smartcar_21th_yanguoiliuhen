/*
 * timer1_pwm.c
 *
 *  Created on: 2023年6月21日
 *      Author: Admin
 */
#include "timer.h"

uint16 adjust_time4_count = 0; // interrupt count
uint8 adjust_fps = 0;          // fps result
uint8 adjust_fps1 = 0;         // fps result
uint8 adjust_fps_count = 0;    // fps counter
uint8 adjust_fps_count1 = 0;   // fps counter

void timer1_Call_Back()
{
    //printf("timer1 interrupt\r\n");
    GUI_scan();     //按键扫描
    //imu_get_data();//获取陀螺仪数据
    //beeping();//蜂鸣器计时
    //beep_warning(music);//蜂鸣器警告计时
    imu.pitch+=imu.gyroy*0.002;//角度积分
//    imu.pitch+=imu.gyrox*0.002;//角度积分

////  mpu_get_data();
    angal_integeral(imu.gyroz*0.001);//角度积分函数
//    mycar.imu_speed+=imu.accx*0.001;
    //dir_control();//舵机控制
    motor_control();//电机控制
//    timer1_adjust();//调试代码
    //dir_control();//舵机控制

    adjust_time4_count++;//中断计次
    if(adjust_time4_count==10000)adjust_time4_count=0;
    if(adjust_time4_count%500==0)//每秒执行一次
    {
        //printf("timer1 interrupt\r\n");
        //get_ad_value();
        if(Element!=Slope)
        {
            imu.pitch=0;
        }

    }
    if(adjust_time4_count%1000==0)//每秒执行一次
      {
          adjust_fps=adjust_fps_count;//计算帧率
          adjust_fps1=adjust_fps_count1;
          adjust_fps_count=0;
          adjust_fps_count1=0;
      }

    get_distance_mm();
}

void timer2_Call_Back()
{
//    get_current_value();
//    current_control();

}
