/*
 * cross.c
 *
 *  Created on: 2023年7月9日
 *      Author: Admin
 */
#include "cross.h"

#define CROSS_IMG_ROW_MAX       119U
#define CROSS_IMG_COL_LEFT      1U
#define CROSS_IMG_COL_CENTER_L  93U
#define CROSS_IMG_COL_CENTER_R  94U
#define CROSS_IMG_COL_RIGHT     186U

/*
 * Purpose: 识别并进入十字元素
 * Param  : 无
 * Return : 无
 * Note   : 包含正向十字进入与车身倾斜进入两类触发
 */
void cross_enter(void)
{
    uint8 cross_count;     /* 远处连续满足十字特征的行计数 */
    uint8 y;               /* 图像行索引 */
    uint8 col_right;       /* 候选右边界列坐标 */
    uint8 col_left;        /* 候选左边界列坐标 */

    cross_count = 0U;
    for(y = 40U; y < 100U; y++)
    {
        if(lineinfo[y].left_lost &&
           lineinfo[y].right_lost &&
           (g_frame_img[CROSS_IMG_ROW_MAX - y][CROSS_IMG_COL_CENTER_L] > watch.threshold) &&
           (g_frame_img[CROSS_IMG_ROW_MAX - y][CROSS_IMG_COL_CENTER_R] > watch.threshold) &&
           (g_frame_img[CROSS_IMG_ROW_MAX - y][CROSS_IMG_COL_LEFT] > watch.threshold) &&
           (g_frame_img[CROSS_IMG_ROW_MAX - y][CROSS_IMG_COL_RIGHT] > watch.threshold) &&
           (y < watch.track_count_far))
        {
            cross_count++; /* 累加远处横向全白且双边丢线行数 */
        }
    }

    if((cross_count > 6U) && (watch.cross_flag == 0U))
    {
        enter_element(crossing);               /* 状态机切换为十字元素 */
        set_speed(setpara.cross_speed);        /* 进入十字时按独立速度参数运行 */
        watch.cross_flag = 3U;                 /* 3: 标准十字进入状态 */
        begin_distant_integeral(9000U);        /* 启动里程积分窗口用于出十字判定 */
    }

    if(setpara.cross_open_flag == 1U)
    {
        for(y = 20U; y < 60U; y++)
        {
            col_right = lineinfo[y].right;     /* 缓存右边界，减少重复访存 */
            col_left = lineinfo[y].left;       /* 缓存左边界，减少重复访存 */

            if(lineinfo[y].left_lost &&
               lineinfo[y + 1U].left_lost &&
               lineinfo[y + 2U].left_lost &&
               lineinfo[y + 3U].left_lost &&
               lineinfo[y - 1U].left_lost &&
               lineinfo[y - 2U].left_lost &&
               lineinfo[y - 5U].left_lost &&
               !lineinfo[y].right_lost &&
               !lineinfo[y - 1U].right_lost &&
               !lineinfo[y - 2U].right_lost &&
               (lineinfo[y].right <= lineinfo[y + 1U].right) &&
               (lineinfo[y + 1U].right <= lineinfo[y + 2U].right) &&
               (lineinfo[y + 2U].right <= lineinfo[y + 3U].right) &&
               (lineinfo[y].right <= lineinfo[y - 1U].right) &&
               (lineinfo[y - 1U].right <= lineinfo[y - 2U].right) &&
               (lineinfo[y - 2U].right <= lineinfo[y - 3U].right) &&
               (lineinfo[y - 2U].right < lineinfo[y - 4U].right) &&
               (lineinfo[y - 1U].right < lineinfo[y - 3U].right) &&
               (lineinfo[y].right < lineinfo[y - 2U].right) &&
               (lineinfo[y].right < lineinfo[y + 2U].right) &&
               (lineinfo[y + 1U].right < lineinfo[y + 3U].right) &&
               (col_right <= (CROSS_IMG_COL_RIGHT - 5U)) && /* 防越界: col_right + 5 */
               (g_frame_img[CROSS_IMG_ROW_MAX - y - 2U][col_right] > watch.threshold) &&
               (g_frame_img[CROSS_IMG_ROW_MAX - y - 3U][col_right] > watch.threshold) &&
               (g_frame_img[CROSS_IMG_ROW_MAX - y - 4U][col_right] > watch.threshold) &&
               (g_frame_img[CROSS_IMG_ROW_MAX - y - 5U][col_right + 5U] > watch.threshold))
            {
                enter_element(crossing);       /* 右倾进入十字，切换元素状态 */
                set_speed(setpara.cross_speed);
                watch.cross_flag = 2U;         /* 2: 右倾进入状态 */
                watch.cross_RD_angle = y;      /* 保存右倾交点初值行号 */
                begin_distant_integeral(8000U);
            }
            else if(lineinfo[y].right_lost &&
                    lineinfo[y + 1U].right_lost &&
                    lineinfo[y + 2U].right_lost &&
                    lineinfo[y + 3U].right_lost &&
                    lineinfo[y - 1U].right_lost &&
                    lineinfo[y - 2U].right_lost &&
                    lineinfo[y - 5U].right_lost &&
                    !lineinfo[y].left_lost &&
                    !lineinfo[y - 1U].left_lost &&
                    !lineinfo[y - 2U].left_lost &&
                    (lineinfo[y].left >= lineinfo[y + 1U].left) &&
                    (lineinfo[y + 1U].left >= lineinfo[y + 2U].left) &&
                    (lineinfo[y + 2U].left >= lineinfo[y + 3U].left) &&
                    (lineinfo[y].left >= lineinfo[y - 1U].left) &&
                    (lineinfo[y - 1U].left >= lineinfo[y - 2U].left) &&
                    (lineinfo[y - 2U].left >= lineinfo[y - 3U].left) &&
                    (lineinfo[y - 2U].left > lineinfo[y - 4U].left) &&
                    (lineinfo[y - 1U].left > lineinfo[y - 3U].left) &&
                    (lineinfo[y].left > lineinfo[y - 2U].left) &&
                    (lineinfo[y].left > lineinfo[y + 2U].left) &&
                    (lineinfo[y + 1U].left > lineinfo[y + 3U].left) &&
                    (col_left >= 5U) &&         /* 防越界: col_left - 5 */
                    (g_frame_img[CROSS_IMG_ROW_MAX - y - 2U][col_left] > watch.threshold) &&
                    (g_frame_img[CROSS_IMG_ROW_MAX - y - 3U][col_left] > watch.threshold) &&
                    (g_frame_img[CROSS_IMG_ROW_MAX - y - 4U][col_left] > watch.threshold) &&
                    (g_frame_img[CROSS_IMG_ROW_MAX - y - 5U][col_left - 5U] > watch.threshold))
            {
                enter_element(crossing);       /* 左倾进入十字，切换元素状态 */
                watch.cross_LD_angle = y;      /* 保存左倾交点初值行号 */
                set_speed(setpara.cross_speed);
                watch.cross_flag = 1U;         /* 1: 左倾进入状态 */
                begin_distant_integeral(8000U);
            }
        }
    }
}

