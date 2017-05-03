/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  KH25V1635F_Driver.c
* Author             :  王宇
* CPU Type           :  nRF51
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  11/04/2017
* Description        :  KH25V1635F SPI FLASH应用程序
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "KH25V1635F_Driver.h"
#include "nrf_gpio.h"
//#include "Global.h"
#include "nrf_delay.h"

/* Private variables ---------------------------------------------------------*/
        
/* Private function prototypes -----------------------------------------------*/
void KH25_Flash_Init(void);												// 初始化FLASH
void KH25_GetID(u8* ManuID, u16* DeviID);								// 获取ID
void KH25_Write_Enable(void);											// 写使能
u8   KH25_Read_Status_Reg(void);										// 读状态寄存器
void KH25_WaiteForWriteEnd(void);										// 等待写空闲
void KH25_Read251_Data(u32 Addr, u8* DataBuffer, u8 DataLen);			// 读取数据	251字节
void KH25_Read_Data(u32 Addr, u8* DataBuffer, u32 DataLen);				// 读取数据 任意
void KH25_Erase(u8 Erase_Size, u32 Addr);								// 擦除区
void KH25_Chip_Erase(void);												// 芯片擦除
void KH25_Chip_Init(void);												// 芯片初始化
void KH25_Page251_Program(u32 Addr, u8* DataBuff, u8 Data_Len);			// 页写251字节
void KH25_Page_Program(u32 Addr, u8* DataBuff, u8 Data_Len);			// 页写256字节
void KH25_Write_WithNoRead(u32 Addr, u8* DataBuff, u32 Data_Len);		// 任意地址写任意数据 不读取
void KH25_DeepDownMode_Enter(void);										// 进入低功耗模式
void KH25_DeepDownMode_Exit(void);										// 退出低功耗模式

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           王宇@2017-04-11
* Function Name  :  KH25_Flash_Init
* Description    :  初始化FLASH
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Flash_Init(void)
{
	// 选择SPI实体
//	spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);
	
	// 配置SPI
	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   			= KH25_PIN_CS;
    spi_config.miso_pin 			= KH25_PIN_MISO;
    spi_config.mosi_pin 			= KH25_PIN_MOSI;
    spi_config.sck_pin  			= KH25_PIN_SCK;
    spi_config.mode					= NRF_DRV_SPI_MODE_3;
    spi_config.frequency  			= NRF_DRV_SPI_FREQ_500K;
    spi_config.bit_order 			= NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;

    // 初始化SPI
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, NULL));

    nrf_gpio_cfg_output(KH25_PIN_WP);
	nrf_gpio_pin_write(KH25_PIN_WP, 1);
	nrf_gpio_cfg_output(KH25_PIN_HOLD);
	nrf_gpio_pin_write(KH25_PIN_HOLD, 1);

}
// end of void KH25_Flash_Init(void)

/*******************************************************************************
*                           王宇@2017-04-11
* Function Name  :  KH25_GetID
* Description    :  获取FLASH ID
* Input          :  u8*		ManuID		工厂ID
* 					u16* 	DeviID		设备ID
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_GetID(u8* ManuID, u16* DeviID)
{
	u8	KH25_ID[4];
	u8  CMD_ID = CMD_READ_ID;

	// 发1Byte命令 收3Bytes数据
	nrf_drv_spi_transfer(&spi, &CMD_ID, 1, KH25_ID, 4);

	*ManuID = KH25_ID[1];
	*DeviID = ((KH25_ID[2] << 8) + KH25_ID[3]);
}
// end of void KH25_GetID(u8* ManuID, u16* DeviID)

/*******************************************************************************
*                           王宇@2017-04-12
* Function Name  :  KH25_Write_Enable
* Description    :  FLASH写使能
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Write_Enable(void)
{
	u8 CMD_WriteEnable = CMD_WRITE_ENABLE;

	// 1Byte命令 
	nrf_drv_spi_transfer(&spi, &CMD_WriteEnable, 1, NULL, 0);
}
// end of void KH25_Write_Enable(void)

/*******************************************************************************
*                           王宇@2017-04-12
* Function Name  :  KH25_Read_Status_Reg
* Description    :  读状态寄存器
* Input          :  None
* Output         :  None
* Return         :  状态寄存器值
*******************************************************************************/
u8 KH25_Read_Status_Reg(void)
{
	u8 CMD_ReadStaReg = CMD_READ_STATUS_REG;
	u8 Reg_Buffer[2];
	
	nrf_drv_spi_transfer(&spi, &CMD_ReadStaReg, 1, Reg_Buffer, 2);

	return(Reg_Buffer[1]);
}
// end of void KH25_Read_Status_Reg(u8* StatusReg)

