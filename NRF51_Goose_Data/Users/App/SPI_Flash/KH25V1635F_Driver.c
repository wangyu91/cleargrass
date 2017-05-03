/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  KH25V1635F_Driver.c
* Author             :  ����
* CPU Type           :  nRF51
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  11/04/2017
* Description        :  KH25V1635F SPI FLASHӦ�ó���
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "KH25V1635F_Driver.h"
#include "nrf_gpio.h"
//#include "Global.h"
#include "nrf_delay.h"

/* Private variables ---------------------------------------------------------*/
        
/* Private function prototypes -----------------------------------------------*/
void KH25_Flash_Init(void);												// ��ʼ��FLASH
void KH25_GetID(u8* ManuID, u16* DeviID);								// ��ȡID
void KH25_Write_Enable(void);											// дʹ��
u8   KH25_Read_Status_Reg(void);										// ��״̬�Ĵ���
void KH25_WaiteForWriteEnd(void);										// �ȴ�д����
void KH25_Read251_Data(u32 Addr, u8* DataBuffer, u8 DataLen);			// ��ȡ����	251�ֽ�
void KH25_Read_Data(u32 Addr, u8* DataBuffer, u32 DataLen);				// ��ȡ���� ����
void KH25_Erase(u8 Erase_Size, u32 Addr);								// ������
void KH25_Chip_Erase(void);												// оƬ����
void KH25_Chip_Init(void);												// оƬ��ʼ��
void KH25_Page251_Program(u32 Addr, u8* DataBuff, u8 Data_Len);			// ҳд251�ֽ�
void KH25_Page_Program(u32 Addr, u8* DataBuff, u8 Data_Len);			// ҳд256�ֽ�
void KH25_Write_WithNoRead(u32 Addr, u8* DataBuff, u32 Data_Len);		// �����ַд�������� ����ȡ
void KH25_DeepDownMode_Enter(void);										// ����͹���ģʽ
void KH25_DeepDownMode_Exit(void);										// �˳��͹���ģʽ

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           ����@2017-04-11
* Function Name  :  KH25_Flash_Init
* Description    :  ��ʼ��FLASH
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Flash_Init(void)
{
	// ѡ��SPIʵ��
//	spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);
	
	// ����SPI
	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   			= KH25_PIN_CS;
    spi_config.miso_pin 			= KH25_PIN_MISO;
    spi_config.mosi_pin 			= KH25_PIN_MOSI;
    spi_config.sck_pin  			= KH25_PIN_SCK;
    spi_config.mode					= NRF_DRV_SPI_MODE_3;
    spi_config.frequency  			= NRF_DRV_SPI_FREQ_500K;
    spi_config.bit_order 			= NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;

    // ��ʼ��SPI
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, NULL));

    nrf_gpio_cfg_output(KH25_PIN_WP);
	nrf_gpio_pin_write(KH25_PIN_WP, 1);
	nrf_gpio_cfg_output(KH25_PIN_HOLD);
	nrf_gpio_pin_write(KH25_PIN_HOLD, 1);

}
// end of void KH25_Flash_Init(void)

