
#include "beep.h"
uint16 beep_time;
uint16 yinjie[]={247,262,294,330,349,392,440,494,0};
//unsigned int yinjie[]={
//    0,
//    131,139,147,156,165,175,185,196,208,220,233,247,
//    262,277,294,311,330,349,370,392,415,440,466,494,
//    523,554,587,622,659,698,740,784,831,880,932,988};
uint8 warn_flag;
uint8 music_list1[8]={1,2,3,4,5,6,7,0};
uint8 music_list2[]={1,1,5,5,6,6,5,8,4,4,3,3,2,2,1,0};
//uint8 stop_music_list3[15]={1,2,3,1,1,2,3,4,3,4,5,3,4,5,0};
uint8 stop_music_list3[]={3,3,4,5,5,4,3,2,1,1,2,3,3,2,2,8,3,3,4,5,5,4,3,2,1,1,2,3,2,1,1,8,2,2,3,1,2,4,3,1,2,4,3,2,1,2,5,8,3,3,4,5,5,4,3,2,1,1,2,3,2,1,1,0};
uint8* music;
void beep_init()
{
    pwm_init(IO_P65,1000,0);
}
//蜂鸣器计时
void beeping()
{
    if(beep_time!=0)
    {
        beep_time--;
    }
    if(beep_time==1)
        pwm_set_duty(IO_P65,0);

}
void beep(uint16 time)
{
    pwm_init(IO_P65,yinjie[0],10000);
    beep_time=time;
}
void beep2(uint8 freq,uint16 time)
{
    if(freq==0)return;
    pwm_init(IO_P65,yinjie[freq],5000);
    beep_time=time;
}
//发车前蜂鸣器响,响完再发车
void car_start_warning()
{
    warn_flag=1;
    music=music_list1;
    mycar.start_warn_flag=1;
}
void warning1()
{
    music=music_list1;
    warn_flag=1;
}
void stop_warning()
{
    music=music_list2;
    warn_flag=1;
}
void zebra_stop_warning()
{
    music=stop_music_list3;
    warn_flag=1;
}

void beep_warning(uint8* music)
{
    static uint16 num,step=0;
    if(warn_flag==1)
    {
        if(num==250)
        {
            num=0;
            if(music[step]==0)
            {
                warn_flag=0;
                step=0;
                num=0;
                if(mycar.start_warn_flag==1)
                {
                    mycar.start_warn_flag=0;
                    car_begin(); //发车指令
                }
                return;
            }
            beep2(music[step],100);
            if(step==3&&mycar.start_warn_flag==1)fan_begin(setpara.fan_speed);
            step++;
        }
       num++;
    }
    else
    {
        num=0;
        step=0;
    }
}
/*
//蜂鸣器报警
void beep_warning(uint8* music)
{
    static uint16 num;
    if(warn_flag==1)
    {
       if(num==0)
       {
           beep2(music[0],100);
       }
       else if(num==250)
       {
           //fan_begin(setpara.fan_speed);
           beep2(music[1],100);
       }
       else if(num==500)
       {
           beep2(music[2],100);
       }
       else if(num==750)
       {
           beep2(music[3],100);
           if(mycar.start_warn_flag==1)
            {
               fan_begin(setpara.fan_speed);
            }
       }
       else if(num==1000)
       {
           beep2(music[4],100);
       }
       else if(num==1250)
       {
           beep2(music[5],100);
       }
       else if(num==1500)
       {
           beep2(music[6],100);
       }
       num++;
       if(num==1750)
       {
           num=0;
           warn_flag=0;
           if(mycar.start_warn_flag==1)
            {
                mycar.start_warn_flag=0;
                car_begin(); //发车指令
            }
       }
    }

}
*/

