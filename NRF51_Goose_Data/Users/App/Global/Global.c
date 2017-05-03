#include "Global.h"
#include "nordic_common.h"

const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);
SW_IIC_t			Goose_IIC_s;
Date_t 				Date_Buff_s;
u8 					I_temp = 0;

