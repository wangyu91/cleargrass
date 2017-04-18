/******************** (C) COPYRIGHT 2016 ���� **********************************
* File Name          :  SW_IIC.c
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  22/03/2017
* Description        :  I2C��ʵ��
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "SW_IIC.h"

/* Private Variables ---------------------------------------------------------*/

/* Private Typedefs ----------------------------------------------------------*/

/* Private Function Prototypes -----------------------------------------------*/
unsigned char SW_IIC_Init(SW_IIC_t * SW_IIC_s);							// IIC ��ʼ��
unsigned char SW_IIC_Start(SW_IIC_t * SW_IIC_s);						// IIC �����ź�
unsigned char SW_IIC_Stop(SW_IIC_t * SW_IIC_s);							// IIC ֹͣ�ź�
unsigned char SW_IIC_Write_Byte(SW_IIC_t * SW_IIC_s, unsigned char IIC_Data);	// дһ���ֽ�
unsigned char SW_IIC_Read_Byte(SW_IIC_t * SW_IIC_s, unsigned char * Byte_Buff, unsigned char Need_ACK);	// ��һ���ֽ�
unsigned char SW_IIC_Transfer(SW_IIC_t * SW_IIC_s, 
							  unsigned char   Chip_Addr, 
							  unsigned char*  Data, 
							  unsigned short  Data_Len, 
							  unsigned char   Stop_Condition);			// �������� ͨ�Ž���
static unsigned char SW_IIC_Wait_SCL_Turn_High(SW_IIC_t * SW_IIC_s);	// �ȴ�SCL���ӻ��ͷ�
//void SW_IIC_ClearBus(void);

/* Private Functions ---------------------------------------------------------*/
/*******************************************************************************
*                           ����@2017-03-22
* Function Name  :  SW_IIC_Init
* Description    :  I2C�˿ڳ�ʼ�� 
* Input          :  SW_IIC_t *SW_IIC_s  I2C����
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
unsigned char SW_IIC_Init(SW_IIC_t * SW_IIC_s)
{
	unsigned char Bus_Clear;											// ������ձ�־
	unsigned char i;

	// �ͷ�����
	SW_IIC_s->SDA_High();												// ����ʱ��SDAΪ�ߵ�ƽ��
	SW_IIC_s->SCL_High();												// SCLҲΪ�ߵ�ƽ
	SW_IIC_s->Delay_us();

	if (SW_IIC_s->SDA_Read() == 1 && SW_IIC_s->SCL_Read() == 1)
	{
		Bus_Clear = 1;													// ���߿��п���
	}
	else if (SW_IIC_s->SCL_Read() == 1)									// SDA���Ǹߵ�ƽ
	{
		Bus_Clear = 0;													// �������ݴ�����

		for (i = 0; i < 18; i++)										// ��������2Byte�����ݺ�2λУ��
		{
			SW_IIC_s->SCL_Low();										//*!-> ����ȷ�� <-!*/
			SW_IIC_s->Delay_us();
			SW_IIC_s->SCL_High();
			SW_IIC_s->Delay_us();

			if (SW_IIC_s->SDA_Read() == 1)								// ���ݷ������
			{
				Bus_Clear = 1;											// ���߾���
				break;
			}
		}
	}
	else
	{
		Bus_Clear = 0;
	}

	return Bus_Clear;
	
}
// end of unsigned char SW_IIC_Init(SW_IIC_t * SW_IIC_s)

/*******************************************************************************
*                           ����@2017-03-22
* Function Name  :  SW_IIC_Wait_SCL_Turn_High
* Description    :  �ȴ��ӻ��ͷ�SCL 
* Input          :  SW_IIC_t *SW_IIC_s  I2C����
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
static unsigned char SW_IIC_Wait_SCL_Turn_High(SW_IIC_t * SW_IIC_s)
{
	unsigned int volatile Timeout_Counter = SW_IIC_s->Wait_Scl_Free_Timeout;

    // �ͷ�SCL
    SW_IIC_s->SCL_High();

    // �ȴ�ʱ��Ϊ��
    while (SW_IIC_s->SCL_Read() == 0)
    {
        if (Timeout_Counter == 0)
        {
            // ��ʱ
            return 0;
        }
        else
        {
            Timeout_Counter--;
            SW_IIC_s->Delay_us();
        }
    }
    
    SW_IIC_s->Delay_us();
    
    return 1;
    
}
// end of static unsigned char SW_IIC_Wait_SCL_Turn_High(SW_IIC_t * SW_IIC_s)

/*******************************************************************************
*                           ����@2017-03-22
* Function Name  :  SW_IIC_Start
* Description    :  ����I2C��ʼ�ź�
* Input          :  SW_IIC_t * SW_IIC_s  ����ָ��
* Output         :  None
* Return         :  �ɹ�����1 ʧ�ܷ���0
*******************************************************************************/
unsigned char SW_IIC_Start(SW_IIC_t * SW_IIC_s)
{
	SW_IIC_s->SDA_High();												// ����SDA
	SW_IIC_s->Delay_us();
	
	if (SW_IIC_Wait_SCL_Turn_High(SW_IIC_s) == 0)						// �ȴ�SCL����
	{
		return 0;
	}

	SW_IIC_s->SDA_Low();												// ����SDA������ʼ��Ϣ
	SW_IIC_s->Delay_us();
	SW_IIC_s->SCL_Low();												// SCL������Ӧ
	SW_IIC_s->Delay_us();

	return 1;
}
// end of unsigned char SW_IIC_Start(SW_IIC_t * SW_IIC_s)

