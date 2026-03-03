/*
 * slope.c
 *
 *  Created on: 2023年7月10日
 *      Author: Admin
 */
#include "slope.h"
//进入坡道函数

int slope_count;

void slope_enter()
{
    //坡道前减速
    if(Element==NONE){
    if(watch.tof_dis<setpara.slope_tof)
	{
        slope_count++;
    }
	else
		slope_count=0;
	if(slope_count>=3){
        //set_speed(setpara.slope_speed);
        watch.slope_flag=1;
		watch.slope_time=80;
        }
    else if(--watch.slope_time<=0)
	{
		watch.slope_time=0;
        if(watch.slope_flag==1)
		{
            mycar.speed_ctrl=1;
            watch.slope_flag=0;
        }
    }
    }
    if(mpu.pitch>setpara.slope_ang&&mycar.RUNTIME>setpara.slope_begin_time)//因陀螺仪位置不同修改
    {
        clear_all_flags();
        enter_element(Slope);
        //set_speed(setpara.slope_speed);
        watch.slope_flag=2;
        watch.angle_far_line=50;
        begin_distant_integeral(setpara.slope_dis*100);
        //beep2(4,100);
    }
}
//下坡函数
void slope_down()
{
    if((mpu.pitch<0||get_integeral_state(&distance_integral)==2)&&watch.slope_flag==2)//因陀螺仪位置不同修改
    {
        watch.angle_far_line=setpara.far_line;
        watch.slope_flag=3;
        begin_distant_integeral(setpara.slope_dis*100+2000);
    }

}
//出斜坡函数
void slope_out()
{
    if(watch.slope_flag==3&&get_integeral_state(&distance_integral)==2)
    {
        mpu.pitch=0;
        out_element();
		
    }
}


