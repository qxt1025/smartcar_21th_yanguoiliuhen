/*
 * pid.c
 *
 *  Created on: 2023年6月21日
 *      Author: Admin
 */
#include "pid.h"
PID_STRUCT  ADC_Turn={0},     //电磁转向PID
            gyro={0},        //角速度PID
            CAM_Turn={0},    //摄像头转向PID
            Speed_left={0},  //左轮速度PID
            Speed_right={0}, //右轮速度PID
            Speed_middle={0},//速度中值PID
            current_left={0},//左轮电流环PID
            current_right={0};//右轮电流环PID
void PID_init()
{
    ADC_Turn.kp_ratio=0.00001;
    ADC_Turn.ki_ratio=0.00001;
    ADC_Turn.kd_ratio=0.00001;
    ADC_Turn.integral_Limit=1000;
    ADC_Turn.total_limit=1000;
    ADC_Turn.d_limit=100;
//    ADC_Turn.PID_para=&setpara.adc_turn_PID;

    CAM_Turn.kp_ratio=0.1;
    CAM_Turn.ki_ratio=0.001;
    CAM_Turn.kd_ratio=1;
    CAM_Turn.integral_Limit=150;
//    CAM_Turn.total_limit=490+setpara.steer_adjust;
//    CAM_Turn.PID_para=&CAM_FUZZY_PID;
    CAM_Turn.d_limit=0;

    Speed_left.kp_ratio=1;//纯速度环1，电流环串速度环0.01
    Speed_left.ki_ratio=0.1;//增量式0.1,位置式0.01//纯速度环0.1，电流环串速度环0.0001
    Speed_left.kd_ratio=0.0;//纯速度环0，电流环串速度环0.01
    Speed_left.kf_ratio=1;
    Speed_left.integral_Limit=9000;//纯速度环9000
    Speed_left.total_limit=9900;//纯速度环9900
//    Speed_left.d_limit=50;
//    Speed_left.PID_para=&setpara.com_speed_PID;


    Speed_right.kp_ratio=1;
    Speed_right.ki_ratio=0.1;
    Speed_right.kd_ratio=0.0;
    Speed_left.kf_ratio=1;
    Speed_right.integral_Limit=9000;
    Speed_right.total_limit=9900;
//    Speed_right.d_limit=50;
//    Speed_right.PID_para=&setpara.com_speed_PID;

    current_left.kp_ratio=100;
    current_left.ki_ratio=1;
    current_left.kd_ratio=0.1;
    current_left.integral_Limit=9000;
    current_left.total_limit=9900;
//    current_left.PID_para=&setpara.current_PID;

    current_right.kp_ratio=100;
    current_right.ki_ratio=1;
    current_right.kd_ratio=0.1;
    current_right.integral_Limit=9000;
    current_right.total_limit=9900;
//    current_right.PID_para=&setpara.current_PID;

    gyro.kp_ratio=0.1;
    gyro.ki_ratio=0.001;
    gyro.kd_ratio=0.1;
    gyro.integral_Limit=3000;
    gyro.total_limit=6000;
//    gyro.PID_para=&setpara.gyro_PID;
    gyro.d_limit=600;
}
void PID_struct_init(PID_STRUCT *PID,float kp_ratio,float ki_ratio,float kd_ratio,float integral_Limit,PID_para_STRUCT*PID_para)
{
    PID->PID_para=PID_para;
    PID->kp_ratio=kp_ratio;
    PID->ki_ratio=ki_ratio;
    PID->kd_ratio=kd_ratio;
    PID->integral_Limit=integral_Limit;
}
//改变PID参数组
void change_pid_para(PID_STRUCT *PID,PID_para_STRUCT*PID_para)
{
    PID->PID_para=PID_para;
}
//清除积分项
void clear_integral(PID_STRUCT *PID)
{
    PID->PID_out_I=0;
    PID->PID_output=0;
}


