/*********************************************************************************************************************
* STC32G144K Opensourec Library 即（STC32G144K 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2025 SEEKFREE 逐飞科技
*
* 本文件是STC32G144K开源库的一部分
*
* STC32G144K 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK FOR C251
* 适用平台          STC32G144K
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者           备注
* 2025-11-20        大W            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "search_line.h"
#include "application.h"
#include "GUI.h"
#include "base.h"

// *************************** 例程硬件连接说明 ***************************
//
// *************************** 例程硬件连接说明 ***************************
//
// 接入2寸IPS模块
//      模块管脚            单片机管脚
//      双排排针 并口两寸屏 硬件引脚
//      RD                  查看 zf_device_ips200_parallel8.h 中 IPS200_RD_PIN 宏定义 默认 B13
//      WR                  查看 zf_device_ips200_parallel8.h 中 IPS200_WR_PIN 宏定义 默认 B15
//      RS                  查看 zf_device_ips200_parallel8.h 中 IPS200_RS_PIN 宏定义 默认 B7
//      RST                 查看 zf_device_ips200_parallel8.h 中 IPS200_RST_PIN宏定义 默认 D7
//      CS                  查看 zf_device_ips200_parallel8.h 中 IPS200_CS_PIN 宏定义 默认 D4
//      BL                  查看 zf_device_ips200_parallel8.h 中 IPS200_BL_PIN 宏定义 默认 D0
//      D0-D7               查看 zf_device_ips200_parallel8.h 中 IPS200_Dx_PIN 宏定义 默认 E0-E7
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源
//      单排排针 SPI 两寸屏 硬件引脚
//      SCL                 查看 zf_device_ips200.h 中 IPS200_SCL_PIN_SPI  宏定义 默认 B13
//      SDA                 查看 zf_device_ips200.h 中 IPS200_SDA_PIN_SPI  宏定义 默认 B15
//      RST                 查看 zf_device_ips200.h 中 IPS200_RST_PIN_SPI  宏定义 默认 B7
//      DC                  查看 zf_device_ips200.h 中 IPS200_DC_PIN_SPI   宏定义 默认 D7
//      CS                  查看 zf_device_ips200.h 中 IPS200_CS_PIN_SPI   宏定义 默认 D4
//      BL                  查看 zf_device_ips200.h 中 IPS200_BLk_PIN_SPI  宏定义 默认 D0
//      GND                 核心板电源地 GND                                      
//      3V3                 核心板 3V3 电源
//
//
// 接入总钻风灰度数字摄像头 对应主板摄像头接口 请注意线序
//      模块管脚            单片机管脚
//      TXD                 查看 zf_device_mt9v03x.h 中 MT9V03X_COF_UART_TX 宏定义 默认 D2  摄像头 UART-TX 引脚 要接在单片机 RX 上
//      RXD                 查看 zf_device_mt9v03x.h 中 MT9V03X_COF_UART_RX 宏定义 默认 C12 摄像头 UART-RX 引脚 要接在单片机 TX 上
//      D0                  查看 zf_device_mt9v03x.h 中 MT9V03X_D0_PIN      宏定义 默认 A9
//      D1                  查看 zf_device_mt9v03x.h 中 MT9V03X_D1_PIN      宏定义 默认 A10
//      D2                  查看 zf_device_mt9v03x.h 中 MT9V03X_D2_PIN      宏定义 默认 C8 
//      D3                  查看 zf_device_mt9v03x.h 中 MT9V03X_D3_PIN      宏定义 默认 C9 
//      D4                  查看 zf_device_mt9v03x.h 中 MT9V03X_D4_PIN      宏定义 默认 C11
//      D5                  查看 zf_device_mt9v03x.h 中 MT9V03X_D5_PIN      宏定义 默认 B6 
//      D6                  查看 zf_device_mt9v03x.h 中 MT9V03X_D6_PIN      宏定义 默认 B8 
//      D7                  查看 zf_device_mt9v03x.h 中 MT9V03X_D7_PIN      宏定义 默认 B9 
//      PCLK                查看 zf_device_mt9v03x.h 中 MT9V03X_PCLK_PIN    宏定义 默认 A6
//      VSYNC               查看 zf_device_mt9v03x.h 中 MT9V03X_VSY_PIN     宏定义 默认 A5 
//      HSYNC               查看 zf_device_mt9v03x.h 中 MT9V03X_HERF_PIN    宏定义 默认 A4 

// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 将核心板插在主板上 插到底
//
// 2.摄像头接在主板的摄像头接口 注意线序2寸IPS模块插入主板屏幕接口
//
// 3.主板上电 或者核心板链接完毕后上电 核心板按下复位按键
//
// 4.屏幕会显示初始化信息然后显示摄像头图像
//
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************
static volatile uint32 g_ms_ticks = 0;                     // 1ms系统节拍，由PIT中断累加
static volatile uint8 g_tim1_1ms_flag = 0;                 // TIM1每1ms置位一次，在主循环中消费
static uint8 temp_img_raw[MT9V03X_IMAGE_SIZE + 1];         // 后备缓冲原始内存，+1字节用于手动地址对齐
uint8  (*temp_img)[MT9V03X_W] = 0;                         // 对齐后的后备帧指针（供双缓冲与算法共享）
static void pit_handler(void);
static void tim1_ctrl_handler(void);
static uint32 get_ms_ticks(void);
static void camera_switch_buffer(void);
static void init_aligned_back_buffer(void);
static void loop_1ms_task(void);
uint8  (*g_frame_img)[MT9V03X_W] = 0;                       // 当前算法读取帧指针
static uint8  (*g_dma_img)[MT9V03X_W] = 0;                  // 当前DMA写入帧指针
static uint8  (*g_back_img)[MT9V03X_W] = 0;                 // 后备帧指针
// 关键计时与显示状态变量放在文件作用域，避免参与overlay
static uint32 now = 0;                                   // 当前时间戳(ms)
static uint32 elapsed = 0;                               // FPS统计窗口时长(ms)
static uint32 t0 = 0;                                    // 阶段起始时间(ms)
static uint32 t1 = 0;                                    // 阶段结束时间(ms)

/*
 * 功能: 交换摄像头双缓冲，并将DMA重定向到新的写缓冲
 * 参数: 无
 * 返回: 无
 * 说明: 确保算法读取帧与DMA写入帧分离，避免处理中被覆盖
 */
