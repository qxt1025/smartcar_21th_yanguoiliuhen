/*
 * 电机与执行器驱动初始化
 * 已按当前车模工程完成移植和调整
 * 更新时间：2026-01-29
 *
 * 说明：
 * 1. 舵机中值仍需结合实车标定微调。
 * 2. 电机与风扇 PWM 频率按当前硬件方案设置。
 */

#include "motor.h"

void motor_init()
{
    // 主驱电机：两路 PWM 输出，两路方向控制输出。
    pwm_init(RIGHT_MOTOR_PWM_PIN,17000,0);
    pwm_init(LEFT_MOTOR_PWM_PIN,17000,0);

    gpio_init(RIGHT_MOTOR_DIR_PIN,GPO,0,GPO_PUSH_PULL);
    gpio_init(LEFT_MOTOR_DIR_PIN,GPO,0,GPO_PUSH_PULL);

    // 预留的 PWM 通道，保留作硬件接线参考。
    // pwm_init(ATOM1_CH6_P23_1,50,0);

    // 无刷风扇
    pwm_init(PWMF_CH1_PA1,50,0);
    pwm_init(PWMF_CH2_PA3,50,0);

    // 按参数中的启动占空比启动风扇。
    fan_begin(setpara.fan_start);

    // 舵机控制输出，初始化为机械中值。
    pwm_init(PWME_CH1P_PA0,300,STEER_MID);
}

#define steer_pwm_max (STEER_MID+450)//(5600)
#define steer_pwm_min (STEER_MID-460)//(4900)

void set_steer(int steer_pwm)
{
    // 将舵机输出限制在标定后的安全范围内。
    if(steer_pwm>steer_pwm_max)
        steer_pwm=steer_pwm_max;
    else if(steer_pwm<steer_pwm_min)
        steer_pwm=steer_pwm_min;
    pwm_set_duty(PWME_CH1P_PA0,steer_pwm);
}

void fan_start()
{
    // 当前未使用 GPIO 方式直接使能风扇。
    //gpio_set_level(P23_1,1);
    //gpio_set_level(P22_3,1);
}

void fan_stop()
{
    // 将两路风扇占空比清零，实现停转。
    pwm_set_duty(PWMF_CH1_PA1,0);
    pwm_set_duty(PWMF_CH2_PA3,0);
}

#define FAN_PWM_MAX 6000

void set_fan(uint16 PWM_L,uint16 PWM_R)
{
    // 将两路风扇 PWM 限制在允许范围内。
    if(PWM_L>FAN_PWM_MAX)PWM_L=FAN_PWM_MAX;
    if(PWM_R>FAN_PWM_MAX)PWM_R=FAN_PWM_MAX;
    pwm_set_duty(PWMF_CH1_PA1,PWM_L);
    pwm_set_duty(PWMF_CH2_PA3,PWM_R);
}

#define PWM_MAX 9900

void set_pwm(int PWM_L,int PWM_R)
{
    // 先按总限幅约束左右轮 PWM 输出。
    if(PWM_L>PWM_MAX)PWM_L=PWM_MAX;
    if(PWM_L<-PWM_MAX)PWM_L=-PWM_MAX;
    if(PWM_R>PWM_MAX)PWM_R=PWM_MAX;
    if(PWM_R<-PWM_MAX)PWM_R=-PWM_MAX;

    // 在启动初期进一步降低输出上限，避免瞬时过猛。
    if(mycar.RUNTIME<400){
        PWM_L=(PWM_L > 8000 )? 8000 : (PWM_L < -8000 ? -8000 : PWM_L);
        PWM_R=(PWM_R > 8000 )? 8000 :  (PWM_R < -8000 ? -8000 : PWM_R);
    }

    // 右轮：负值表示反转，正值表示正转。
    if(PWM_R<=0)
    {
        pwm_set_duty(RIGHT_MOTOR_PWM_PIN,-PWM_R);
        gpio_set_level(RIGHT_MOTOR_DIR_PIN,0);
    }
    else if(PWM_R>0)
    {
        pwm_set_duty(RIGHT_MOTOR_PWM_PIN,PWM_R);
        gpio_set_level(RIGHT_MOTOR_DIR_PIN,1);
    }

    // 左轮方向定义与右轮接线相反。
    if(PWM_L>=0)
    {
        pwm_set_duty(LEFT_MOTOR_PWM_PIN,PWM_L);
        gpio_set_level(LEFT_MOTOR_DIR_PIN,0);
    }
    else if(PWM_L<0)
    {
        pwm_set_duty(LEFT_MOTOR_PWM_PIN,-PWM_L);
        gpio_set_level(LEFT_MOTOR_DIR_PIN,1);
    }
}