/*******************************************************************************
*                           ����@2017-03-22
* Function Name  :  SW_I2C_Stop
* Description    :  ����I2Cֹͣ�ź�
* Input          :  SW_IIC_t *SW_IIC_s  ����ָ��
* Output         :  None
* Return         :  �ɹ�����1 ʧ�ܷ���0
*******************************************************************************/
unsigned char SW_IIC_Stop(SW_IIC_t * SW_IIC_s)
{
	SW_IIC_s->SDA_Low();												// SDAΪ��
	SW_IIC_s->Delay_us();

	if (SW_IIC_Wait_SCL_Turn_High(SW_IIC_s) == 0)						// �ȴ�SCL����
	{
		return 0;
	}

	SW_IIC_s->SDA_High();												// ����SDA�Բ���ֹͣ�ź�
	SW_IIC_s->Delay_us();
	
	return 1;
}
// end of unsigned char SW_IIC_Stop(SW_IIC_t * SW_IIC_s)

/*******************************************************************************
*                           ����@2017-03-22
* Function Name  :  SW_IIC_Write_Byte
* Description    :  IICд����
* Input          :  SW_IIC_t      *SW_IIC_s   �����ṹ��ָ��
*                   unsigned char IIC_Data    Ҫ���͵�����       
* Output         :  None
* Return         :  �ɹ�����1 ʧ�ܷ���0
*******************************************************************************/
unsigned char SW_IIC_Write_Byte(SW_IIC_t * SW_IIC_s, unsigned char IIC_Data)
{
	unsigned char Succeed = 1;
	unsigned char i;

	SW_IIC_s->Set_SDA_Output();											// ����SDAΪ���ģʽ
	
	SW_IIC_s->SCL_Low();												// ����SCL
	SW_IIC_s->Delay_us();

	for (i = 0x80; i > 0; i >>= 1)										// �������� �Ӹ�λ��ʼ
	{
		if (IIC_Data & i)
		{
			SW_IIC_s->SDA_High();
		}
		else
		{
			SW_IIC_s->SDA_Low();
		}

		if (SW_IIC_Wait_SCL_Turn_High(SW_IIC_s) == 0)					// �ȴ�SCL����
		{
			Succeed = 0;												// ��ʱ����
			break;
		}

		SW_IIC_s->SCL_Low();											// ����SCL�ȴ���һ�η���
		SW_IIC_s->Delay_us();
	}

	SW_IIC_s->Set_SDA_Input();											// ����SDAΪ����ģʽ ��ȡSDA��ƽ
	SW_IIC_s->Delay_us();
	
	Succeed &= SW_IIC_Wait_SCL_Turn_High(SW_IIC_s);						// �ȴ�SCL���н���ACK

	if (SW_IIC_s->SDA_Read() & SW_IIC_No_ACK)							// ��ȡACKӦ��
	{
		Succeed = 0x00;													// û���յ�Ӧ��
	}
	else
	{
		Succeed &= 0x01;												// �յ�Ӧ��
	}

	SW_IIC_s->SCL_Low();												// ����SCL׼����һ�η���
	SW_IIC_s->Delay_us();
	//STM32_Delay_us(5);
	SW_IIC_s->Set_SDA_Output();											// ��ԭSDAΪ���ģʽ

	return Succeed;
	
}
// end of unsigned char SW_IIC_Write_Byte(SW_IIC_t * SW_IIC_s, unsigned char IIC_Data)

