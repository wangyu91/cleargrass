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

//	SW_IIC_t *init_s[1];
//	init_s[0] = &LED_t;
//	init_s[1] = &SHT30_s;
//	printf("\r\n  s1 = %X, s2 = %x", init_s[0],init_s[1]);

//	u8 SHT30_SN[8];
	u8 i;
//	
	float ftemp, fhumi;
	u8 Succeed[8];
//	Succeed = LED_Init();
//	Succeed = SHT30_Init();
//	printf("\r\n  Succeed Init = %d\r\n", Succeed);
//	
//	LED_SCL_Low();
//	LED_SDA_Low();

//	Succeed = SHT30_Soft_Reset();
//	printf("\r\n  Succeed Reset = %d\r\n", Succeed);
//	Succeed = SHT30_Read_SN(SHT30_SN);									// 读取SN

//	for(i = 0; i < 4; i++)
//	{
//		printf("\r\n  SN = %x\r\n", SHT30_SN[i]);						// 打印SN
//	}
//	
//	printf("\r\n  Succeed read sn = %d\r\n", Succeed);

	Succeed[0] = Sensor8_IIC_Init();
	//Succeed[1] = Sensor8_IIC1_Init();
	Succeed[2] = Sensor8_IIC2_Init();
	Succeed[3] = Sensor8_IIC3_Init();
	Succeed[4] = Sensor8_IIC4_Init();
	Succeed[5] = Sensor8_IIC5_Init();
	Succeed[6] = Sensor8_IIC6_Init();
	Succeed[7] = Sensor8_IIC7_Init();

	for (i = 0; i < 8; i++)
	{
		printf("\r\n  Succeed Init IIC[%d] sn = %d\r\n", i, Succeed[i]);
	}
	
	while (1)
	{
		
		//Succeed = SHT30_Get_Data(&ftemp, &fhumi);						// 读取数据
		Sensor8_IIC_Get_Data(&Sensor8_IIC_s, &ftemp, &fhumi);						// 读取数据
		STM32_Delay_ms(100);
		printf("\r\n  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

		//Sensor8_IIC_Get_Data(&Sensor8_IIC1_s, &ftemp, &fhumi);						// 读取数据
		STM32_Delay_ms(100);
		printf("\r\n  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

		Sensor8_IIC_Get_Data(&Sensor8_IIC2_s, &ftemp, &fhumi);						// 读取数据
		STM32_Delay_ms(100);
		printf("\r\n  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

		Sensor8_IIC_Get_Data(&Sensor8_IIC3_s, &ftemp, &fhumi);						// 读取数据
		STM32_Delay_ms(100);
		printf("\r\n  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

		Sensor8_IIC_Get_Data(&Sensor8_IIC4_s, &ftemp, &fhumi);						// 读取数据
		STM32_Delay_ms(100);
		printf("\r\n  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

		Sensor8_IIC_Get_Data(&Sensor8_IIC5_s, &ftemp, &fhumi);						// 读取数据
		STM32_Delay_ms(100);
		printf("\r\n  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

		Sensor8_IIC_Get_Data(&Sensor8_IIC6_s, &ftemp, &fhumi);						// 读取数据
		STM32_Delay_ms(100);
		printf("\r\n  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

		Sensor8_IIC_Get_Data(&Sensor8_IIC7_s, &ftemp, &fhumi);						// 读取数据
		STM32_Delay_ms(100);
		printf("\r\n  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);
	}
}
/********************** End of File *******************************************/
