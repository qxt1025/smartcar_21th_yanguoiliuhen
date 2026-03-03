#include "search_line.h"
#include "scan_line.h"
#include "Element_recognition.h"

extern uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W];
extern uint8 (*temp_img)[MT9V03X_W];

uint8 reference_point = 0; // 动态参考点
uint8 reference_col = 0; // 动态参考列
uint8 white_max_point = 0; // 白点上限
uint8 white_min_point = 0; // 白点下限
uint8 reference_contrast_ratio = 20; // 对比度阈值

uint8 remote_distance[SEARCH_IMAGE_W]     = {0}; // 白点最远距离
uint8 far reference_col_line[SEARCH_IMAGE_H]  = {0}; // 参考列线
uint8 left_edge_line[SEARCH_IMAGE_H]      = {0}; // 左边界
uint8 right_edge_line[SEARCH_IMAGE_H]     = {0}; // 右边界
uint8 far middle_line[SEARCH_IMAGE_H]     = {0}; // 中线

uint8 zebra_detected = 0; // 斑马线检测标志
uint8 zebra_jump_count = 0; // 斑马线跳变计数

uint8 far if_count = 0;

void get_reference_point(const uint8 *image)
{
    uint8 *p = (uint8 *)&image[(SEARCH_IMAGE_H - REFRENCEROW) * SEARCH_IMAGE_W]; // 统计区起始指针
    uint16 temp = 0; // 统计点数量
    uint32 temp1 = 0; // 像素灰度累加
	uint16 i = 0;
    temp = REFRENCEROW * SEARCH_IMAGE_W; // 统计点总数
	
    for(i = 0; i < temp; i++)
    {
        temp1 += *(p + i); // 累加灰度
    }
    reference_point = (uint8)(temp1 / temp); // 平均灰度作为参考点
    white_max_point = (uint8)func_limit_ab((int32)reference_point * WHITEMAXMUL / 10, BLACKPOINT, 255); // 计算白点上限
    white_min_point = (uint8)func_limit_ab((int32)reference_point * WHITEMINMUL / 10, BLACKPOINT, 255); // 计算白点下限
}

