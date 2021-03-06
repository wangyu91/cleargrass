/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  SHT30_Init5.h
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  27/03/2017
* Description        :  Temp_Humi_Port header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSOR8_IIC_INIT5_H
#define __SENSOR8_IIC_INIT5_H

/* Includes ------------------------------------------------------------------*/
#include "Global.h"

/* Private define ------------------------------------------------------------*/
#define     SHT30_ADDR                  0x44                            // SHT30��ַ
#define     SHT30_WRITE_ADDR            (SHT30_ADDR << 1)               // SHT30д��ַ
#define     SHT30_READ_ADDR             (SHT30_WRITE_ADDR | 0x01)       // SHT30����ַ
#define     SENSOR8_TIMEOUT 			(5 * 1000)                      // ��ʱ����us

#define		Sensor8_IIC5_GPIO_RCC		RCC_APB2Periph_GPIOG			// SDA5 G15
#define		Sensor8_IIC5_GPIO_RCC_SCL	RCC_APB2Periph_GPIOB			// SCL5 B5
#define 	Sensor8_IIC5_SDA			GPIO_Pin_15
#define 	Sensor8_IIC5_SCL			GPIO_Pin_5
#define 	Sensor8_GPIO5				GPIOG
#define 	Sensor8_GPIO5_SCL			GPIOB
/* Private typedef -----------------------------------------------------------*/
// Sensor Commands

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
//extern  void SHT30_Variable_Init(void);                             	// ��ʪ�ȱ�����ʼ��
extern  u8 Sensor8_IIC5_Init(void);                                   	// �ܽų�ʼ��
//extern  u8 Sensor8_IIC_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u16 usRead_Addr, u8* pBuffer, u16 usRead_Len);// ���Ĵ���
//extern  u8 Sensor8_IIC_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len);// д�Ĵ���

#endif /* __SENSOR8_IIC_INIT5_H */

/******************* (C) COPYRIGHT 2017 ���� *****END OF FILE******************/