//位置式PID
float PID_Positional(PID_STRUCT* PID,float present_value,float target_value)
{
    PID->target_value=target_value;                      //获取目标变量值
    PID->present_value=present_value;                    //获取当前变量值
    PID->value_err[0]=PID->target_value- PID->present_value; //更新本次偏差

    PID->PID_out_P=(PID->PID_para->kp)*(PID->kp_ratio)*PID->value_err[0];//计算PID中的KP分量
    PID->PID_out_I+=(PID->PID_para->ki)*(PID->ki_ratio)*(PID->value_err[0]);//计算PID中的KI分量
    PID->PID_out_D=(PID->PID_para->kd)*(PID->kd_ratio)*(PID->value_err[0]-PID->value_err[1]);//计算PID中的KD分量
    if(PID->PID_out_I>PID->integral_Limit)PID->PID_out_I=PID->integral_Limit;//积分限幅
    else if(PID->PID_out_I<-PID->integral_Limit)PID->PID_out_I=-PID->integral_Limit;

    PID->PID_output=PID->PID_out_P+PID->PID_out_I+PID->PID_out_D;

    PID->value_err[1]=PID->value_err[0];                 //更新上次偏差

    if(PID->d_limit!=0) //变化量限幅
    {
        if(PID->PID_output-PID->PID_output_last>PID->d_limit)
            PID->PID_output=PID->PID_output_last+PID->d_limit;
        else if(PID->PID_output-PID->PID_output_last<-PID->d_limit)
            PID->PID_output=PID->PID_output_last-PID->d_limit;
        PID->PID_output_last=PID->PID_output;
    }
    if(PID->PID_output>PID->total_limit)PID->PID_output=PID->total_limit;
    else if(PID->PID_output<-PID->total_limit)PID->PID_output=-PID->total_limit;
    return PID->PID_output;
}


//增量式PID
float PID_Incremental(PID_STRUCT* PID,float present_value,float target_value)
{
    PID->target_value=target_value;                      //获取目标变量值
    PID->present_value=present_value;                    //获取当前变量值
    PID->value_err[0]=PID->target_value- PID->present_value; //更新本次偏差
    //if(fabs((PID->value_err[0]) < 2)&&(PID==&Speed_left||PID==&Speed_right)) PID->value_err[0] = 0;//pid死区
    PID->PID_out_P=(PID->PID_para->kp)*(PID->kp_ratio)*(PID->value_err[0]-PID->value_err[1]);//计算PID中的KP分量
    PID->PID_out_I=(PID->PID_para->ki)*(PID->ki_ratio)*(PID->value_err[0]);//计算PID中的KI分量
    PID->PID_out_D=(PID->PID_para->kd)*(PID->kd_ratio)*(PID->value_err[0]-2*PID->value_err[1]+PID->value_err[2]);//计算PID中的KD分量
//    if(PID->PID_out_I>PID->integral_Limit)PID->PID_out_I=PID->integral_Limit;//积分限幅
//    else if(PID->PID_out_I<-PID->integral_Limit)PID->PID_out_I=-PID->integral_Limit;

    PID->PID_output+=PID->PID_out_P+PID->PID_out_I+PID->PID_out_D;

    PID->value_err[2]=PID->value_err[1];
    PID->value_err[1]=PID->value_err[0];                 //更新上次偏差

    if(PID->d_limit!=0) //变化量限幅
    {
        if(PID->PID_output-PID->PID_output_last>PID->d_limit)
            PID->PID_output=PID->PID_output_last+PID->d_limit;
        else if(PID->PID_output-PID->PID_output_last<-PID->d_limit)
            PID->PID_output=PID->PID_output_last-PID->d_limit;
        PID->PID_output_last=PID->PID_output;
    }
    if(PID->PID_output>PID->total_limit)PID->PID_output=PID->total_limit;
    else if(PID->PID_output<-PID->total_limit)PID->PID_output=-PID->total_limit;
    return PID->PID_output;
}




