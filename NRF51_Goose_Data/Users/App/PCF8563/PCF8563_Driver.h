/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  PCF8563_Driver.h
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  06/04/2017
* Description        :  PCF8563_Driver header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PCF8563_DRIVER_H
#define __PCF8563_DRIVER_H

/* Includes ------------------------------------------------------------------*/
//#include "Global.h"

/* Private define ------------------------------------------------------------*/
#define 	PCF8563_ADDR				0x51							// 芯片地址
#define		PCF8563_WRITE_ADDR			(PCF8563_ADDR << 1)
#define		PCF8563_READ_ADDR			(PCF8563_WRITE_ADDR | 0x01)

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{
    CONTROL_STATUS_1				=	0x00,
	CONTROL_STATUS_2				=	0x01,
	VL_SECONDS				 		=	0x02,
	REG_MINUTES						=	0x03,
	REG_HOURS						=	0x04,
	REG_DAYS						=	0x05,
	REG_WEEKS						=	0x06,
	REG_MONTHS						=	0x07,
	REG_YEARS						=	0x08,
	MINUTE_ALARM					=	0x09,
	HOUR_ALARM						=	0x0A,
	DAY_ALARM						=	0x0B,
	WEEK_ALARM						=	0x0C,
	CLKOUT_CONTR0L					=	0x0D,
	TIMER_CONTROL					=	0x0E,
	REG_TIMER						=	0x0F
}
PCF8563Reg ;															// 寄存器地址

typedef struct
{
	unsigned char		Seconds;
	unsigned char		Minutes;
	unsigned char		Hours;
	unsigned char		Days;
	unsigned char		WeekDay;
	unsigned char		Months;
	unsigned char		Years;
}Date_t;																// 日期结构体

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern unsigned char PCF8563_Get_Date(Date_t* Date_s);
extern unsigned char PCF8563_Set_Date(Date_t* Date_s);
#endif /* __PCF8563_H */

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/