/*******************************************************************************
*                           ����@2017-03-22
* Function Name  :  SW_IIC_Read_Byte
* Description    :  IIC��ȡһ���ֽ�����
* Input          :  SW_IIC_t      *SW_IIC_s      �����ṹ��ָ��
*                   unsigned char *Byte_Buff     ��ȡ���ݻ���
*                   unsigned char  Need_Ack      ��һ�Ƿ���ACK
* Output         :  None
* Return         :  �ɹ�����1 ʧ�ܷ���0
*******************************************************************************/
unsigned char SW_IIC_Read_Byte(SW_IIC_t * SW_IIC_s, unsigned char * Byte_Buff, unsigned char Need_ACK)
{
	unsigned char Byte_Read = 0;
	unsigned char Succeed = 1;
	unsigned char i;

	SW_IIC_s->Set_SDA_Input();											// ����Ϊ����ģʽ ��ȡ����
	
	for (i = 0x80; i > 0; i >>= 1)										// �������� �Ӹ�λ��ʼ
	{
		if (SW_IIC_Wait_SCL_Turn_High(SW_IIC_s) == 0)					// �ȴ�SCL����
		{
			Succeed = 0;
			break;
		}

		if (SW_IIC_s->SDA_Read())										// ����SDA��ƽ�ߵ�
		{
			Byte_Read |= i;												// ��ȡ����
			//printf("\r\n  Byte_Read = %d", Byte_Read);
		}

		SW_IIC_s->SCL_Low();											// ����SCLΪ��һ�ν�����׼��
		SW_IIC_s->Delay_us();
		
	}

	*Byte_Buff = Byte_Read;												// �������ݴ���BUFFER

	SW_IIC_s->Set_SDA_Output();											// ��ԭSDAΪ���ģʽ
	
	if (Need_ACK == SW_IIC_SEND_ACK)									// ����ACK
	{
		SW_IIC_s->SDA_Low();
	}
	else
	{
		SW_IIC_s->SDA_High();
	}
	SW_IIC_s->Delay_us();

	if (SW_IIC_Wait_SCL_Turn_High(SW_IIC_s) == 0)						// �ȴ��ӻ���Ӧ
	{
		Succeed = 0;
	}

	SW_IIC_s->SCL_Low();												// ����SCL׼����һ�ν���
	SW_IIC_s->Delay_us();

	return Succeed;
	
}
// end of unsigned char SW_IIC_Read_Byte(SW_IIC_t * SW_IIC_s, unsigned char * Byte_Buff, unsigned char Need_ACK)

/*******************************************************************************
*                           ����@2017-03-22
* Function Name  :  SW_IIC_Transfer
* Description    :  I2C��������
* Input          :  SW_I2C_Typedef *SW_I2C  ����ָ��
*                   unsigned char Chip_Addr оƬ��ַ
*                   unsigned char  *Data    ����ָ��
*                   unsigned short Data_Len ���ݳ���
*                   unsigned char Stop_Condition    ��һʱ����ֹͣ�ź�
* Output         :  None
* Return         :  �ɹ�����1 ʧ�ܷ���0
*******************************************************************************/
unsigned char SW_IIC_Transfer(SW_IIC_t * SW_IIC_s, unsigned char Chip_Addr, unsigned char *Data, unsigned short Data_Len, unsigned char Stop_Condition)
{
	unsigned char Succeed = 1;

    Succeed &= SW_IIC_Start(SW_IIC_s);									// ͨ�ſ�ʼ
    Succeed &= SW_IIC_Write_Byte(SW_IIC_s, Chip_Addr);					// ���͵�ַ

    // ���ȱ���
    if (Data_Len == 0)
    {
        return 0;
    }
    
    // �ж����䷽��
    if (Chip_Addr & 0x01)												// ����
    {
        while (Data_Len-- && Succeed)									// ��������
        {
            // ���һ���ֽڷ���NO ACK
            if (Data_Len == 0)
            {
                Succeed &= SW_IIC_Read_Byte(SW_IIC_s, Data, SW_IIC_DONT_SEND_ACK);
                //printf("\r\n data = %d", *Data);
            }
            else
            {
                Succeed &= SW_IIC_Read_Byte(SW_IIC_s, Data, SW_IIC_SEND_ACK);
                //printf("\r\n data = %d", *Data);
            }
            Data++;
        }
    }
    else																// ����
    {
        while (Data_Len-- && Succeed)									// ��������
        {
            Succeed &= SW_IIC_Write_Byte(SW_IIC_s, *Data);
            Data++;
        }
    }

    // ��ֹͣ�źŻ��߷���ʧ��
    if (Stop_Condition || (Succeed == 0))
    {
        Succeed &= SW_IIC_Stop(SW_IIC_s);
    }

    return Succeed;
    
}
// end of unsigned char SW_IIC_Transfer(SW_IIC_t * SW_IIC_s, unsigned char Chip_Addr, unsigned char *Data, unsigned short Data_Len, unsigned char Stop_Condition)

/******************* (C) COPYRIGHT 2017 ���� ********END OF FILE***************/