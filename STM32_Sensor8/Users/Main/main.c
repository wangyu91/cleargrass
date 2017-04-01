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
//u8	 IIC8_Get_SHT30_Data(void);											// 获取SHT30数据
void IIC8_Test(void);

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
	Succeed[0] = Sensor8_IIC_Init();
	Succeed[1] = Sensor8_IIC1_Init();
	Succeed[2] = Sensor8_IIC2_Init();
	Succeed[3] = Sensor8_IIC3_Init();
	Succeed[4] = Sensor8_IIC4_Init();
	Succeed[5] = Sensor8_IIC5_Init();
	Succeed[6] = Sensor8_IIC6_Init();
	Succeed[7] = Sensor8_IIC7_Init();
	
	for (i = 0; i < 8; i++)
	{
		printf("\r\n  Succeed Init IIC[%d]  %d\r\n", i, Succeed[i]);	// 打印初始化信息 成功1 失败0
	}

	Sgpc10_Init_Air_Quality(&Sensor8_IIC_s);
	Sgpc10_Init_Air_Quality(&Sensor8_IIC1_s);
	Sgpc10_Init_Air_Quality(&Sensor8_IIC2_s);
	Sgpc10_Init_Air_Quality(&Sensor8_IIC3_s);
	Sgpc10_Init_Air_Quality(&Sensor8_IIC4_s);
	Sgpc10_Init_Air_Quality(&Sensor8_IIC5_s);
	Sgpc10_Init_Air_Quality(&Sensor8_IIC6_s);
	Sgpc10_Init_Air_Quality(&Sensor8_IIC7_s);

	STM32_Delay_us(5);

	// 初始化BH1721 DVI线
	GPIO_Pin_Config(RCC_APB2Periph_GPIOC, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOC);
	GPIO_SetBits(GPIOC, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);
	GPIO_Pin_Config(RCC_APB2Periph_GPIOB, GPIO_Pin_8 | GPIO_Pin_9, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOB);
	GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
	GPIO_Pin_Config(RCC_APB2Periph_GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOE);
	GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3);
}
// end of void Sys_Init(void)

