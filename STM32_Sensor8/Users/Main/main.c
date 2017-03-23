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

/* Private Variables ---------------------------------------------------------*/

/* Private Typedef -----------------------------------------------------------*/

/* Private Function Prototype ------------------------------------------------*/
void Delay(__IO u32 nCount);
/* Private Functions ---------------------------------------------------------*/
void Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
}

void main()
{
//	GPIO_Pin_Config(RCC_APB2Periph_GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOB);
//	
//	GPIO_SetBits(GPIOB, GPIO_Pin_0);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
//	Delay(0xFFFEFF);
//	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	u8 SHT30_SN;
	u8 Succeed;
	Succeed = LED_Init();
	Succeed = SHT30_Init();
	
	LED_SCL_Low();
	LED_SDA_Low();

	Succeed = 1;
	
	while (Succeed)
	{
		Succeed = SHT30_Read_SN(&SHT30_SN);
		LED_SDA_High();													// LED_3点亮
	}
}
/********************** End of File *******************************************/