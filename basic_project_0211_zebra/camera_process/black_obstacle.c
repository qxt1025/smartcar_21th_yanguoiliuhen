/*
 * black_obstacle.c
 *
 *  Created on: 2024年3月23日
 *      Author: hwj
 */
#include "black_obstacle.h"

/* 黑色路障尺寸：240mm*115mm*53mm*
   摆放距中心线100mm */

void black_obstacle_enter()
{
    int y;
    //if(mycar.RUNTIME<setpara.bla_obs_begin_time)return;
    for(y=25;y<80;y++)
    {
        if(watch.black_obstacle_flag==0&&
           (((lineinfo[y-6].right-lineinfo[y-6].left)-(lineinfo[y-5].right-lineinfo[y-5].left))<=4)&&//检测上下两行宽度之差
           (((lineinfo[y-5].right-lineinfo[y-5].left)-(lineinfo[y-4].right-lineinfo[y-4].left))<=4)&&
           (((lineinfo[y-4].right-lineinfo[y-4].left)-(lineinfo[y].right-lineinfo[y].left))> 25 /*setpara.obs_width*/ ||
			((lineinfo[y-4].right-lineinfo[y-4].left)-(lineinfo[y+1].right-lineinfo[y+1].left))> 25 /*setpara.obs_width*/+5)&&//上下两行宽度突变（减小）
           (((lineinfo[y-1].right-lineinfo[y-1].left)-(lineinfo[y].right-lineinfo[y].left))>4)&&
           (((lineinfo[y].right-lineinfo[y].left)-(lineinfo[y+1].right-lineinfo[y+1].left))<=4)&&
           (((lineinfo[y+1].right-lineinfo[y+1].left)-(lineinfo[y+2].right-lineinfo[y+2].left))<=4)&&
           ((lineinfo[y-11].right-lineinfo[y-5].right)<10)&&((lineinfo[y-5].left-lineinfo[y-11].left)<10)&&
             !lineinfo[y+10].left_lost&&!lineinfo[y+10].right_lost&&//左右侧均不丢线
             !lineinfo[y+8].left_lost&&!lineinfo[y+8].right_lost&&
             !lineinfo[y+6].left_lost&&!lineinfo[y+6].right_lost&&
             !lineinfo[y+4].left_lost&&!lineinfo[y+4].right_lost&&
             !lineinfo[y+2].left_lost&&!lineinfo[y+2].right_lost&&
             !lineinfo[y].left_lost&&!lineinfo[y].right_lost&&
             !lineinfo[y-5].left_lost&&!lineinfo[y-5].right_lost&&
             !lineinfo[y-10].left_lost&&!lineinfo[y-10].right_lost&&
             !lineinfo[y-15].left_lost&&!lineinfo[y-15].right_lost&&
             !lineinfo[y-20].left_lost&&!lineinfo[y-20].right_lost&&
             !lineinfo[y-3].left_lost&&!lineinfo[y-3].right_lost
        )
        {

            watch.black_obstacle_line=y;
            if(Element==None)     //在当前无元素时进行以下操作
            {
             black_obstacle_confirm();
            }
//            change_pid_para(&CAM_Turn,&setpara.black_obstacle_turn_PID);//将转向PID参数调为路障转向PID（p、d调小，车转而不多）;
            if(watch.left_obstacle_flag){
                watch.left_obstacle_x=lineinfo[watch.black_obstacle_line].left;
            }
            if(watch.right_obstacle_flag){
                watch.right_obstacle_x=lineinfo[watch.black_obstacle_line].right;
            }

        }
//        if(watch.left_obstacle_flag){
//            find_angle_left(watch.left_obstacle_x, watch.black_obstacle_line, &watch.left_obstacle_x, &watch.black_obstacle_line);
//          }
//        if(watch.right_obstacle_flag){
//            find_angle_right_down(watch.right_obstacle_x, watch.black_obstacle_line, &watch.right_obstacle_x, &watch.black_obstacle_line);
//          }
    }
}

