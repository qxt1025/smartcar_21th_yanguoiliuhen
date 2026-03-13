/*
 * beep.h
 *
 *  Created on: 2023Äê3ÔÂ14ÈÕ
 *      Author: Admin
 */

#ifndef CODE_BASE_BEEP_H_
#define CODE_BASE_BEEP_H_
#include "base.h"
extern volatile uint16 beep_time;
extern uint8* music;
void beep_init();
void beep_power_on_self_test();
void beeping();
void beep(uint16 time);
void beep2(uint8 yinjie,uint16 time);
void beep_warning(uint8* music);
void warning1();
void car_start_warning();
void stop_warning();
void zebra_stop_warning();
#endif /* CODE_BASE_BEEP_H_ */
