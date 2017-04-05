/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  Sys_Timer.c
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  01/04/2017
* Description        :  STM32��ʱ������
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Sys_Timer.h"
#include "Stm32f10x_rcc.h"

/* Private Variables ---------------------------------------------------------*/

/* Private Function Prototypes -----------------------------------------------*/
void Timer_Init(void);
void Timer_Handler(void);
void System_Delay_us(u16 Delay_Time);
void Sys_Timer_On(void);
void Sys_Timer_Off(void);

/* Private Functions ---------------------------------------------------------*/
/*******************************************************************************
*                           ����@2017-04-01
* Function Name  :  Timer_Init
* Description    :  ��ʱ����ʼ��
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Timer_Init(void)
{
	// ���ü����� ����Ƿ��д� ��ʾ1ms
	if (SysTick_Config(SystemCoreClock / SYSTEM_FREQUENCY_1ms))			
	{
		// ���������ѭ��
		while(1);														
	}

	// �رռ�����
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE;								
}
// end of void Timer_Init(void)

/*******************************************************************************
*                           ����@2017-04-01
* Function Name  :  Timer_Handler
* Description    :  �жϷ�������
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Timer_Handler(void)
{
//	if (System_Time_Delay != 0)
//	{
//		System_Time_Delay--;
//	}

	System_Time_Delay++;

	// ������ʱ׼ȷ
    if((System_Time_Delay % 2) == 1)
        {
            GPIO_ResetBits(GPIOD, GPIO_Pin_4);
        }
        else
        {
            GPIO_SetBits(GPIOD, GPIO_Pin_4);
        }
}

/*******************************************************************************
*                           ����@2017-04-01
* Function Name  :  System_Delay_us
* Description    :  ϵͳ��ʱ ΢��
* Input          :  u16		Delay_Time	��ʱʱ��
* Output         :  None
* Return         :  None
*******************************************************************************/
void System_Delay_us(u16 Delay_Time)
{
	System_Time_Delay = Delay_Time;		
	
	// ����������
	SysTick->CTRL |=  SysTick_CTRL_ENABLE;								

	// ��ʱ
	while(System_Time_Delay != 0);										
}
// end of void System_Delay_us(u16 Delay_Time)
/*******************************************************************************
*                           ����@2017-04-01
* Function Name  :  Timer_On
* Description    :  ����������
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sys_Timer_On(void)
{
	// ����������
	SysTick->CTRL |=  SysTick_CTRL_ENABLE;	
}

/*******************************************************************************
*                           ����@2017-04-01
* Function Name  :  Timer_Off
* Description    :  ����������
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sys_Timer_Off(void)
{
	// �رռ�����
	SysTick->CTRL &=  ~ SysTick_CTRL_ENABLE;	
}

/******************* (C) COPYRIGHT 2017 ���� *****END OF FILE******************/