/*
 * Purpose: 十字运行中更新倾斜进入时的交点行号
 * Param  : 无
 * Return : 无
 * Note   : 仅在 cross_flag 为 1 或 2 时执行
 */
void cross_running2(void)
{
    uint8 y;           /* 图像行索引 */
    uint8 col;         /* 当前侧边界列坐标 */

    if((watch.cross_flag != 2U) && (watch.cross_flag != 1U))
    {
        return;
    }

    for(y = 15U; y < 87U; y++) /* 逐行扫描交点特征 */
    {
        if(watch.cross_flag == 2U)
        {
            col = lineinfo[y].right;
            if((lineinfo[y].right <= lineinfo[y + 1U].right) &&
               (lineinfo[y + 1U].right <= lineinfo[y + 2U].right) &&
               (lineinfo[y + 2U].right <= lineinfo[y + 3U].right) &&
               (lineinfo[y].right <= lineinfo[y - 1U].right) &&
               (lineinfo[y - 1U].right <= lineinfo[y - 2U].right) &&
               (lineinfo[y - 2U].right <= lineinfo[y - 3U].right) &&
               (y < watch.cross_RD_angle) &&
               (g_frame_img[CROSS_IMG_ROW_MAX - y - 2U][col] > watch.threshold) &&
               (lineinfo[y - 2U].right < lineinfo[y - 4U].right) &&
               (lineinfo[y - 1U].right < lineinfo[y - 3U].right) &&
               (lineinfo[y].right < lineinfo[y - 2U].right) &&
               (lineinfo[y].right < lineinfo[y + 2U].right) &&
               (lineinfo[y + 1U].right < lineinfo[y + 3U].right))
            {
                watch.cross_RD_angle = y; /* 锁定更靠近车体的右倾交点 */
                break;
            }
        }

        if(watch.cross_flag == 1U)
        {
            col = lineinfo[y].left;
            if((lineinfo[y].left >= lineinfo[y + 1U].left) &&
               (lineinfo[y + 1U].left >= lineinfo[y + 2U].left) &&
               (lineinfo[y + 2U].left >= lineinfo[y + 3U].left) &&
               (lineinfo[y].left >= lineinfo[y - 1U].left) &&
               (lineinfo[y - 1U].left >= lineinfo[y - 2U].left) &&
               (lineinfo[y - 2U].left >= lineinfo[y - 3U].left) &&
               (y < watch.cross_LD_angle) &&
               (g_frame_img[CROSS_IMG_ROW_MAX - y - 2U][col] > watch.threshold) &&
               (lineinfo[y - 2U].left > lineinfo[y - 4U].left) &&
               (lineinfo[y - 1U].left > lineinfo[y - 3U].left) &&
               (lineinfo[y].left > lineinfo[y - 2U].left) &&
               (lineinfo[y].left > lineinfo[y + 2U].left) &&
               (lineinfo[y + 1U].left > lineinfo[y + 3U].left))
            {
                watch.cross_LD_angle = y; /* 锁定更靠近车体的左倾交点 */
                break;
            }
        }
    }
}

