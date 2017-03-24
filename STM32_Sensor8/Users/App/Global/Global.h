/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  Global.h
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  21/03/2017
* Description        :  Global header file
*******************************************************************************/
/* Ifndef --------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "SW_IIC.h"
#include "SHT30_Init.h"
#include "SHT30_Driver.h"
//#include "GPIO_LED.h"

/* Private Defines -----------------------------------------------------------*/
#define SHT30_SDA			GPIO_Pin_7									// SHT30 SDA��
#define SHT30_SCL			GPIO_Pin_6									// SHT30 SCL��
#define SHT30_GPIO_RCC		RCC_APB2Periph_GPIOA						// SHT30ʱ������
#define SHT30_GPIO			GPIOA										// SHT30�ܽ���
#define SHT30_TIMEOUT		(5 * 1000)									// ��ʱ
/* Private Variables ---------------------------------------------------------*/
extern SW_IIC_t LED_t;
extern SW_IIC_t SHT30_s;												// SHT30 �ṹ��

/* Private Typedefs ----------------------------------------------------------*/

/* Private Function Prototypes -----------------------------------------------*/
extern void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx);	// �ܽų�ʼ��

extern void STM32_Delay_us(uint16_t time_us);							// ����ʱ ΢��
extern void STM32_Delay_ms(uint16_t time_ms);							// ����

static void IIC_Delay(unsigned char count);								// IIC��ʱ

/* __GLOBAL_H */

/******************* (C) COPYRIGHT 2017 ���� ********END OF FILE***************/
