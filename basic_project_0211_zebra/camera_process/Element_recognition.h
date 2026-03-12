#ifndef CODE_CAMERA_PROCESS_ELEMENT_RECOGNITION_H_
#define CODE_CAMERA_PROCESS_ELEMENT_RECOGNITION_H_

#include "type_def.h"
#include "zf_common_headfile.h"
#include "patch_line.h"
#include "act_persp.h"

#include "circle.h"
//#include "cross.h"
#include "slope.h"

#include "black_obstacle.h"
// 元素类型
typedef enum
{
    None=0,         // 无元素      0
    Left_ring,      // 左环        1
    Right_ring,     // 右环        2
    Slope,          // 斜坡        3
    broken_circuit, // 断路        4
    obstacle,       // 障碍        5
    ingarage,       // 入库        6
    outgarage,      // 出库        7
    crossing,       // 十字        8
    black_obstacle, // 小型黑色路障  9
    zebra,          // 斑马线     10
    outroad,        // 出界       11
    straight,       // 直线       12
    curve           // 急弯       13

} Element_range;

struct Element_struct
{
    uint8 Element_count;   // 元素计数
    uint8 Element_data[21];
    uint8 loop_count;      // 圆环计数
    uint8 loop_data[4];    // 圆环数据
};

// 祖传摄像头处理代码的结构体，里面主要是摄像头识别的赛道信息
struct watch_o
{
    /* 大津法使用 */
    uint8 threshold;   // 图像二值化阈值

    /* 摄像头视野 */
    uint8 watch_line;
    uint8 watch_lost;    // 摄像头所能看到赛道的最远端

    uint8 cross_state;

    /* 统计丢线相关变量 */
    uint8 cross;            // 左右都丢线的行数
    uint8 left_lost;        // 左边线丢线行数
    uint8 right_lost;       // 右边线丢线行数
    uint8 left_near_lost;   // 左边线开始丢线的行数
    uint8 right_near_lost;  // 右边线开始丢线的行数
    uint8 left_far_lost;
    uint8 right_far_lost;
    uint8 left_down_find;
    uint8 right_down_find;
    uint8 left_up_find;
    uint8 right_up_find;
    uint8 cross_flag;
	uint8 left_black;
	uint8 right_black;

    uint8 stop_flag;

    /* 直道检测 */
    uint8 straight_flag;
    float variance;
    /* 急弯检测 */
    uint8 large_curve_flag;
    uint8 curve_num;

    uint8_t black_line;

    /* 圆环 */
    uint8_t Island_State;
    uint8_t Left_Island_Flag;
    uint8_t Right_Island_Flag;
    uint8_t monotonicity_change_right_flag;
    uint8_t monotonicity_change_left_flag;
    uint8_t continuity_change_left_flag;
    uint8_t continuity_change_right_flag;
    uint8 left_down_guai;
    uint8 left_up_guai;
    uint8 err;
    uint8 err_far;
    uint8 line_end;
    uint8 slope_err;
    uint8 err_circle;

    // 祖传圆环
    uint8 InLoop;
    uint8 OutLoop;

    uint8 InLoopAngleL;
    uint8 InLoopAngleR;
    uint8 InLoopCirc;
    uint8 InLoopAngle2;
    uint8 InLoopAngle2_x;
    uint8 InLoopAngle2_y;
    uint8 OutLoopAngle2;
    uint8 OutLoopAngle1;
    uint8 OutLoop_turn_point_x;
    uint8 OutLoop2;
    uint8 dis_Loop;
    uint8 top_x;
    float fix_slope;

    // 断路行计数标记
    uint8 broken_circuit_line_count;
    uint8 broken_circuit_time_count;
    uint8 broken_circuit_flag; // 1:开始进入断路 0:不在断路 2:完全进入断路

    // 斜坡标记位
    uint8 slope_flag;

    // 小型黑色路障标记位
    uint8 black_obstacle_flag;
    uint8 left_obstacle_flag;
    uint8 right_obstacle_flag;
    uint8 black_obstacle_line;
    uint8 left_obstacle_x;
    uint8 right_obstacle_x;

    // 前方障碍物距离
    uint32_t tof_dis;
    float forward_distance;
    uint8 obstacle_flag;   // 避障标记位
    uint8 obstacle_angle;  // 障碍物角点
    uint8 Line_patrol_mode;// 巡线模式 0:中线 1:左线 2:右线
    uint8 out_garage_flag; // 出库标记位
    uint8 garage_stop;     // 停车标记位
    uint8 garage_flag;

    /* 斑马线相关 */
    uint8 ZebraInLine;
    uint8 zebra_flag;
    uint8 zebra_flag2;
    uint8 ZebraLine;
    uint8 ZebraRowL;
    uint8 ZebraRowR;
    uint8 zebra_stop;
    uint8 stop_count;
    uint8 Garge_line;
    uint8 Gargeout_Flag;
    uint8 Zebra_Angle;
    uint8 Zebra_Angle2;
    uint8 Zebra_Angle2_x;
	uint8 jump_count;

    // 误差计算相关
    uint8 angle_far_line;
    uint8 angle_near_line;
    uint8 track_count;
    uint8 track_count_far;
	uint8 watchleft;
	uint8 watchright;

    uint8 present_speed;

    uint8_t obs_pos;
    uint8_t obs_flag;
    uint8_t black_cnt;
    uint8_t lost_cnt;

    uint8 slope_time;
};

extern struct watch_o far watch;
extern Element_range Element;
extern struct Element_struct Element_rem;

void line_lost(void);
void find_down_point(int start, int end);
void find_up_point(int start, int end);
int max_num(int x, int y);
int min_num(int x, int y);
void Left_Add_Line(int x1, int y1, int x2, int y2);
void Right_Add_Line(int x1, int y1, int x2, int y2);
void Lengthen_Right_Boundry(int start, int end);
void Lengthen_Left_Boundry(int start, int end);
void cross_detect(void);
void straight_detect(void);
void out_road_detect(void);
void curve_detect(void);
void Element_recognition(void);
void enter_element(Element_range element);
void out_element(void);
void element_init(void);
void clear_all_flags(void);
void enter_task(void);
//void count_lost(void);

#endif /* CODE_CAMERA_PROCESS_ELEMENT_RECOGNITION_H_ */

