/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  BH1721_Driver.h
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  28/03/2017
* Description        :  BH1721_Driver header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BH1721_DRIVER_H
#define __BH1721_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "Global.h"
//#include "SHT30_Init.h"

/* Private define ------------------------------------------------------------*/
#define BH1721_ADDR			0x23										// BH172 Address

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{
    CMD_Power_Down		=	0x00,										// No Active State
    CMD_Power_On		=	0x01,										// Waiting for Measurement State
    CMD_AutoResolution	=	0x10,										// Continuously Auto-Resolution Mode
    CMD_H_Resolution	=	0x12,										// Continuously H-Resolution Mode
    CMD_L_Resolution	=	0x13,										// Continuously L-Resolution Mode
//    CMD_Change_Time_Hbit	=	0b010_MT,								// Change Measurement High Bit
//    CMD_Change_Time_Lbit	=	0b011_MT								// Change Measurement Low Bit
}
opticalCMD ;

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
//extern  u8 SHT30_Read_SN(u8 *ucSN);                                     // 读取SN
//extern  u8 SHT30_Soft_Reset(void);                                      // 软复位
extern u8 BH1721_Get_Data(SW_IIC_t *IIC_s, float *fLight);				// 获取光强数据

#endif /* __BH1721_H */

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/