/*******************************************************************************
*                           王宇@2017-03-29
* Function Name  :  IIC8_Test
* Description    :  测试8条IIC上的数据
* Input          :  None
* Output         :  None
* Return         :  成功1 失败0
*******************************************************************************/
void IIC8_Test(void)
{
	u8		Succeed[8];
	u16		TVOC[8], CO2[8];
	float   fLight[8];
	float   ftemp[8], fhumi[8];

#ifdef IIC8_BH1721
	Succeed[0] = BH1721_Get_Data(&Sensor8_IIC_s, &fLight[0]);						// 读取数据
	Succeed[1] = BH1721_Get_Data(&Sensor8_IIC1_s, &fLight[1]);
	Succeed[2] = BH1721_Get_Data(&Sensor8_IIC1_s, &fLight[2]);
	Succeed[3] = BH1721_Get_Data(&Sensor8_IIC1_s, &fLight[3]);
	Succeed[4] = BH1721_Get_Data(&Sensor8_IIC1_s, &fLight[4]);
	Succeed[5] = BH1721_Get_Data(&Sensor8_IIC1_s, &fLight[5]);
	Succeed[6] = BH1721_Get_Data(&Sensor8_IIC1_s, &fLight[6]);
	Succeed[7] = BH1721_Get_Data(&Sensor8_IIC1_s, &fLight[7]);
#endif
#ifdef IIC8_SHT30
	Succeed[0] = SHT30_IIC_Get_Data(&Sensor8_IIC_s, &ftemp[0], &fhumi[1]);
	Succeed[1] = SHT30_IIC_Get_Data(&Sensor8_IIC1_s, &ftemp[1], &fhumi[1]);
	Succeed[2] = SHT30_IIC_Get_Data(&Sensor8_IIC2_s, &ftemp[2], &fhumi[2]);
	Succeed[3] = SHT30_IIC_Get_Data(&Sensor8_IIC3_s, &ftemp[3], &fhumi[3]);
	Succeed[4] = SHT30_IIC_Get_Data(&Sensor8_IIC4_s, &ftemp[4], &fhumi[4]);
	Succeed[5] = SHT30_IIC_Get_Data(&Sensor8_IIC5_s, &ftemp[5], &fhumi[5]);
	Succeed[6] = SHT30_IIC_Get_Data(&Sensor8_IIC6_s, &ftemp[6], &fhumi[6]);
	Succeed[7] = SHT30_IIC_Get_Data(&Sensor8_IIC7_s, &ftemp[7], &fhumi[7]);
#endif
#ifdef IIC8_SGPC10
	Succeed[0] = Sgpc10_Get_Data(&Sensor8_IIC_s, &TVOC[0], &CO2[0]);
	Succeed[1] = Sgpc10_Get_Data(&Sensor8_IIC1_s, &TVOC[1], &CO2[1]);
	Succeed[2] = Sgpc10_Get_Data(&Sensor8_IIC2_s, &TVOC[2], &CO2[2]);
	Succeed[3] = Sgpc10_Get_Data(&Sensor8_IIC3_s, &TVOC[3], &CO2[3]);
	Succeed[4] = Sgpc10_Get_Data(&Sensor8_IIC4_s, &TVOC[4], &CO2[4]);
	Succeed[5] = Sgpc10_Get_Data(&Sensor8_IIC5_s, &TVOC[5], &CO2[5]);
	Succeed[6] = Sgpc10_Get_Data(&Sensor8_IIC6_s, &TVOC[6], &CO2[6]);
	Succeed[7] = Sgpc10_Get_Data(&Sensor8_IIC7_s, &TVOC[7], &CO2[7]);
#endif
	
	// 成功亮绿灯 失败红灯
	if (Succeed[0])
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOC, GPIO_Pin_6, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOC);
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	}
	else
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOG, RED0_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, RED0_GPIO);
		GPIO_ResetBits(RED0_GPIO, RED0_PIN);
	}
		
	if (Succeed[1])
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOA, GPIO_Pin_8, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOA);
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	}
	else
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOC, RED1_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, RED1_GPIO);
		GPIO_ResetBits(RED1_GPIO, RED1_PIN);
	}

	if (Succeed[2])
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOA, GPIO_Pin_12, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOA);
		GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	}
	else
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOA, RED2_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, RED2_GPIO);
		GPIO_ResetBits(RED2_GPIO, RED2_PIN);
	}

	if (Succeed[3])
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOD, GPIO_Pin_3, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOD);
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);
	}
	else
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOD, RED3_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, RED3_GPIO);
		GPIO_ResetBits(RED3_GPIO, RED3_PIN);
	}

	if (Succeed[4])
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOG, GPIO_Pin_14, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOG);
		GPIO_ResetBits(GPIOG, GPIO_Pin_14);
	}
	else
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOG, RED4_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, RED4_GPIO);
		GPIO_ResetBits(RED4_GPIO, RED4_PIN);
	}
	
	if (Succeed[5])
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOB, GPIO_Pin_7, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOB);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	}
	else
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOB, RED5_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, RED5_GPIO);
		GPIO_ResetBits(RED5_GPIO, RED5_PIN);
	}

	if (Succeed[6])
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOC, GPIO_Pin_15, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOC);
		GPIO_ResetBits(GPIOC, GPIO_Pin_15);
	}
	else
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOC, RED6_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, RED6_GPIO);
		GPIO_ResetBits(RED6_GPIO, RED6_PIN);
	}

	if (Succeed[7])
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOF, GPIO_Pin_3, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOF);
		GPIO_ResetBits(GPIOF, GPIO_Pin_3);
	}
	else
	{
		GPIO_Pin_Config(RCC_APB2Periph_GPIOF, RED7_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, RED7_GPIO);
		GPIO_ResetBits(RED7_GPIO, RED7_PIN);
	}
	
//	return(Succeed);
//	while(1)
//	{
//		Sgpc10_Get_Data(&Sensor8_IIC2_s, &TVOC[2], &CO2[2]);
//		printf("\r\n  TVOC = %d, CO2 = %d !\r\n", TVOC[2], CO2[2]);
//		STM32_Delay_ms(500);
//	}
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
	USART1_Config();
	printf("\r\n  test!\r\n");											// 测试UART通信

	Sys_Init();
	IIC8_Test();
		
	while(1);
}

/******************* (C) COPYRIGHT 2017 王宇 **************END OF FILE*********/
