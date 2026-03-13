#include "base.h"

void hardwareinit(void)
{
//    uart_init(UART_0,115200,UART0_TX_P14_0,UART0_RX_P14_1);
//    uart_rx_interrupt(UART_0,1);
//    uart_read_byte(UART_0);

//    wireless_uart_init();
//    system_delay_ms(200);
    gpio_init(LED1, GPO, 0, GPO_PUSH_PULL);
    gpio_init(GUI_SWITCH_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(IMG_SWITCH_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);

    ips200_init();
    motor_init();
    encoder_init();
//    gpio_init(P14_2,GPO,0,GPO_PUSH_PULL);
//    gpio_init(P14_3,GPI,0,GPI_PULL_DOWN);
//    gpio_init(P15_8,GPI,0,GPI_PULL_UP);
//    gpio_init(P15_7,GPI,0,GPI_PULL_UP);
//    gpio_init(P15_6,GPI,0,GPI_PULL_UP);
//    gpio_init(P10_6,GPI,0,GPI_PULL_UP);
//    gpio_init(P10_5,GPI,0,GPI_PULL_UP);
//    gpio_init(P10_2,GPI,0,GPI_PULL_UP);
//    exti_init(ERU_CH1_REQ10_P14_3,EXTI_TRIGGER_BOTH);
//    tft180_init();
//    ips200_init(IPS200_TYPE_SPI);
//    gpio_init(P33_10,GPO,0,GPO_PUSH_PULL);
    beep_init();
    beep_power_on_self_test();

//    gpio_init(P20_3,GPI,0,GPI_PULL_UP);
//    gpio_init(P20_0,GPI,0,GPI_PULL_UP);
//    gpio_init(P20_0,GPI,0,GPI_PULL_UP);
//    gpio_init(P20_8,GPO,1,GPO_PUSH_PULL);
//    gpio_init(P20_9,GPO,1,GPO_PUSH_PULL);
//    mpu_init();
    imu660ra_init();
    act_perst_init();
    ips200_show_string(0, 0, "mt9v03x init.");

    while(1)
    {
        system_delay_ms(100);
        if(mt9v03x_init())
        {
            ips200_show_string(0, 16, "mt9v03x reinit.");
            gpio_toggle_level(LED1);
        }
        else
        {
            break;
        }
    }

    ips200_show_string(0, 16, "init success.");

    /*ips200_show_string(0, 140, "FPS:");
    ips200_show_string(0, 160, "original_err:");
    ips200_show_string(0, 180, "zebra_flag:");
    ips200_show_string(0, 200, "jump_cnt:");
    ips200_show_string(0, 220, "cross:");
    ips200_show_string(0, 240, "obs_flag:");
    ips200_show_string(0, 280, "InLoop:");
    ips200_show_string(100, 140, "InLoopAngleL:");
    ips200_show_string(100, 160, "InLoopAngleR:");
    */
    GUI_Init();
//    mt9v03x_init();
//    dl1b_init();
//    GUI_Init();
    PID_init();
//    ADRC_init();
//    system_delay_ms(500);
//    mpu_gyro_init();
//    gyroOffset_init();
//    pit_ms_init(CCU60_CH0,2);
//    inductance_adc_init();
//    pit_ms_init(CCU60_CH0,1);
//    pit_us_init(CCU60_CH1,250);
////    pit_ms_init(CCU61_CH0,1);
////    pit_ms_init(CCU61_CH1,5);
//    adc_init(ADC0_CH11_A11,ADC_12BIT);
////    element_init();
////    element_init();
    clear_all_flags();
}