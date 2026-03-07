
/*
基于祖传motor移植
2026.1.29
舵机中值不确定
电机舵机频率参考祖传
*/

#include "motor.h"

void motor_init()
{
	
	//drv 2个pwm  7.5   7.7
//2个dir      7.4   7.6
	
    pwm_init(PWMB_CH1_P74,17000,0);
    pwm_init(PWMB_CH3_P76,17000,0);
	
    gpio_init(IO_P75,GPO,0,GPO_PUSH_PULL);
    gpio_init(IO_P77,GPO,0,GPO_PUSH_PULL);

	
   // pwm_init(ATOM1_CH6_P23_1,50,0);不知道是哪个

	//无刷电机风扇
    pwm_init(PWMF_CH1_PA1,50,0);
    pwm_init(PWMF_CH2_PA3,50,0);

    fan_begin(setpara.fan_start);
    pwm_init(PWME_CH1P_PA0,300,STEER_MID);
}
#define steer_pwm_max (STEER_MID+setpara.steer_adjust+490)
#define steer_pwm_min (STEER_MID+setpara.steer_adjust-490)
void set_steer(int steer_pwm)
{
    if(steer_pwm>steer_pwm_max)
        steer_pwm=steer_pwm_max;
    else if(steer_pwm<steer_pwm_min)
        steer_pwm=steer_pwm_min;
    pwm_set_duty(PWME_CH1P_PA0,steer_pwm);
}
void fan_start()
{
    //gpio_set_level(P23_1,1);
    //gpio_set_level(P22_3,1);
}
void fan_stop()
{
    pwm_set_duty(PWMF_CH1_PA1,0);
    pwm_set_duty(PWMF_CH2_PA3,0);
}
#define FAN_PWM_MAX 6000
void set_fan(uint16 PWM_L,uint16 PWM_R)
{
    if(PWM_L>FAN_PWM_MAX)PWM_L=FAN_PWM_MAX;
    if(PWM_R>FAN_PWM_MAX)PWM_R=FAN_PWM_MAX;
    pwm_set_duty(PWMF_CH1_PA1,PWM_L);
    pwm_set_duty(PWMF_CH2_PA3,PWM_R);

}
#define PWM_MAX 9900
void set_pwm(int PWM_L,int PWM_R)
{
    if(PWM_L>PWM_MAX)PWM_L=PWM_MAX;
    if(PWM_L<-PWM_MAX)PWM_L=-PWM_MAX;
    if(PWM_R>PWM_MAX)PWM_R=PWM_MAX;
    if(PWM_R<-PWM_MAX)PWM_R=-PWM_MAX;
    if(mycar.RUNTIME<400){
        PWM_L=(PWM_L > 8000 )? 8000 : (PWM_L < -8000 ? -8000 : PWM_L);
        PWM_R=(PWM_R > 8000 )? 8000 :  (PWM_R < -8000 ? -8000 : PWM_R);
    }
    if(PWM_R<=0)
    {
        pwm_set_duty(PWMB_CH1_P74,-PWM_R);
        gpio_set_level(IO_P75,0);
    }
    else if(PWM_R>0)
    {
        pwm_set_duty(PWMB_CH1_P74,PWM_R);
        gpio_set_level(IO_P75,1);
    }
    if(PWM_L>=0)
    {
        pwm_set_duty(PWMB_CH3_P76,PWM_L);
        gpio_set_level(IO_P77,0);
    }
    else if(PWM_L<0)
    {
        pwm_set_duty(PWMB_CH3_P76,-PWM_L);
        gpio_set_level(IO_P77,1);
    }

}

