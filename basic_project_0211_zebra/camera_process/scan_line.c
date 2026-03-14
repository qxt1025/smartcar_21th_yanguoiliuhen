/*
 * 扫线模块
 *
 *  创建日期: 2023年6月21日
 *      作者: Admin
 */
#include "scan_line.h"
#include "Binarization.h"
#include "Element_recognition.h"

struct lineinfo_s lineinfo[120];
#define LINE_WIDTH 188
#define EDGE_SCAN_MARGIN        24U    // 参考边界外扩像素
#define EDGE_SCAN_MIN_WIDTH     72U    // 局部窗口最小扫描宽度
#define ZEBRA_CHECK_ROW_MASK    0x01U  // 斑马线隔行检测掩码
#define ABS_DIFF_INT(a, b)      (((a) >= (b)) ? ((a) - (b)) : ((b) - (a)))
#define SCAN_USE_ROI            1U     // 1:使用局部窗口扫描 0:使用全行扫描(用于对照测试)

void scan_line()
{
    uint8 y;
    uint8 (*img_row)[MT9V03X_W];
    struct lineinfo_s *cur_line;
    struct lineinfo_s *ref_line;

    watch.watch_line = base_line; // 默认最远可见行为基准行

    // 基准行：避免整块清零，只初始化必要字段
    cur_line = &lineinfo[base_line];
    cur_line->y = base_line;
    cur_line->zebra_flag = 0;
    line_single(cur_line, g_frame_img[119 - base_line]);   // 寻找基准行

    // 保持与旧逻辑一致：基准行这些状态位默认清零
    cur_line->mid = 0;
    cur_line->left_lost = 0;
    cur_line->right_lost = 0;
    cur_line->whole_lost = 0;
    cur_line->whole_black = 0;

    // 向下搜线到最近端：使用行指针步进，减少重复地址计算
    if(base_line > forward_near)
    {
        y = (uint8)(base_line - 1);
        img_row = &g_frame_img[119 - y];
        ref_line = &lineinfo[y + 1];

        while(1)
        {
            cur_line = &lineinfo[y];
            cur_line->y = y;
            cur_line->zebra_flag = 0;
            line_findnext(cur_line, *img_row, ref_line);

            if(y == (uint8)forward_near)
            {
                break;
            }

            y--;
            img_row++;
            ref_line = cur_line;
        }
    }

    // 向上搜线到最远端
    y = (uint8)(base_line + 1);
    if(y < (uint8)forward_far)
    {
        img_row = &g_frame_img[119 - y];
        ref_line = &lineinfo[y - 1];

        for(; y < (uint8)forward_far; y++)
        {
            cur_line = &lineinfo[y];
            cur_line->y = y;
            cur_line->zebra_flag = 0;
            line_findnext(cur_line, *img_row, ref_line);
            watch.watch_line = y; // 摄像头视野的赛道最远端（逐行更新）

            img_row--;
            ref_line = cur_line;
        }
    }
}

/*
 * 基于参考行边界的局部窗口边沿搜索：优先缩小扫描窗口，加速每行处理
 */
static uint8 get_orign_edges_roi(uint8 *inputimg, uint8 *edge_store, struct lineinfo_s *lineinfo_ref)
{
    int ref_left;
    int ref_right;
    int scan_left;
    int scan_right;
    int span;
    int center;
    uint8 start_px;
    uint8 end_px;
    uint8 edge_store_idx;
    uint8 px;
    uint8 thr;
    uint8 prev;
    uint8 cur;
    uint8 prev_side;
    uint8 cur_side;

    ref_left = (int)lineinfo_ref->left;
    ref_right = (int)lineinfo_ref->right;

    if((ref_right > ref_left) && ((ref_right - ref_left) < LINE_WIDTH))
    {
        scan_left = ref_left - (int)EDGE_SCAN_MARGIN;
        scan_right = ref_right + (int)EDGE_SCAN_MARGIN;

        span = scan_right - scan_left;
        if(span < (int)EDGE_SCAN_MIN_WIDTH)
        {
            center = (ref_left + ref_right) / 2;
            scan_left = center - ((int)EDGE_SCAN_MIN_WIDTH / 2);
            scan_right = center + ((int)EDGE_SCAN_MIN_WIDTH / 2);
        }
    }
    else
    {
        scan_left = 0;
        scan_right = LINE_WIDTH - 1;
    }

    if(scan_left < 0)
    {
        scan_left = 0;
    }
    if(scan_right >= LINE_WIDTH)
    {
        scan_right = LINE_WIDTH - 1;
    }

    start_px = (uint8)scan_left;
    end_px = (uint8)scan_right;
    thr = watch.threshold;
    edge_store_idx = 0;

    if(inputimg[start_px] > thr)
    {
        edge_store[edge_store_idx++] = start_px;
    }

    prev = inputimg[start_px];
    prev_side = (prev < thr) ? 1U : 0U;
    for(px = (uint8)(start_px + 1U); px <= end_px; px++)
    {
        if(edge_store_idx >= _EDGE_STORE_SIZE)
        {
            break;
        }

        cur = inputimg[px];
        cur_side = (cur < thr) ? 1U : 0U;

        /* 先判断是否跨阈值，再排除等于阈值的像素点 */
        if(prev_side != cur_side)
        {
            if((prev != thr) && (cur != thr))
            {
                edge_store[edge_store_idx++] = px;
            }
        }

        prev = cur;
        prev_side = cur_side;
    }

    if(inputimg[end_px] > thr)
    {
        if(edge_store_idx < _EDGE_STORE_SIZE)
        {
            edge_store[edge_store_idx++] = end_px;
        }
    }

    // 保证偶数边沿，避免后续按对访问越界
    if((edge_store_idx & 0x01U) != 0U)
    {
        edge_store_idx--;
    }

    // 局部窗口异常时回退全行扫描，保证鲁棒性
    if(edge_store_idx < 2U)
    {
        return get_orign_edges(inputimg, edge_store);
    }

    return edge_store_idx;
}
/*寻找基准行
 * ****************************************************************************
 */
