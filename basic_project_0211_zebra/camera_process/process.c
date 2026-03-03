#include "process.h"
#include "Element_recognition.h"
#include "scan_line.h"
#include "Binarization.h"

//加权控制
const uint8 Weight[MT9V03X_H]=
{
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端00 ——09 行权重
           		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //图像最远端10 ——19 行权重
      		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1 , 
		3, 3, 4, 
		4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 
		14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 19, 18, 
		17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 
		7, 6, 5, 4, 3, 1, 1, 1, 1, 1,  //图像最远端20 ——29 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             //图像最远端40 ——49 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端50 ——59 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,          //图像最远端60 ——69 行权重

 
};//权重只是选取某一范围内作为主要控制行，并且尽可能用上其他图像误差行
  //参数没有固定范围，来源，方法，实际测试好用就可以
  //因为实际使用的时候加权后又取平均，所以数据范围和平均，单行误差没什么区别

const uint8 Weight_far[MT9V03X_H]=
{
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端00 ——09 行权重
           		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //图像最远端10 ——19 行权重
      		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1 , 
	        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端50 ——59 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 13, 13, 
		14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 19, 18, 
		17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 
		7, 6, 5, 4, 3,   //图像最远端20 ——29 行权重
            //图像最远端40 ——49 行权重
          //图像最远端60 ——69 行权重

 
};

const uint8 Weight_near[MT9V03X_H]=
{
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 13, 13, 
		14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 19, 18, 
		17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 
		7, 6, 5, 4, 3,   //图像最远端20 ——29 行权重
            //图像最远端40 ——49 行权重
          //图像最远端60 ——69 行权重
	        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端00 ——09 行权重
           		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //图像最远端10 ——19 行权重
      		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1 , 
	        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端50 ——59 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 

 
};

float Err_Sum(void)
{
    int i,temp_lost_line;
    float err=0;
    float weight_count=0;
	float slope_cnt = 0;
	float cnt = 0;
	temp_lost_line = watch.watch_lost;
	watch.slope_err = 0;
	if(temp_lost_line<=1)
		temp_lost_line = 1;
    //常规误差

	for(i=0;i<=119;i++)
	{
		if(lineinfo[i].right_adjust == 0)
			break;
	}
	watch.line_end = i;
	
	for(i=watch.line_end;i>=0;i--)//常规误差计算
	{
		if(lineinfo[i].right_adjust!=0)
		{
			err+=(MT9V03X_W/2-((lineinfo[i].left_adjust +lineinfo[i].right_adjust)>>1))*Weight[i];//右移1位，等效除2
			weight_count+=Weight[i];
		}

	}
	err=err/weight_count;
	
	for(i=115;i>=85;i--)//常规误差计算
	{
		watch.err_far+=(MT9V03X_W/2-((lineinfo[i].left_adjust+lineinfo[i].right_adjust)>>1))*Weight_far[i];//右移1位，等效除2
		cnt+=Weight_far[i];
	}
	watch.err_far = watch.err_far/cnt;
	
	for(i=watch.angle_far_line;i>=0;i--)//斜坡误差计算
	{
		if(lineinfo[i].right_adjust!=0)
		{
			watch.slope_err+=(MT9V03X_W/2-((lineinfo[i].left_adjust +lineinfo[i].right_adjust)>>1))*Weight_near[i];//右移1位，等效除2
			slope_cnt+=Weight_near[i];
		}

	}
	watch.slope_err=watch.slope_err/slope_cnt;
	
	return err;//注意此处，误差有正负，还有小数，注意数据类型
	
	
    
}