static void camera_switch_buffer(void)
{
    uint8 (*tmp)[MT9V03X_W];                               // 交换缓冲时使用的临时指针

    g_frame_img = g_dma_img;                               // 锁存上一帧图像供算法处理
    tmp = g_dma_img;                                       // 保存当前DMA缓冲指针
    g_dma_img = g_back_img;                                // 后备缓冲切换为DMA目标缓冲
    g_back_img = tmp;                                      // 原DMA缓冲回收为后备缓冲
    mt9v03x_dma_init((uint8 *)&g_dma_img[0][0], MT9V03X_IMAGE_SIZE, 0, 3, 3); // 立即启动下一帧DMA采集
}

/*
 * 功能: 计算并绑定偶地址后备缓冲区
 * 参数: 无
 * 返回: 无
 * 说明: 避免双缓冲轮换到奇地址时引发DMA采样相位抖动
 */
static void init_aligned_back_buffer(void)
{
    uint8 *back_base = &temp_img_raw[0];

    if(((uint32)back_base & 1U) != 0U)
    {
        back_base++;                                       // 强制后备缓冲首地址为偶地址
    }

    temp_img = (uint8 (*)[MT9V03X_W])back_base;           // 绑定为二维帧指针
}
/*
 * 功能: 原子读取毫秒计数
 * 参数: 无
 * 返回: 当前毫秒计数值
 * 说明: 读取32位共享计数时使用关中断保护，避免ISR打断
 */
static uint32 get_ms_ticks(void)
{
    uint32 ms;                                             // 保存节拍快照

    interrupt_global_disable();                            // 进入临界区
    ms = g_ms_ticks;                                       // 读取全局毫秒计数
    interrupt_global_enable();                             // 退出临界区

    return ms;                                             // 返回稳定的计数值
}

/*
 * 功能: 系统初始化并循环执行图像处理流程
 * 参数: 无
 * 返回: 无
 * 说明: 以摄像头帧完成标志为驱动，每帧执行预处理、元素识别和统计输出
 */
