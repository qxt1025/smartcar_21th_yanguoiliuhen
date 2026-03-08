#ifndef CODE_BASE_MOTOR_H_
#define CODE_BASE_MOTOR_H_
#include "base.h"

#define RIGHT_MOTOR_PWM_PIN PWMB_CH1_P74
#define RIGHT_MOTOR_DIR_PIN IO_P75
#define LEFT_MOTOR_PWM_PIN  PWMB_CH3_P76
#define LEFT_MOTOR_DIR_PIN  IO_P77

#define STEER_MID 5250
void motor_init();
void set_pwm(int PWM_L,int PWM_R);
void set_steer(int steer_pwm);
void fan_stop();
void fan_start();
void set_pwm(int PWM_L,int PWM_R);

void set_fan(uint16 PWM_L,uint16 PWM_R);
#endif /* CODE_BASE_MOTOR_H_ */
