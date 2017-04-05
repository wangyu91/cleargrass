/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  Sys_timer.h
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  01/04/2017
* Description        :  Sys_timer header file
*******************************************************************************/

/* Ifndef --------------------------------------------------------------------*/
#ifndef __SYS_TIMER_H
#define __SYS_TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "Global.h"

/* Private define ------------------------------------------------------------*/
#define		SYSTEM_FREQUENCY_1us		1000000
#define		SYSTEM_FREQUENCY_10us		100000
#define		SYSTEM_FREQUENCY_1ms		1000
/* Private typedefs ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern void Timer_Init(void);											// 计数器初始化
extern void Timer_Handler(void);										// 中断服务函数
extern void System_Delay_us(u16 Delay_Time);							// 系统延时函数
extern void Sys_Timer_On(void);
extern void Sys_Timer_Off(void);

#endif /* __SYS_TIMER_H */

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/

