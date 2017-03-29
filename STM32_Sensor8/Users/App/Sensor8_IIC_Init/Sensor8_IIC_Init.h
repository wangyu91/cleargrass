/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  SHT30_Init.h
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  23/03/2017
* Description        :  Temp_Humi_Port header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSOR8_IIC_INIT_H
#define __SENSOR8_IIC_INIT_H

/* Includes ------------------------------------------------------------------*/
#include "Global.h"

/* Private define ------------------------------------------------------------*/
#define     SHT30_ADDR                  0x44                            // SHT30��ַ
#define     SHT30_WRITE_ADDR            (SHT30_ADDR << 1)               // SHT30д��ַ
#define     SHT30_READ_ADDR             (SHT30_WRITE_ADDR | 0x01)       // SHT30����ַ
#define     SENSOR8_TIMEOUT 			(5 * 1000)                      // ��ʱ����us

#define 	Sensor8_IIC_GPIO_RCC		RCC_APB2Periph_GPIOG			// G��ܽ�
#define 	Sensor8_IIC_SDA				GPIO_Pin_6						// SDA0 PG6
#define 	Sensor8_IIC_SCL				GPIO_Pin_7						// SCL0 PG7
#define 	Sensor8_GPIO				GPIOG
/* Private typedef -----------------------------------------------------------*/
// Sensor Commands

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern  void SHT30_Variable_Init(void);                             	// ��ʪ�ȱ�����ʼ��
extern  u8 Sensor8_IIC_Init(void);                                   	// �ܽų�ʼ��

#endif /* __SHT30_INIT_H */

/******************* (C) COPYRIGHT 2017 ���� *****END OF FILE******************/
