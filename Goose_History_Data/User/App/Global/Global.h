/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  Global.h
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  06/04/2017
* Description        :  Global header file
*******************************************************************************/
/* Ifndef --------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "SW_IIC.h"
#include "PCF8563_Driver.h"

/* Private Defines -----------------------------------------------------------*/

/* Private Variables ---------------------------------------------------------*/
extern SW_IIC_t Goose_IIC_s;											// IIC��ʼ���ṹ��

extern u16		System_Time_Delay;										// ϵͳ��ʱ����
/* Private Typedefs ----------------------------------------------------------*/

/* Private Function Prototypes -----------------------------------------------*/
extern void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx);	// �ܽų�ʼ��

extern void STM32_Delay_us(uint16_t time_us);							// ����ʱ ΢��
extern void STM32_Delay_ms(uint16_t time_ms);							// ����

//static void IIC_Delay(unsigned char count);								// IIC��ʱ

/* __GLOBAL_H */

/******************* (C) COPYRIGHT 2017 ���� ********END OF FILE***************/
