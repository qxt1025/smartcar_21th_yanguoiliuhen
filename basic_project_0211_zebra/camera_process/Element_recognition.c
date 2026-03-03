//本文件留作接口文件，剩下的元素识别相关的需要自己移植
#include "Element_recognition.h"
#include "scan_line.h"

//小车状态变量
struct watch_o far watch;
Element_range Element=None;
struct Element_struct Element_rem;
/************************************************************************************************************************************/
/*祖传移植*/
/**/
//总的元素判断的代码
void Element_recognition()
{
    //Element=broken_circuit;
    switch(Element)
    {
        case None:
            enter_task(); //元素入口函数，在此识别并进入函数
            break;
        case Left_ring:
             left_ring_first_angle();       //寻找左环第一个角点
             left_ring_circular_arc();      //检测左环入环第一个角点后的圆弧,用于补线
             left_ring_begin_turn();        //左环开始转向
             left_ring_second_angle();      //检测左环第二个角点,用于补线
             left_ring_in_loop();           //检验小车是否完全入环
             left_ring_prepare_out();       //小车角度积分完成，准备出环
             left_ring_out_loop_turn();     //出左环左转
             left_ring_out_angle();         //检测出环时右角点位置
             left_ring_out_loop();          //右侧为直线时直行
             left_ring_straight_out_angle();//检测出左环进入直线后左侧角点，用于补线
             left_ring_complete_out();      //检测完全出环
            break;
        case Right_ring:
             right_ring_first_angle();       //检测右环第一个角点
             right_ring_circular_arc();      //检测右环入环第一个角点后的圆弧
             right_ring_begin_turn();        //右环开始转向函数
             right_ring_second_angle();      //检测右环第二个角点
             right_ring_in_loop();           //检验小车是否完全入右环
             right_ring_prepare_out();       //小车角度积分完成，准备出环
             right_ring_out_loop_turn();     //出右环左转
             right_ring_out_angle();         //检测出环时左角点位置
             right_ring_out_loop();          //左侧为直线时直行
             right_ring_straight_out_angle();//检测出右环进入直线后左侧角点
             right_ring_complete_out();      //检测完全出环
            break;
        /*case ingarage:
//             left_garage_first_angle();//检测左侧车库第一个角点
/*             left_garage_second_turn();
             right_garage_first_angle();
             right_garage_second_turn();
             garage_stop();*/
            /*if(setpara.start_mode==1||setpara.start_mode==3)
            {
                left_garage_first_angle();
                left_garage_second_angle();
                left_garage_begin_turn();
                left_garage_stop_turn();
                garage_stop2();
            }
            else if(setpara.start_mode==2||setpara.start_mode==4)
            {
                right_garage_first_angle();
                right_garage_second_angle();
                right_garage_begin_turn();
                right_garage_stop_turn();
                garage_stop2();
            }
            break;*/
        /*case broken_circuit:
            broken_circuit_enter();
            broken_circuit_slow();
            broken_circuit_complete_enter();
            broken_circuit_complete_out();
            break;
			*/
        /*case obstacle:
			//obstacle_identification();
			obs_first_turn();
			obs_first_straight();
			obs_second_turn();
			obs_sencond_straight();
			obs_free();
            obstacle_stop();
            break;*/
        case crossing:
            /*cross_running();
            cross_running2();
            cross_out();*/
			cross_detect();
            break;
        /*case Slope:
            slope_down();
            slope_out();
            break;*/
        /*case outgarage:
            out_garage_turn();
            complete_out_garage();
            break;*/
        case black_obstacle:
            //black_obstacle_enter();
            black_obstacle_out();
        case zebra:
            zebra_stop();
            zebra_out();

        default:break;
    }
}
//元素识别入口函数
void enter_task()
{
    switch( Element_rem.Element_data[Element_rem.Element_count])
    {
        /*case 1:
            left_ring_first_angle(); //检测左环第一个角点
            break;
        case 2:
            right_ring_first_angle();//检测右环第一个角点
            break;
        case 3:
            slope_enter();
            break;
        case 4:
            broken_circuit_prepare();
            break;
        case 5:

            break;
        case 6:
            garage_prepare();
            break;
        case 7:
            out_garage();
            break;
        case 8:
            cross_enter();
            break;*/
        case 0:            //元素标记为0时自动扫描元素
           // if(mycar.RUNTIME>=setpara.begin_time||mycar.car_running==0)
		{
			cross_detect();//务必放在第一个
			//slope_enter();
			black_obstacle_enter();
			//obstacle_identification();
            left_ring_first_angle(); //检测左环第一个角点
            right_ring_first_angle();//检测右环第一个角点

            /*if(setpara.cross_open_flag>=1){
            cross_enter();}*/

            //if(setpara.bla_obs_open_flag>=1){

             zebra_enter();
            }
            break;
        default:
            out_element();
            zebra_out();
            break;
    }
    //zebra_indentification(); //斑马线识别
}
//元素初始化
void element_init()//不用
{
   /* uint8 count=0;
    Element_rem.Element_count=0;
    Element_rem.Element_data[0]=7;    //出库
    uint8 Element[21];
    if(setpara.start_mode==1||setpara.start_mode==3)
    {
        for(int i=1;i<21;i++)
        {
            Element_rem.Element_data[i]= setpara.set_element[i];
        }
    }
    else if(setpara.start_mode==2||setpara.start_mode==4)
    {
        for(int i=1;i<21;i++)
        {
            if(setpara.set_element[i]!=6)
            {
                Element[i]=setpara.set_element[i];
                count++;
            }
            else if(setpara.set_element[i]==6)
            {
                count++;
                break;
            }
        }
        for(int i=1;i<count;i++)
        {
            if(Element[i]==1)
                Element_rem.Element_data[count-i]=2;
            else if(Element[i]==2)
                Element_rem.Element_data[count-i]=1;
            else
            Element_rem.Element_data[count-i]=Element[i];
        }
        Element_rem.Element_data[count]=6;
    }
    Element_rem.loop_count=0;
    Element_rem.loop_data[0]=setpara.loop_data[0];
    Element_rem.loop_data[1]=setpara.loop_data[1];
    Element_rem.loop_data[2]=setpara.loop_data[2];
    Element_rem.loop_data[3]=setpara.loop_data[3];*/
}
//进入元素
void enter_element(Element_range element)
{
    Element=element;
}
//离开元素
void out_element()
{
//    if(Element==Left_ring||Element==Right_ring)
//    Element_rem.loop_count++;
    clear_all_flags();
//    Element_rem.Element_count++;
}

