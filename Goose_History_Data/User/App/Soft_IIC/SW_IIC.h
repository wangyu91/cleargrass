/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  SW_IIC.h
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  22/03/2017
* Description        :  SW_I2C header file
*******************************************************************************/

/* Ifndef --------------------------------------------------------------------*/
#ifndef __SW_IIC_H
#define __SW_IIC_H

/* Includes ------------------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define     SW_IIC_SEND_ACK             0x01                            // ����Ӧ��
#define     SW_IIC_DONT_SEND_ACK        0x00                            // ������Ӧ��
#define     SW_IIC_NEED_SEND_STOP       1                               // ��Ҫ����ֹͣλ
#define     SW_IIC_DONT_SEND_STOP       0                               // ���跢��ֹͣλ

#define		SW_IIC_Get_ACK				0x00							// ��Ӧ��
#define		SW_IIC_No_ACK				0x01							// ��Ӧ��

#define		IIC_DELAY_COUNT				0xFF							// ��ʱ����

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    //void                (*Delay)(void);                                 // ��ʱ
    void                (*Delay_us)(void);                              // us��ʱ
    //void                (*Pin_Init)(void);                              // �ܽų�ʼ��
    void                (*SDA_High)(void);                              // ����SDA
    void                (*SDA_Low)(void);                               // ����SDA
    void                (*SCL_High)(void);                              // ����SCL
    void                (*SCL_Low)(void);                               // ����SCL                
    void                (*Set_SDA_Input)(void);                         // ��������λ����ܽ�
    void                (*Set_SDA_Output)(void);                        // ��������λ����ܽ�
    //void                (*Set_SCL_Output)(void);                        // ����ʱ��λ����ܽ�
    unsigned char       (*SDA_Read)(void);                              // ��SDA�ܽ�
    unsigned char       (*SCL_Read)(void);                              // ��SCL�ܽ�  
    unsigned int        Wait_Scl_Free_Timeout;                          // �ȴ�SCL���ͷų�ʱʱ��                             
    
} SW_IIC_t;

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
extern  unsigned char SW_IIC_Init(SW_IIC_t * SW_IIC_s);					// �ܽų�ʼ��
extern  unsigned char SW_IIC_Write_Byte(SW_IIC_t * SW_IIC_s, unsigned char ICC_Data);  // IICд�ֽ�
extern  unsigned char SW_IIC_Read_Byte(SW_IIC_t * SW_IIC_s, unsigned char * Byte_Buff, unsigned char Need_Ack); // IIC���ֽ�
extern  unsigned char SW_IIC_Start(SW_IIC_t * SW_IIC_s);      // ������ʼ�ź�
extern  unsigned char SW_IIC_Stop(SW_IIC_t * SW_IIC_s);       // ����ֹͣ�ź�
extern  unsigned char SW_IIC_Transfer(SW_IIC_t * SW_IIC_s, unsigned char Chip_Addr, unsigned char * Data, unsigned short Data_Len, unsigned char Stop_Condition); // ��������

#endif /* __SW_IIC_H */

/******************* (C) COPYRIGHT 2017 ���� *****END OF FILE******************/
