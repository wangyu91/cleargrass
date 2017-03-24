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
#include "GPIO_LED.h"
#include "bsp_usart1.h"

/* Private Variables ---------------------------------------------------------*/

/* Private Typedef -----------------------------------------------------------*/

/* Private Function Prototype ------------------------------------------------*/

/* Private Functions ---------------------------------------------------------*/
void main()
{

	USART1_Config();
	printf("\r\n  test!\r\n");

	u8 SHT30_SN[8];
	u8 i;
	
	float ftemp, fhumi;
	u8 Succeed;
//	Succeed = LED_Init();
	Succeed = SHT30_Init();
	
//	LED_SCL_Low();
//	LED_SDA_Low();

	Succeed = SHT30_Soft_Reset();
	printf("\r\n  Succeed Reset = %d\r\n", Succeed);
	Succeed = SHT30_Read_SN(SHT30_SN);									// 读取SN

	for(i = 0; i < 4; i++)
	{
		printf("\r\n  SN = %x\r\n", SHT30_SN[i]);						// 打印SN
	}
	
	printf("\r\n  Succeed read sn = %d\r\n", Succeed);
	
	while (Succeed)
	{
		Succeed = SHT30_Get_Data(&ftemp, &fhumi);						// 读取数据
		STM32_Delay_ms(1000);
		//LED_SDA_High();												// LED_3点亮
		printf("\r\n  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);
	}
}
/********************** End of File *******************************************/