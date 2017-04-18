/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "Simple_uart.h"
#include "Global.h"
#include "PCF8563_Driver.h"

/**
 * @brief SPI user event handler.
 * @param event
 */

int main(void)
{
    bsp_board_leds_init();

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    simple_uart_config(8, 9, 10, 11, 0);

	uint32_t retcode = 1;
//	uint16_t re;
//	uint8_t tr;
	u32 i,j;
	u32 Addr_test 	  = 0x00000000;
	u8  Start_Addr[2] = {0x00, 0x00};
	
	u8 Buffer[512];
//	Date_t Dnow;
//	Dnow.Seconds = 0x00;
//	Dnow.Minutes = 0x22;
//	Dnow.Hours	 = 0x20;
//	Dnow.Days    = 0x17;
//	Dnow.Months  = 0x04;
//	Dnow.Years   = 0x17;
//	Dnow.WeekDay = 0x02;
	
//	memset(Buffer, 0, 300);
    
    NRF_LOG_INFO("SPI example\r\n");

    retcode = Goose_IIC_Init();
//    PCF8563_Read_Register(0x00, &tr, 1);
//    printf("\r\n	tr = %X!\r\n", tr);
//    tr = 0x00;
//    PCF8563_Write_Register(0x06, &(Dnow.WeekDay), 1);
//    PCF8563_Read_Register(0x00, &tr, 1);
//    printf("\r\n	tr = %X!\r\n", tr);
//    retcode = PCF8563_Set_Date(&Dnow);
//	while(retcode)
//	{
//    	retcode = PCF8563_Get_Date(&Dnow);
//		printf("PCF8563 RTC year%x mon/%x day/%x hour%x min%x sec%x\r\n",Dnow.Years, Dnow.Months, Dnow.Days, Dnow.Hours, Dnow.Minutes, Dnow.Seconds);
//	}
	
	KH25_Flash_Init();
	KH25_Chip_Init();
//	KH25_GetID(&tr, &re);

	KH25_Erase(4, Addr_test);
	KH25_Erase(4, 0x00001000);
	KH25_Erase(4, 0x001FF000);
	KH25_Page_Program(0x001FF000, Start_Addr, 2);

	Store_History_Data();
//	nrf_delay_ms(1000);
//	Store_History_Data();
//	nrf_delay_ms(1000);
//	Store_History_Data();
//	nrf_delay_ms(1000);
//	Store_History_Data();

	KH25_WaiteForWriteEnd();
	KH25_Read_Data(0x00000000, Buffer, 512);
	
//    printf("\r\n	tr = %X, re = %X !\r\n", tr, re);

//	for (i = 0; i < 500; i++)
//	{
//		printf("\r\n Buffer[%d] = %X \n\r", i, Buffer[i]);
//	}
	j = 0;
	while(j < 3)
	{
		KH25_Read_Data(0x00000000 + j * 512, Buffer, 512);
		
		for (i = 0; i < 512; i += 8)
		{
			printf("\r\n %X, %X, %X, %X, %X, %X, %X, %X, Times %d\n\r", Buffer[i],Buffer[i+1],Buffer[i+2],Buffer[i+3],Buffer[i+4],Buffer[i+5],Buffer[i+6],Buffer[i+7], j);
		}

		j++;
	}
}
