/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  KH25_Flash_Store.c
* Author             :  王宇
* CPU Type           :  nRF51
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  14/04/2017
* Description        :  KH25V1635F FLASH 存储应用程序 记录历史数据 RAM<2K
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "KH25_Flash_Store.h"
//#include "Global.h"
#include "nrf_delay.h"

/* Private variables ---------------------------------------------------------*/
        
/* Private function prototypes -----------------------------------------------*/
u16  Get_Page_Number(u32 Addr);											// 获取页码
u16  Get_Sector_Number(u32 Addr);										// 获取扇区号
u8   Page_Pos_Determine(u32 Addr);										// 页位置判定
u16  SearchFor_Valid_Addr(u16 cPageAddr, u16* cAddr_Valid);				// 查找可用地址
void Store_History_Data(void);											// 存储历史数据
void Get_Current_Data(u8* Data_Buff);									// 获取当前数据

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           王宇@2017-04-14
* Function Name  :  Get_Page_Number
* Description    :  获得当前操作页码
* Input          :  u32		Addr		数据起始地址
* Output         :  None
* Return         :  u16		PageNumber	页码
*******************************************************************************/
u16 Get_Page_Number(u32 Addr)
{
	u16 PageNumber = (Addr >> 12) & 0x0FFF;
	return PageNumber;
}
// end of u16 Get_Page_Number(u32 Addr)

/*******************************************************************************
*                           王宇@2017-04-14
* Function Name  :  Get_Sector_Number
* Description    :  获得当前操作扇区号
* Input          :  u32		Addr			数据起始地址
* Output         :  None
* Return         :  u16		SectorNumber	扇区号
*******************************************************************************/
u16 Get_Sector_Number(u32 Addr)
{
	u16 SectorNumber = (Addr >> 0) & 0x0FFF;
	return SectorNumber;
}
// end of u16 Get_Sector_Number(u32 Addr)

/*******************************************************************************
*                           王宇@2017-04-14
* Function Name  :  Page_Pos_Determine
* Description    :  判断扇区位置
* Input          :  u32		Addr			数据起始地址
* Output         :  None
* Return         :  00-7F 1, 80-FF 0
*******************************************************************************/
u8 Page_Pos_Determine(u32 Addr)
{
	u8 Pos2K = ((Addr >> 8) & 0x0F) < 8;
	return Pos2K;
}
//end of u8 Page_Pos_Determine(u32 Addr)

/*******************************************************************************
*                           王宇@2017-04-14
* Function Name  :  SearchFor_Valid_Addr
* Description    :  获取可用地址
* Input          :  u16 	cPageAddr	当前搜索页码
*					u16*	cAddr_Valid 当前页可用地址
* Output         :  None
* Return         :  2Bytes数据 检索页->目标页码 数据页->校验码
*******************************************************************************/
u16 SearchFor_Valid_Addr(u16 cPageAddr, u16* cAddr_Valid)
{
	u32 Current_Addr;
	u8  Current_Data[16];
	
	// 当前页起始地址
	Current_Addr = (cPageAddr << 12) & 0xFFF000;
	// 清空数据存储Buffer
	memset(Current_Data, 0, 16);

	// 判断是否为检索页
	if (Page_Index == cPageAddr)
	{
		// 判断页地址高位 Byte[0]
		do
		{
			// 循环读取2Bytes数据 查找未操作过的地址
			KH25_Read_Data(Current_Addr, Current_Data, 2);
			*cAddr_Valid  = (Current_Addr >> 0) & 0x0FFF;
			Current_Addr += 2;
		}while (Current_Data[0] < 0xFF);
		
		// 读有效数据
		KH25_Read_Data(Current_Addr - 4, Current_Data, 2);
		// 返回2Bytes目标页码
		return((Current_Data[0] << 8) + Current_Data[1]);
	}
	else
	{
		// 判断校验位第一个字节 Byte[8] 和 冗余第一个字节 Byte[10]
		do
		{
			// 循环读取16Bytes 
			KH25_Read_Data(Current_Addr, Current_Data, 16);
			*cAddr_Valid  = (Current_Addr >> 0) & 0x0FFF;
			Current_Addr += 16;
		}while ((Current_Data[8] < 0xFF) && (Current_Data[10] == 0xFF));

		KH25_Read_Data(Current_Addr - 32, Current_Data, 16);
		// 返回2Bytes校验位
		return((Current_Data[8] << 8) + Current_Data[9]);
	}
}
// end of u16 SearchFor_Valid_Addr(u16 cPageAddr, u16 cAddr_Valid)