void clear_all_flags()
{
    //清除元素标记
    Element=None;
    //清除积分标志位
    clear_angle_integeral();
    clear_distant_integeral();
    //清楚圆环标志
    watch.InLoop = 0;
    watch.InLoopAngleL = 120;
    watch.InLoopAngleR = 120;
    watch.InLoopCirc = 120;
    watch.InLoopAngle2 = 120;
    watch.OutLoop=0;

/*    watch.OutLoopRight = 0;
    watch.OutLoopRightY =120;
    watch.OutLoopLeft = 187;
    watch.OutLoopLeftY = 120;*/
    watch.OutLoopAngle2 = 120;
    watch.OutLoopAngle1 = 120;
/*
    //清除十字路口的变量
    watch.cross_flag=0;
    watch.cross_line=120;
    watch.Garge_line=120;
    watch.cross_RD_angle=120;
    watch.cross_LD_angle=120;

    watch.cross_AngleL=120;
    watch.cross_AngleR=120;
    watch.cross_AngleL_x=0;
    watch.cross_AngleR_x=187;*/
    //清除断路标志位
    watch.broken_circuit_flag=0;
    //清除斑马线标志位


    // flag.stop = 0;
    // indata.YawAngle = 0;
    //gpio_set(C10,0);
    // if(caminfo.apriltag_count<3)
    //     watch.AprilLine = 120 ;
    //清除斜坡标志位
    watch.slope_flag=0;

    //恢复寻中线模式
    watch.Line_patrol_mode=0;
    /*mycar.speed_ctrl=1;                         //恢复正常速度
    mycar.pid_ctrl=1;                           //恢复正常PID
    change_pid_para(&CAM_Turn,&CAM_FUZZY_PID);//恢复正常转向PID
    change_pid_para(&Speed_middle,&setpara.com_speed_PID);
    change_pid_para(&Speed_left,&setpara.com_speed_PID);
    change_pid_para(&Speed_right,&setpara.com_speed_PID);*/
    //清除车库标记位
    watch.zebra_flag = 0;
    watch.Zebra_Angle=120;
    watch.Zebra_Angle2=120;
    watch.stop_count=0;
    //清除出库标记位
    watch.out_garage_flag=0;
    //清除停车标记位
    watch.garage_stop=0;
    //清除停车标记位
    //mycar.car_stop=0;
    watch.cross_flag=0;

    watch.obstacle_flag=0;
    watch.black_obstacle_flag=0;
    watch.black_obstacle_line=120;
    watch.left_obstacle_x=0;
    watch.right_obstacle_x=187;
    //mycar.tracking_mode=0;  //恢复摄像头循迹

    watch.angle_near_line=30;
    watch.angle_far_line=80;
    watch.garage_flag=0;
    //mycar.target_speed=setpara.speed_min;
	watch.obs_pos=120;
	watch.obs_flag=0;

}
/******************************************************************************************************************************************/

