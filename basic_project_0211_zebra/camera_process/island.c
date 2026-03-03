#include "island.h"

/*-------------------------------------------------------------------------------------------------------------------
  @brief     左赛道连续性检测
  @param     起始点，终止点;为适配祖传，start和end意义互换
  @return    连续返回0，不连续返回断线出行数
  Sample     Continuity_Change_Left(int start,int end);
  @note      连续性的阈值设置为5，可更改
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Left(int start,int end)//连续性阈值设置为5
{
    int i;
    int t;
    int continuity_change_flag=0;
    if(watch.left_lost>=108)//大部分都丢线，没必要判断了
       return 1;
    if(watch.watch_lost<=5)//搜所截止行很矮
       return 1;
    if(start>=120-5)//数组越界保护
        start=120-5;
    if(end<=5)
       end=5;
    if(start<end)//都是从下往上计算的，反了就互换一下
    {
       t=start;
       start=end;
       end=t;
    }
    
    for(i=end;i<=start;i++)
    {
       if(abs(lineinfo[i].left-lineinfo[i-1].left)>=25)//连续判断阈值是5,可更改
       {
            continuity_change_flag=i;
            break;
       }
    }
    return continuity_change_flag;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右赛道连续性检测
  @param     起始点，终止点
  @return    连续返回0，不连续返回断线出行数
  Sample     continuity_change_flag=Continuity_Change_Right(int start,int end)
  @note      连续性的阈值设置为5，可更改
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Right(int start,int end)
{
    int i;
    int t;
    int continuity_change_flag=0;
    if(watch.right_lost>=108)//大部分都丢线，没必要判断了
       return 1;
    if(start>=120-5)//数组越界保护
        start=120-5;
    if(end<=5)
       end=5;
    if(start<end)//都是从下往上计算的，反了就互换一下
    {
       t=start;
       start=end;
       end=t;
    }

    for(i=end;i<=start;i++)
    {
        if(abs(lineinfo[i].right-lineinfo[i-1].right)>=25)//连续性阈值是5，可更改
       {
            continuity_change_flag=i;
            break;
       }
    }
    return continuity_change_flag;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     单调性突变检测
  @param     起始点，终止行
  @return    点所在的行数，找不到返回0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      前5后5它最大（最小），那他就是角点
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Left(int start,int end)//单调性改变，返回值是单调性改变点所在的行数
{
    int i;
    int monotonicity_change_line=0;
    if(watch.left_lost>=108)//大部分都丢线，没有单调性判断的意义
       return monotonicity_change_line;
    if(start>=120-1-5)//数组越界保护，在判断第i个点时
       start=120-1-5; //要访问它前后5个点，数组两头的点要不能作为起点终点
    if(end<=5)
        end=5;
    if(start<=end)//递减计算，入口反了，直接返回0
      return monotonicity_change_line;
    for(i=end;i<=start;i++)//会读取前5后5数据，所以前面对输入范围有要求
    {
        if(lineinfo[i].left==lineinfo[i+5].left&&lineinfo[i].left==lineinfo[i-5].left&&
        lineinfo[i].left==lineinfo[i+4].left&&lineinfo[i].left==lineinfo[i-4].left&&
        lineinfo[i].left==lineinfo[i+3].left&&lineinfo[i].left==lineinfo[i-3].left&&
        lineinfo[i].left==lineinfo[i+2].left&&lineinfo[i].left==lineinfo[i-2].left&&
        lineinfo[i].left==lineinfo[i+1].left&&lineinfo[i].left==lineinfo[i-1].left)
        {//一堆数据一样，显然不能作为单调转折点
            continue;
        }
        else if(lineinfo[i].left>=lineinfo[i+5].left&&lineinfo[i].left>=lineinfo[i-5].left&&
        lineinfo[i].left>=lineinfo[i+4].left&&lineinfo[i].left>=lineinfo[i-4].left&&
        lineinfo[i].left>=lineinfo[i+3].left&&lineinfo[i].left>=lineinfo[i-3].left&&
        lineinfo[i].left>=lineinfo[i+2].left&&lineinfo[i].left>=lineinfo[i-2].left&&
        lineinfo[i].left>=lineinfo[i+1].left&&lineinfo[i].left>=lineinfo[i-1].left)
        {//就很暴力，这个数据是在前5，后5中最大的（可以取等），那就是单调突变点
            monotonicity_change_line=i;
            break;
        }
    }
    return monotonicity_change_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     单调性突变检测
  @param     起始点，终止行
  @return    点所在的行数，找不到返回0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      前5后5它最大（最小），那他就是角点
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Right(int start,int end)//单调性改变，返回值是单调性改变点所在的行数
{
    int i;
    int monotonicity_change_line=0;

    if(watch.right_lost>=0.9*120)//大部分都丢线，没有单调性判断的意义
        return monotonicity_change_line;
    if(start>=120-1-5)//数组越界保护
        start=120-1-5;
     if(end<=5)
        end=5;
    if(start<=end)
        return monotonicity_change_line;
    for(i=end;i<=start;i++)//会读取前5后5数据，所以前面对输入范围有要求
    {
        if(lineinfo[i].right==lineinfo[i+5].right&&lineinfo[i].right==lineinfo[i-5].right&&
        lineinfo[i].right==lineinfo[i+4].right&&lineinfo[i].right==lineinfo[i-4].right&&
        lineinfo[i].right==lineinfo[i+3].right&&lineinfo[i].right==lineinfo[i-3].right&&
        lineinfo[i].right==lineinfo[i+2].right&&lineinfo[i].right==lineinfo[i-2].right&&
        lineinfo[i].right==lineinfo[i+1].right&&lineinfo[i].right==lineinfo[i-1].right)
        {//一堆数据一样，显然不能作为单调转折点
            continue;
        }
        else if(lineinfo[i].right<=lineinfo[i+5].right&&lineinfo[i].right<=lineinfo[i-5].right&&
        lineinfo[i].right<=lineinfo[i+4].right&&lineinfo[i].right<=lineinfo[i-4].right&&
        lineinfo[i].right<=lineinfo[i+3].right&&lineinfo[i].right<=lineinfo[i-3].right&&
        lineinfo[i].right<=lineinfo[i+2].right&&lineinfo[i].right<=lineinfo[i-2].right&&
        lineinfo[i].right<=lineinfo[i+1].right&&lineinfo[i].right<=lineinfo[i-1].right)
        {//就很暴力，这个数据是在前5，后5中最大的，那就是单调突变点
            monotonicity_change_line=i;
            break;
        }
    }
    return monotonicity_change_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     左下角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Left_Down_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Down_Point(int start,int end)//找四个角点，返回值是角点所在的行数
{
    int i,t;
    int left_down_line=0;
    if(watch.left_lost>=0.9*120)//大部分都丢线，没有拐点判断的意义
       return left_down_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=120-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=120-1-5;
    //if(end<=120-watch.watch_lost)
    //    end=120-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=end;i<=start;i++)
    {
        if(left_down_line==0&&//只找第一个符合条件的点
           abs(lineinfo[i].left-lineinfo[i-1].left)<=5&&//角点的阈值可以更改
           abs(lineinfo[i-1].left-lineinfo[i-2].left)<=5&&  
           abs(lineinfo[i-2].left-lineinfo[i-3].left)<=5&&
              (lineinfo[i].left-lineinfo[i+2].left)>=5&&
              (lineinfo[i].left-lineinfo[i+3].left)>=10&&
              (lineinfo[i].left-lineinfo[i+4].left)>=10)
        {
            left_down_line=i;//获取行数即可
            break;
        }
    }
    return left_down_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     左上角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Left_Up_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Up_Point(int start,int end)//找四个角点，返回值是角点所在的行数
{
    int i,t;
    int left_up_line=0;
    //if(watch.left_lost>=0.9*120)//大部分都丢线，没有拐点判断的意义
     //  return left_up_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    //if(end<=MT9V03X_H-Search_Stop_Line)//搜索截止行往上的全都不判
    //    end=MT9V03X_H-Search_Stop_Line;
    if(end<=5)//及时最长白列非常长，也要舍弃部分点，防止数组越界
        end=5;
    if(start>=120-1-5)
        start=120-1-5;
    for(i=end;i<=start;i++)
    {
        if(left_up_line==0&&//只找第一个符合条件的点
           abs(lineinfo[i].left-lineinfo[i+1].left)<=5&&
           abs(lineinfo[i+1].left-lineinfo[i+2].left)<=5&&  
           abs(lineinfo[i+2].left-lineinfo[i+3].left)<=5&&
              (lineinfo[i].left-lineinfo[i-2].left)>=7&& 
              (lineinfo[i].left-lineinfo[i-3].left)>=8&&
              (lineinfo[i].left-lineinfo[i-4].left)>=10)
        {
            left_up_line=i;//获取行数即可
            break;
        }
    }
    return left_up_line;//如果是MT9V03X_H-1，说明没有这么个拐点
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     右下角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Right_Down_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Down_Point(int start,int end)//找四个角点，返回值是角点所在的行数
{
    int i,t;
    int right_down_line=0;
    if(watch.right_lost>=0.9*120)//大部分都丢线，没有拐点判断的意义
        return right_down_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=120-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=120-1-5;
    if(end<=5)
       end=5;
    for(i=end;i<=start;i++)
    {
        if(right_down_line==0&&//只找第一个符合条件的点
           abs(lineinfo[i].right-lineinfo[i+1].right)<=5&&//角点的阈值可以更改
           abs(lineinfo[i+1].right-lineinfo[i+2].right)<=5&&  
           abs(lineinfo[i+2].right-lineinfo[i+3].right)<=5&&
              (lineinfo[i].right-lineinfo[i-2].right)<=-5&&
              (lineinfo[i].right-lineinfo[i-3].right)<=-10&&
              (lineinfo[i].right-lineinfo[i-4].right)<=-10)
        {
            right_down_line=i;//获取行数即可
            break;
        }
    }
    return right_down_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右上角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Up_Point(int start,int end)//找四个角点，返回值是角点所在的行数
{
    int i,t;
    int right_up_line=0;
    if(watch.right_lost>=0.9*120)//大部分都丢线，没有拐点判断的意义
        return right_up_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }    
    
    if(end<=5)//及时最长白列非常长，也要舍弃部分点，防止数组越界
        end=5;
    if(start>=120-1-5)
        start=120-1-5;
    for(i=end;i<=start;i++)
    {
        if(right_up_line==0&&//只找第一个符合条件的点
           abs(lineinfo[i].right-lineinfo[i-1].right)<=5&&//下面两行位置差不多
           abs(lineinfo[i-1].right-lineinfo[i-2].right)<=5&&  
           abs(lineinfo[i-2].right-lineinfo[i-3].right)<=5&&
              (lineinfo[i].right-lineinfo[i+2].right)<=-8&&
              (lineinfo[i].right-lineinfo[i+3].right)<=-15&&
              (lineinfo[i].right-lineinfo[i+4].right)<=-15)
        {
            right_up_line=i;//获取行数即可
            break;
        }
    }
    return right_up_line;
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     通过斜率，定点补线--
  @param     k       输入斜率
             startY  输入起始点纵坐标
             endY    结束点纵坐标
  @return    null
  Sample     K_Add_Boundry_Left(float k,int startY,int endY);
  @note      补得线直接贴在边线上
-------------------------------------------------------------------------------------------------------------------*/
void K_Add_Boundry_Left(float k,int startX,int startY,int endY)
{
    int i,t;
    if(startY>=120-1)
        startY=120-1;
    else if(startY<=0)
        startY=0;
    if(endY>=120-1)
        endY=120-1;
    else if(endY<=0)
        endY=0;
    if(startY<endY)//--操作，start需要大
    {
        t=startY;
        startY=endY;
        endY=t;
    }
    for(i=endY;i<=startY;i++)
    {
        lineinfo[i].left=(int)((i-startY)/k+startX);//(y-y1)=k(x-x1)变形，x=(y-y1)/k+x1
        if(lineinfo[i].left>=188-1)
        {
            lineinfo[i].left=188-1;
        }
        else if(lineinfo[i].left<=0)
        {
            lineinfo[i].left=0;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     通过斜率，定点补线
  @param     k       输入斜率
             startY  输入起始点纵坐标
             endY    结束点纵坐标
  @return    null    直接补边线
  Sample     K_Add_Boundry_Right(float k,int startY,int endY);
  @note      补得线直接贴在边线上
-------------------------------------------------------------------------------------------------------------------*/
void K_Add_Boundry_Right(float k,int startX,int startY,int endY)
{
    int i,t;
    if(startY>=120-1)
        startY=120-1;
    else if(startY<=0)
        startY=0;
    if(endY>=120-1)
        endY=120-1;
    else if(endY<=0)
        endY=0;
    if(startY<endY)
    {
        t=startY;
        startY=endY;
        endY=t;
    }
    for(i=endY;i<=startY;i++)
    {
        lineinfo[i].right=(int)((i-endY)/k+startX);//(y-y1)=k(x-x1)变形，x=(y-y1)/k+x1
        if(lineinfo[i].right>=188-1)
        {
            lineinfo[i].right=188-1;
        }
        else if(lineinfo[i].right<=0)
        {
            lineinfo[i].right=0;
        }
    }
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     环岛检测
  @param     null
  @return    null
  Sample     Island_Detect(void);
  @note      利用四个拐点判别函数，单调性改变函，连续性数撕裂点，分为8步
-------------------------------------------------------------------------------------------------------------------*/
void Island_Detect()
{ 
    static float k=0;//3和5状态的k
    static int island_state_5_down[2]={0};//状态5时即将离开环岛，左右边界边最低点，[0]存y，第某行，{1}存x，第某列
    static int island_state_3_up[2]={0};//状态3时即将进入环岛用，左右上面角点[0]存y，第某行，{1}存x，第某列
    static int left_down_guai[2]={0};//四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列
    static int right_down_guai[2]={0};//四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列
	static int Left_Up_Guai[2]={0};
    int monotonicity_change_line[2];//单调性改变点坐标，[0]寸某行，[1]寸某列
    int monotonicity_change_left_flag=0;//不转折是0
    int monotonicity_change_right_flag=0;//不转折是0
    int continuity_change_right_flag=0; //连续是0
    int continuity_change_left_flag=0;  //连续是0
    //以下是常规判断法
    continuity_change_left_flag=Continuity_Change_Left(120-1-5,10);//连续性判断
    continuity_change_right_flag=Continuity_Change_Right(120-1-5,10);
    monotonicity_change_right_flag=Monotonicity_Change_Right(80,10);
    monotonicity_change_left_flag=Monotonicity_Change_Left(80,10);
    if(watch.cross_flag==0&&watch.straight_flag==0)//&&Ramp_Flag==0
    {
        continuity_change_left_flag=Continuity_Change_Left(80,10);//连续性判断
        continuity_change_right_flag=Continuity_Change_Right(80,10);
		
		watch.monotonicity_change_left_flag = monotonicity_change_left_flag;
		watch.continuity_change_left_flag = continuity_change_left_flag;
		watch.continuity_change_right_flag = continuity_change_right_flag;
		
        if(watch.Left_Island_Flag==0)//左环
        {
            if(//monotonicity_change_right_flag==0&& //右边是单调的
               continuity_change_left_flag!=0&& //左边是不连续的
               continuity_change_right_flag==0&& //左环岛右边是连续的
               watch.left_lost>=5&& //左边丢线很多
               watch.left_lost<=50&& //也不能全丢了
               watch.right_lost<=10&&//右边丢线较少
               watch.watch_lost>=120*0.85&& //搜索截止行看到很远
               //Boundry_Start_Left>=MT9V03X_H-20&&Boundry_Start_Right>=MT9V03X_H-20&& //边界起始点靠下
               watch.cross<=10)//双边丢线少
				
            {
                left_down_guai[0]=Find_Left_Down_Point(119,20);//找左下角点
				watch.left_down_guai = left_down_guai[0];
                if(left_down_guai[0]<=80)//条件1很松，在这里判断拐点，位置不对，则是误判，跳出
                {
                    watch.Island_State=1;
                    watch.Left_Island_Flag=1;
                }
                else//误判，归零
                {
                    watch.Island_State=0;
                    watch.Left_Island_Flag=0;
                }
            }
        }
        if(watch.Right_Island_Flag==0)//右环
        {
            if(monotonicity_change_left_flag==0&&
               continuity_change_left_flag==0&& //右环岛左边是连续的
               continuity_change_right_flag!=1&& //右边是不连续的
               watch.right_lost>=10&&           //右丢线多
               watch.right_lost<=50&&           //右丢线不能太多
               watch.left_lost<=10&&            //左丢线少
               watch.watch_lost>=120*0.85&& //搜索截止行看到很远
               //Boundry_Start_Left>=MT9V03X_H-20&&Boundry_Start_Right>=MT9V03X_H-20&& //边界起始点靠下
               watch.cross<=10)
            {
                right_down_guai[0]=Find_Right_Down_Point(120-1,20);//右下点
                if(right_down_guai[0]<=80)//条件1很松，在这里加判拐点，位置不对，则是误判，跳出
                {
                    watch.Island_State=1;
                    watch.Right_Island_Flag=1;
                }
                else
                {
                    watch.Island_State=0;
                    watch.Right_Island_Flag=0;
                }
            }
        }
    }
//    ips200_show_int(0,7*16,continuity_change_left_flag,2);
//    ips200_show_int(0,8*16,continuity_change_right_flag,2);
//    ips200_show_int(0,9*16,Left_Lost_Time,2);
//    ips200_show_int(0,10*16,Right_Lost_Time,2);
//    ips200_show_int(0,11*16,Search_Stop_Line,2);
//    ips200_show_int(0,12*16,Boundry_Start_Left,2);

//    //电磁符合强跳3状态，默认注释掉
    // if(Cross_Flag==0&&(Island_State==0||Island_State==2||Island_State==1)&&Zebra_State==0&&Ramp_Flag==0)
    // {
    //     if(ADC_Nor_Value[2]>=65)//中心电感值大
    //     {
    //         if (continuity_change_left_flag==0&&Right_Lost_Time>=20&&Both_Lost_Time<5&&Boundry_Start_Left>MT9V03X_H-10)
    //         {//左边连续，左边起始点，左丢线少，双边丢线少，右边丢线多直接右环岛三状态
    //             Right_Island_Flag=1;
    //             Island_State=3;
    //         }
    //         else if(continuity_change_right_flag==0&&Left_Lost_Time>=20&&Both_Lost_Time<5&&Boundry_Start_Right>MT9V03X_H-10)
    //         {
    //             Left_Island_Flag=1;
    //             Island_State=3;
    //         }
    //     }
    // }

    if(watch.Left_Island_Flag==1)//1状态下拐点还在，没丢线
    {
        if(watch.Island_State==1)
        {
            monotonicity_change_line[0]=Monotonicity_Change_Left(70,5);//寻找单调性改变点
            monotonicity_change_line[1]=lineinfo[monotonicity_change_line[0]].left;
            Left_Add_Line((int)(monotonicity_change_line[1]*0.15),0,monotonicity_change_line[1],monotonicity_change_line[0]);
            if((watch.Island_State==1)&&(watch.left_near_lost<5))//下方当丢线时候进2
            {
                watch.Island_State=2;
            }
        }


        else if(watch.Island_State==2)//下方角点消失，2状态时下方应该是丢线，上面是弧线
        {
            monotonicity_change_line[0]=Monotonicity_Change_Left(100,10);//寻找单调性改变点
            monotonicity_change_line[1]=lineinfo[monotonicity_change_line[0]].left;
            Left_Add_Line((int)(monotonicity_change_line[1]*0.1),0,monotonicity_change_line[1],monotonicity_change_line[0]);
            if(watch.Island_State==2&&(watch.left_far_lost<=10||monotonicity_change_line[0]<20))//当圆弧靠下时候，进3
            {
                watch.Island_State=3;//最长白列寻找范围也要改，见camera.c
                watch.Left_Island_Flag=1;
            }
        }
        else if(watch.Island_State==3)//3状态准备进环，寻找上拐点，连线
        {
            if(k!=0)
            {
                K_Add_Boundry_Right(k,187-5,100,1);
				for(int i = 0;i<=115;i++)
				{
					if(lineinfo[i].left>lineinfo[i].right)
					{
						lineinfo[i].left = 0;
					}
				}
            }
            else
            {
                Left_Up_Guai[0]=Find_Left_Up_Point(90,40);//找左上拐点
                Left_Up_Guai[1]=lineinfo[Left_Up_Guai[0]].left;

				watch.left_up_guai = Left_Up_Guai[0];
                if (Left_Up_Guai[0]>115)//此处为了防止误判，如果经常从3状态归零，建议修改此处判断条件
                {
                    watch.Island_State=0;
                    watch.Left_Island_Flag=0;
                }

                if(k==0&&(50<=Left_Up_Guai[0]&&Left_Up_Guai[0]<115)/*&&(50<Left_Up_Guai[1]&&Left_Up_Guai[1]<110)*/)//拐点出现在一定范围内，认为是拐点出现
                {
                    island_state_3_up[0]= Left_Up_Guai[0];
                    island_state_3_up[1]= Left_Up_Guai[1];
                    k=(float)((float)(0-island_state_3_up[0])/(float)(187-10-island_state_3_up[1]));
                    K_Add_Boundry_Right(k,187-5,100,1);
                }
            }
            if((watch.Island_State==3)&&(abs(mpu6050_gyro_z)>=60))//纯靠陀螺仪积分入环
            {
                k=0;//斜率清零
                watch.Island_State=4;//这一步时候顺便调整了最长白列的搜索范围
                //Longest_White_Column();//最长白列
            }
        }
        else if(watch.Island_State==4)//状态4已经在里面
        {
            if(abs(mpu6050_gyro_z)>200)//积分200度以后在打开出环判断
            {
                monotonicity_change_line[0]=Monotonicity_Change_Right(120-10,10);//单调性改变
                monotonicity_change_line[1]=lineinfo[monotonicity_change_line[0]].right;
                if((watch.Island_State==4)&&(35<=monotonicity_change_line[0]&&monotonicity_change_line[0]<=85/*&&monotonicity_change_line[1]>=10*/))//单调点靠下，进去5
                {//monotonicity_change_line[1]>=90&&
                    island_state_5_down[0]=0;
                    island_state_5_down[1]=lineinfo[0].right;
                    k=(float)((float)(0-monotonicity_change_line[0])/(float)(island_state_5_down[1]-monotonicity_change_line[1]));
                    K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],119);//和状态3一样，记住斜率
                    watch.Island_State=5;
                }
            }
        }
        else if(watch.Island_State==5)//出环
        {
            K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],119);
            if((watch.Island_State==5)&&(watch.right_near_lost>20))//右边先丢线
            {
                watch.Island_State=6;
            }
        }
        else if(watch.Island_State==6)//还在出
        {
            K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],119);
            if((watch.Island_State==6)&&((watch.right_near_lost<10)||(abs(mpu6050_gyro_z)>=320)))//右边不丢线
            {//
                k=0;
                watch.Island_State=7;
            }
        }
        else if(watch.Island_State==7)//基本出去了，在寻找拐点，准备离开环岛状态
        {
            Left_Up_Guai[0]=Find_Left_Up_Point(110,0);//获取左上点坐标，坐标点合理去8
            Left_Up_Guai[1]=lineinfo[Left_Up_Guai[0]].left;
            if((watch.Island_State==7)&&(Left_Up_Guai[1]<=100)&&(20<=Left_Up_Guai[0]&&Left_Up_Guai[0]<=115))//注意这里，对横纵坐标都有要求
            {
                watch.Island_State=8;//基本上找到拐点就去8
            }
        }
        else if(watch.Island_State==8)//连线，出环最后一步
        {
            Left_Up_Guai[0]=Find_Left_Up_Point(110,0);//获取左上点坐标
            Left_Up_Guai[1]=lineinfo[Left_Up_Guai[0]].left;
            Lengthen_Left_Boundry(Left_Up_Guai[0]+1,0);
            if((watch.Island_State==8)&&(Left_Up_Guai[0]<=20/*||(Left_Up_Guai[0]>10&&watch.left_far_lost<10)*/))//当拐点靠下时候，认为出环了，环岛结束
            {//要么拐点靠下，要么拐点丢了，切下方不丢线，认为环岛结束了
                mpu6050_gyro_z=0;//数据清零
                watch.Island_State=9;//8时候环岛基本结束了，为了防止连续判环，8后会进9，大概几十毫秒后归零，
                watch.Left_Island_Flag=0;
            }
        }
    }
	/*
    else if(Right_Island_Flag==1)
    {
        if(Island_State==1)//1状态下拐点还在，没丢线
        {
            monotonicity_change_line[0]=Monotonicity_Change_Right(30,5);//单调性改变
            monotonicity_change_line[1]=lineinfo[monotonicity_change_line[0]];
            Right_Add_Line((int)(MT9V03X_W-1-(monotonicity_change_line[1]*0.15)),MT9V03X_H-1,monotonicity_change_line[1],monotonicity_change_line[0]);
            if(Boundry_Start_Right<=30)//右下角先丢线
            {
                Island_State=2;
            }
        }
        else if(Island_State==2)//2状态下方丢线，上方即将出现大弧线
        {
            monotonicity_change_line[0]=Monotonicity_Change_Right(70,5);//单调性改变
            monotonicity_change_line[1]=lineinfo[monotonicity_change_line[0]];
            Right_Add_Line((int)(MT9V03X_W-1-(monotonicity_change_line[1]*0.15)),MT9V03X_H-1,monotonicity_change_line[1],monotonicity_change_line[0]);
//            if(Island_State==2&&(Boundry_Start_Right>=MT9V03X_H-10))//右下角再不丢线进3
            if(Island_State==2&&(Boundry_Start_Right>=MT9V03X_H-5||monotonicity_change_line[0]>50))//右下角再不丢线进3
            {
                Island_State=3;//下方丢线，说明大弧线已经下来了
                Right_Island_Flag=1;
            }
        }
        else if(Island_State==3)//下面已经出现大弧线，且上方出现角点
        {
            if(k!=0)//将角点与下方连接，画一条死线
            {
                K_Draw_Line(k,30,MT9V03X_H-1,0);
                Longest_White_Column();//刷新最长白列
            }
            else
            {
                Right_Up_Guai[0]=Find_Right_Up_Point(40,10);//找右上拐点
                Right_Up_Guai[1]=lineinfo[Right_Up_Guai[0]];

                if(Right_Up_Guai[0]<10)//这里改过，此处为了防止环岛误判，如果经常出现环岛3归零，请修改此处判断条件
                {
                    Island_State=0;
                    Right_Island_Flag=0;
                }

                if(k==0&&(15<=Right_Up_Guai[0]&&Right_Up_Guai[0]<50)&&(70<Right_Up_Guai[1]&&Right_Up_Guai[1]<150))//找第一个符合条件的角点，连线
                {
                    island_state_3_up[0]= Right_Up_Guai[0];
                    island_state_3_up[1]= Right_Up_Guai[1];
                    k=(float)((float)(MT9V03X_H-island_state_3_up[0])/(float)(20-island_state_3_up[1]));
                    K_Draw_Line(k,30,MT9V03X_H-1,0);
                    Longest_White_Column();//刷新赛道数据
                }
            }
            if((Island_State==3)&&(abs(FJ_Angle)>=60))//只依靠陀螺仪积分
            {
                k=0;//斜率清零
                Island_State=4;
                Longest_White_Column();//最长白列
            }//记得去最长白列那边改一下，区分下左右环岛
        }
        else if(Island_State==4)//4状态完全进去环岛了
        {
            if(FJ_Angle>200)//环岛积分200度后再打开单调转折判断
            {
                monotonicity_change_line[0]=Monotonicity_Change_Left(90,10);//单调性改变
                monotonicity_change_line[1]=Left_Line[monotonicity_change_line[0]];
                if((Island_State==4)&&(35<=monotonicity_change_line[0]&&monotonicity_change_line[0]<=55&&monotonicity_change_line[1]<MT9V03X_W-10))//单调点靠下，进去5
                {//monotonicity_change_line[1]<=120&&
                   island_state_5_down[0]=MT9V03X_H-1;
                   island_state_5_down[1]=Left_Line[MT9V03X_H-1]-5;//抓住第一次出现的斜率，定死
                   k=(float)((float)(MT9V03X_H-monotonicity_change_line[0])/(float)(island_state_5_down[1]-monotonicity_change_line[1]));
                   K_Add_Boundry_Left(k,island_state_5_down[1],island_state_5_down[0],0);
                   Island_State=5;
                }
            }
        }
        else if(Island_State==5)//准备出环岛
        {
            K_Add_Boundry_Left(k,island_state_5_down[1],island_state_5_down[0],0);
            if(Island_State==5&&Boundry_Start_Left<MT9V03X_H-20)//左边先丢线
            {
                Island_State=6;
            }
        }
        else if(Island_State==6)//继续出
        {
            K_Add_Boundry_Left(k,island_state_5_down[1],island_state_5_down[0],0);
            if((Island_State==6)&&(Boundry_Start_Left>MT9V03X_H-10||abs(FJ_Angle)>=320))//左边先丢线
            {//
                k=0;
                Island_State=7;
            }
        }
        else if(Island_State==7)//基本出环岛，找角点
        {
            Right_Up_Guai[0]=Find_Right_Up_Point(MT9V03X_H-10,0);//获取左上点坐标，找到了去8
            Right_Up_Guai[1]=lineinfo[Right_Up_Guai[0]];
            if((Island_State==7)&&((Right_Up_Guai[1]>=MT9V03X_W-88&&(5<=Right_Up_Guai[0]&&Right_Up_Guai[0]<=MT9V03X_H-20))))//注意这里，对横纵坐标都有要求，因为赛道不一样，会意外出现拐点
            {//当角点位置合理时，进8
                Island_State=8;
            }
        }
        else if(Island_State==8)//环岛8
        {
            Right_Up_Guai[0]=Find_Right_Up_Point(MT9V03X_H-1,10);//获取右上点坐标
            Right_Up_Guai[1]=lineinfo[Right_Up_Guai[0]];
            Lengthen_Right_Boundry(Right_Up_Guai[0]-1,MT9V03X_H-1);
            if((Island_State==8)&&(Right_Up_Guai[0]>=MT9V03X_H-20||(Right_Up_Guai[0]<10&&Boundry_Start_Left>=MT9V03X_H-10)))//当拐点靠下时候，认为出环了，环岛结束
            {//角点靠下，或者下端不丢线，认为出环了
                FJ_Angle=0;
                Island_State=9;
                Right_Island_Flag=0;
            }
        }
    }*/

//    if(Err>=0)
//    {
//       Flag_Show_101(MT9V03X_W,image_two_value,(uint8)(uint8)Err/10);//上面那个数字
//       Flag_Show_102(MT9V03X_W,image_two_value,(uint8)((uint8)Err%10));//下面的数字
//    }
//    else
//    {
//        Flag_Show_101(MT9V03X_W,image_two_value,(uint8)(uint8)-Err/10);//上面那个数字
//        Flag_Show_102(MT9V03X_W,image_two_value,(uint8)((uint8)-Err%10));//下面的数字
//    }

//    ips200_showfloat(0,12,FJ_Angle,3,3);
//    ips200_showuint16(0,11,Island_State);
}

