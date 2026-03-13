/*
 * scan_line.c
 *
 *  Created on: 2023年6月21日
 *      Author: Admin
 */
#include <stdlib.h>
#include <string.h>
#include "scan_line.h"
#include "Binarization.h"
#include "Element_recognition.h"

struct lineinfo_s lineinfo[120];
#define LINE_WIDTH 188

void scan_line()
{
    int y;

    memset(lineinfo, 0, 120 * sizeof(struct lineinfo_s));
    line_single(&lineinfo[base_line], g_frame_img[119 - base_line]);   // 寻找基准行

    for (y = base_line - 1; y >= forward_near; y--) // 向下搜线到near行
    {
        line_findnext(&lineinfo[y], g_frame_img[119 - y], &lineinfo[y + 1]);
    }

    for (y = base_line + 1; y < forward_far; y++) // 向上搜线到far行
    {
        lineinfo[y].y = y;
        line_findnext(&lineinfo[y], g_frame_img[119 - y], &lineinfo[y - 1]);
        watch.watch_line = y; // 摄像头视野的赛道最远端
    }
}

/*寻找基准行
 * ****************************************************************************
 */
int line_single(struct lineinfo_s *lineinfo, unsigned char *inputimg)
{
    uint8 edge_store[_EDGE_STORE_SIZE];

    memset(edge_store, 0, sizeof(edge_store));
    // 扫描inputimg数组（即行数组），并将跳变沿所对应的列数依次放到edge_store数组中
    lineinfo->edge_count = get_orign_edges(inputimg, edge_store);

    // 得到最大边沿
    get_max_edge(lineinfo, edge_store);
    return 0;
}

/*
 * 从基准行向下搜线
 */
int line_findnext(struct lineinfo_s *lineinfo, uint8 *inputimg, struct lineinfo_s *lineinfo_ref)
{
    // 得到所有凸边沿
    uint8 *edge_store;

    edge_store = lineinfo->edge_store;
    lineinfo->edge_count = get_orign_edges(inputimg, edge_store);

    // watch.addline_y = lineinfo->y;
    if (lineinfo->y > 20 && lineinfo->y < 100)
    {
        zebra_detect(lineinfo, edge_store, inputimg); // 斑马线检测
        // apriltag_detect(lineinfo, edge_store, inputimg); // apriltag检测
    }

    // 得到最佳边沿
    get_best_edge(lineinfo, edge_store, lineinfo_ref);
    // Junc_detect(lineinfo, edge_store, inputimg); // apriltag检测

    return 0;
}

/*
 * 存储所有黑白跳变沿位置到数组edge_store中
 */
uint8 get_orign_edges(uint8 *inputimg, uint8 *edge_store)
{
#if _EDGE_STORE_SIZE % 2 != 0
#error "_EDGE_STORE_SIZE must be even!"
#endif
    uint8 edge_store_idx;
    uint8 px;

    edge_store_idx = 0;

    // 数组第0位大于阈值（图像最左端为白）则认为此处为跳变沿
    if (inputimg[0] > watch.threshold)
    {
        edge_store[edge_store_idx] = 0;
        edge_store_idx++;
    }

    for (px = 1; px < LINE_WIDTH; px++)
    {
        // 分布在阈值两侧认为是跳边沿
        if (!((inputimg[px - 1] > watch.threshold && inputimg[px] < watch.threshold) ||
              (inputimg[px - 1] < watch.threshold && inputimg[px] > watch.threshold)))
        {
            continue;
        }

        if (edge_store_idx >= _EDGE_STORE_SIZE)
        {
            break;
        }

        edge_store[edge_store_idx] = px; // 将跳变沿存入数组
        edge_store_idx++;
    }

    // 数组最后一位大于阈值（图像最右端为白）则认为此处为跳变沿
    if (inputimg[LINE_WIDTH - 1] > watch.threshold)
    {
        if (edge_store_idx < _EDGE_STORE_SIZE)
        {
            edge_store[edge_store_idx] = LINE_WIDTH - 1;
            edge_store_idx++;
        }
    }

    return edge_store_idx;
}

/*得到最大边沿
 * ***************************************************************************
 */
int get_max_edge(struct lineinfo_s *lineinfo, uint8 *edge_store)
{
    int max_width;
    int max_idx;
    int k;

    max_width = 0;
    max_idx = 0;

    if (lineinfo->edge_count < 2)
    {
        lineinfo->left = 0;
        lineinfo->right = LINE_WIDTH - 1;
        return 0;
    }

    // 选取最大的块
    if (lineinfo->edge_count < 10)
    {
        for (k = 0; (k + 1) < lineinfo->edge_count; k += 2)
        {
            if ((edge_store[k + 1] - edge_store[k]) > max_width)
            {
                max_width = edge_store[k + 1] - edge_store[k];
                max_idx = k;
            }
        }
    }
    else
    {
        max_idx = 0;
    }

    lineinfo->left = edge_store[max_idx];
    lineinfo->right = edge_store[max_idx + 1];
    return 0;
}

/*
 * 寻找最佳边沿
 * 根据相邻的一行找到的左边沿和右边沿位置,迭代找出本行与之距离最小的边沿位置
 */