void line_lost()
{
    /*
    int y = 0;
    watch.left_lost = 0;
    watch.right_lost = 0;
    watch.left_near_lost = forward_far;
    watch.right_near_lost = forward_far;
    watch.left_far_lost = 0;
    watch.right_far_lost = 0;
    watch.cross = 0;

    for(y = forward_near; y < forward_far; y++)
    {
        if(lineinfo[y].left_lost == 1)
        {
            watch.left_lost++;
            if(y < watch.left_near_lost)
            {
                watch.left_near_lost = y; // 更新为更小的y值
            }
            if(y > watch.left_far_lost && y < watch.watch_lost)
            {
                watch.left_far_lost = y;
            }
        }
        if(lineinfo[y].right_lost == 1)
        {
            watch.right_lost++;
            if(y < watch.right_near_lost)
            {
                watch.right_near_lost = y; // 更新为更小的y值
            }
            if(y > watch.right_far_lost && y < watch.watch_lost)
            {
                watch.right_far_lost = y;
            }
        }
        if(lineinfo[y].left_lost == 1 && lineinfo[y].right_lost == 1)
        {
            watch.cross++;
        }
    }*/
}

void find_down_point(int start,int end)
{
	int i ,t;
	watch.left_down_find=0;
	watch.right_down_find=0;
	
	if(start>end)
    {
        t=start;
        start=end;
        end=t;
    }
	if(start>=110)start=110;
	else if(start<=5) start = 5;
	
	if(end>=110)end=110;
	else if(end<=5) end = 5;
	
	for(i=start;i<=end;i++)
    {
        if(watch.left_down_find==0&&//只找第一个符合条件的点
           abs(lineinfo[i].left-lineinfo[i-1].left)<=3&&//角点的阈值可以更改
           abs(lineinfo[i-1].left-lineinfo[i-2].left)<=3&&
           abs(lineinfo[i-2].left-lineinfo[i-3].left)<=3&&
           lineinfo[i].left-lineinfo[i+2].left>=7&&
           lineinfo[i].left-lineinfo[i+3].left>=8&&
           lineinfo[i].left-lineinfo[i+4].left>=9)
        {
            watch.left_down_find=i;//获取行数即可
        }
		
        if(watch.right_down_find==0&&//只找第一个符合条件的点
           abs(lineinfo[i].right-lineinfo[i-1].right)<=3&&//角点的阈值可以更改
           abs(lineinfo[i-1].right-lineinfo[i-2].right)<=3&&
           abs(lineinfo[i-2].right-lineinfo[i-3].right)<=3&&
           lineinfo[i].right-lineinfo[i+2].right<=-7&&
           lineinfo[i].right-lineinfo[i+3].right<=-8&&
           lineinfo[i].right-lineinfo[i+4].right<=-9)
        {
            watch.right_down_find=i;//获取行数即可
        }
        if(watch.left_down_find!=0&&watch.right_down_find!=0&&lineinfo[watch.left_down_find].left<lineinfo[watch.right_down_find].right)//两个找到就退出
        {
            break;
        }
    }
	if(abs(watch.left_down_find-watch.right_down_find)>=50)//纵向撕裂过大，视为误判
    {
        watch.right_down_find=0;
        watch.left_down_find=0;
    }
}

