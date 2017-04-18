/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  PCF8563_Driver.c
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  06/04/2017
* Description        :  PCF8563 RTC应用程序
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "PCF8563_Driver.h"
#include "SW_IIC.h"
#include "nrf_delay.h"
#include "Global.h"

/* Private variables ---------------------------------------------------------*/
        
/* Private function prototypes -----------------------------------------------*/
u8 PCF8563_Read_Register(u8 Reg_Addr, u8* pBuffer, u16 usRead_Len);
u8 PCF8563_Write_Register(u8 Reg_Addr, u8* pBuffer, u16 usWrite_Len);

u8 PCF8563_Get_Date(Date_t* Date_s);									// 获取当前时间
u8 PCF8563_Set_Date(Date_t* Date_s);									// 设置当前时间

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           王宇@2017-04-06
* Function Name  :  PCF8563_Read_Register
* Description    :  读寄存器
* Input          :  u8*			Reg_Addr	寄存器地址
*					u8*			pBuffer		数据缓存
*					u16			usRead_Len  数据长度
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 PCF8563_Read_Register(u8 Reg_Addr, u8* pBuffer, u16 usRead_Len)
{
	u8 Succeed = 1;

	if (usRead_Len == 0)
	{
		return 1;
	}

	Succeed &= SW_IIC_Transfer(&Goose_IIC_s, PCF8563_WRITE_ADDR, &Reg_Addr, 1, SW_IIC_NEED_SEND_STOP);
	Succeed &= SW_IIC_Transfer(&Goose_IIC_s, PCF8563_READ_ADDR, pBuffer, usRead_Len, SW_IIC_NEED_SEND_STOP);

	nrf_delay_ms(100);

	return Succeed;
}
// end of u8 PCF8563_Read_Register(u8 Reg_Addr, u8* pBuffer, u16 usRead_Len)

/*******************************************************************************
*                           王宇@2017-04-06
* Function Name  :  PCF8563_Write_Register
* Description    :  写寄存器
* Input          :  u8			Reg_Addr	寄存器地址
*					u8*			pBuffer		数据缓存
*					u16			usWrite_Len 数据长度
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 PCF8563_Write_Register(u8 Reg_Addr, u8* pBuffer, u16 usWrite_Len)
{
	u8 Succeed = 1;

	Succeed &= SW_IIC_Transfer(&Goose_IIC_s, PCF8563_WRITE_ADDR, &Reg_Addr, 1, SW_IIC_DONT_SEND_STOP);

	if (usWrite_Len == 0)
	{
		return 0;
	}

	while (usWrite_Len-- && Succeed)
	{
		Succeed &= SW_IIC_Write_Byte(&Goose_IIC_s, *pBuffer);
		pBuffer++;
	}
	
	nrf_delay_ms(100);

	return Succeed;
}
// end of u8 PCF8563_Write_Register(u8 Reg_Addr, u8* pBuffer, u16 usWrite_Len)

/*******************************************************************************
*                           王宇@2017-04-06
* Function Name  :  PCF8563_Get_Date
* Description    :  获取日期
* Input          :  Date*	Date_s	日期存储结构体
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 PCF8563_Get_Date(Date_t* Date_s)
{
	u8 Result = 1;
	u8 Dates[7];

	// 读7字节 Reg2 ~ Reg8
	Result &= PCF8563_Read_Register(VL_SECONDS, Dates, 7);

	Date_s->Seconds = Dates[0];
	Date_s->Minutes = Dates[1];
	Date_s->Hours	= Dates[2];
	Date_s->Days	= Dates[3];
	Date_s->WeekDay = Dates[4];
	Date_s->Months  = Dates[5];
	Date_s->Years	= Dates[6];
	
	return Result;
}
// end of u8 PCF8563_Get_Date(Date_t* Date_s)

/*******************************************************************************
*                           王宇@2017-04-06
* Function Name  :  PCF8563_Set_Date
* Description    :  设置日期
* Input          :  Date*	Date_s	日期存储结构体
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 PCF8563_Set_Date(Date_t* Date_s)
{
	u8 Result = 1;
	u8 Dates[7];

	Dates[0] = Date_s->Seconds;
	Dates[1] = Date_s->Minutes;
	Dates[2] = Date_s->Hours;
	Dates[3] = Date_s->Days;
	Dates[4] = Date_s->WeekDay;
	Dates[5] = Date_s->Months;
	Dates[6] = Date_s->Years;

	Result &= PCF8563_Write_Register(VL_SECONDS, Dates, 7);
	
	return Result;
}
//end of u8 PCF8563_Set_Date(Date_t* Date_s)

/******************* (C) COPYRIGHT 2017 王宇 **************END OF FILE*********/
