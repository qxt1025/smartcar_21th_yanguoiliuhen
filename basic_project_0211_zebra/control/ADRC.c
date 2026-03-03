/*
 * ADRC.c
 *
 *  Created on: 2024年3月29日
 *      Author: hwj
 *  Revised for Keil C51 compatibility
 */

#include "ADRC.h"
#include <math.h>

/* 单实例：摄像头转向 ADRC */
ADRC_STRUCT cam_adrc = {0};

/* -------------------- 单实例内部状态（如果后续要多实例，建议放到 ADRC_STRUCT 里） -------------------- */
/****************TD******************/
float x1 = 0.0f;    /* 跟踪输入 */
float x2 = 0.0f;    /* 跟踪输入的微分 */

/****************ESO******************/
float e  = 0.0f;    /* 误差 */
float z1 = 0.0f;    /* 跟踪反馈值 */
float z2 = 0.0f;    /* 跟踪反馈值的微分 */
float z3 = 0.0f;    /* 跟踪系统总扰动 */

/**************NLSEF******************/
float u  = 0.0f;    /* 输出值 */

static float y_last = 0.0f;

/* -------------------- 本地工具函数 -------------------- */
static float absf_local(float x)
{
    return (x >= 0.0f) ? x : -x;
}

static float pow_local(float x, float a)
{
    /* Keil C51 常见只有 pow(double,double)，这里统一封装 */
    return (float)pow((double)x, (double)a);
}

/* -------------------- 初始化 -------------------- */
void ADRC_init(void)
{
    /* TD para */
    cam_adrc.r_ratio = 10.0f;       /* 300 */
    cam_adrc.h_ratio = 0.001f;      /* 0.01 */

    /* ESO para */
    cam_adrc.b_ratio      = 0.01f;  /* 系统系数 */
    cam_adrc.delta_ratio  = 0.01f;  /* fal(e,alpha,delta) 线性区间宽度 */

    cam_adrc.belta01_ratio = 0.01f; /* 扩张状态观测器反馈增益1 */
    cam_adrc.belta02_ratio = 1.0f;  /* 扩张状态观测器反馈增益2 */
    cam_adrc.belta03_ratio = 1.0f;  /* 扩张状态观测器反馈增益3 */
    cam_adrc.w0_ratio      = 0.01f;

    /* NLSEF para */
    cam_adrc.a1_ratio = 0.1f;       /* 0.8 */
    cam_adrc.a2_ratio = 0.1f;       /* 1.1 */
    cam_adrc.kp_ratio = 0.1f;       /* 2 */
    cam_adrc.kd_ratio = 0.001f;     /* 0.01 */

    /* 参数指针 */
    cam_adrc.ADRC_para = &setpara.com_turn_ADRC;

    /* 状态清零（避免重复初始化后残留状态） */
    x1 = 0.0f;
    x2 = 0.0f;
    e  = 0.0f;
    z1 = 0.0f;
    z2 = 0.0f;
    z3 = 0.0f;
    u  = 0.0f;
    y_last = 0.0f;
}

/* -------------------- 基础函数 -------------------- */
float sign(float x)
{
    if (x > 0.0f)
        return 1.0f;
    else if (x < 0.0f)
        return -1.0f;
    else
        return 0.0f;
}

float fsg(float x, float d)
{
    /* 常见实现返回 0 / 1 */
    return 0.5f * (sign(x + d) - sign(x - d));
}

float fhan(float x1_in, float x2_in, float r, float h)
{
    float d;
    float a0;
    float y;
    float a;
    float a1;
    float a2;
    float ay;

    if (r <= 0.0f || h <= 0.0f)
    {
        return 0.0f;
    }

    d  = r * h * h;
    a0 = h * x2_in;
    y  = x1_in + a0;
    ay = absf_local(y);

    /* 注意：sqrt 用 double 版本更兼容 */
    a1 = (float)sqrt((double)(d * (d + 8.0f * ay)));
    a2 = a0 + sign(y) * (a1 - d) / 2.0f;
    a  = (a0 + y) * fsg(y, d) + a2 * (1.0f - fsg(y, d));

    if (d == 0.0f)
    {
        return 0.0f;
    }

    return -r * (a / d) * fsg(y, d) - r * sign(a) * (1.0f - fsg(a, d));
}

