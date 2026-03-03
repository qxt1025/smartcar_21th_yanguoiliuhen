/*
 * actpersp.h
 *
 *  Created on: 2023Äê6ÔÂ21ÈÕ
 *      Author: Admin
 */

#ifndef CODE_CAMERA_PROCESS_ACT_PERSP_H_
#define CODE_CAMERA_PROCESS_ACT_PERSP_H_
#include "zf_common_headfile.h"
struct persp_struct
{
      uint8 persp_x[188];
      int16 y;
};
void act_perst_init();
void persp_task(int16 xl,int16 xr,int16 y);
int16 get_persp_data_ox(int16 x,int16 y);
int16 get_persp_data_oy(int16 x,int16 y);



#endif /* CODE_CAMERA_PROCESS_ACT_PERSP_H_ */
