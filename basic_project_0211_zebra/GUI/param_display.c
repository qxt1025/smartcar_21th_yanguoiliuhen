#include "param_display.h"
#include "display.h"

//测试变量，后期可全部删除
int param_a=1;
int param_b=2;
int param_c=3;
int param_d=4;
int param_e=5;
int param_f=6;

int param_g=7;
int param_h=8;
int param_i=9;
int param_j=10;
int param_k=11;
int param_l=12;
/*****************************************************************************
***@breif	在UI界面右侧列表显示固定参数
		！！！！！所有sprintf()全放这，尽量不要乱放！！！！！！
***@param	无
***@retval	无
*******************************************************************************/
/*
 * Purpose: 更新右侧静态参数页字符串内容
 * Param  : 无
 * Return : 无
 * Note   : 建议在主循环中按需调用，避免高频sprintf
 */
void Static_Parameters_display()
{
	//====================================第1页参数显示================================================//
	sprintf(page[0].line1,"original_err=%d",mycar.original_err);
	sprintf(page[0].line2,"param_b=%d",param_b); 
	sprintf(page[0].line3,"param_c=%d",param_c);

	sprintf(page[0].line4,"param_d=%d",param_d); 
	sprintf(page[0].line5,"param_e=%d",param_e);
	sprintf(page[0].line6,"param_f=%d",param_f);
//===========================第2页显示参数==========================================		
	sprintf(page[1].line1,"param_g=%d",param_g);
	sprintf(page[1].line2,"param_h=%d",param_h); 
	sprintf(page[1].line3,"param_i=%d",param_i);
				 
	sprintf(page[1].line4,"param_j=%d",param_j); 
	sprintf(page[1].line5,"param_k=%d",param_k);
	sprintf(page[1].line6,"param_l=%d",param_l);
//=========================第三页参数==========================================================
	sprintf(page[0].line1,"param_a=%d",param_a);
	sprintf(page[0].line2,"param_b=%d",param_b); 
	sprintf(page[0].line3,"param_c=%d",param_c);

	sprintf(page[0].line4,"param_d=%d",param_d); 
	sprintf(page[0].line5,"param_e=%d",param_e);
	sprintf(page[0].line6,"param_f=%d",param_f);
//======================第4页参数==========================================================
	sprintf(page[1].line1,"param_g=%d",param_g);
	sprintf(page[1].line2,"param_h=%d",param_h); 
	sprintf(page[1].line3,"param_i=%d",param_i);
				 
	sprintf(page[1].line4,"param_j=%d",param_j); 
	sprintf(page[1].line5,"param_k=%d",param_k);
	sprintf(page[1].line6,"param_l=%d",param_l);
//===================================第5页参数==================================================
	sprintf(page[0].line1,"param_a=%d",param_a);
	sprintf(page[0].line2,"param_b=%d",param_b); 
	sprintf(page[0].line3,"param_c=%d",param_c);

	sprintf(page[0].line4,"param_d=%d",param_d); 
	sprintf(page[0].line5,"param_e=%d",param_e);
	sprintf(page[0].line6,"param_f=%d",param_f);
}
