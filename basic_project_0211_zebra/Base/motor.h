#ifndef CODE_BASE_MOTOR_H_
#define CODE_BASE_MOTOR_H_
#include "base.h"
#define STEER_MID 4850
void motor_init();
void set_pwm(int PWM_L,int PWM_R);
void set_steer(int steer_pwm);
void fan_stop();
void fan_start();
void set_pwm(int PWM_L,int PWM_R);

void set_fan(uint16 PWM_L,uint16 PWM_R);
#endif /* CODE_BASE_MOTOR_H_ */