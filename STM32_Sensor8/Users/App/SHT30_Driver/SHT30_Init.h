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
#ifndef __SHT30_INIT_H
#define __SHT30_INIT_H

/* Includes ------------------------------------------------------------------*/
#include "Global.h"

/* Private define ------------------------------------------------------------*/
#define     SHT30_ADDR                  0x44                            // SHT30��ַ
#define     SHT30_WRITE_ADDR            (SHT30_ADDR << 1)               // SHT30д��ַ
#define     SHT30_READ_ADDR             (SHT30_WRITE_ADDR | 0x01)       // SHT30����ַ
#define     TEMP_HUMI_MAX_TIMEOUT_LOOPS (5 * 1000)                      // ��ʱ����us

#define		SHT30_GPIO_RCC				RCC_APB2Periph_GPIOG
#define		SHT30_SDA					GPIO_Pin_0
#define		SHT30_SCL					GPIO_Pin_1
#define		SHT30_GPIO					GPIOG
#define		SHT30_TIMEOUT				(5 * 1000)

/* Private typedef -----------------------------------------------------------*/
// Sensor Commands

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern  void SHT30_Variable_Init(void);                             	// ��ʪ�ȱ�����ʼ��
extern  u8 SHT30_Init(void);                                   			// �ܽų�ʼ��
extern  u8 SHT30_Read_Register(u16 usRead_Addr, u8* pBuffer, u16 usRead_Len);// ���Ĵ���
extern  u8 SHT30_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len);// д�Ĵ���

#endif /* __SHT30_INIT_H */

/******************* (C) COPYRIGHT 2017 ���� *****END OF FILE******************/

