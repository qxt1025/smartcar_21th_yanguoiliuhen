/*
 * GUI.c
 *
 *  Created on: 2023年6月20日
 *      Author: Admin
 */
#include "GUI.h"
#include "application.h"
#include "display.h"
#include "port.h"

#define Data_mode 0  //0:正常模式 1：读取二级备用数据

TSUI_TypeDef tsui;

static u32 far g_tsui_data_to_write[100];
static u32 far g_tsui_data_to_write2[100];
static u32 far g_tsui_data_to_read[100];
static u32 far g_tsui_data_to_read2[100];

/*
 * Purpose: 计算参数列表可调项最大索引
 * Param  : adjust_paralist - 参数列表首地址，以 precision==0 结尾
 * Return : 最大参数索引（从0开始）
 * Note   : 参数列表至少包含结尾项 {0}
 */
//获取可调整参数的总数
uint8 Get_maxpage(struct paralist_s* adjust_paralist)
{
    u16 i=0;
    while(adjust_paralist[i].precision > 0)
	{
		i++;
	}
    return i - 1;    //由于参数编号从0开始，所以要减1
}

/*
 * Purpose: 初始化UI状态并统计两页参数数量
 * Param  : 无
 * Return : 无
 * Note   : 仅做状态初始化，不在此处做耗时显示逻辑
 */
//UI初始化
void UI_init()
{
    tsui.paraMax = Get_maxpage(&paralist[0]);//获取参数总数
    tsui.paraMax2= Get_maxpage(&paralist2[0]);
    tsui.ui_enable = 1;
    tsui.img_enable = 1;
    //TSUI_FlashRead();
}

/*
 * Purpose: 将当前参数页数据保存到主/备份Flash页
 * Param  : 无
 * Return : 无
 * Note   : 先写主页，再按有效数据阈值同步到备份页
 */
//FALSH存参
void TSUI_FlashSave()
{
	
	
	u32 para_count = 0; // 有效参数计数
	u8 i;              // 参数索引
	if(tsui.para_page==0)
	{
		for( i=0;i<(tsui.paraMax+1);i++)
		{
			g_tsui_data_to_write[i] = *paralist[i].para;
		}
		GUI_flash_erase_page(EEPROM_SAVE_SECTOR);          //擦除主储存单元（在写入flash前要先擦除整页flash）
		GUI_flash_write_page(EEPROM_SAVE_SECTOR,g_tsui_data_to_write,sizeof(g_tsui_data_to_write));//写入数据
		for( i = 0 ; i < (tsui.paraMax+1) ; i ++)
		{        //计算主储存单元的已有的数据量
			if( g_tsui_data_to_write[i])
			{
				para_count ++;
			}
		}
		if( para_count > (tsui.paraMax+1) / 3)//认定为主储存单元数据没有被擦除
		{
			GUI_flash_erase_page(EEPROM_BACKUP_SECTOR);        //若主储存单元数据已经填入，主储存安全，则擦除备份储存单元
			GUI_flash_write_page(EEPROM_BACKUP_SECTOR,g_tsui_data_to_write,sizeof(g_tsui_data_to_write));//将数据存入备份储存单元，若主储存单元在发车时复位，下次开机可以调用备份储存单元的数据，防止数据丢失
		}
		para_count=0;
	}
	else if(tsui.para_page==1)
	{
		for( i=0;i<(tsui.paraMax2+1);i++)
		{
			g_tsui_data_to_write2[i] = *paralist2[i].para;
		}
		GUI_flash_erase_page(EEPROM2_SAVE_SECTOR);          //擦除主储存单元（在写入flash前要先擦除整页flash）
		GUI_flash_write_page(EEPROM2_SAVE_SECTOR,g_tsui_data_to_write2,sizeof(g_tsui_data_to_write2));//写入数据
		for( i = 0 ; i < (tsui.paraMax2+1) ; i ++)
		{        //计算主储存单元的已有的数据量
			if( g_tsui_data_to_write2[i])
			{
				para_count ++;
			}
		}
		if( para_count > (tsui.paraMax2+1) / 3)//认定为主储存单元数据没有被擦除
		{
			GUI_flash_erase_page(EEPROM2_BACKUP_SECTOR);        //若主储存单元数据已经填入，主储存安全，则擦除备份储存单元
			GUI_flash_write_page(EEPROM2_BACKUP_SECTOR,g_tsui_data_to_write2,sizeof(g_tsui_data_to_write2));//将数据存入备份储存单元，若主储存单元在发车时复位，下次开机可以调用备份储存单元的数据，防止数据丢失
		}
		para_count=0;
	}
	/*for( i =tsui.paraMax+1 ; i>0; i --) //计算主储存单元的已有的数据量
    {
        if( g_tsui_data_to_read[i]==0)
            para_count ++;
        else
            break;
    }
    if(para_count<10)//认定为主储存单元数据没有被擦除
    {
        GUI_flash_erase_page(EEPROM_BACKBACKUP_SECTOR);        //若主储存单元数据已经填入，主储存安全，则擦除备份储存单元
        GUI_flash_write_page(EEPROM_BACKBACKUP_SECTOR,g_tsui_data_to_write,sizeof(g_tsui_data_to_write));//将数据存入备份储存单元，若主储存单元在发车时复位，下次开机可以调用备份储存单元的数据，防止数据丢失
    }*/
    //else TSUI_FlashRead();
}

/*
 * Purpose: 从Flash主/备份页恢复参数到运行变量
 * Param  : 无
 * Return : 无
 * Note   : 通过有效数据计数判断主页是否可用
 */