void find_up_point(int start,int end)
{
	int i,t;
	watch.left_up_find=0;
	watch.right_up_find=0;
	
    if(start>end)
    {
        t=start;
        start=end;
        end=t;
    }
	if(start>=110)start=110;
	else if(start<=5) start = 5;
	
	if(end>=110)end=110;
	else if(end<=5) end = 5;
	
	for(i=start;i<=end;i++)
    {
        if(watch.left_up_find==0&&//只找第一个符合+条件的点
           abs(lineinfo[i].left-lineinfo[i+1].left)<=3&&//角点的阈值可以更改
           abs(lineinfo[i+1].left-lineinfo[i+2].left)<=3&&
           abs(lineinfo[i+2].left-lineinfo[i+3].left)<=3&&
           lineinfo[i].left-lineinfo[i-2].left>=7&&
           lineinfo[i].left-lineinfo[i-3].left>=8&&
           lineinfo[i].left-lineinfo[i-4].left>=9)
        {
            watch.left_up_find=i;//获取行数即可
        }
        if(watch.right_up_find==0&&//只找第一个符合条件的点
           abs(lineinfo[i].right-lineinfo[i+1].right)<=3&&//角点的阈值可以更改
           abs(lineinfo[i+1].right-lineinfo[i+2].right)<=3&&
           abs(lineinfo[i+2].right-lineinfo[i+3].right)<=3&
           lineinfo[i].right-lineinfo[i-2].right<=-7&&
           lineinfo[i].right-lineinfo[i-3].right<=-8&&
           lineinfo[i].right-lineinfo[i-4].right<=-9  )
        {
            watch.right_up_find=i;//获取行数即可
        }
        if(watch.left_up_find!=0&&watch.right_up_find!=0&&lineinfo[watch.left_up_find+1].left<lineinfo[watch.right_up_find+1].right&&lineinfo[watch.left_up_find].left<lineinfo[watch.right_up_find].right)//下面两个找到就出去
        {
            break;
        }
    }
	if(abs(watch.left_up_find-watch.right_up_find)>=50)//纵向撕裂过大，视为误判
    {
        watch.right_up_find=0;
        watch.left_up_find=0;
    }
}

int max_num(int x,int y)
{
	if(x>=y)
		return x;
	else
		return y;
		
}

int min_num(int x,int y)
{
	if(x<=y)
		return x;
	else
		return y;
		
}

void cross_detect()
{
	
    int down_search_start=0;//下角点搜索开始行
    watch.cross_flag=0;
	watch.left_up_find=0;
	watch.right_up_find=0;
	if(watch.cross>=15)//十字必定有双边丢线，在有双边丢线的情况下再开始找角点
	{
		find_up_point( min_num(watch.left_far_lost,watch.right_far_lost),watch.watch_lost-2 );
		if(watch.left_up_find==0 && watch.right_up_find==0)//只要没有同时找到两个上点，直接结束
		{
			out_element();
			return;
		}
	}
	if(watch.left_up_find!=0 && watch.right_up_find!=0)//找到两个上点，就认为找到十字了
	{
		watch.cross_flag=1;//确定对应标志位，便于各元素互斥掉
		enter_element(crossing);
		down_search_start=watch.left_up_find<watch.right_up_find?watch.left_up_find:watch.right_up_find;//用两个上拐点坐标靠下者作为下点的搜索上限
		find_down_point(5,max_num(watch.left_near_lost,watch.right_near_lost));//在上拐点下2行作为下角点的截止行
		if(watch.left_up_find<=watch.left_down_find)
		{
			watch.left_down_find=0;//下点不可能比上点还靠上
		}
		if(watch.right_up_find<=watch.right_down_find)
		{
			watch.right_down_find=0;//下点不可能比上点还靠上
		}
		if(watch.left_down_find!=0&&watch.right_down_find!=0)
		{//四个点都在，无脑连线，这种情况显然很少
			Left_Add_Line (lineinfo[watch.left_up_find].left,watch.left_up_find ,lineinfo[watch.left_down_find].left,watch.left_down_find);
			Right_Add_Line(lineinfo[watch.right_up_find].right,watch.right_up_find ,lineinfo[watch.right_down_find].right,watch.right_down_find);
		}
		else if(watch.left_down_find==0&&watch.right_down_find!=0)//11//这里使用的是斜率补线
		{//三个点                                     //01
			Lengthen_Left_Boundry(watch.left_up_find+1,0);
			Right_Add_Line(lineinfo[watch.right_up_find].right,watch.right_up_find ,lineinfo[watch.right_down_find].right,watch.right_down_find);
		}
		else if(watch.left_down_find!=0&&watch.right_down_find==0)//11
		{//三个点                                      //10
			Left_Add_Line (lineinfo[watch.left_up_find].left,watch.left_up_find ,lineinfo[watch.left_down_find].left,watch.left_down_find);
			Lengthen_Right_Boundry(watch.right_up_find+1,0);
		}
		else if(watch.left_down_find==0&&watch.right_down_find==0)//11
		{//就俩上点                                    //00
			Lengthen_Left_Boundry(watch.left_up_find+1,0);
			Lengthen_Right_Boundry(watch.right_up_find+1,0);
		}
	}
	else
	{
		watch.cross_flag=0;
		out_element();
	}
    
}