int line_single(struct lineinfo_s *lineinfo, unsigned char *inputimg)
{
    uint8 *edge_store;

    edge_store = lineinfo->edge_store;
    // 扫描图像行，并将跳变沿对应的列号依次写入边沿数组
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
    // 得到所有凸边沿（优先局部窗口扫描）
    uint8 *edge_store;

    edge_store = lineinfo->edge_store;
#if (SCAN_USE_ROI == 1U)
    lineinfo->edge_count = get_orign_edges_roi(inputimg, edge_store, lineinfo_ref);
#else
    lineinfo->edge_count = get_orign_edges(inputimg, edge_store);
#endif

    // 斑马线检测：隔行执行，降低热路径负载
    if((lineinfo->y > 20U) && (lineinfo->y < 100U) && ((lineinfo->y & ZEBRA_CHECK_ROW_MASK) == 0U))
    {
        zebra_detect(lineinfo, edge_store, inputimg);
    }

    // 得到最佳边沿
    get_best_edge(lineinfo, edge_store, lineinfo_ref);

    return 0;
}

/*
 * 存储所有黑白跳变沿位置到边沿数组中
 * 面向单片机的优化版
 */
uint8 get_orign_edges(uint8 *inputimg, uint8 *edge_store)
{
#if (_EDGE_STORE_SIZE % 2) != 0
#error "_EDGE_STORE_SIZE must be even!"
#endif

    uint8 edge_store_idx = 0;
    uint8 px;
    uint8 thr;
    uint8 prev;
    uint8 cur;
    uint8 prev_side;
    uint8 cur_side;
    uint8 last_px;

    thr = watch.threshold;
    last_px = (uint8)(LINE_WIDTH - 1);

    /* 左端白，记 0 */
    if (inputimg[0] > thr)
    {
        if (edge_store_idx < _EDGE_STORE_SIZE)
        {
            edge_store[edge_store_idx++] = 0;
        }
    }

    prev = inputimg[0];
    prev_side = (prev < thr) ? 1U : 0U;

    for (px = 1; px < LINE_WIDTH; px++)
    {
        if (edge_store_idx >= _EDGE_STORE_SIZE)
        {
            break;
        }

        cur = inputimg[px];
        cur_side = (cur < thr) ? 1U : 0U;

        /* 先判断是否跨阈值，再排除等于阈值的像素点 */
        if (prev_side != cur_side)
        {
            if ((prev != thr) && (cur != thr))
            {
                edge_store[edge_store_idx++] = px;
            }
        }

        prev = cur;
        prev_side = cur_side;
    }

    /* 右端白，记最后一点 */
    if (inputimg[last_px] > thr)
    {
        if (edge_store_idx < _EDGE_STORE_SIZE)
        {
            edge_store[edge_store_idx++] = last_px;
        }
    }

    return edge_store_idx;
}

/*得到最大边沿
 * ***************************************************************************
 */
int get_max_edge(struct lineinfo_s *lineinfo, uint8 *edge_store)
{
    uint8 max_width;
    uint8 max_idx;
    uint8 k;
    uint8 this_width;

    max_width = 0;
    max_idx = 0;

    if(lineinfo->edge_count < 2)
    {
        lineinfo->left = 0;
        lineinfo->right = LINE_WIDTH - 1;
        return 0;
    }

    // 选取最大的白块
    if(lineinfo->edge_count < 10)
    {
        for(k = 0; (k + 1U) < lineinfo->edge_count; k += 2U)
        {
            this_width = (uint8)(edge_store[k + 1U] - edge_store[k]);
            if(this_width > max_width)
            {
                max_width = this_width;
                max_idx = k;
            }
        }
    }
    else
    {
        max_idx = 0;
    }

    lineinfo->left = edge_store[max_idx];
    lineinfo->right = edge_store[max_idx + 1U];
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
        temp_err = ABS_DIFF_INT((int)lineinfo_ref->left, (int)edge_store[k]);
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
        temp_err = ABS_DIFF_INT((int)lineinfo_ref->right, (int)edge_store[k]);
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
            temp_err = ABS_DIFF_INT((int)(edge_store[k + 1] + edge_store[k]), LINE_WIDTH);
            if (temp_err < min_width)
            {
                min_width = temp_err;
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
    uint8 zebra_white_count;
    uint8 edge_now;
    uint8 k;

    lineinfo->zebra_flag = 0;
    if(lineinfo->edge_count <= 12U)
    {
        return 0;
    }

    zebra_white_count = 0;
    for(k = 0; k < lineinfo->edge_count; k += 2U)
    {
        edge_now = edge_store[k];
        if(edge_now >= LINE_WIDTH)
        {
            continue;
        }

        if((inputimg[edge_now] & 0x80U) != 0x80U)
        {
            continue;
        }

        white_width = (uint8)(edge_store[k + 1U] - edge_store[k]);
        if((white_width > 1U) && (white_width < 14U))
        {
            zebra_white_count++;
            watch.ZebraInLine = lineinfo->y; // 记录当前满足条件的行号

            if(zebra_white_count > 6U)
            {
                lineinfo->zebra_flag = 1;
                break; // 达到阈值立即退出，减少无效遍历
            }
        }
    }

    return 0;
}