float fal(float e_in, float alpha, float delta)
{
    float result;
    float ae;

    result = 0.0f;
    ae = absf_local(e_in);

    if (delta <= 0.0f)
    {
        return 0.0f;
    }

    if (ae <= delta)
    {
        /* e / delta^(1-alpha) */
        result = e_in / pow_local(delta, (1.0f - alpha));
    }
    else
    {
        /* |e|^alpha * sign(e) */
        result = pow_local(ae, alpha) * sign(e_in);
    }

    return result;
}

/* -------------------- TD -------------------- */
void ADRC_TD(ADRC_STRUCT *adrc, float target, float *err, float *d_err)
{
    float h;
    float r;

    if (adrc == 0 || adrc->ADRC_para == 0 || err == 0 || d_err == 0)
    {
        return;
    }

    h = (adrc->ADRC_para->h) * (adrc->h_ratio);
    r = (adrc->ADRC_para->r) * (adrc->r_ratio);

    x1 = x1 + h * x2;
    x2 = x2 + h * fhan(x1 - 1000.0f * target, x2, r, h);

    *err   = x1 / 1000.0f;
    *d_err = x2 / 1000.0f;
}

/* -------------------- ADRC 主计算 -------------------- */
/* 参数：
 * y：反馈值
 * v：目标值
 */
float ADRC(ADRC_STRUCT *adrc, float y, float v)
{
    float u0;
    float e1;
    float e2;
    float h;
    float r;
    float delta;
    float kp;
    float kd;
    float a1;
    float a2;
    float b;
    float w;
    float belta01;
    float belta02;
    float belta03;

    if (adrc == 0 || adrc->ADRC_para == 0)
    {
        return 0.0f;
    }

    u0 = 0.0f;
    e1 = 0.0f;
    e2 = 0.0f;

    h = (adrc->ADRC_para->h) * (adrc->h_ratio);
    r = (adrc->ADRC_para->r) * (adrc->r_ratio);

    delta = (adrc->ADRC_para->delta) * (adrc->delta_ratio);
    kp    = (adrc->ADRC_para->kp)    * (adrc->kp_ratio);
    kd    = (adrc->ADRC_para->kd)    * (adrc->kd_ratio);
    a1    = (adrc->ADRC_para->a1)    * (adrc->a1_ratio);
    a2    = (adrc->ADRC_para->a2)    * (adrc->a2_ratio);
    b     = (adrc->ADRC_para->b)     * (adrc->b_ratio);

    /* 用乘法替代 powf，嵌入式更稳更快 */
    w = (adrc->ADRC_para->w0) * (adrc->w0_ratio);
    belta01 = 3.0f * w;         /* 3*w */
    belta02 = 3.0f * w * w;     /* 3*w^2 */
    belta03 = w * w * w;        /* w^3 */

    /****************************** TD **************************************/
    x1 = x1 + h * x2;
    x2 = x2 + h * fhan(x1 - v, x2, r, h);

    /****************************** ESO *************************************/
    e  = z1 - y;
    z1 = z1 + h * (z2 - belta01 * e);
    z2 = z2 + h * (z3 - belta02 * fal(e, 0.5f,  delta) + b * u);
    z3 = z3 + h * (      - belta03 * fal(e, 0.25f, delta));

    y_last = y; /* 保留，如需调试可用 */

    /****************************** NLSEF ***********************************/
    e1 = x1 - z1;
    e2 = x2 - z2;

    /* 其中通常 0 < alpha1 < 1 < alpha2（你当前参数由 a1/a2 决定） */
    u0 = kp * fal(e1, a1, delta) + kd * fal(e2, a2, delta);

    if (b != 0.0f)
    {
        u = u0 + z3 / b;
    }
    else
    {
        /* 防止除零 */
        u = u0;
    }

    /****************************** 限幅 ************************************/
    if (u >= 480.0f)  u = 480.0f;
    if (u <= -480.0f) u = -480.0f;

    return u;
}