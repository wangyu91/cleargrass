/******************** (C) COPYRIGHT 2016 Õı”Ó **********************************
* File Name          :  Main.c
* Author             :  Õı”Ó
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  21/03/2017
* Description        :  main∫Ø ˝
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Global.h"
#include "stm32f10x.h"

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
	GPIO_Pin_Config(RCC_APB2Periph_GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOB);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	Delay(0xFFFEFF);
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}
/********************** End of File *******************************************/