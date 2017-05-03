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
#include "KH25_Flash_Store.h"
#include "nrf_drv_timer.h"
#include "BLE_Init.h"
#include "ble.h"
#include "softdevice_handler.h"
#include "Nrf_nvic.h"

#include "app_timer.h"
#include "app_timer_appsh.h"
#include "app_scheduler.h"

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"

#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "bsp.h"
#include "ble_gap.h"

/**
 * @brief SPI user event handler.
 * @param event
 */

//void timer2_init(void)
//{
//	 NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
//	 NRF_CLOCK->TASKS_HFCLKSTART    = 1;

//	while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) 
//	{
//	
//	}

//	NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
//	NRF_TIMER2->PRESCALER = 4;

//	NRF_TIMER2->CC[2] = 1000U;
//	NRF_TIMER2->INTENSET = TIMER_INTENSET_COMPARE2_Enabled << TIMER_INTENSET_COMPARE2_Pos;
//	NRF_TIMER2->SHORTS = TIMER_SHORTS_COMPARE2_CLEAR_Enabled << TIMER_SHORTS_COMPARE2_CLEAR_Pos;

//	NVIC_ClearPendingIRQ(TIMER2_IRQn);
//	NVIC_SetPriority(TIMER2_IRQn, 3);
//	NVIC_EnableIRQ(TIMER2_IRQn);
//	
//	NRF_TIMER2->TASKS_START = 1;
//}

void Power_Manage(void)
{

#ifdef BLE_OPEN
    uint32_t err_code;
    
    app_sched_execute();
    err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
        
#else
        __WFE();
        
#endif
}

void timer_handler(void *p)
{
	if (9 <= I_temp)
		{
			I_temp = 0;
		}
		I_temp++;
}

void Data_Handler(void *p)
{
	u8 DataBuff[16];

	Get_Current_Data(DataBuff);

	printf("\r\n	I_temp = %d", DataBuff[6]);
}

int main(void)
{
    bsp_board_leds_init();

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    simple_uart_config(8, 9, 10, 11, 0);

	uint32_t retcode = 1;
	uint16_t re;
	uint8_t tr;
	uint32_t err_code;
	u32 i,j;
	u32 Addr_test 	  = 0x00000000;
	u8  Start_Addr[2] = {0x00, 0x00};
	
//	u8 Buffer[512];
	
	retcode = Goose_IIC_Init();
	
//	timer2_init();
	
	APP_TIMER_INIT(0, 3, app_timer_evt_schedule);
	APP_SCHED_INIT(sizeof(app_timer_event_t) , 4);
//	app_timer_id_t Test_timer;
//	app_timer_id_t Data_timer;
	APP_TIMER_DEF(Test_timer);
	APP_TIMER_DEF(Data_timer);
	err_code = app_timer_create(&Test_timer, APP_TIMER_MODE_REPEATED, timer_handler);
	err_code = app_timer_create(&Data_timer, APP_TIMER_MODE_SINGLE_SHOT, Data_Handler);

	BLE_Init();
	
//	app_timer_start(Test_timer, APP_TIMER_TICKS(100, 0), NULL);
//	app_timer_start(Data_timer, APP_TIMER_TICKS(500, 0), NULL);
//	
//	while(1)
//		{
//		printf("  I = %d", I_temp);
//		nrf_delay_us(4);
//		}
//	Date_t Dnow;
//	Dnow.Seconds = 0x00;
//	Dnow.Minutes = 0x00;
//	Dnow.Hours	 = 0x17;
//	Dnow.Days    = 0x24;
//	Dnow.Months  = 0x04;
//	Dnow.Years   = 0x17;
//	Dnow.WeekDay = 0x01;
	
//	memset(Buffer, 0, 300);
    
    NRF_LOG_INFO("SPI example\r\n");

    
//    PCF8563_Read_Register(0x00, &tr, 1);
//    printf("\r\n	tr = %X!\r\n", tr);
//    tr = 0x00;
//    PCF8563_Write_Register(0x07, &(Dnow.Months), 2);
//    PCF8563_Read_Register(0x00, &tr, 1);
//    printf("\r\n	tr = %X!\r\n", tr);

//    retcode = PCF8563_Set_Date(&Dnow);

//	while(retcode)
//	{
//    	retcode = PCF8563_Get_Date(&Dnow);
//		printf("PCF8563 RTC year%x mon/%x day/%x hour%x min%x sec%x\r\n",Dnow.Years, Dnow.Months, Dnow.Days, Dnow.Hours, Dnow.Minutes, Dnow.Seconds);
//	}
	
//	KH25_Flash_Init();
//	KH25_Chip_Init();
//	KH25_GetID(&tr, &re);

//	KH25_Erase(4, Addr_test);
//	KH25_Erase(4, 0x00001000);
//	KH25_Erase(4, 0x001FF000);
//	KH25_Page_Program(0x001FF000, Start_Addr, 2);

//	Store_History_Data();
//	nrf_delay_ms(1000);
//	Store_History_Data();
//	nrf_delay_ms(1000);
//	Store_History_Data();
//	nrf_delay_ms(1000);
//	Store_History_Data();

//	KH25_WaiteForWriteEnd();
//	KH25_Read_Data(0x00000000, Buffer, 512);
	
//    printf("\r\n	tr = %X, re = %X !\r\n", tr, re);
	
//	j = 0;
//	while(j < 2705)
//	{
//		KH25_Read_Data(0x00000000 + j * 512, Buffer, 512);
//		
//		for (i = 0; i < 512; i += 8)
//		{
//			printf("\r\n %X, %X, %X, %X, %X, %X, %X, %X, Times %d\n\r", Buffer[i],Buffer[i+1],Buffer[i+2],Buffer[i+3],Buffer[i+4],Buffer[i+5],Buffer[i+6],Buffer[i+7], j);
//		}

//		j++;
//	}

//	KH25_DeepDownMode_Enter();

//	KH25_DeepDownMode_Exit();

	for (;;)
	{
		app_sched_execute();
	}
	
}