void Left_Add_Line(int x1,int y1,int x2,int y2)//左补线,补的是边界
{
    int i,max,a1,a2;
    int hx;
    if(x1>=187)//起始点位置校正，排除数组越界的可能
       x1=187;
    else if(x1<=0)
        x1=0;
     if(y1>=119)
        y1=119;
     else if(y1<=0)
        y1=0;
     if(x2>=187)
        x2=187;
     else if(x2<=0)
             x2=0;
     if(y2>=119)
        y2=119;
     else if(y2<=0)
             y2=0;
    a1=y1;
    a2=y2;
 
//这里有bug，下方循环++循环，只进行y的互换，但是没有进行x的互换
//建议进行判断，根据a1和a2的大小关系，决定++或者--访问
//这里修改各位自行操作
    if(a1<=a2)//坐标互换，这里建议修改，x坐标，y坐标一起交换，单纯换y坐标可能会导致bug
    {
        for(i=a1;i<=a2;i++)//根据斜率补线即可
		{
			hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
			if(hx>=187)
				hx=187;
			else if(hx<=0)
				hx=0;
			lineinfo[i].left=hx;
		}
    }
	else
	{
		for(i=a1;i>=a2;i--)//根据斜率补线即可
		{
			hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
			if(hx>=187)
				hx=187;
			else if(hx<=0)
				hx=0;
			lineinfo[i].left=hx;
		}
	}
    
}

void Right_Add_Line(int x1,int y1,int x2,int y2)//左补线,补的是边界
{
    int i,max,a1,a2;
    int hx;
    if(x1>=187)//起始点位置校正，排除数组越界的可能
       x1=187;
    else if(x1<=0)
        x1=0;
     if(y1>=119)
        y1=119;
     else if(y1<=0)
        y1=0;
     if(x2>=187)
        x2=187;
     else if(x2<=0)
             x2=0;
     if(y2>=119)
        y2=119;
     else if(y2<=0)
             y2=0;
    a1=y1;
    a2=y2;
 
//这里有bug，下方循环++循环，只进行y的互换，但是没有进行x的互换
//建议进行判断，根据a1和a2的大小关系，决定++或者--访问
//这里修改各位自行操作
    if(a1<=a2)//坐标互换，这里建议修改，x坐标，y坐标一起交换，单纯换y坐标可能会导致bug
    {
        for(i=a1;i<=a2;i++)//根据斜率补线即可
		{
			hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
			if(hx>=187)
				hx=187;
			else if(hx<=0)
				hx=0;
			lineinfo[i].right=hx;
		}
    }
	else
	{
		for(i=a1;i>=a2;i--)//根据斜率补线即可
		{
			hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
			if(hx>=187)
				hx=187;
			else if(hx<=0)
				hx=0;
			lineinfo[i].right=hx;
		}
	}
    
	
}


void Lengthen_Right_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=119)//起始点位置校正，排除数组越界的可能
        start=119;
    else if(start<=0)
        start=0;
    if(end>=119)
        end=119;
    else if(end<=0)
        end=0;
 
    if(start>=115 && start >= end)//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Right_Add_Line(lineinfo[start].right,start ,lineinfo[end].right,end);
    }
    else
    {
        k=(float)(-lineinfo[start].right+lineinfo[start+4].right)/5.0;//这里的k是1/斜率
        if(start>=end)
        {
            for(i=start;i>=end;i--)
            {
                lineinfo[i].right=(int)(i-start)*k+lineinfo[start].right;//(x=(y-y1)*k+x1),点斜式变形
                if(lineinfo[i].right>=187)
                {
                    lineinfo[i].right=187;
                }
                else if(lineinfo[i].right<=0)
                {
                    lineinfo[i].right=0;
                }
            }
        }
        else
        {
            for(i=start;i<=end;i++)
            {
                lineinfo[i].right=(int)(i-start)*k+lineinfo[start].right;//(x=(y-y1)*k+x1),点斜式变形
                if(lineinfo[i].right>=187)
                {
                    lineinfo[i].right=187;
                }
                else if(lineinfo[i].right<=0)
                {
                    lineinfo[i].right=0;
                }
            }
        }
        
    }
}

