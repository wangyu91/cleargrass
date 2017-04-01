/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  SHT30_Init2.h
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  27/03/2017
* Description        :  Temp_Humi_Port header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSOR8_IIC_INIT2_H
#define __SENSOR8_IIC_INIT2_H

/* Includes ------------------------------------------------------------------*/
#include "Global.h"

/* Private define ------------------------------------------------------------*/
#define     SHT30_ADDR                  0x44                            // SHT30地址
#define     SHT30_WRITE_ADDR            (SHT30_ADDR << 1)               // SHT30写地址
#define     SHT30_READ_ADDR             (SHT30_WRITE_ADDR | 0x01)       // SHT30读地址
#define     SENSOR8_TIMEOUT 			(5 * 1000)                      // 超时周期us

#define		Sensor8_IIC2_GPIO_RCC		RCC_APB2Periph_GPIOD
#define 	Sensor8_IIC2_SDA			GPIO_Pin_7
#define 	Sensor8_IIC2_SCL			GPIO_Pin_6
#define 	Sensor8_GPIO2				GPIOD
/* Private typedef -----------------------------------------------------------*/
// Sensor Commands

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
//extern  void SHT30_Variable_Init(void);                             	// 温湿度变量初始化
extern  u8 Sensor8_IIC2_Init(void);                                   	// 管脚初始化
//extern  u8 Sensor8_IIC_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u16 usRead_Addr, u8* pBuffer, u16 usRead_Len);// 读寄存器
//extern  u8 Sensor8_IIC_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len);// 写寄存器

#endif /* __SENSOR8_IIC_INIT2_H */

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/
