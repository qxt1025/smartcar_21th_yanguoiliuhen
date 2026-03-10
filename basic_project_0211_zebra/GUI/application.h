/*
 * application.h
 *
 *  Created on: 2023年6月20日
 *      Author: Admin
 */

#ifndef CODE_GUI_APPLICATION_H_
#define CODE_GUI_APPLICATION_H_
#include "zf_common_headfile.h"

struct paralist_s
{
    int* para;                    // 参数存放的地址
    char label[20];               // 参数名称
    unsigned short precision;     // 参数调整精度
};

extern struct paralist_s far paralist[100];
extern struct paralist_s far paralist2[100];

typedef enum
{
    GUI_MODE_ALL_OFF = 0,             // 00: 关闭所有UI
    GUI_MODE_PARAM_ONLY = 1,          // 01: 显示GUI变量页
    GUI_MODE_IMAGE_AND_MAIN_INFO = 2, // 10: 显示图像+main信息
    GUI_MODE_RESERVED = 3             // 11: 预留
} GUI_DisplayModeEnum;

// GUI初始化函数, 放在主函数while循环之前
void GUI_Init();
// 扫描用户输入函数，放在定时器中断里
void GUI_scan();
// 同步两位拨码到显示状态，放在主循环中调用
void GUI_UpdateSwitchState();
// 获取当前两位拨码映射后的显示状态
GUI_DisplayModeEnum GUI_GetDisplayMode(void);
// 显示函数，此函数要放在main函数中的while内
void GUI_Display();

#endif /* CODE_GUI_APPLICATION_H_ */
