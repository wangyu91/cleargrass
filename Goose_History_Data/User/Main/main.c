/******************** (C) COPYRIGHT 2016 王宇 **********************************
* File Name          :  Main.c
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  21/03/2017
* Description        :  main函数
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Global.h"
#include "stm32f10x.h"
#include "SW_IIC.h"
#include "bsp_usart1.h"

/* Private Variables ---------------------------------------------------------*/

/* Private Typedef -----------------------------------------------------------*/

/* Private Function Prototype ------------------------------------------------*/

/* Private Functions ---------------------------------------------------------*/
/*******************************************************************************
*                           王宇@2017-03-29
* Function Name  :  main
* Description    :  
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void main()
{
	// 配置uart通信
	USART1_Config();
	printf("\r\n  test!\r\n");	

	Goose_IIC_Init();

	Date_t Dnow, Dchange;
	u8	   Succeed = 1;

	while(1)
	{
	Succeed = PCF8563_Get_Date(&Dnow);
//	printf("\r\n  Succeed = %d\r\n", Succeed);
	printf("PCF8563 RTC year%x mon/%x day/%x hour%x min%x sec%x\r\n",Dnow.Years, Dnow.Months, Dnow.Days, Dnow.Hours, Dnow.Minutes, Dnow.Seconds);

	Dchange = Dnow;
	Dchange.Years = 0x17;
	Succeed = PCF8563_Set_Date(&Dchange);
	}
}

/******************* (C) COPYRIGHT 2017 王宇 **************END OF FILE*********/