/*******************************************************************************
*                           王宇@2017-04-12
* Function Name  :  KH25_WaiteForWriteEnd
* Description    :  等待写空闲
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_WaiteForWriteEnd(void)
{
	u8 StaRegBuffer;

	StaRegBuffer = KH25_Read_Status_Reg();
	// 读取状态寄存器最低位 WIP 
	while ((0x01 & StaRegBuffer) == 0x01)								// 循环检查WIP直到为0
	{
		StaRegBuffer = KH25_Read_Status_Reg();
//		printf("\r\n	StateReg = %X !", StaRegBuffer);
	};													
}
// end of void KH25_WaiteForWriteEnd(void)

/*******************************************************************************
*                           王宇@2017-04-12
* Function Name  :  KH25_Read251_Data
* Description    :  读取数据 最大251字节
* Input          :  u32		Addr		数据起始地址
*					u8* 	DataBuffer	数据缓存
*					u8 		DataLen		数据长度 最大251字节
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Read251_Data(u32 Addr, u8* DataBuffer, u8 DataLen)
{
	u8 CMDRead[4];
	u8 ReadBuffer[255];
	u8 i = 4;
	
	CMDRead[0] = CMD_READ;
	CMDRead[1] = (Addr >> 16) & 0xFF;
	CMDRead[2] = (Addr >> 8)  & 0xFF;
	CMDRead[3] = (Addr >> 0)  & 0xFF;
	
	nrf_drv_spi_transfer(&spi, CMDRead, 4, ReadBuffer, DataLen + 4);

	// 传递有效数据 从第四字节开始
	while (DataLen--)
	{
	    *DataBuffer = *(ReadBuffer + i);
		
		DataBuffer++;
	    i++;
	}
	
}
// end of void KH25_Read251_Data(u32 Addr, u8* DataBuffer, u8 DataLen)

/*******************************************************************************
*                           王宇@2017-04-12
* Function Name  :  KH25_Read_Data
* Description    :  读取数据 任意字节
* Input          :  u32		Addr		数据起始地址
*					u8* 	DataBuffer	数据缓存
*					u32 	DataLen		数据长度 
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Read_Data(u32 Addr, u8* DataBuffer, u32 DataLen)
{
	while (251 <= DataLen)
	{
		KH25_Read251_Data(Addr, DataBuffer, 251);

		Addr       = Addr       + 251;
		DataBuffer = DataBuffer + 251;
		DataLen    = DataLen    - 251;
	}

	KH25_Read251_Data(Addr, DataBuffer, DataLen);
}
// end of void KH25_Read_Data(u32 Addr, u8* DataBuffer, u32 DataLen)

/*******************************************************************************
*                           王宇@2017-04-12
* Function Name  :  KH25_Erase
* Description    :  擦除区块
* Input          :  u8 		Erase_Size	擦除大小 4,32,64(K)
*					u32		Addr		页起始地址
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Erase(u8 Erase_Size, u32 Addr)
{
	u8 CMDSectorErase[4];

	// 选定擦除大小
	switch(Erase_Size)
	{
		case 4:
			CMDSectorErase[0] = CMD_SECTOR_ERASE;
			break;
		case 32:
			CMDSectorErase[0] = CMD_BLOCK32_ERASE;
			break;
		case 64:
			CMDSectorErase[0] = CMD_BLOCK64_ERASE;
			break;
		default:
			CMDSectorErase[0] = NULL;
			break;
	}
	CMDSectorErase[1] = (Addr >> 16) & 0xFF;
	CMDSectorErase[2] = (Addr >> 8)  & 0xFF;
	CMDSectorErase[3] = (Addr >> 0)  & 0xFF;

	// 等待空闲
	KH25_WaiteForWriteEnd();
	// 写使能
	KH25_Write_Enable();

	nrf_drv_spi_transfer(&spi, CMDSectorErase, 4, NULL, 0);
}
// end of void KH25_Erase(u8 Erase_Size, u32 Addr)

/*******************************************************************************
*                           王宇@2017-04-18
* Function Name  :  KH25_Chipe_Erase
* Description    :  芯片擦除
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Chip_Erase(void)
{
	u8 CMD_ChipErase = CMD_CHIP_ERASE;
	
    KH25_WaiteForWriteEnd();
    // 写使能
    KH25_Write_Enable();
	// 1Byte命令 
	nrf_drv_spi_transfer(&spi, &CMD_ChipErase, 1, NULL, 0);
	// 等待空闲
	KH25_WaiteForWriteEnd();

}
// end of void KH25_Chip_Erase(void)

/*******************************************************************************
*                           王宇@2017-04-18
* Function Name  :  KH25_Chip_Init
* Description    :  芯片擦除
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Chip_Init(void)
{
	u8 Data_Buff[2];
	// 读取检索页头两个字节
	KH25_Read_Data(0x001FF000, Data_Buff, 2);
	// 判断是否存储过历史数据 
	if ((Data_Buff[0] == 0xFF) && (Data_Buff[1] == 0xFF))
	{
		// 从第一页开始写
		Data_Buff[0] = 0x00;
		Data_Buff[1] = 0x00;
		KH25_Chip_Erase();
		KH25_Page_Program(0x001FF000, Data_Buff, 2);
	}
}
// end of void KH25_Chip_Init(void)

/*******************************************************************************
*                           王宇@2017-04-12
* Function Name  :  KH25_Page251_Program
* Description    :  页写251
* Input          :  u32 	Addr		数据起始地址
*					u8* 	DataBuff	写入数据缓存指针
*					u8 		Data_Len	数据长度 最大251Bytes
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Page251_Program(u32 Addr, u8* DataBuff, u8 Data_Len)
{
	u8 CMDPageProgram[4];
	u8 Send_Buffer[256];												// 数据发送Buffer
	u16 i;
	
	CMDPageProgram[0] = CMD_PAGE_PROGRAM;
	CMDPageProgram[1] = (Addr >> 16) & 0xFF;
	CMDPageProgram[2] = (Addr >> 8)  & 0xFF;
	CMDPageProgram[3] = (Addr >> 0)  & 0xFF;

	for (i = 0; i < 4; i++)
	{
		Send_Buffer[i] = CMDPageProgram[i];
//		printf("	\r\n Send_Buffer[%d] = %d!\r\n", i, Send_Buffer[i]);
	}

	for (i = 4; i <= 4 + Data_Len; i++)
	{
		Send_Buffer[i] = *DataBuff;
		DataBuff++;
//		printf("	\r\n Send_Buffer[%d] = %d!\r\n", i, Send_Buffer[i]);
	}

	// 等待空闲
	KH25_WaiteForWriteEnd();
	// 写使能
	KH25_Write_Enable();

	nrf_drv_spi_transfer(&spi, Send_Buffer, Data_Len + 4, NULL, 0);

	KH25_WaiteForWriteEnd();
}
// end of void KH25_Page251_Program(u32 Addr, u8* DataBuff, u8 Data_Len)

/*******************************************************************************
*                           王宇@2017-04-13
* Function Name  :  KH25_Page_Program
* Description    :  页写
* Input          :  u32 	Addr		数据起始地址
*					u8* 	DataBuff	写入数据缓存指针
*					u8 		Data_Len	数据长度 最大256Bytes
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Page_Program(u32 Addr, u8* DataBuff, u8 Data_Len)
{
	if (Data_Len <= 251)
	{
		KH25_Page251_Program(Addr, DataBuff, Data_Len);
	}
	else
	{
		KH25_Page251_Program(Addr, DataBuff, 251);
		KH25_Page251_Program(Addr + 251, DataBuff + 251, Data_Len - 251);
	}
}
//end of void KH25_Page_Program(u32 Addr, u8* DataBuff, u8 Data_Len)

/*******************************************************************************
*                           王宇@2017-04-13
* Function Name  :  KH25_Write_WithNoRead
* Description    :  任意位置写入 不读取数据
* Input          :  u32 	Addr		数据起始地址
*					u8* 	DataBuff	写入数据缓存指针
*					u32 	Data_Len	数据长度
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Write_WithNoRead(u32 Addr, u8* DataBuff, u32 Data_Len)
{
	u32 Addr_New, Data_Len_New;
	u8* pDataBuff_New;
	u8  DataLen_Page1remain;

	Addr_New 	  = Addr;
	pDataBuff_New = DataBuff;
	Data_Len_New  = Data_Len;

	// 是否从页首开始
	if (((Addr >> 0) & PageSize) != 0)
	{
		// 非页首 计算剩余空间
		DataLen_Page1remain = PageSize - ((Addr >> 0) & PageSize);

		// 判断剩余空间是否足够
		if (Data_Len <= DataLen_Page1remain)
		{
			// 足够 直接写入
			KH25_Page_Program(Addr, DataBuff, Data_Len);
			return;
		}
		else 
		{
			// 不足 计算新地址 数据起始 数据长度
			Addr_New 	   = Addr     + DataLen_Page1remain;
			pDataBuff_New  = DataBuff + DataLen_Page1remain;
			Data_Len_New   = Data_Len - DataLen_Page1remain;
			// 起始第一页写入 首地址[7:0]非0
			KH25_Page_Program(Addr, DataBuff, DataLen_Page1remain);
		}
	}

	// 数据是否大于一页
	while (PageSize < Data_Len_New)
	{
		// 写一页
		KH25_Page_Program(Addr_New, pDataBuff_New, PageSize);

		Addr_New 	  = Addr_New      + PageSize + 1;
		pDataBuff_New = pDataBuff_New + PageSize + 1;
		Data_Len_New  = Data_Len_New  - PageSize - 1;
	}

	// 写剩余数据
	KH25_Page_Program(Addr_New, pDataBuff_New, Data_Len_New);
	
}
// end of void KH25_Write_WithNoRead(u32 Addr, u8* DataBuff, u32 Data_Len)

/*******************************************************************************
*                           王宇@2017-04-18
* Function Name  :  KH25_DeepDownMode_Enter
* Description    :  进入低功耗模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_DeepDownMode_Enter(void)
{
	u8 CMD_DDMEnter = CMD_DDM_ENTER;

	// 1Byte命令 
	nrf_drv_spi_transfer(&spi, &CMD_DDMEnter, 1, NULL, 0);

	nrf_delay_us(tDP);
}
// end of void KH25_DeepDownMode_Enter(void)

/*******************************************************************************
*                           王宇@2017-04-18
* Function Name  :  KH25_DeepDownMode_Exit
* Description    :  进入低功耗模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_DeepDownMode_Exit(void)
{
	// 等待有效操作时间
	nrf_delay_us(tDPDD);

	nrf_gpio_pin_clear(KH25_PIN_CS);

	nrf_delay_us(tCRDP);

	nrf_gpio_pin_set(KH25_PIN_CS);

	nrf_delay_us(tRDP);
}
// end of void KH25_DeepDownMode_Exit(void)

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/
