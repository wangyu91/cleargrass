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
void Sys_Init(void);													// 系统初始化
u8	 IIC8_Get_SHT30_Data(void);											// 获取SHT30数据
u8	 IIC8_Get_BH1721_Data(void);										// 获取BH1721数据

/* Private Functions ---------------------------------------------------------*/
/*******************************************************************************
*                           王宇@2017-03-29
* Function Name  :  Sys_Init
* Description    :  系统初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sys_Init(void)
{
	u8 Succeed[8];
	u8 i;
	
	// 初始化8条IIC
//	Succeed[0] = Sensor8_IIC_Init();
	Succeed[1] = Sensor8_IIC1_Init();
	Succeed[2] = Sensor8_IIC2_Init();
//	Succeed[3] = Sensor8_IIC3_Init();
//	Succeed[4] = Sensor8_IIC4_Init();
	Succeed[5] = Sensor8_IIC5_Init();
//	Succeed[6] = Sensor8_IIC6_Init();
//	Succeed[7] = Sensor8_IIC7_Init();
	
	for (i = 0; i < 8; i++)
	{
		printf("\r\n  Succeed Init IIC[%d]  %d\r\n", i, Succeed[i]);	// 打印初始化信息 成功1 失败0
	}

	STM32_Delay_us(5);

	// 初始化BH1721 DVI线
	GPIO_Pin_Config(RCC_APB2Periph_GPIOA, GPIO_Pin_3, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOA);
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
	
}
// end of void Sys_Init(void)

/*******************************************************************************
*                           王宇@2017-03-29
* Function Name  :  IIC8_Get_SHT30_Data
* Description    :  获取8条IIC上的SHT30数据
* Input          :  None
* Output         :  None
* Return         :  成功1 失败0
*******************************************************************************/
u8 IIC8_Get_SHT30_Data(void)
{
	u8		Succeed = 1;
	float   ftemp, fhumi;
//	
//	Succeed &= SHT30_IIC_Get_Data(&Sensor8_IIC_s, &ftemp, &fhumi);						// 读取数据
//	STM32_Delay_ms(20);
//	printf("\r\n IIC_0  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

	Succeed &= SHT30_IIC_Get_Data(&Sensor8_IIC1_s, &ftemp, &fhumi);						// 读取数据
	STM32_Delay_ms(20);
	printf("\r\n IIC_1  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

	Succeed &= SHT30_IIC_Get_Data(&Sensor8_IIC2_s, &ftemp, &fhumi);						// 读取数据
	STM32_Delay_ms(20);
	printf("\r\n IIC_2  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

//	Succeed &= SHT30_IIC_Get_Data(&Sensor8_IIC3_s, &ftemp, &fhumi);						// 读取数据
//	STM32_Delay_ms(20);
//	printf("\r\n IIC_3  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

//	Succeed &= SHT30_IIC_Get_Data(&Sensor8_IIC4_s, &ftemp, &fhumi);						// 读取数据
//	STM32_Delay_ms(20);
//	printf("\r\n IIC_4  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

	Succeed &= SHT30_IIC_Get_Data(&Sensor8_IIC5_s, &ftemp, &fhumi);						// 读取数据
	STM32_Delay_ms(20);
	printf("\r\n IIC_5  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

//	Succeed &= SHT30_IIC_Get_Data(&Sensor8_IIC6_s, &ftemp, &fhumi);						// 读取数据
//	STM32_Delay_ms(20);
//	printf("\r\n IIC_6  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

//	Succeed &= SHT30_IIC_Get_Data(&Sensor8_IIC7_s, &ftemp, &fhumi);						// 读取数据
//	STM32_Delay_ms(20);
//	printf("\r\n IIC_7  temperature = %f, humidity = %f !\r\n", ftemp, fhumi);

	return(Succeed);
	
}
// end of u8 IIC8_Get_SHT30_Data(void)

/*******************************************************************************
*                           王宇@2017-03-29
* Function Name  :  IIC8_Get_BH1721_Data
* Description    :  获取8条IIC上的BH1721数据
* Input          :  None
* Output         :  None
* Return         :  成功1 失败0
*******************************************************************************/
u8 IIC8_Get_BH1721_Data(void)
{
	u8		Succeed = 1;
	float   fLight[8];
	
//	Succeed &= BH1721_Get_Data(&Sensor8_IIC_s, &fLight[0]);						// 读取数据
//	STM32_Delay_ms(20);
//	printf("\r\n  IIC0 Light Intensity = %f !\r\n", fLight[0]);

	Succeed &= BH1721_Get_Data(&Sensor8_IIC1_s, &fLight[1]);						// 读取数据
	STM32_Delay_ms(20);
	printf("\r\n  IIC1 Light Intensity = %f !\r\n", fLight[1]);

	Succeed &= BH1721_Get_Data(&Sensor8_IIC2_s, &fLight[2]);						// 读取数据
	STM32_Delay_ms(20);
	printf("\r\n  IIC2 Light Intensity = %f !\r\n", fLight[2]);

//	Succeed &= BH1721_Get_Data(&Sensor8_IIC3_s, &fLight[3]);						// 读取数据
//	STM32_Delay_ms(20);
//	printf("\r\n  IIC3 Light Intensity = %f !\r\n", fLight[3]);

//	Succeed &= BH1721_Get_Data(&Sensor8_IIC4_s, &fLight[4]);						// 读取数据
//	STM32_Delay_ms(20);
//	printf("\r\n  IIC4 Light Intensity = %f !\r\n", fLight[4]);

	Succeed &= BH1721_Get_Data(&Sensor8_IIC5_s, &fLight[5]);						// 读取数据
	STM32_Delay_ms(20);
	printf("\r\n  IIC5 Light Intensity = %f !\r\n", fLight[5]);

//	Succeed &= BH1721_Get_Data(&Sensor8_IIC6_s, &fLight[6]);						// 读取数据
//	STM32_Delay_ms(20);
//	printf("\r\n  IIC6 Light Intensity = %f !\r\n", fLight[6]);

//	Succeed &= BH1721_Get_Data(&Sensor8_IIC7_s, &fLight[7]);						// 读取数据
//	STM32_Delay_ms(20);
//	printf("\r\n  IIC7 Light Intensity = %f !\r\n", fLight[7]);
	
	return(Succeed);
	
}
// end of u8 IIC8_Get_BH1721_Data(void)

/*******************************************************************************
*                           王宇@2017-03-29
* Function Name  :  main
* Description    :  系统初始化 读取8条线上的数据并检测是否有误
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void main()
{
	u8 Succeed;
	
	USART1_Config();
	printf("\r\n  test!\r\n");											// 测试UART通信

	Sys_Init();

	#ifdef IIC8_SHT30
		Succeed = IIC8_Get_SHT30_Data();

		if (Succeed == 0)
		{
			printf("\r\n  error in some IIC !\r\n");
		}
		
	#endif

	#ifdef IIC8_BH1721
		Succeed = IIC8_Get_BH1721_Data();

		if (Succeed == 0)
		{
			printf("\r\n  error in some IIC !\r\n");
		}
		
	#endif
	
	while (1)
	{
//		IIC8_Get_SHT30_Data();
//		IIC8_Get_BH1721_Data();
	}
}

/******************* (C) COPYRIGHT 2017 王宇 **************END OF FILE*********/
