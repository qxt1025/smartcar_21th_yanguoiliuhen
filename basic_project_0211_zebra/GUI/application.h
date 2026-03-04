/*
 * application.h
 *
 *  Created on: 2023锟斤拷6锟斤拷20锟斤拷
 *      Author: Admin
 */

#ifndef CODE_GUI_APPLICATION_H_
#define CODE_GUI_APPLICATION_H_
#include "zf_common_headfile.h"

struct paralist_s
{
	int* para;        			//参数存放的地址
	char label[20];   			//参数的名称
	unsigned short precision;	//参数的调整精度
};

extern struct paralist_s far paralist[100];//两个参数列表结构体数组
extern struct paralist_s far paralist2[100];

//GUI初始化函数,放在主函数while循环之前
void GUI_Init();
//扫描用户输入函数，放在定时器中断里
void GUI_scan();
//同步UI/图像显示开关状态，放在主循环中调用
void GUI_UpdateSwitchState();
//显示函数，此函数要放在main函数中的while内
void GUI_Display();


#endif /* CODE_GUI_APPLICATION_H_ */