//读取存储的FLASH参数
void TSUI_FlashRead()  //将存入flash的参数读取出来
{
  //定义参数读取
    u32 i;
    u32 para_count = 0;
    
    
	if(Data_mode==0)
	{
		GUI_flash_read_page(EEPROM_SAVE_SECTOR,g_tsui_data_to_read,(tsui.paraMax+1));
		for( i = 0 ; i < (tsui.paraMax+1) ; i ++)//计算主储存单元的已有的数据量
		{
			if( g_tsui_data_to_read[i])
			{
				para_count ++;
			}
		}
		
		if( para_count > (tsui.paraMax+1) / 5)//认定为主储存单元数据没有被擦除
		{
			for( i=0;i<(tsui.paraMax+1);i++)
			{
					*paralist[i].para = g_tsui_data_to_read[i];
			}
		}
		else
		{              //主储存单元数据被擦除了
			GUI_flash_read_page(EEPROM_BACKUP_SECTOR,g_tsui_data_to_read,(tsui.paraMax+1));//读取备份储存单元数据
			for( i=0;i<(tsui.paraMax+1);i++)
			{
				*paralist[i].para = g_tsui_data_to_read[i];
			}
		}
		para_count=0;
		GUI_flash_read_page(EEPROM2_SAVE_SECTOR,g_tsui_data_to_read2,(tsui.paraMax2+1));
		for( i = 0 ; i < (tsui.paraMax2+1) ; i ++)//计算主储存单元的已有的数据量
		{
			if( g_tsui_data_to_read2[i])
			{
				para_count ++;
			}
		}
		
		if( para_count > (tsui.paraMax2+1) / 5)//认定为主储存单元数据没有被擦除
		{
			for( i=0;i<(tsui.paraMax2+1);i++)
			{
				*paralist2[i].para = g_tsui_data_to_read2[i];
			}
		}
		else
		{              //主储存单元数据被擦除了
			GUI_flash_read_page(EEPROM2_BACKUP_SECTOR,g_tsui_data_to_read2,(tsui.paraMax2+1));//读取备份储存单元数据
			for( i=0;i<(tsui.paraMax2+1);i++)
			{
				*paralist2[i].para = g_tsui_data_to_read2[i];
			}
		}
	}
	/*else if(Data_mode==1)
	{
		GUI_flash_read_page(EEPROM_BACKBACKUP_SECTOR,g_tsui_data_to_read,(tsui.paraMax+1));//读取备份储存单元数据
		for( i=0;i<(tsui.paraMax+1);i++)
			*paralist[i].para = g_tsui_data_to_read[i];
	}*/
}

/*
 * Purpose: 根据按键/编码器事件更新UI状态与参数值
 * Param  : 无
 * Return : 无
 * Note   : 该函数应被周期调用，事件处理后会清空事件标志
 */
//读取用户输入的操作
void TSUI_ButtonEvent()//如果同时有按键与旋转编码器可以改这里的代码以添加功能
{
    struct paralist_s* adjust_paralist;   // 当前页参数列表首地址
    uint8 para_num;                       // 当前页最大参数索引
    if(tsui.para_page==1)
    {
		adjust_paralist=&paralist2[0];
		para_num=tsui.paraMax2;
	}
    else
	{
		adjust_paralist=&paralist[0];
		para_num=tsui.paraMax;
	}
	switch(tsui.buttonevent)
	{
		case OK:  //key3按下（拨轮按下）
		{			
			if(tsui.img_showmode==4)
			{
				tsui.img_showmode=0;
			}
			else 
			{
				tsui.img_showmode++;
			}
			break;
		}
		case DOWN:   //key2按下（拨轮下）
		{			
			if(tsui.flag_page==IPS_display_max_page-1)
			{
				tsui.flag_page=0;
			}
			else 
			{
				tsui.flag_page++;
			}
			break;
		}
		case UP: //key1按下（拨轮上拨）
		{			
			if(tsui.flag_page==0)
			{
			  tsui.flag_page=IPS_display_max_page-1;
			}
			else 
			{
			  tsui.flag_page--;
			}
			break;
		}
		case PRESS:
		{
			tsui.paraStatus ^= 1;    //状态取反，参数选择和参数改变切换
			break;
		}
		case CW:                //如果是顺时针旋转
		{
			if(tsui.paraStatus)//参数改变模式     注：这里的“参数改变“与“参数选择” 均为模式的名称，不是名词加动词
			{
				*adjust_paralist[tsui.paraSelect].para += adjust_paralist[tsui.paraSelect].precision;//让所选中的参数加上其调整精度
			}
			else            //参数选择模式
			{
				if(tsui.paraSelect < para_num )//如果当前选中的参数的序号小于参数总数
				{
					tsui.paraSelect ++;             //选择下一个参数
				}
				else
				{
					tsui.paraSelect = 0;            //回到第一个参数
				}
			}
			break;
		}
		case CCW:           //如果是逆时针旋转
		{
			if(tsui.paraStatus)  //参数改变模式
			{
			  *adjust_paralist[tsui.paraSelect].para -= adjust_paralist[tsui.paraSelect].precision;//让所选中的参数减去其调整精度
			}
			else                    //参数选择模式
			{
			  if(tsui.paraSelect > 0)                   //如果当前选中的参数的序号大于0
			  {
				tsui.paraSelect --;                     //选择上一个参数
			  }
			  else
			  {
				tsui.paraSelect = para_num;         //直接选中最后一个参数
			  }
			}
			break;
		}
		case Press2:	//EC11双击翻页
		{
			tsui.para_page=!tsui.para_page;
			tsui.paraSelect=0;
			tsui.paraStatus=0;
			break;
		}
	  default:break;
	}
	tsui.buttonevent = NONE;//清除指令
}