void Lengthen_Left_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=119)//起始点位置校正，排除数组越界的可能
        start=119;
    else if(start<=0)
        start=0;
    if(end>=119)
        end=119;
    else if(end<=0)
        end=0;
 
    if(start>=115 && start >= end)//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Left_Add_Line(lineinfo[start].left,start ,lineinfo[end].left,end);
    }
    else
    {
        k=(float)(-lineinfo[start].left+lineinfo[start+4].left)/5.0;//这里的k是1/斜率
        if(start>=end)
        {
            for(i=start;i>=end;i--)
            {
                lineinfo[i].left=(int)(i-start)*k+lineinfo[start].left;//(x=(y-y1)*k+x1),点斜式变形
                if(lineinfo[i].left>=187)
                {
                    lineinfo[i].left=187;
                }
                else if(lineinfo[i].left<=0)
                {
                    lineinfo[i].left=0;
                }
            }
        }
        else
        {
            for(i=start;i<=end;i++)
            {
                lineinfo[i].left=(int)(i-start)*k+lineinfo[start].left;//(x=(y-y1)*k+x1),点斜式变形
                if(lineinfo[i].left>=187)
                {
                    lineinfo[i].left=187;
                }
                else if(lineinfo[i].left<=0)
                {
                    lineinfo[i].left=0;
                }
            }
        }
        
    }
}

void straight_detect()
{
    // 定义直道检测阈值
    #define STRAIGHT_THRESHOLD 100

    int sum = 0;
    int count = 0;
    int left_lost = 0;
    int right_lost = 0;
    int i = 0;
    int y = 0;
    int mid_x = 0;
    float mean = 0;

    watch.variance = 0;
    if(!watch.large_curve_flag && watch.Island_State==0)
    {
        for(i = 60; i <= 115; i++)
        {
            if(lineinfo[i].left_lost && !lineinfo[i].whole_lost)
            {
                left_lost++;
            }
            if(lineinfo[i].right_lost && !lineinfo[i].whole_lost)
            {
                right_lost++;
            }
        }
        if(left_lost >= 8 || right_lost >= 8)
        {
            watch.straight_flag = 0;
        }
        else
        {
            if(watch.line_end >= 110) // 最远行
            {
                // 计算中线x坐标平均值
                for(y = forward_near; y <= forward_far; y++)
                {
                    if(lineinfo[y].left < lineinfo[y].right && lineinfo[y].right != 0)
                    {
                        mid_x = (lineinfo[y].left + lineinfo[y].right) / 2;
                        sum += mid_x;
                        count++;
                    }
                }

                if(count == 0)
                {
                    watch.straight_flag = 0; // 没有有效中线点
                }
                else
                {
                    mean = (float)sum / count;

                    // 计算方差
                    for(y = forward_near; y <= forward_far; y++)
                    {
                        if(lineinfo[y].left < lineinfo[y].right && lineinfo[y].right != 0)
                        {
                            mid_x = (lineinfo[y].left + lineinfo[y].right) / 2;
                            watch.variance += pow(mid_x - mean, 2);
                        }
                    }
                    watch.variance /= count;
                    if(watch.variance < STRAIGHT_THRESHOLD && watch.variance > 0 && abs(watch.err_far) <= 20)
                    {
                        watch.straight_flag = 1;
                        //enter_element(straight);
                    }
                    else
                    {
                        watch.straight_flag = 0;
                    }
                }
            }
            else
            {
                watch.straight_flag = 0;
            }
        }
    }
    else
    {
        watch.straight_flag = 0;
    }
}

void out_road_detect()
{
    int i = 0;
    if(Element==obstacle)return;
    //uint8_t black_line = 0;
    watch.stop_flag = 0;
    for(i = 0; i < 120; i++)
    {
        if(lineinfo[i].right==0)
            break;
    }
    if(i <= 3)
    {
        watch.stop_flag = 1;
        //enter_element(outroad);
    }
}

void curve_detect()
{
    #define curve_threshold 13
    int left_cnt = 0;
    int right_cnt = 0;
    int y = 0;
    if(!watch.straight_flag && !watch.cross)
    {
        for(y = 0; y < 40; y++)
        {
            if(lineinfo[y].left_lost && lineinfo[y].whole_lost == 0)
            {
                left_cnt++;
            }
        }
        for(y = 0; y < 40; y++)
        {
            if(lineinfo[y].right_lost && lineinfo[y].whole_lost == 0)
            {
                right_cnt++;
            }
        }
        if(left_cnt >= curve_threshold || right_cnt >= curve_threshold)
        {
            watch.large_curve_flag = 1;
            //enter_element(curve);
        }
        else
        {
            watch.large_curve_flag = 0;
        }
    }
    else
    {
        watch.large_curve_flag = 0;
    }
}

