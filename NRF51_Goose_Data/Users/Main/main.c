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

/**
 * @brief SPI user event handler.
 * @param event
 */

int main(void)
{
    bsp_board_leds_init();
    nrf_gpio_cfg_output(27);
	nrf_gpio_pin_write(27, 1);
	nrf_gpio_cfg_output(29);
	nrf_gpio_pin_write(29, 1);

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));

	uint32_t retcode = 1;
	uint16_t re;
	uint8_t tr;
	u8 i;
	u32 Addr_test 	 = 0x00008000;
//	u8  Data_Write[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
//						0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	u8 Data_Buff[600];

	for (i = 0; i < 255; i++)
	{
		Data_Buff[i] = i;
	}

	for (i = 0; i < 255; i++)
	{
		Data_Buff[i + 255] = 255 - i;
	}
	
	u8 Buffer[300];
	
	memset(Buffer, 0, 300);
	
	simple_uart_config(8, 9, 10, 11, 0);
//    
    NRF_LOG_INFO("SPI example\r\n");

	KH25_Flash_Init();
	KH25_GetID(&tr, &re);

//	KH25_Erase(4, Addr_test);
//	KH25_Page_Program(Addr_test, Data_Buff, 255);
	KH25_Write_WithNoRead(Addr_test, Data_Buff, 600);
	KH25_WaiteForWriteEnd();
	KH25_Read_Data(Addr_test, Buffer, 300);
	
    printf("\r\n	tr = %X, re = %X !\r\n", tr, re);

	for (i = 0; i < 255; i++)
	{
		printf("\r\n	Buffer[%d] = %X !\r\n", i, Buffer[i]);
	}

	for (i = 0; i < 45; i++)
	{
		printf("\r\n	Buffer[%d] = %X !\r\n", i + 255, Buffer[i + 255]);
	}
}
