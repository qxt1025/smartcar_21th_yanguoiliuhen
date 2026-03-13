#include "beep.h"

#define BEEP_GPIO_PIN IO_P65

volatile uint16 beep_time;
uint16 yinjie[]={247,262,294,330,349,392,440,494,0};
uint8 warn_flag;
uint8 music_list1[8]={1,2,3,4,5,6,7,0};
uint8 music_list2[]={1,1,5,5,6,6,5,8,4,4,3,3,2,2,1,0};
uint8 stop_music_list3[]={3,3,4,5,5,4,3,2,1,1,2,3,3,2,2,8,3,3,4,5,5,4,3,2,1,1,2,3,2,1,1,8,2,2,3,1,2,4,3,1,2,4,3,2,1,2,5,8,3,3,4,5,5,4,3,2,1,1,2,3,2,1,1,0};
uint8* music;

void beep_init()
{
    gpio_init(BEEP_GPIO_PIN, GPO, 0, GPO_PUSH_PULL);
    beep_time = 0;
    warn_flag = 0;
}

void beep_power_on_self_test()
{
    gpio_set_level(BEEP_GPIO_PIN, 1);
    beep_time = 300U;
    system_delay_ms(300);
    gpio_set_level(BEEP_GPIO_PIN, 0);
    beep_time = 0;
}

void beeping()
{
    if(beep_time > 0)
    {
        beep_time--;
        if(beep_time == 0)
        {
            gpio_set_level(BEEP_GPIO_PIN, 0);
        }
    }
}

void beep(uint16 time)
{
    if(time == 0U)
    {
        gpio_set_level(BEEP_GPIO_PIN, 0);
        beep_time = 0U;
        return;
    }

    gpio_set_level(BEEP_GPIO_PIN, 1);
    beep_time = time;
}

void beep2(uint8 freq,uint16 time)
{
    if(time == 0U)
    {
        gpio_set_level(BEEP_GPIO_PIN, 0);
        beep_time = 0U;
        return;
    }

    if(freq >= (uint8)(sizeof(yinjie) / sizeof(yinjie[0])) || yinjie[freq] == 0U)
    {
        gpio_set_level(BEEP_GPIO_PIN, 0);
        beep_time = time;
        return;
    }

    gpio_set_level(BEEP_GPIO_PIN, 1);
    beep_time = time;
}

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
                    car_begin();
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