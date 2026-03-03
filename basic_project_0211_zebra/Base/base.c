
#include "base.h"
void hardwareinit()//硬件初始化
{
//    uart_init(UART_0,115200,UART0_TX_P14_0,UART0_RX_P14_1);// 初始化串口0 波特率115200 发送引脚使用P14_0 接收引脚使用P14_1
//    uart_rx_interrupt(UART_0,1);
//    //uart_read_byte(UART_0);

//    //wireless_uart_init();
//    system_delay_ms(200);
    motor_init();
//    encoder_init();
//    gpio_init(P14_2,GPO,0,GPO_PUSH_PULL);
//    gpio_init(P14_3,GPI,0,GPI_PULL_DOWN);
//    gpio_init(P15_8,GPI,0,GPI_PULL_UP);
//    gpio_init(P15_7,GPI,0,GPI_PULL_UP);
//    gpio_init(P15_6,GPI,0,GPI_PULL_UP);
//    gpio_init(P10_6,GPI,0,GPI_PULL_UP);
//    gpio_init(P10_5,GPI,0,GPI_PULL_UP);
//    gpio_init(P10_2,GPI,0,GPI_PULL_UP);
//    //exti_init(ERU_CH1_REQ10_P14_3,EXTI_TRIGGER_BOTH);
//    //tft180_init();
//    ips200_init(IPS200_TYPE_SPI);
//    //gpio_init(P33_10,GPO,0,GPO_PUSH_PULL);
//    //beep_init();
//    act_perst_init();


//    gpio_init(P20_3,GPI,0,GPI_PULL_UP);
//    gpio_init(P20_0,GPI,0,GPI_PULL_UP);
//    gpio_init(P20_0,GPI,0,GPI_PULL_UP);

//    gpio_init(P20_8,GPO,1,GPO_PUSH_PULL);
//    gpio_init(P20_9,GPO,1,GPO_PUSH_PULL);
//    //mpu_init();
		imu660ra_init();
//    mt9v03x_init();             //摄像头初始化
//    dl1b_init();                //tof初始化
//    GUI_Init();
//    PID_init();
//    ADRC_init();
//   //system_delay_ms(500);
//     //mpu_gyro_init();
//     //gyroOffset_init();
//    //pit_ms_init(CCU60_CH0,2);//定时器中断1(PWM)
//    inductance_adc_init();
//    pit_ms_init(CCU60_CH0,1);

//    pit_us_init(CCU60_CH1,250);//定时器中断2(current)
////    pit_ms_init(CCU61_CH0,1);//定时器中断3(key)
////    pit_ms_init(CCU61_CH1,5);//定时器中断4(Device)
//    //adc_init(ADC0_CH11_A11,ADC_12BIT);
////    element_init();
////    element_init();
//    clear_all_flags();//所有标记位初始化
}



