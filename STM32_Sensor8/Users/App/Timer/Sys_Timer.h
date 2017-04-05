/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  Sys_timer.h
* Author             :  ����
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
extern void Timer_Init(void);											// ��������ʼ��
extern void Timer_Handler(void);										// �жϷ�����
extern void System_Delay_us(u16 Delay_Time);							// ϵͳ��ʱ����
extern void Sys_Timer_On(void);
extern void Sys_Timer_Off(void);

#endif /* __SYS_TIMER_H */

/******************* (C) COPYRIGHT 2017 ���� *****END OF FILE******************/

