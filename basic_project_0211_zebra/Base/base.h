/*
 * base.h
 *
 *  Created on: 2023-06-20
 *      Author: Admin
 */

#ifndef _BASE_H_
#define _BASE_H_

#include "global.h"
#include "beep.h"
#include "encoder.h"
#include "motor.h"
#include "tof.h"
#include "mpu6050.h"
#include "imu660.h"
#include "pid.h"
#include "application.h"
#include "zf_common_headfile.h"

#define LED1                    (IO_P52)
#define GUI_SWITCH_PIN          (IO_PB0)
#define GUI_SWITCH_ENABLE_LEVEL (1U)
#define IMG_SWITCH_PIN          (IO_PB1)
#define IMG_SWITCH_ENABLE_LEVEL (1U)
#define PIT_CH                  (TIM0_PIT)
#define PIT_PRIORITY            (TIMER0_IRQn)
#define TIM1_CTRL_CH            (TIM1_PIT)
#define TIM1_CTRL_PRIORITY      (TIMER1_IRQn)
#define TIM1_CTRL_PERIOD_MS     (1)

void hardwareinit(void);

#endif /* _BASE_H_ */