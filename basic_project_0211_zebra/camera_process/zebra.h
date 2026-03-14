/*
 * zebra.h
 *
 *  Created on: 2024-03-24
 *      Author: hwj
 */
#ifndef CODE_CAMERA_PROCESS_ZEBRA_H_
#define CODE_CAMERA_PROCESS_ZEBRA_H_

#include "zf_common_headfile.h"
#include "type_def.h"
#include "Element_recognition.h"
#include "scan_line.h"
#include "Binarization.h"
#include "integral.h"
#include "mycar.h"

void zebra_enter(void);
void zebra_stop(void);
void zebra_out(void);

#endif /* CODE_CAMERA_PROCESS_ZEBRA_H_ */