int get_best_edge(struct lineinfo_s *lineinfo, uint8 *edge_store, struct lineinfo_s *lineinfo_ref)
{
    int min_abs_err;
    int temp_err;
    int black_pix_count;
    int min_width;
    int max_idx;
    int dst;
    uint8 temp_left_idx;
    uint8 temp_right_idx;
    uint8 k;
    uint8 i;
    uint8 dst_flag;

    temp_left_idx = 0;
    temp_right_idx = 1;

    if (lineinfo->edge_count < 2)
    {
        lineinfo->left = 0;
        lineinfo->right = LINE_WIDTH - 1;
        lineinfo->mid = (lineinfo->left + lineinfo->right) / 2;
        lineinfo->left_lost = 1;
        lineinfo->right_lost = 1;
        lineinfo->whole_lost = 1;
        lineinfo->whole_black = 0;
        return 0;
    }

    // 获得最近邻的左边沿
    min_abs_err = LINE_WIDTH * 2;
    for (k = 0; k < lineinfo->edge_count; k += 2)
    {
        temp_err = abs((int)lineinfo_ref->left - (int)edge_store[k]);
        if (temp_err < min_abs_err)
        {
            min_abs_err = temp_err;
            temp_left_idx = k;
        }
        if (edge_store[k] > lineinfo_ref->left + min_abs_err)
        {
            break;
        }
    }

    temp_right_idx = temp_left_idx + 1;

    // 获得最近邻的右边沿
    min_abs_err = LINE_WIDTH * 2;
    for (k = temp_right_idx; k < lineinfo->edge_count; k += 2)
    {
        temp_err = abs((int)lineinfo_ref->right - (int)edge_store[k]);
        if (temp_err < min_abs_err)
        {
            min_abs_err = temp_err;
            temp_right_idx = k;
        }
        if (edge_store[k] > lineinfo_ref->right + min_abs_err)
        {
            break;
        }
    }

    if ((lineinfo->y > 20) && (lineinfo->edge_count > 1) &&
        ((edge_store[temp_right_idx] - edge_store[temp_left_idx]) > 4))
    {
        watch.watch_lost = watch.watch_line;
        lineinfo->whole_lost = 0;
    }
    else
    {
        lineinfo->whole_lost = 1;
    }

    // 判断中间是否有杂点，障碍物，环岛等
    black_pix_count = 0;
    for (k = (uint8)(temp_left_idx + 1); k < temp_right_idx; k += 2)
    {
        black_pix_count += edge_store[k + 1] - edge_store[k];
    }

    if ((8 * black_pix_count) > (edge_store[temp_right_idx] - edge_store[temp_left_idx]))
    {
        min_width = 188;
        max_idx = temp_left_idx;

        dst = temp_left_idx + 1;
        dst_flag = 0;

        for (i = (uint8)(temp_left_idx + 1); i < temp_right_idx; i += 2)
        {
            if ((edge_store[i + 1] - edge_store[i]) < _MIN_EDGE_WIDTH)
            {
                if ((uint8)(i + 2) < lineinfo->edge_count)
                {
                    edge_store[dst] = edge_store[i + 2];
                    lineinfo->edge_count -= 2;
                    dst_flag = 1;
                }
            }
            else
            {
                if (dst_flag == 1)
                {
                    edge_store[dst + 2] = edge_store[i];
                    edge_store[dst + 3] = edge_store[i + 1];
                }
                else
                {
                    edge_store[dst] = edge_store[i];
                    edge_store[dst + 1] = edge_store[i + 1];
                }
                dst += 2;
            }
        }

        for (k = temp_left_idx; (k + 1) < lineinfo->edge_count; k += 2)
        {
            if (abs(edge_store[k + 1] + edge_store[k] - 188) < min_width)
            {
                min_width = abs(edge_store[k + 1] + edge_store[k] - 188);
                max_idx = k;
            }
        }

        temp_left_idx = (uint8)max_idx;
        temp_right_idx = (uint8)(max_idx + 1);
    }

    lineinfo->left = edge_store[temp_left_idx];
    lineinfo->right = edge_store[temp_right_idx];
    lineinfo->mid = (lineinfo->left + lineinfo->right) / 2;

    if (lineinfo->right >= 187)
    {
        lineinfo->right_lost = 1;
    }
    else
    {
        lineinfo->right_lost = 0;
    }

    if (lineinfo->left <= 0)
    {
        lineinfo->left_lost = 1;
    }
    else
    {
        lineinfo->left_lost = 0;
    }

    if (lineinfo->right <= 0 && lineinfo->left <= 0)
    {
        lineinfo->whole_black = 1;
    }
    else
    {
        lineinfo->whole_black = 0;
    }

    return 0;
}

// 斑马线识别函数
int zebra_detect(struct lineinfo_s *lineinfo, uint8 *edge_store, uint8 *inputimg)
{
    uint8 white_width;
    uint8 zobra_white_count;
    uint8 edge_now;
    uint8 k;

    zobra_white_count = 0;

    if (lineinfo->edge_count > 12)
    {
        for (k = 0; k < lineinfo->edge_count; k += 2)
        {
            edge_now = edge_store[k];
            if (edge_now < LINE_WIDTH)
            {
                if ((inputimg[edge_now] & 0x80) == 0x80)
                {
                    white_width = edge_store[k + 1] - edge_store[k];
                    if (white_width > 1 && white_width < 14)
                    {
                        zobra_white_count++;
                        watch.ZebraInLine = lineinfo->y; // 取满足条件时y最大值作为ZebraLine
                    }
                }
            }
        }

        if (zobra_white_count > 6)
        {
            lineinfo->zebra_flag = 1;
        }
    }
    else
    {
        lineinfo->zebra_flag = 0;
    }

    return 0;
}