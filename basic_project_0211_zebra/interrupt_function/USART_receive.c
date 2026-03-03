/*
 * USART_receive.c
 *
 *  Created on: 2023Äê12ÔÂ3ÈÕ
 *      Author: 14250
 */
#include "timer.h"
uint8 buff;
uint8 car_save=0;
void USART_Receive_Call_Back()
{
    if(setpara.USART_flag>=1){
    if(mycar.car_running==0&&car_save<2)
    {
        car_save++;
        beep2(5,100);
        if(car_save==2)
        {
            car_start_warning();
        }
    }
    else if(mycar.car_running==1)
    {
        car_save=3;
        car_stop();
    }
    }
    //gpio_toggle_level(P20_9);
}