//原application内容
struct setpara_STRUCT far setpara;         //可调量(即祖传代码的所有参量)
void init_setpara()//各个参数的初始化，不在参数表上的参数仍然可以正常初始化与使用，只是不可调
{
//************************************第一页********************************
    setpara.speed_max=105;
    setpara.speed_add=12;
    setpara.speed_min=52;
    setpara.speed_adjust_kp=55;
    setpara.obstacle_speed=50;
    setpara.broken_target_speed=50;
    setpara.differ_ratio=55;
    setpara.loop_target_speed=48;
    setpara.big_loop_speed=50;
    setpara.slope_speed=50;
    setpara.cross_speed=50;
    setpara.fuzzy_kp=90;
    setpara.fuzzy_kd=110;
    setpara.fuzzy_k=40;//50
    setpara.SteerKpchange=0;
    setpara.com_speed_PID.kp=350;       //速度环pi
    setpara.com_speed_PID.ki=35;
    setpara.com_speed_PID.kd=150;
    setpara.speed_down_PID.kp=230;//减速kp
    setpara.com_turn_PID.kp=43;
    setpara.com_turn_PID.kd=105;
    setpara.gyro_PID.kp=118;
    setpara.gyro_PID.ki=110;
    setpara.gyro_PID.kd=0;
    setpara.adc_turn_PID.kp=50;
    setpara.adc_turn_PID.kd=20;
    setpara.loop_turn_PID.kp=65;
    setpara.loop_turn_PID.kd=100;
    setpara.big_loop_PID.kp=13;
    setpara.big_loop_PID.kd=11;
    setpara.stop_PID.kp=360;
    setpara.stop_PID.ki=265;
    setpara.obstacle_turn1_PID.kp=3;
    setpara.obstacle_turn1_PID.kd=3;
    setpara.obstacle_turn2_PID.kp=3;
    setpara.obstacle_turn2_PID.kd=3;
    setpara.black_obstacle_turn_PID.kp=20;
    setpara.black_obstacle_turn_PID.kd=3;
    setpara.camwf=-31;
    setpara.camwl=92;
    setpara.camwr=97;
    setpara.far_line=70;
      //{&setpara.TextRow,  "TextRow", 1},
    setpara.threshold_max=120;
    setpara.threshold_min=80;
      //{&setpara.begin_pwm,"begin_pwm",10},
      //{&setpara.begin_time,"begin_time",10},
    setpara.adc_L_gain=100;
    setpara.adc_M_gain=100;
    setpara.adc_R_gain=100;
    setpara.adc_protect=50;
    setpara.fan_speed=100;
    setpara.fan_differ_ratio=0;
    setpara.current_PID.kp=15;
    setpara.current_PID.ki=46;
      //{&setpara.swj_adjust,"swj_adjust",1},
//************************************第二页************************
    setpara.start_mode=1;
    setpara.stop_time=15000;
    setpara.begin_time=400;
    setpara.set_element[1]=1;
    setpara.set_element[2]=8;
    setpara.set_element[3]=8;
    setpara.set_element[4]=4;
    setpara.set_element[5]=3;
    setpara.set_element[6]=5;
    setpara.set_element[7]=6;
    setpara.set_element[8]=0;
    setpara.set_element[9]=0;
    setpara.set_element[10]=0;
    setpara.set_element[11]=0;
    setpara.set_element[12]=0;
    setpara.set_element[13]=0;
    setpara.set_element[14]=0;
    setpara.set_element[15]=0;
    setpara.set_element[16]=0;
    setpara.set_element[17]=0;
    setpara.set_element[18]=0;
    setpara.set_element[19]=0;
    setpara.set_element[20]=0;
    setpara.loop_data[0]=0;
    setpara.loop_data[1]=0;
    setpara.loop_data[2]=0;
    setpara.loop_data[3]=0;
    setpara.obstacle_dir=0;//0左1右
    setpara.out_garage_distance=2200;
    setpara.out_garage_angle=30;
    setpara.out_garage_err=8;
    setpara.garage_dis=4000;
    setpara.garage_angle=25;
    setpara.garage_err=25;
    setpara.garage_stop_distance=2000;
    setpara.distanceThres_before_broken_circuit=0;
    setpara.distanceThres_in_broken_circuit=9100;
    setpara.distanceThres_out_broken_circuit=5000;
    setpara.obstacle_dis=24000;
    setpara.obstacle_dis2=2500;
    setpara.obstacle_dis3=5700;
    setpara.obstacle_dis4=0;
    setpara.obstacle_angle1=42;
    setpara.obstacle_angle2=30;
    setpara.loop_out_distance=6000;
    setpara.big_loop_out_distance=8200;
    setpara.garage_begin_distance=180000;
    setpara.obstacle_detect_distance=300;
    setpara.garage_turn_PID.kp=15;
    setpara.garage_turn_PID.kd=10;
    setpara.broken_circuit_slow_distane=200;

    setpara.track_diff_cutoff=100;
    setpara.steer_buchang=0;
    setpara.steer_adjust=0;
    setpara.zebra_distance=0;
    setpara.cross_open_flag=1;
    setpara.bla_obs_open_flag=1;
    setpara.track_open_flag=1;
    setpara.bla_obs_begin_time=300;
    setpara.slope_begin_time=300;
    setpara.zebra_begin_time=2000;
    setpara.stop_over_count=55;
    setpara.zebra_speed=-10;
    setpara.zebra_line_count=4;
    setpara.USART_flag=0;

    //mycar.steer_pwm=4000;
    //setpara.loop_angle_out=60;
    //setpara.big_loop_out=60;
 /*   setpara.speed_max=60;
    setpara.speed_min=54;
    setpara.speed_adjust_kp = 37;
    setpara.loop
    setpara.com_speed_PID.kp=170;
    setpara.com_speed_PID.ki=45;
    setpara.com_speed_PID.kd=0;
    setpara.com_turn_PID.kp=19;
    setpara.com_turn_PID.ki=0;
    setpara.com_turn_PID.kd=13;
    setpara.gyro_PID.kp=119;
    setpara.gyro_PID.ki=0;
    setpara.gyro_PID.kd=98;
    setpara.adc_turn_PID.kp=60;
    setpara.adc_turn_PID.ki=0;
    setpara.adc_turn_PID.kd=30;
    setpara.garage_turn_PID.kp=400;
    setpara.garage_turn_PID.ki=0;
    setpara.garage_turn_PID.kd=100;
    setpara.stop_PID.kp=500;
    setpara.stop_PID.ki=150;
    setpara.stop_PID.kd=0;
    setpara.obstacle_turn1_PID.kp=25;
    setpara.obstacle_turn1_PID.kd=5;
    setpara.obstacle_turn2_PID.kp=28;
    setpara.obstacle_turn2_PID.kd=6;
    setpara.obstacle_turn3_PID.kp=0;
    setpara.obstacle_turn3_PID.kd=0;
    setpara.start_mode=1;
    setpara.out_garage_distance=1000;
    setpara.out_garage_angle=30;
    setpara.out_garage_err=8;
    setpara.obstacle_dis=16000;
    setpara.obstacle_dis2=5400;
    setpara.obstacle_dis3=4500;
    setpara.distanceThres_before_broken_circuit=27000;
    setpara.distanceThres_in_broken_circuit=11600;
    setpara.distanceThres_out_broken_circuit=5000;
    setpara.obstacle_dir=0;
    setpara.garage_dis=8700;
    setpara.garage_angle=70;
    setpara.garage_err=15;
    setpara.obstacle_angle1=20;
    setpara.obstacle_angle2=33;
    setpara.com_target_speed=40;
    setpara.loop_target_speed=50;
    setpara.loop_speed_PID=setpara.com_speed_PID;
    setpara.loop_turn_PID.kp=25;
    setpara.loop_turn_PID.kd=20;
    setpara.threshold_min=80;
    setpara.threshold_max=120;
    setpara.camwl=78;
    setpara.camwr=110;
    setpara.camwf=-31;

    setpara.speed_adjust_kp;
    setpara.stop_time=500;
    setpara.TextRow=1;
    setpara.adc_L_gain=100;
    setpara.adc_M_gain=100;
    setpara.adc_R_gain=100;
    setpara.out_garage_angle=60;
    setpara.out_garage_err=14;
    setpara.out_garage_distance=1000;
    setpara.broken_target_speed=40;*/
    setpara.pwm_adjust=0;

/************以下为祖传代码涉及到的参数，可以参考一下******************///觉得不需要下面的可以全部注释
    setpara.rolling_angle_mid = 0;
    setpara.LoopWl = 87;
    setpara.LoopWr = 102;
    setpara.inter_d = -35;
    setpara.inter_i = -54;
    setpara.inter_p = -102;
    setpara.outer_p = -150;
    setpara.Pos_kp = 100;
    setpara.Pos_ki = 40;
    setpara.CamAveSpeed = 60;
    setpara.delmax_dir = 40;
    setpara.Bal_kp = 1;
    setpara.mode        = 2     ; // 0前进  1左出库2右出库
    setpara.settime     = 1210    ;
    setpara.outer_p     = -156;
    setpara.jun_wl = 87;
    setpara.jun_wr = 102;
    setpara.ad_kp = 60;
    setpara.ad_ki = 0;
    setpara.ad_kd = 20;
/////////速度相关
    setpara.CamAveSpeed = 120   ;
    setpara.SpeedKp     = 300   ;
    setpara.SpeedKi     = 60    ;
    setpara.SpeedKd     = 150   ;
    setpara.diffK       = 40    ;
    setpara.SpeedUP     = 100   ;
    setpara.SpeedDOWN   = 85    ;
/////////舵机相关
    setpara.SteerMid    = 735;
    setpara.MaxTurn     = 150   ;
    setpara.CamKp       = 14    ;
    setpara.CamKd       = 2    ;
///////属性相关
/*
    setpara.camcfg.pitch        = 540   ;
    setpara.camcfg.forward_far  = 105   ;   //视野不能超过120
    setpara.camcfg.forward_near = 26     ;   //我觉得入环不行和这个参大小无关
    setpara.camcfg.wf           = -31    ;   //13cm
    setpara.camcfg.wl           = 78    ;
    setpara.camcfg.wr           = 110    ; // 右轮位置，不能调大，影响入环
    setpara.camcfg.rotate_x     = -4    ;
    setpara.camcfg.rotate_y     = 2     ;
    setpara.camcfg.shift        = 270   ;
///////视觉相关
    setpara.camcfg.manbri       = 0     ;
    setpara.camcfg.exptime      = 153   ;
    setpara.exp_ki              = 1     ;
    setpara.camcfg.autogain     = 30    ;
*/
/////////车库相关
    setpara.garage_outangle     = 250   ; // 出库角度，+左-右（可优化多加一个12标志，不用拧这么久
    setpara.garage_outtime      = 3     ;   // 出库锁角度时间，×0.01s
     setpara.ZebraCount          = 3    ;//必须给值
    setpara.stop_kp             = 100   ;
    setpara.stop_kd             = 25    ;
    // setpara.StopSpeed           = 70    ;
/////////圆环相关
    setpara.CamLoopKp    = 13    ;
    setpara.CamLoopKd    = 2    ;

    setpara.InLoopLine   = 80    ;
    setpara.WhenInLoop   = 83    ;
    //setpara.InLoopRow    = 67    ;
    setpara.LoopThres    = 8     ;
    setpara.OutLoopThres = 40    ;
    setpara.distanceThres_in_loop=6000;
    setpara.distanceThres_out_loop=15000;
    setpara.distanceThres_in_broken_circuit=9000;
    setpara.distanceThres_out_broken_circuit=5000;
//////////三岔路相关
    setpara.StrThres     = 10    ;
    setpara.TempRL       = 2     ; //三岔路临时变量，左1右2，后期由OpenMV发送
    setpara.JuncProcess  = 20     ;
    setpara.Jun_out_distance = 10    ;
    //setpara.WhenInLoop = 30;
    setpara.OutJunc2Lambda = -800;
    setpara.OutJunc2speed = 100;
    //setpara.InLoopLine=90;
    //setpara.JuncSpeed    = -2    ;
    setpara.DetectTime   = 30    ;
    setpara.number       = 1     ;
//////////二维码相关
    // setpara.AprilCount   = 5     ;
    // setpara.AprilStop    = 78    ;
    // setpara.AprilSpeed   = 25    ;
    // setpara.AprilTime    = 110   ;
    // setpara.BackTime     = 15    ;
    // setpara.LaserTime    = 80    ;
/////////测试相关
    setpara.TextLine     = 33    ;
    setpara.TextRow      = 2     ;
    // setpara.TextDistance = 0     ;
/////////坡道
    setpara.SlopeDown = 3;
   // fuzzy.s_output.rolling_angle = 0;
    setpara.loop_dLambda_dir = 55;
    setpara.jun_dLambda_dir = 20;
    setpara.jun_CamKp = 12;
    setpara.jun_CamKd = 2;
    //setpara.cross_base = 80;
//////其他参数
    setpara.rolling_angle_mid = 0;
    setpara.steer_far = 110;
    setpara.trans = 3;
    setpara.delmax_dir = 80;
/*
    setpara.LoopOutAngle = 300;
    setpara.fuzzy_s.s_ku = 700;
    setpara.fuzzy_s.s_kp = 60;
    setpara.fuzzy_s.s_ki = 30;
    setpara.fuzzy_s.s_kd = 10;
*/
    setpara.Loop_deldir = 67;
    setpara.Junc_deldir = 80;
    setpara.O_steerfar = 60;
    setpara.Slopedistance = 100;

//出库
    setpara.garage_outangle = 121;
    setpara.garage_outtime = 10;
}

