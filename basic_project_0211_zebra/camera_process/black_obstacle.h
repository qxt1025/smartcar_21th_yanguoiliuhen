#include "zf_common_headfile.h"
/*
 * black_obstacle.h
 *
 *  Created on: 2024Äê3ÔÂ23ÈÕ
 *      Author: hwj
 */

#ifndef CODE_CAMERA_PROCESS_BLACK_OBSTACLE_H_
#define CODE_CAMERA_PROCESS_BLACK_OBSTACLE_H_

#include "type_def.h"
#include "Element_recognition.h"
#include "scan_line.h"
#include "Binarization.h"
#include "integral.h"
#include "mycar.h"

void black_obstacle_enter();
void black_obstacle_out();
void black_obstacle_confirm();
void find_angle_left(uint8 *angle_x,uint8 *angle_y);




#endif /* CODE_CAMERA_PROCESS_BLACK_OBSTACLE_H_ */