void search_reference_col(const uint8 *image)
{
    int col, row;
    int16 temp1 = 0, temp2 = 0, temp3 = 0;
    int16 jump_diff = 0;
    //uint8 jump_count = 0;
    uint16 i = 0;

    for(col = 0; col < SEARCH_IMAGE_W; col++)
    {
        remote_distance[col] = SEARCH_IMAGE_H - 1;
    }

    for(col = 0; col < SEARCH_IMAGE_W; col += CONTRASTOFFSET)
    {
        for(row = SEARCH_IMAGE_H - 1; row > STOPROW; row -= CONTRASTOFFSET)
        {
            temp1 = *(image + row * SEARCH_IMAGE_W + col); // 当前点灰度
            temp2 = *(image + (row - STOPROW) * SEARCH_IMAGE_W + col); // 对比点灰度

            if(temp2 > white_max_point) // 对比点太亮则跳过
            {
                continue;
            }

            if(temp1 < white_min_point) // 当前点太暗则记录
            {
                remote_distance[col] = (uint8)row;
                break;
            }

            temp3 = (temp1 - temp2) * 200 / (temp1 + temp2); // 计算对比度

            if(temp3 > reference_contrast_ratio || row == STOPROW) // 满足阈值或到顶行
            {
                remote_distance[col] = (uint8)row;
                break;
            }
        }
    }
/*斑马线部分*/
    watch.jump_count = 0;
    for(col = CONTRASTOFFSET * 3; col <= SEARCH_IMAGE_W - CONTRASTOFFSET * 3; col += CONTRASTOFFSET)
    {
        jump_diff = (int16)remote_distance[col + CONTRASTOFFSET] - (int16)remote_distance[col];
        if(jump_diff < 0)
        {
            jump_diff = -jump_diff;
        }

        if(jump_diff >= ZEBRA_JUMP_DIFF_MIN && jump_diff <= ZEBRA_JUMP_DIFF_MAX)
        {
			//ips200_draw_point(col, remote_distance[col], RGB565_YELLOW);
            watch.jump_count++;
        }
    }

    zebra_jump_count = watch.jump_count;
    zebra_detected = (zebra_jump_count >= ZEBRA_JUMP_COUNT_THRESHOLD) ? 1 : 0;
    watch.zebra_flag2 = zebra_detected;
    reference_col = find_extreme_value(remote_distance, 10, SEARCH_IMAGE_W - 10, 0) + CONTRASTOFFSET; // 取最远白点列
    reference_col = (uint8)func_limit_ab(reference_col, 1, SEARCH_IMAGE_W - 2); // 参考列限幅
    watch.watch_lost = SEARCH_IMAGE_H - remote_distance[reference_col-CONTRASTOFFSET] - 1;
	
	
    for(i = 0; i < SEARCH_IMAGE_H; i++)
    {
        reference_col_line[i] = reference_col;
    }
}
void search_line(const uint8 *image)
{
    uint8 *p = (uint8 *)image[0]; // 行指针
    uint8 row_max = SEARCH_IMAGE_H - 1; // 行最大
    uint8 row_min = STOPROW; // 行最小
    uint8 col_max = SEARCH_IMAGE_W - CONTRASTOFFSET; // 列最大
    uint8 col_min = CONTRASTOFFSET; // 列最小
    int16 leftstartcol = reference_col; // 左搜线起点
    int16 rightstartcol = reference_col; // 右搜线起点
    int16 leftendcol = 0; // 左搜线终点
    int16 rightendcol = SEARCH_IMAGE_W - 1; // 右搜线终点
    uint8 search_time = 0; // 单点搜索次数
    uint8 temp1 = 0, temp2 = 0; // 临时灰度
    int temp3 = 0; // 临时对比度
    int leftstop = 0, rightstop = 0, stoppoint = 0; // 自锁标志

    int col, row;

    for(row = row_max; row >= row_min; row--)
    {
        left_edge_line[row] = col_min;
        right_edge_line[row] = col_max;
    }

    for(row = row_max; row >= row_min; row--)
    {
        p = (uint8 *)&image[row * SEARCH_IMAGE_W]; // 本行首地址
        if(!leftstop)
        {
            search_time = 2;
            do
            {
                if(search_time == 1)
                {
                    leftstartcol = reference_col;
                    leftendcol = col_min;
                }
                search_time--;
                for(col = leftstartcol; col >= leftendcol; col--)
                {
                    temp1 = *(p + col); // 当前点灰度
                    temp2 = *(p + col - CONTRASTOFFSET); // 对比点灰度

                    if(temp1 < white_min_point && col == leftstartcol && leftstartcol == reference_col) // 参考列为黑点则锁定
                    {
                        leftstop = 1; // 左搜线自锁
                        for(stoppoint = row; stoppoint >= 0; stoppoint--)
                        {
                            left_edge_line[stoppoint] = col_min - CONTRASTOFFSET;
                        }
                        search_time = 0;
                        break;
                    }

                    if(temp1 < white_min_point) // 当前点太暗则记录
                    {
                        left_edge_line[row] = (uint8)col;
                        break;
                    }

                    if(temp2 > white_max_point) // 对比点太亮则跳过
                    {
                        continue;
                    }

                    temp3 = (temp1 - temp2) * 200 / (temp1 + temp2); // 计算对比度
                    if(temp3 > reference_contrast_ratio || col == col_min) // 达阈值或到边界
                    {
                        left_edge_line[row] = col ; // 记录左边界

                        leftstartcol = (uint8)func_limit_ab(col + SEARCHRANGE, col, col_max);
                        leftendcol = (uint8)func_limit_ab(col - SEARCHRANGE, col_min, col);
                        search_time = 0;
                        break;
                    }
                }
            } while(search_time);
        }
        if(!rightstop)
        {
            search_time = 2;
            do
            {
                if(search_time == 1)
                {
                    rightstartcol = reference_col;
                    rightendcol = col_max;
                }
                search_time--;
                for(col = rightstartcol; col <= rightendcol; col++)
                {
                    temp1 = *(p + col); // 当前点灰度
                    temp2 = *(p + col + CONTRASTOFFSET); // 对比点灰度

                    if(temp1 < white_min_point && col == rightstartcol && rightstartcol == reference_col) // 参考列为黑点则锁定
                    {
                        rightstop = 1; // 右搜线自锁
                        for(stoppoint = row; stoppoint >= 0; stoppoint--)
                        {
                            right_edge_line[stoppoint] = col_max + CONTRASTOFFSET;
                        }
                        search_time = 0;
                        break;
                    }

                    if(temp1 < white_min_point) // 当前点太暗则记录
                    {
                        right_edge_line[row] = (uint8)col;
                        break;
                    }

                    if(temp2 > white_max_point) // 对比点太亮则跳过
                    {
                        continue;
                    }

                    temp3 = (temp1 - temp2) * 200 / (temp1 + temp2); // 计算对比度
                    if_count++;
                    if(temp3 > reference_contrast_ratio || col == col_max) // 达阈值或到边界
                    {
                        right_edge_line[row] = col ; // 记录右边界

                        rightstartcol = (uint8)func_limit_ab(col - SEARCHRANGE, col_min, col);
                        rightendcol = (uint8)func_limit_ab(col + SEARCHRANGE, col, col_max);
                        search_time = 0;
                        break;
                    }
                }
            } while(search_time);
        }
    }
}
//------------------------------------------------------------------------
// 函数简介     复制摄像头图像到临时数组
// 参数说明     无
// 返回类型     void
// 使用示例     copy_mt9v03x_to_temp();
// 备注信息     目标数组 temp_img 定义在 user/main.c
//------------------------------------------------------------------------
void copy_mt9v03x_to_temp(void)
{
    uint16 row;
    uint16 col;

    for(row = 0; row < MT9V03X_H; row++)
    {
        for(col = 0; col < MT9V03X_W; col++)
        {
            temp_img[row][col] = mt9v03x_image[row][col];
        }
    }
}
//------------------------------------------------------------------------
// 函数简介     统计各行丢线并汇总到 watch
// 参数说明     无
// 返回类型     void
// 使用示例     count_line_lost();
// 备注信息     left==0 视为左丢线  right>=SEARCH_IMAGE_W-1 视为右丢线
//------------------------------------------------------------------------
void count_line_lost(void)
{
    int i = 0;
    watch.left_lost = 0;
    watch.right_lost = 0;
    watch.cross = 0;
    watch.left_black = 0;
    watch.right_black = 0;
    watch.left_near_lost = SEARCH_IMAGE_H-1;
    watch.right_near_lost = SEARCH_IMAGE_H-1;
    watch.left_far_lost = 0;
    watch.right_far_lost = 0;

    for(i = 0; i < SEARCH_IMAGE_H; i++)
    {
        //左
        if(lineinfo[i].left == CONTRASTOFFSET)
        {
            lineinfo[i].left_lost = 1;
            watch.left_lost ++;
			if(i < watch.left_near_lost)
            {
                watch.left_near_lost = i; // 更新为更小的y值
            }
            if(i > watch.left_far_lost && i < watch.watch_lost)
            {
                watch.left_far_lost = i;
            }
        }
        else 
        {
            lineinfo[i].left_lost = 0;
        }
        if(lineinfo[i].left <= 0)
        {
            lineinfo[i].left_black = 1;
            watch.left_black++;
        }
        else
        {
            lineinfo[i].left_black = 0;
        }

        if(lineinfo[i].right == SEARCH_IMAGE_W - CONTRASTOFFSET - 1)
        {
            lineinfo[i].right_lost = 1;
            watch.right_lost ++;
			if(i < watch.right_near_lost)
            {
                watch.right_near_lost = i; // 更新为更小的y值
            }
            if(i > watch.right_far_lost && i < watch.watch_lost)
            {
                watch.right_far_lost = i;
            }
        }
        else 
        {
            lineinfo[i].right_lost = 0;
        }
        if(lineinfo[i].right >= (SEARCH_IMAGE_W - 1))
        {
            lineinfo[i].right_black = 1;
            watch.right_black++;
        }
        else
        {
            lineinfo[i].right_black = 0;
        }
        //右
        if(lineinfo[i].left_black && lineinfo[i].right_black)
        {
            lineinfo[i].whole_black = 1;
        }
        else
        {
            lineinfo[i].whole_black = 0;
        }
        if(lineinfo[i].left_lost && lineinfo[i].right_lost)
        {
            lineinfo[i].whole_lost = 1;
            watch.cross++;
        }
        else
        {
            lineinfo[i].whole_lost = 0;
        }
    }
}
//------------------------------------------------------------------------
// 函数简介     线数据后处理：限幅、求中线并同步 lineinfo
// 参数说明     无
// 返回类型     void
// 使用示例     post_process_lines();
// 备注信息     lineinfo 使用 119 - y 对应行
//------------------------------------------------------------------------
void post_process_lines(void)
{
    int y = 0;
    for(y = 0; y < SEARCH_IMAGE_H; y++)
    {
        lineinfo[119 - y].left = func_limit_ab(left_edge_line[y], 0, SEARCH_IMAGE_W - 1);
        lineinfo[119 - y].right = func_limit_ab(right_edge_line[y], 0, SEARCH_IMAGE_W - 1);
        //middle_line[y] = (left_edge_line[y] + right_edge_line[y]) / 2;
        //lineinfo[119 - y].left = left_edge_line[y];
        //lineinfo[119 - y].right = right_edge_line[y];
    }
}
