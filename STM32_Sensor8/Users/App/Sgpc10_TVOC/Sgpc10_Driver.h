/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  SHT30_Driver.h
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  31/03/2017
* Description        :  Sgpc10_Driver header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SGPC10_DRIVER_H
#define __SGPC10_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "Global.h"

/* Private define ------------------------------------------------------------*/
#define 	SGPC10_ADDR					0x58
#define		SGPC10_WRITE_ADDR			(0x58 << 1)
#define		SGPC10_READ_ADDR			(SGPC10_WRITE_ADDR | 0x01)

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{
    CMD_MEASURE_RESISTANCE			=	0x2050,
	CMD_INIT_AIR_QUALITY			=	0x2003,
	CMD_MEASURE_AIR_QUALITY 		=	0x2008,
	CMD_MEASURE_TEST				=	0x20FD,
	CMD_GET_FEATURE_SET_VERSION		=	0x3624
}
Sgpc10CMD ;

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
//extern  u8 SHT30_Read_SN(u8 *ucSN);                                     // 读取SN
//extern  u8 SHT30_Soft_Reset(void);                                      // 软复位
extern  u8 Sgpc10_Get_Data(SW_IIC_t *IIC_s, u16 *Tvoc_Data, u16 *Co2_Data);// 读IIC数据
extern  u8 Sgpc10_Init_Air_Quality(SW_IIC_t * IIC_s);					// 初始化空气质量检测
extern  u8 Sgpc10_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u16 usRead_Addr, u8* pBuffer, u16 usRead_Len);// 读寄存器
//extern  u8 SHT30_IIC_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len);// 写寄存器

#endif /* __SHT30_H */

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/



