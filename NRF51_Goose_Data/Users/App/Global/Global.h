/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  Global.h
* Author             :  王宇
* CPU Type           :  nRF51
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  11/04/2017
* Description        :  Global header file
*******************************************************************************/
/* Ifndef --------------------------------------------------------------------*/
#ifndef __GLOBAL_H
#define __GLOBAL_H
/* Includes ------------------------------------------------------------------*/
#include "Nrf_drv_spi.h"
#include "Global_Typedef.h"
#include "app_error.h"
#include "SW_IIC.h"
#include "PCF8563_Driver.h"

/* Private Defines -----------------------------------------------------------*/
#define		SPI_INSTANCE		0										// SPI实体0

/* Private Variables ---------------------------------------------------------*/
extern const nrf_drv_spi_t  spi;										// SPI实体
extern SW_IIC_t				Goose_IIC_s;								// IIC结构体
extern Date_t 				Date_Buff_s;								// RTC结构体
/* Private Typedefs ----------------------------------------------------------*/

/* Private Function Prototypes -----------------------------------------------*/


#endif /* __GLOBAL_H */

/******************* (C) COPYRIGHT 2017 王宇 ********END OF FILE***************/
