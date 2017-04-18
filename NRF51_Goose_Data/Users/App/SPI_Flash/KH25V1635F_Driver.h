/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  KH25V1635F_Driver.h
* Author             :  王宇
* CPU Type           :  nRF51
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  11/04/2017
* Description        :  KH25V1635F_Driver header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KH25V1635F_DRIVER_H
#define __KH25V1635F_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "Global.h"

/* Private define ------------------------------------------------------------*/
#define		KH25_PIN_CS			30
#define		KH25_PIN_MISO		24
#define		KH25_PIN_MOSI		28
#define		KH25_PIN_SCK		22
#define		KH25_PIN_WP			27
#define		KH25_PIN_HOLD		29

#define     tDP					15										// Maximum time for Enter DeepDown Mode max 10us
#define		tDPDD				35										// Minimum time for Exit DeepDown Mode once in DDM min 30us
#define		tCRDP				25										// Minimum time for Extie DDM operation min 20ns
#define		tRDP				50										// Minimum time for Recovery to StandBy Mode min 45us

#define		PageSize			0xFF									// 页大小

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{
    CMD_READ				=	0x03,
	CMD_FAST_READ			=	0x0B,
	CMD_PAGE_PROGRAM		=	0x02,
	CMD_SECTOR_ERASE		=	0x20,
	CMD_BLOCK32_ERASE		=	0x52,
	CMD_BLOCK64_ERASE		=	0xD8,
	CMD_CHIP_ERASE			=	0x60,
	CMD_RDSFDP				=	0x5A,
	CMD_WRITE_ENABLE		=	0x06,
	CMD_WRITE_DISABLE		=	0x04,
	CMD_READ_STATUS_REG		=	0x05,
	CMD_READ_CONFIG_REG		=	0x15,
	CMD_WRITE_STATUS_REG	=	0x01,
	CMD_READ_ID				=	0x9F,
	CMD_READ_EMS			=	0x90,
	CMD_DDM_ENTER			=	0xB9,
	CMD_DUMMY				=	0xFF,
}
KH25V1635FCMD ;

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void KH25_Flash_Init(void);
void KH25_GetID(u8* ManuID, u16* DeviID);
void KH25_Read_Data(u32 Addr, u8* DataBuffer, u32 DataLen);
void KH25_WaiteForWriteEnd(void);
void KH25_Erase(u8 Erase_Size, u32 Addr);
void KH25_Chip_Erase(void);
void KH25_Chip_Init(void);
void KH25_Write_WithNoRead(u32 Addr, u8* DataBuff, u32 Data_Len);

#endif /* __KH25V1635F_DIVER_H */

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/
