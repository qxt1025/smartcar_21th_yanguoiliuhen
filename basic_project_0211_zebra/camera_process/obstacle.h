#include "zf_common_headfile.h"
/*
 * obstacle.h
 *
 *  Created on: 2023Äê7ÔÂ5ÈÕ
 *      Author: HUAWEI
 */

#ifndef CODE_CAMERA_PROCESS_OBSTACLE_H_
#define CODE_CAMERA_PROCESS_OBSTACLE_H_
#include "type_def.h"
#include "Element_recognition.h"
#include "scan_line.h"
#include "Binarization.h"
#include "integral.h"

void obstacle_identification();
void obs_confirm();
void obs_first_turn();
void obs_second_turn();
void obstacle_stop();
void obs_free();
void obs_first_straight();
void obs_sencond_straight();
#endif


