/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  Goose_RTC_Service.h
* Author             :  王宇
* CPU Type           :  nRF51822
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  19/04/2017
* Description        :  RTC 服务 header file
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GOOSE_RTC_SERVICE_H
#define __GOOSE_RTC_SERVICE_H

/* Includes ------------------------------------------------------------------*/
#include "Global.h"
#include "ble_gatts.h"
/* Private define ------------------------------------------------------------*/
#define LBS_UUID_BASE        {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, \
                              0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
#define LBS_UUID_SERVICE     0x1523
#define LBS_UUID_BUTTON_CHAR 0x1524
#define LBS_UUID_LED_CHAR    0x1525

/* Private typedef -----------------------------------------------------------*/
typedef struct Goose_RTC_s Goose_RTC_t;

typedef void (*Goose_RTC_Write_Handler_t) (Goose_RTC_t* pRTC, uint8_t new_state);

typedef struct
{
	Goose_RTC_Write_Handler_t	RTC_Write_Handler;
} Goose_RTC_ServiceInit_t;											// RTC服务初始化结构体

/* Private variables ---------------------------------------------------------*/
struct Goose_RTC_s														// RTC服务结构体
{
    uint16_t                    Service_Handle;
	ble_gatts_char_handles_t	RTC_Char_Handles;
	uint8_t						UUID_Type;
	uint16_t					Conn_Handle;
	Goose_RTC_Write_Handler_t	RTC_Write_Handler;
};

/* Private function prototypes -----------------------------------------------*/
u32  Goose_RTC_Service_Init(Goose_RTC_t* pRTC, const Goose_RTC_ServiceInit_t* pRTC_Init);
void Goose_RTC_ON_BLE_Evt(Goose_RTC_t* pRTC, ble_evt_t* pBLE_Evt);
void Goose_RTC_Set_Handler(Goose_RTC_t* pRTC, uint8_t new_state);
#endif /* __GOOSE_RTC_SERVICE_H */

/******************* (C) COPYRIGHT 2017 王宇 **** END OF FILE *****************/