void main(void)
{
    static uint32 frame_count = 0;                         // 当前FPS统计窗口内的帧计数
    static uint32 last_ms = 0;                             // FPS统计窗口起始时间
    static uint16 fps = 0;                                 // 每秒帧率结果
    static uint32 proc_last_ms = 0;                        // 预处理耗时(ms)
    static uint32 elem_loop_ms = 0;                        // 元素识别与后处理耗时(ms)


    clock_init(SYSTEM_CLOCK_96M);                          // 时钟配置及系统初始化<务必保留>
    debug_init();                                          // 调试串口初始化
    hardwareinit();                                         // 总初始化
    pit_ms_init(PIT_CH, 1, pit_handler);                    // 配置1ms PIT时基中断
    pit_ms_init(TIM1_CTRL_CH, TIM1_CTRL_PERIOD_MS, tim1_ctrl_handler); // 初始化TIM1周期中断
    interrupt_set_priority(PIT_PRIORITY, 0);                // 设置PIT中断优先级
    interrupt_set_priority(TIM1_CTRL_PRIORITY, 1);          // 设置TIM1中断优先级（低于系统时基）

    init_aligned_back_buffer();                             // 先计算后备缓冲对齐地址
    g_dma_img = mt9v03x_image;                              // 首次DMA写入缓冲
    g_back_img = temp_img;                                  // 后备缓冲绑定到temp_img
    g_frame_img = mt9v03x_image;                            // 初始化算法读取缓冲

    GUI_UpdateSwitchState();
    mt9v03x_dma_init((uint8 *)&g_dma_img[0][0], MT9V03X_IMAGE_SIZE, 0, 3, 3); // 启动首帧DMA采集

    while(1)
    {
        // 此处编写需要循环执行的代码
        if(g_tim1_1ms_flag)
        {
            g_tim1_1ms_flag = 0;                            // 消费1ms标志，保证主循环仅执行一次周期任务
            loop_1ms_task();                                // 1ms周期任务入口（在主循环中执行）
        }

        GUI_UpdateSwitchState();

        if(mt9v03x_finish_flag)
        {
            mt9v03x_finish_flag = 0;                       // 消费一帧完成标志

            /******预处理过程耗时计算*******/
            t0 = get_ms_ticks();                           // 记录预处理开始时间
            camera_switch_buffer();                        // 完成DMA/处理缓冲切换
            get_reference_point(g_frame_img[0]);           // 找参考白点灰度值
            search_reference_col(g_frame_img[0]);          // 找最长白列
            search_line(g_frame_img[0]);                   // 对比度搜线
            post_process_lines();                          // lineinfo结构体赋值
            count_line_lost();                             // 丢线统计（watch相关参数赋值）
            t1 = get_ms_ticks();                           // 记录预处理结束时间
            proc_last_ms = t1 - t0;                        // 计算预处理耗时

            /******后处理（元素识别）部分耗时计算*******/
            t0 = get_ms_ticks();                           // 记录元素处理开始时间
            Element_recognition();                         // 元素识别
            linefix();                                     // 补线处理
            original_err_calculation();                    // 计算原始偏差
			dir_control();
            //ips200_displayimage03x(g_frame_img[0], MT9V03X_W, MT9V03X_H);
            GUI_Display();
            
            t1 = get_ms_ticks();                           // 记录元素处理结束时间
            elem_loop_ms = t1 - t0;                        // 计算元素处理耗时

            /******总帧数计算部分*******/
            frame_count++;                                  // 完成一帧算法处理
            now = get_ms_ticks();                           // 读取当前时间
            if((uint32)(now - last_ms) >= 1000U)
            {
                elapsed = now - last_ms;                    // FPS统计窗口时长(ms)
                if(elapsed > 0U)
                {
                    fps = (uint16)((frame_count * 1000U) / elapsed); // 计算FPS
                }
                else
                {
                    fps = 0;                                // 避免除0
                }
                frame_count = 0;                            // 重置窗口帧计数
                last_ms = now;                              // 更新窗口起点

                //ips200_show_int32(32, 140, (int32)fps, 4);
            }

            //ips200_show_int32(80, 160, (int32)mycar.original_err, 4);
            //ips200_show_int32(80, 180, (int32)watch.zebra_flag2, 4);
            //ips200_show_int32(80, 200, (int32)watch.jump_count, 4);
            //ips200_show_int32(80, 220, (int32)watch.cross, 4);
            //ips200_show_int32(80, 240, (int32)watch.black_obstacle_flag, 4);
            //ips200_show_int32(80, 260, (int32)watch.right_black, 4);
            //ips200_show_int32(80, 280, (int32)watch.InLoop, 4);
            //ips200_show_int32(160, 140, (int32)watch.InLoopAngleL, 4);
            //ips200_show_int32(160, 160, (int32)watch.InLoopAngleR, 4);
            //printf("o_err:%d\r\n",mycar.original_err);
            //printf("FPS:%u,PROC_MS:%lu,ELEM_MS:%lu\r\n",fps, proc_last_ms, elem_loop_ms); // 串口输出运行耗时
        }

        // 此处编写需要循环执行的代码
    }
}

// **************************** 代码区域 ****************************

/*
 * 功能: PIT中断回调，每1ms累加系统节拍
 * 参数: 无
 * 返回: 无
 * 说明: ISR中仅做计数，不做复杂处理
 */
static void pit_handler(void)
{
	//printf("hello");
    g_ms_ticks++;                                           // 累加全局毫秒计数
}

/*
 * 功能: TIM1中断回调，每1ms置位一次周期任务标志
 * 参数: 无
 * 返回: 无
 * 说明: 仅置位标志，避免在ISR中执行耗时逻辑
 */
static void tim1_ctrl_handler(void)
{
    g_tim1_1ms_flag = 1U;                                   // 置位1ms周期任务标志
    //set_pwm(2000,2000);
    //timer1_Call_Back();
}

/*
 * 功能: 1ms周期任务入口（由主循环在收到TIM1标志后调用）
 * 参数: 无
 * 返回: 无
 * 说明: 需要1ms节拍执行的控制逻辑可放到这里
 */
static void loop_1ms_task(void)
{
    // 预留1ms周期任务入口
    //timer1_Call_Back();
}
// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
//
// 问题1：屏幕不显示
//      如果使用主板测试，主板必须要用电池供电 检查屏幕供电引脚电压
//      检查屏幕是不是插错位置了 检查引脚对应关系
//      如果对应引脚都正确 检查一下是否有引脚波形不对 需要有示波器
//      无法完成波形测试则复制一个GPIO例程将屏幕所有IO初始化为GPIO翻转电平 看看是否受控
//
// 问题2：显示 reinit 字样
//      检查接线是否正常
//      主板供电是否使用电量充足的电池供电
//
// 问题2：显示图像杂乱 错位
//      检查摄像头信号线是否有松动






