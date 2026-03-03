/*
 * obstacle.c
 *
 *  Created on: 2023年7月5日
 *      Author: HUAWEI
 */
//避障思路：1.tof检测到障碍物距离小于0.6m时识别为路障（此处可有防误判条件）
//2.函数1：补线，左线为0，右线从左上向右下拉线；同时开启角度积分
//3.函数2：角度积分约30度后停止积分；此时补左右边线为竖直线，开启路程积分
//4.转向积分停止，进行路程积分；积分50cm左右（6000）
//5.关闭积分，开启第三段补线右转，并开启角度积分
//6.角度积分停止，小车此时应回到赛道中央恢复巡线模式
//吴双:
#include "obstacle.h"
//断路准备函数
int cnt;
//1.tof检测距离0.8m识别为路障
void obstacle_identification()
{
	if(watch.obs_flag!=0/*&&watch.obs_flag!=1*/)return;
	watch.black_cnt = 0;
	watch.lost_cnt = 0;
	for(int y = 5;y<115;y++)
	{
		if((lineinfo[y].left_lost&&lineinfo[y].right!=0)||lineinfo[y].right_lost)
			watch.lost_cnt++;
		if(lineinfo[y].whole_black)
			watch.black_cnt++;
	}
	for(int y = watch.watch_lost;y >= 25;y--)
	{
		if(
			watch.tof_dis!=8888
		   &&watch.tof_dis <= (setpara.obstacle_detect_distance)
		   &&(watch.watch_lost <= 100 || watch.black_cnt>=5)
		   &&watch.lost_cnt<=17 
		   &&!lineinfo[y].left_lost&&!lineinfo[y].right_lost&&
             !lineinfo[y-5].left_lost&&!lineinfo[y-5].right_lost&&
             !lineinfo[y-10].left_lost&&!lineinfo[y-10].right_lost&&
             !lineinfo[y-15].left_lost&&!lineinfo[y-15].right_lost&&
             !lineinfo[y-20].left_lost&&!lineinfo[y-20].right_lost&&
             !lineinfo[y-3].left_lost&&!lineinfo[y-3].right_lost&&
		   (((lineinfo[y-6].right-lineinfo[y-6].left)-(lineinfo[y-5].right-lineinfo[y-5].left))<=4)&&//检测上下两行宽度之差
           (((lineinfo[y-5].right-lineinfo[y-5].left)-(lineinfo[y-4].right-lineinfo[y-4].left))<=4)&&
            /*abs(lineinfo[y].left-lineinfo[y+1].left>30)&&
		    abs(lineinfo[y].left-lineinfo[y+2].left>30)&&
			abs(lineinfo[y].left-lineinfo[y+3].left>30)&&
			abs(lineinfo[y].right-lineinfo[y+1].right>50)&&
			abs(lineinfo[y].right-lineinfo[y+2].right>50)&&
			abs(lineinfo[y].right-lineinfo[y+3].right>50)&&	*/
			lineinfo[y+3].whole_black&&
			lineinfo[y+5].whole_black&&
			lineinfo[y+6].whole_black&&
			//lineinfo[y+7].whole_black&&		
			//lineinfo[y+10].whole_black&&
			//lineinfo[y+15].whole_black&&					
			!lineinfo[y].whole_black&&
			y<100&&
			watch.obs_pos>y
		   )
		{
			watch.obs_pos = y;
			watch.obs_flag = 1;
			if(Element==None)
			{
					obs_confirm();
			}
			break;
			//set_speed(0);
			//change_pid_para(&Speed_left,&setpara.stop_PID);
			//change_pid_para(&Speed_right,&setpara.stop_PID);

			
		}

	}
}
void obs_confirm()
{
	if(mpu.pitch<3)
	{
		enter_element(obstacle);
		begin_angle_integeral(setpara.obs_ang_1);
		begin_distant_integeral(setpara.obs_dis_1*100);
	}
}
void obs_first_turn()
{
	if(watch.obs_flag!=1)return;
	if(get_integeral_state(&angle_integral)==2)
	{
		watch.obs_flag=2;
		
		clear_angle_integeral();
		//begin_angle_integeral(setpara.obs_ang_2);
		//begin_distant_integeral(setpara.obs_dis_2);
	}
	
	
}

void obs_first_straight()
{
	if(watch.obs_flag!=2)return;
	if(get_integeral_state(&distance_integral)==2)
	{
		watch.obs_flag=3;
		
		clear_distant_integeral();
		begin_angle_integeral(setpara.obs_ang_2);
		begin_distant_integeral(setpara.obs_dis_2*100);
	}
}
void obs_second_turn()
{
	if(watch.obs_flag!=3)
		return;
	if(get_integeral_state(&angle_integral)==2)
	{
		watch.obs_flag=4;
		//clear_distant_integeral();
		clear_angle_integeral();
		//begin_distant_integeral(-3000);
	}
	
}
void obs_sencond_straight()
{
	if(watch.obs_flag!=4)
		return;
	if(get_integeral_state(&distance_integral)==2)
	{
		watch.obs_flag=5;
		
		clear_distant_integeral();
		//begin_angle_integeral(se tpara.obs_ang_2);
		begin_distant_integeral(-1000);
	}
}
void obs_free()
{
	if(watch.obs_flag!=5)
		return;
	if(get_integeral_state(&distance_integral)==2)
	{
		clear_distant_integeral();
		watch.obs_flag=6;
	}
}
void obstacle_stop()
{
	if(watch.obs_flag!=6)
		return;
	clear_all_flags();
	out_element();
}


