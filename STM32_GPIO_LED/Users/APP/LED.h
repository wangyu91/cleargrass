/*********************** @copy right ���� **************************************
* �ļ���: Led.h
* ����  : led Ӧ�ú�����
* CPU	: STM32f103
* ����  : IAR 7.8x
* ����  : 20/03/2017
*******************************************************************************/
/* Ifndef --------------------------------------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Private Includes ----------------------------------------------------------*/
#include "stm32f10x.h"

/* Private Defines -----------------------------------------------------------*/
#define LED1(LED_STATE) if (LED_STATE) \
										GPIO_SetBits(GPIOB, GPIO_Pin_0);		\
									else	\
										GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define LED3(LED_STATE) if (LED_STATE) \
										GPIO_SetBits(GPIOF, GPIO_Pin_8);		\
									else	\
										GPIO_ResetBits(GPIOF, GPIO_Pin_8)
#define LED2(LED_STATE) if (LED_STATE) \
										GPIO_SetBits(GPIOF, GPIO_Pin_7);		\
									else	\
										GPIO_ResetBits(GPIOF, GPIO_Pin_7)
										
/* Private Variables ---------------------------------------------------------*/
#define LED_ON  1
#define LED_OFF 0

/* Private Typedef -----------------------------------------------------------*/

/* Private Function Prototype ------------------------------------------------*/
void LED_GPIO_Config(void);
									
#endif

/************************** End of File ***************************************/
