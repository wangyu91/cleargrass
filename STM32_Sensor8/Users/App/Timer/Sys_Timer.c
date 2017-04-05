/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  Sys_Timer.c
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  01/04/2017
* Description        :  STM32定时器程序
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
*                           王宇@2017-04-01
* Function Name  :  Timer_Init
* Description    :  定时器初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Timer_Init(void)
{
	// 配置计数器 检查是否有错 演示1ms
	if (SysTick_Config(SystemCoreClock / SYSTEM_FREQUENCY_1ms))			
	{
		// 错误进入死循环
		while(1);														
	}

	// 关闭计数器
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE;								
}
// end of void Timer_Init(void)

/*******************************************************************************
*                           王宇@2017-04-01
* Function Name  :  Timer_Handler
* Description    :  中断服务处理函数
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

	// 测试延时准确
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
*                           王宇@2017-04-01
* Function Name  :  System_Delay_us
* Description    :  系统延时 微妙
* Input          :  u16		Delay_Time	延时时间
* Output         :  None
* Return         :  None
*******************************************************************************/
void System_Delay_us(u16 Delay_Time)
{
	System_Time_Delay = Delay_Time;		
	
	// 开启计数器
	SysTick->CTRL |=  SysTick_CTRL_ENABLE;								

	// 延时
	while(System_Time_Delay != 0);										
}
// end of void System_Delay_us(u16 Delay_Time)
/*******************************************************************************
*                           王宇@2017-04-01
* Function Name  :  Timer_On
* Description    :  开启计数器
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sys_Timer_On(void)
{
	// 开启计数器
	SysTick->CTRL |=  SysTick_CTRL_ENABLE;	
}

/*******************************************************************************
*                           王宇@2017-04-01
* Function Name  :  Timer_Off
* Description    :  开启计数器
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sys_Timer_Off(void)
{
	// 关闭计数器
	SysTick->CTRL &=  ~ SysTick_CTRL_ENABLE;	
}

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/