/*******************************************************************************
*                           王宇@2017-04-14
* Function Name  :  Store_History_Data
* Description    :  存储历史数据 间隔1min
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Store_History_Data(void)
{
	u8  Current_Page_Index[2];											// 当前操作页储存Buffer
	u8  Current_DataBuff[16];
	u16 Page_Start;														// 存储起始页码
	u16 cData_Addr, cIndex_Addr;										// 页内当前可用地址
	u32 Op_Data_Addr, Op_Index_Addr;									// 当前操作地址
	u8  i = 100;

	// 查找储存起始页 Index页可用地址
	Page_Start    = SearchFor_Valid_Addr(Page_Index, &cIndex_Addr);
	// 计算Index可储存地址
	Op_Index_Addr = (Page_Index << 12) + cIndex_Addr;

	// 查找数据存储页可用地址
	SearchFor_Valid_Addr(Page_Start, &cData_Addr);
	// 计算数据可存储地址
	Op_Data_Addr  = (Page_Start << 12) + cData_Addr;

	// 存储数据
	while(i)
	{
		// 获取当前数据 16Bytes
		Get_Current_Data(Current_DataBuff);
		// 存储当前数据
		KH25_Write_WithNoRead(Op_Data_Addr, Current_DataBuff, 16);
		// 移动指针到下一地址 16Bytes
		Op_Data_Addr += 16;

		// 判断是否跨页
		if (((Op_Data_Addr >> 0) & 0x0FFF) == 0)
		{
			// 计算新页地址
			Current_Page_Index[0] = (Op_Data_Addr >> 20) & 0xFF;
			Current_Page_Index[1] = (Op_Data_Addr >> 12) & 0xFF;

			// 存入Index新页地址 2Bytes
			KH25_Write_WithNoRead(Op_Index_Addr, Current_Page_Index, 2);

			// 移动Index指针到下一可用地址
			Op_Index_Addr += 2;
			// 判断Index页是否写满
			if (((Op_Index_Addr >> 0) & 0x0FFF) == 0)
			{
				KH25_Erase(4, Page_Index);
				Op_Index_Addr = (Page_Index << 12) & 0xFFF000;
			}
		}
		// 延时1min
		nrf_delay_ms(10);
		i--;
	}
}
// end of void Store_History_Data(void)

/*******************************************************************************
*                           王宇@2017-04-14
* Function Name  :  Get_Current_Data
* Description    :  获取当前数据 16Bytes 8Bytes数据 + 2Bytes校验 + 6Bytes冗余
* Input          :  None
* Output         :  None
* Return         :  成功1 失败0
*******************************************************************************/
void Get_Current_Data(u8* Data_Buff)
{
	u8 i;

	PCF8563_Get_Date(&Date_Buff_s);
	
	*Data_Buff     = Date_Buff_s.Seconds;
	*(++Data_Buff) = Date_Buff_s.Minutes;
	*(++Data_Buff) = Date_Buff_s.Hours;
	*(++Data_Buff) = Date_Buff_s.Days;
	*(++Data_Buff) = Date_Buff_s.WeekDay;
	*(++Data_Buff) = Date_Buff_s.Months;
	*(++Data_Buff) = Date_Buff_s.Years;
	*(++Data_Buff) = 0x20;

	i = 2;
	while(i)
	{
		Data_Buff++;
		*Data_Buff = 0x01;
		i--;
	}
	i = 6;
	while(i)
	{
		Data_Buff++;
		*Data_Buff = 0xFF;
		i--;
	}
}

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/