/*
 * Purpose: 十字通过过程中的双边拐点跟踪
 * Param  : 无
 * Return : 无
 * Note   : 更新 cross_AngleL/cross_AngleR 供补线与控制使用
 */
void cross_running(void)
{
    uint8 y;           /* 图像行索引 */

    if((watch.cross_flag != 1U) && (watch.cross_flag != 2U) && (watch.cross_flag != 3U))
    {
        return;
    }

    for(y = 25U; y < 87U; y++) /* 逐行扫描左侧拐点 */
    {
        if(!lineinfo[y + 2U].left_lost &&
           !lineinfo[y + 1U].left_lost &&
           !lineinfo[y].left_lost &&
           (((lineinfo[y].left - lineinfo[y - 1U].left) >= 3) &&
            ((lineinfo[y].left - lineinfo[y - 2U].left) >= 7) &&
            ((lineinfo[y].left - lineinfo[y - 3U].left) > 12) ||
            (lineinfo[y - 2U].left_lost && lineinfo[y - 3U].left_lost && lineinfo[y - 4U].left_lost)) &&
           ((lineinfo[y + 1U].left - lineinfo[y].left) <= 3) &&
           ((lineinfo[y + 1U].left - lineinfo[y].left) >= 0) &&
           ((lineinfo[y + 2U].left - lineinfo[y + 1U].left) <= 3) &&
           ((lineinfo[y + 2U].left - lineinfo[y + 1U].left) >= 0) &&
           ((lineinfo[y + 3U].left - lineinfo[y + 2U].left) <= 3) &&
           ((lineinfo[y + 3U].left - lineinfo[y + 2U].left) >= 0) &&
           ((lineinfo[y + 4U].left - lineinfo[y + 3U].left) <= 3) &&
           ((lineinfo[y + 4U].left - lineinfo[y + 3U].left) >= 0) &&
           (lineinfo[y + 1U].left > 30) &&
           (y > 40U))
        {
            watch.cross_AngleL = y;                   /* 更新左拐点行号 */
            watch.cross_AngleL_x = lineinfo[y].left;  /* 同步左拐点列坐标 */
        }
    }

    for(y = 25U; y < 87U; y++) /* 逐行扫描右侧拐点 */
    {
        if(!lineinfo[y + 2U].right_lost &&
           !lineinfo[y + 1U].right_lost &&
           !lineinfo[y].right_lost &&
           (((lineinfo[y - 1U].right - lineinfo[y].right) > 3) &&
            ((lineinfo[y - 2U].right - lineinfo[y].right) > 7) &&
            ((lineinfo[y - 3U].right - lineinfo[y].right) > 12) ||
            (lineinfo[y - 1U].right_lost && lineinfo[y - 2U].right_lost && lineinfo[y - 3U].right_lost)) &&
           ((lineinfo[y].right - lineinfo[y + 1U].right) <= 3) &&
           ((lineinfo[y].right - lineinfo[y + 1U].right) >= 0) &&
           ((lineinfo[y + 1U].right - lineinfo[y + 2U].right) <= 3) &&
           ((lineinfo[y + 1U].right - lineinfo[y + 2U].right) >= 0) &&
           ((lineinfo[y + 2U].right - lineinfo[y + 3U].right) <= 3) &&
           ((lineinfo[y + 2U].right - lineinfo[y + 3U].right) >= 0) &&
           ((lineinfo[y + 3U].right - lineinfo[y + 4U].right) <= 3) &&
           ((lineinfo[y + 3U].right - lineinfo[y + 4U].right) >= 0) &&
           (lineinfo[y + 1U].right < 158) &&
           (y > 40U))
        {
            watch.cross_AngleR = y;                    /* 更新右拐点行号 */
            watch.cross_AngleR_x = lineinfo[y].right;  /* 同步右拐点列坐标 */
        }
    }

    if((watch.cross_AngleL < 110) && (watch.cross_AngleR < 110))
    {
        watch.cross_flag = 3U; /* 双侧角点均有效，进入稳定十字通过状态 */
    }

    if((watch.cross_AngleL < 50) || (watch.cross_AngleR < 50))
    {
        watch.cross_AngleL = 120; /* 近处误检时回退到无效默认值 */
        watch.cross_AngleR = 120;
    }
}

/*
 * Purpose: 十字元素退出判定
 * Param  : 无
 * Return : 无
 * Note   : 优先按积分里程退出，次级按中远处双边恢复退出
 */
void cross_out(void)
{
    if((get_integeral_state(&distance_integral) == 2) && (watch.cross_flag == 3U))
    {
        clear_distant_integeral();   /* 清除十字积分状态 */
        out_element();               /* 退出当前元素状态机 */
        beep2(5, 200U);             /* 蜂鸣提示一次元素完成 */
    }
    else if(!lineinfo[65].left_lost && !lineinfo[65].right_lost)
    {
        watch.angle_far_line = 70;   /* 双边恢复后收回远线关注高度 */
    }
}

