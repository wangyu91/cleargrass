/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  KH25_Flash_Store.c
* Author             :  ����
* CPU Type           :  nRF51
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  14/04/2017
* Description        :  KH25V1635F FLASH �洢Ӧ�ó��� ��¼��ʷ���� RAM<2K
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "KH25_Flash_Store.h"
//#include "Global.h"
#include "nrf_delay.h"

/* Private variables ---------------------------------------------------------*/
        
/* Private function prototypes -----------------------------------------------*/
u16  Get_Page_Number(u32 Addr);											// ��ȡҳ��
u16  Get_Sector_Number(u32 Addr);										// ��ȡ������
u8   Page_Pos_Determine(u32 Addr);										// ҳλ���ж�
u16  SearchFor_Valid_Addr(u16 cPageAddr, u16* cAddr_Valid);				// ���ҿ��õ�ַ
void Store_History_Data(void);											// �洢��ʷ����
void Get_Current_Data(u8* Data_Buff);									// ��ȡ��ǰ����

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           ����@2017-04-14
* Function Name  :  Get_Page_Number
* Description    :  ��õ�ǰ����ҳ��
* Input          :  u32		Addr		������ʼ��ַ
* Output         :  None
* Return         :  u16		PageNumber	ҳ��
*******************************************************************************/
u16 Get_Page_Number(u32 Addr)
{
	u16 PageNumber = (Addr >> 12) & 0x0FFF;
	return PageNumber;
}
// end of u16 Get_Page_Number(u32 Addr)

/*******************************************************************************
*                           ����@2017-04-14
* Function Name  :  Get_Sector_Number
* Description    :  ��õ�ǰ����������
* Input          :  u32		Addr			������ʼ��ַ
* Output         :  None
* Return         :  u16		SectorNumber	������
*******************************************************************************/
u16 Get_Sector_Number(u32 Addr)
{
	u16 SectorNumber = (Addr >> 0) & 0x0FFF;
	return SectorNumber;
}
// end of u16 Get_Sector_Number(u32 Addr)

/*******************************************************************************
*                           ����@2017-04-14
* Function Name  :  Page_Pos_Determine
* Description    :  �ж�����λ��
* Input          :  u32		Addr			������ʼ��ַ
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
*                           ����@2017-04-14
* Function Name  :  SearchFor_Valid_Addr
* Description    :  ��ȡ���õ�ַ
* Input          :  u16 	cPageAddr	��ǰ����ҳ��
*					u16*	cAddr_Valid ��ǰҳ���õ�ַ
* Output         :  None
* Return         :  2Bytes���� ����ҳ->Ŀ��ҳ�� ����ҳ->У����
*******************************************************************************/
u16 SearchFor_Valid_Addr(u16 cPageAddr, u16* cAddr_Valid)
{
	u32 Current_Addr;
	u8  Current_Data[16];
	
	// ��ǰҳ��ʼ��ַ
	Current_Addr = (cPageAddr << 12) & 0xFFF000;
	// ������ݴ洢Buffer
	memset(Current_Data, 0, 16);

	// �ж��Ƿ�Ϊ����ҳ
	if (Page_Index == cPageAddr)
	{
		// �ж�ҳ��ַ��λ Byte[0]
		do
		{
			// ѭ����ȡ2Bytes���� ����δ�������ĵ�ַ
			KH25_Read_Data(Current_Addr, Current_Data, 2);
			*cAddr_Valid  = (Current_Addr >> 0) & 0x0FFF;
			Current_Addr += 2;
		}while (Current_Data[0] < 0xFF);
		
		// ����Ч����
		KH25_Read_Data(Current_Addr - 4, Current_Data, 2);
		// ����2BytesĿ��ҳ��
		return((Current_Data[0] << 8) + Current_Data[1]);
	}
	else
	{
		// �ж�У��λ��һ���ֽ� Byte[8] �� �����һ���ֽ� Byte[10]
		do
		{
			// ѭ����ȡ16Bytes 
			KH25_Read_Data(Current_Addr, Current_Data, 16);
			*cAddr_Valid  = (Current_Addr >> 0) & 0x0FFF;
			Current_Addr += 16;
		}while ((Current_Data[8] < 0xFF) && (Current_Data[10] == 0xFF));

		KH25_Read_Data(Current_Addr - 32, Current_Data, 16);
		// ����2BytesУ��λ
		return((Current_Data[8] << 8) + Current_Data[9]);
	}
}
// end of u16 SearchFor_Valid_Addr(u16 cPageAddr, u16 cAddr_Valid)

/*******************************************************************************
*                           ����@2017-04-14
* Function Name  :  Store_History_Data
* Description    :  �洢��ʷ���� ���1min
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Store_History_Data(void)
{
	u8  Current_Page_Index[2];											// ��ǰ����ҳ����Buffer
	u8  Current_DataBuff[16];
	u16 Page_Start;														// �洢��ʼҳ��
	u16 cData_Addr, cIndex_Addr;										// ҳ�ڵ�ǰ���õ�ַ
	u32 Op_Data_Addr, Op_Index_Addr;									// ��ǰ������ַ
	u8  i = 100;

	// ���Ҵ�����ʼҳ Indexҳ���õ�ַ
	Page_Start    = SearchFor_Valid_Addr(Page_Index, &cIndex_Addr);
	// ����Index�ɴ����ַ
	Op_Index_Addr = (Page_Index << 12) + cIndex_Addr;

	// �������ݴ洢ҳ���õ�ַ
	SearchFor_Valid_Addr(Page_Start, &cData_Addr);
	// �������ݿɴ洢��ַ
	Op_Data_Addr  = (Page_Start << 12) + cData_Addr;

	// �洢����
	while(i)
	{
		// ��ȡ��ǰ���� 16Bytes
		Get_Current_Data(Current_DataBuff);
		// �洢��ǰ����
		KH25_Write_WithNoRead(Op_Data_Addr, Current_DataBuff, 16);
		// �ƶ�ָ�뵽��һ��ַ 16Bytes
		Op_Data_Addr += 16;

		// �ж��Ƿ��ҳ
		if (((Op_Data_Addr >> 0) & 0x0FFF) == 0)
		{
			// ������ҳ��ַ
			Current_Page_Index[0] = (Op_Data_Addr >> 20) & 0xFF;
			Current_Page_Index[1] = (Op_Data_Addr >> 12) & 0xFF;

			// ����Index��ҳ��ַ 2Bytes
			KH25_Write_WithNoRead(Op_Index_Addr, Current_Page_Index, 2);

			// �ƶ�Indexָ�뵽��һ���õ�ַ
			Op_Index_Addr += 2;
			// �ж�Indexҳ�Ƿ�д��
			if (((Op_Index_Addr >> 0) & 0x0FFF) == 0)
			{
				KH25_Erase(4, Page_Index);
				Op_Index_Addr = (Page_Index << 12) & 0xFFF000;
			}
		}
		// ��ʱ1min
		nrf_delay_ms(10);
		i--;
	}
}
// end of void Store_History_Data(void)

/*******************************************************************************
*                           ����@2017-04-14
* Function Name  :  Get_Current_Data
* Description    :  ��ȡ��ǰ���� 16Bytes 8Bytes���� + 2BytesУ�� + 6Bytes����
* Input          :  None
* Output         :  None
* Return         :  �ɹ�1 ʧ��0
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

/******************* (C) COPYRIGHT 2017 ���� *****END OF FILE******************/