/*******************************************************************************
*                           ����@2017-04-11
* Function Name  :  KH25_GetID
* Description    :  ��ȡFLASH ID
* Input          :  u8*		ManuID		����ID
* 					u16* 	DeviID		�豸ID
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_GetID(u8* ManuID, u16* DeviID)
{
	u8	KH25_ID[4];
	u8  CMD_ID = CMD_READ_ID;

	// ��1Byte���� ��3Bytes����
	nrf_drv_spi_transfer(&spi, &CMD_ID, 1, KH25_ID, 4);

	*ManuID = KH25_ID[1];
	*DeviID = ((KH25_ID[2] << 8) + KH25_ID[3]);
}
// end of void KH25_GetID(u8* ManuID, u16* DeviID)

/*******************************************************************************
*                           ����@2017-04-12
* Function Name  :  KH25_Write_Enable
* Description    :  FLASHдʹ��
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Write_Enable(void)
{
	u8 CMD_WriteEnable = CMD_WRITE_ENABLE;

	// 1Byte���� 
	nrf_drv_spi_transfer(&spi, &CMD_WriteEnable, 1, NULL, 0);
}
// end of void KH25_Write_Enable(void)

/*******************************************************************************
*                           ����@2017-04-12
* Function Name  :  KH25_Read_Status_Reg
* Description    :  ��״̬�Ĵ���
* Input          :  None
* Output         :  None
* Return         :  ״̬�Ĵ���ֵ
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
*                           ����@2017-04-12
* Function Name  :  KH25_WaiteForWriteEnd
* Description    :  �ȴ�д����
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_WaiteForWriteEnd(void)
{
	u8 StaRegBuffer;

	StaRegBuffer = KH25_Read_Status_Reg();
	// ��ȡ״̬�Ĵ������λ WIP 
	while ((0x01 & StaRegBuffer) == 0x01)								// ѭ�����WIPֱ��Ϊ0
	{
		StaRegBuffer = KH25_Read_Status_Reg();
//		printf("\r\n	StateReg = %X !", StaRegBuffer);
	};													
}
// end of void KH25_WaiteForWriteEnd(void)

/*******************************************************************************
*                           ����@2017-04-12
* Function Name  :  KH25_Read251_Data
* Description    :  ��ȡ���� ���251�ֽ�
* Input          :  u32		Addr		������ʼ��ַ
*					u8* 	DataBuffer	���ݻ���
*					u8 		DataLen		���ݳ��� ���251�ֽ�
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

	// ������Ч���� �ӵ����ֽڿ�ʼ
	while (DataLen--)
	{
	    *DataBuffer = *(ReadBuffer + i);
		
		DataBuffer++;
	    i++;
	}
	
}
// end of void KH25_Read251_Data(u32 Addr, u8* DataBuffer, u8 DataLen)

/*******************************************************************************
*                           ����@2017-04-12
* Function Name  :  KH25_Read_Data
* Description    :  ��ȡ���� �����ֽ�
* Input          :  u32		Addr		������ʼ��ַ
*					u8* 	DataBuffer	���ݻ���
*					u32 	DataLen		���ݳ��� 
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
*                           ����@2017-04-12
* Function Name  :  KH25_Erase
* Description    :  ��������
* Input          :  u8 		Erase_Size	������С 4,32,64(K)
*					u32		Addr		ҳ��ʼ��ַ
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Erase(u8 Erase_Size, u32 Addr)
{
	u8 CMDSectorErase[4];

	// ѡ��������С
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

	// �ȴ�����
	KH25_WaiteForWriteEnd();
	// дʹ��
	KH25_Write_Enable();

	nrf_drv_spi_transfer(&spi, CMDSectorErase, 4, NULL, 0);
}
// end of void KH25_Erase(u8 Erase_Size, u32 Addr)

/*******************************************************************************
*                           ����@2017-04-18
* Function Name  :  KH25_Chipe_Erase
* Description    :  оƬ����
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Chip_Erase(void)
{
	u8 CMD_ChipErase = CMD_CHIP_ERASE;
	
    KH25_WaiteForWriteEnd();
    // дʹ��
    KH25_Write_Enable();
	// 1Byte���� 
	nrf_drv_spi_transfer(&spi, &CMD_ChipErase, 1, NULL, 0);
	// �ȴ�����
	KH25_WaiteForWriteEnd();

}
// end of void KH25_Chip_Erase(void)

/*******************************************************************************
*                           ����@2017-04-18
* Function Name  :  KH25_Chip_Init
* Description    :  оƬ����
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Chip_Init(void)
{
	u8 Data_Buff[2];
	// ��ȡ����ҳͷ�����ֽ�
	KH25_Read_Data(0x001FF000, Data_Buff, 2);
	// �ж��Ƿ�洢����ʷ���� 
	if ((Data_Buff[0] == 0xFF) && (Data_Buff[1] == 0xFF))
	{
		// �ӵ�һҳ��ʼд
		Data_Buff[0] = 0x00;
		Data_Buff[1] = 0x00;
		KH25_Chip_Erase();
		KH25_Page_Program(0x001FF000, Data_Buff, 2);
	}
}
// end of void KH25_Chip_Init(void)

/*******************************************************************************
*                           ����@2017-04-12
* Function Name  :  KH25_Page251_Program
* Description    :  ҳд251
* Input          :  u32 	Addr		������ʼ��ַ
*					u8* 	DataBuff	д�����ݻ���ָ��
*					u8 		Data_Len	���ݳ��� ���251Bytes
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_Page251_Program(u32 Addr, u8* DataBuff, u8 Data_Len)
{
	u8 CMDPageProgram[4];
	u8 Send_Buffer[256];												// ���ݷ���Buffer
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

	// �ȴ�����
	KH25_WaiteForWriteEnd();
	// дʹ��
	KH25_Write_Enable();

	nrf_drv_spi_transfer(&spi, Send_Buffer, Data_Len + 4, NULL, 0);

	KH25_WaiteForWriteEnd();
}
// end of void KH25_Page251_Program(u32 Addr, u8* DataBuff, u8 Data_Len)

/*******************************************************************************
*                           ����@2017-04-13
* Function Name  :  KH25_Page_Program
* Description    :  ҳд
* Input          :  u32 	Addr		������ʼ��ַ
*					u8* 	DataBuff	д�����ݻ���ָ��
*					u8 		Data_Len	���ݳ��� ���256Bytes
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
*                           ����@2017-04-13
* Function Name  :  KH25_Write_WithNoRead
* Description    :  ����λ��д�� ����ȡ����
* Input          :  u32 	Addr		������ʼ��ַ
*					u8* 	DataBuff	д�����ݻ���ָ��
*					u32 	Data_Len	���ݳ���
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

	// �Ƿ��ҳ�׿�ʼ
	if (((Addr >> 0) & PageSize) != 0)
	{
		// ��ҳ�� ����ʣ��ռ�
		DataLen_Page1remain = PageSize - ((Addr >> 0) & PageSize);

		// �ж�ʣ��ռ��Ƿ��㹻
		if (Data_Len <= DataLen_Page1remain)
		{
			// �㹻 ֱ��д��
			KH25_Page_Program(Addr, DataBuff, Data_Len);
			return;
		}
		else 
		{
			// ���� �����µ�ַ ������ʼ ���ݳ���
			Addr_New 	   = Addr     + DataLen_Page1remain;
			pDataBuff_New  = DataBuff + DataLen_Page1remain;
			Data_Len_New   = Data_Len - DataLen_Page1remain;
			// ��ʼ��һҳд�� �׵�ַ[7:0]��0
			KH25_Page_Program(Addr, DataBuff, DataLen_Page1remain);
		}
	}

	// �����Ƿ����һҳ
	while (PageSize < Data_Len_New)
	{
		// дһҳ
		KH25_Page_Program(Addr_New, pDataBuff_New, PageSize);

		Addr_New 	  = Addr_New      + PageSize + 1;
		pDataBuff_New = pDataBuff_New + PageSize + 1;
		Data_Len_New  = Data_Len_New  - PageSize - 1;
	}

	// дʣ������
	KH25_Page_Program(Addr_New, pDataBuff_New, Data_Len_New);
	
}
// end of void KH25_Write_WithNoRead(u32 Addr, u8* DataBuff, u32 Data_Len)

/*******************************************************************************
*                           ����@2017-04-18
* Function Name  :  KH25_DeepDownMode_Enter
* Description    :  ����͹���ģʽ
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_DeepDownMode_Enter(void)
{
	u8 CMD_DDMEnter = CMD_DDM_ENTER;

	// 1Byte���� 
	nrf_drv_spi_transfer(&spi, &CMD_DDMEnter, 1, NULL, 0);

	nrf_delay_us(tDP);
}
// end of void KH25_DeepDownMode_Enter(void)

/*******************************************************************************
*                           ����@2017-04-18
* Function Name  :  KH25_DeepDownMode_Exit
* Description    :  ����͹���ģʽ
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void KH25_DeepDownMode_Exit(void)
{
	// �ȴ���Ч����ʱ��
	nrf_delay_us(tDPDD);

	nrf_gpio_pin_clear(KH25_PIN_CS);

	nrf_delay_us(tCRDP);

	nrf_gpio_pin_set(KH25_PIN_CS);

	nrf_delay_us(tRDP);
}
// end of void KH25_DeepDownMode_Exit(void)

/******************* (C) COPYRIGHT 2017 ���� *****END OF FILE******************/