void black_obstacle_confirm()
{
    if((lineinfo[watch.black_obstacle_line].left-lineinfo[watch.black_obstacle_line-4].left)>10&&
       (lineinfo[watch.black_obstacle_line+1].left-lineinfo[watch.black_obstacle_line-5].left)>10&&
       (lineinfo[watch.black_obstacle_line-4].right-lineinfo[watch.black_obstacle_line].right)<6&&
       (lineinfo[watch.black_obstacle_line-5].right-lineinfo[watch.black_obstacle_line+1].right)<8){
        watch.left_obstacle_flag=1;
        watch.right_obstacle_flag=0;
        enter_element(black_obstacle);
        //set_speed(setpara.bla_obs_speed);
        begin_distant_integeral(-9000/*setpara.obs_dis*/);//路障长240mm，折合脉冲数约2824个
        watch.black_obstacle_flag=1;
        //beep2(7,100);
        }
    else if((lineinfo[watch.black_obstacle_line-4].right-lineinfo[watch.black_obstacle_line].right)>10&&
            (lineinfo[watch.black_obstacle_line-5].right-lineinfo[watch.black_obstacle_line+1].right)>10&&
            (lineinfo[watch.black_obstacle_line].left-lineinfo[watch.black_obstacle_line-4].left)<6&&
            (lineinfo[watch.black_obstacle_line+1].left-lineinfo[watch.black_obstacle_line-5].left)<8){
        watch.left_obstacle_flag=0;
        watch.right_obstacle_flag=1;
        enter_element(black_obstacle);
        //set_speed(setpara.bla_obs_speed);
        begin_distant_integeral(-9000 /*setpara.obs_dis*/);//路障长240mm，折合脉冲数约2824个
        watch.black_obstacle_flag=1;
        //beep2(7,100);
        }
    else{
        watch.left_obstacle_flag=0;
        watch.right_obstacle_flag=0;
        }

}

void black_obstacle_out()
{
    if(watch.left_obstacle_flag)
    {
       find_angle_left(&watch.left_obstacle_x, &watch.black_obstacle_line);
    }
    else if(watch.right_obstacle_flag)
    {
        find_angle_right_down(&watch.right_obstacle_x, &watch.black_obstacle_line);
    }
    if(get_integeral_state(&distance_integral)==2&&watch.black_obstacle_flag==1)
    {
        clear_distant_integeral();
        out_element();
        //beep2(7,100);
    }
}

void find_angle_left(uint8 *angle_x,uint8 *angle_y)
{
    uint8 x=*angle_x,y=*angle_y;
    while(g_frame_img[119-y][x] >= BLACKPOINT /* !=0-><=reference_point */&&y<110)
    {
        y++;
    }
    while(g_frame_img[119-y][x+1] <= reference_point /* !=255-><=reference_point */&&x<187)
    {
        x++;
    }
    while(y>40)
    {
        if(g_frame_img[119-y][x] < BLACKPOINT /* ==0->>BLACKPOINT */)
        {

        }
        else if(g_frame_img[119-y][x+1] < BLACKPOINT /* ==0->>BLACKPOINT */)
        {
            x++;
        }
        else if(g_frame_img[119-y][x-1] < BLACKPOINT /* ==0->>BLACKPOINT */)
        {
            x--;
        }
        else if(g_frame_img[119-y][x+2] < BLACKPOINT /* ==0->>BLACKPOINT */)
        {
            x=x+2;
        }
        else if(g_frame_img[119-y][x-2] < BLACKPOINT /* ==0->>BLACKPOINT */)
        {
            x=x-2;
        }
        else if(g_frame_img[119-y][x+3] < BLACKPOINT /* ==0->>BLACKPOINT */)
        {
            x=x+3;
        }
        else if(g_frame_img[119-y][x-3] < BLACKPOINT /* ==0->>BLACKPOINT */)
        {
            x=x-3;
        }
        else break;
        y--;
    }
    *angle_x=x;
    *angle_y=y;
}
