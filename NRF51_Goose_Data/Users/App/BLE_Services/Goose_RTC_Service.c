/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  Goose_RTC_Service.c
* Author             :  ����
* CPU Type           :  nRF51822
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  19/04/2017
* Description        :  RTC����Ӧ�ó���
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Goose_RTC_Service.h"
#include <stdlib.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
u32  Goose_RTC_Service_Init(Goose_RTC_t* pRTC, const Goose_RTC_ServiceInit_t* pRTC_Init);
u32  RTC_Char_Add(Goose_RTC_t* pRTC, const Goose_RTC_ServiceInit_t* pRTC_Init);
void Goose_RTC_ON_BLE_Evt(Goose_RTC_t* pRTC, ble_evt_t* pBLE_Evt);
void Goose_RTC_ON_Write(Goose_RTC_t* pRTC, ble_evt_t * pBLE_Evt);
void Goose_RTC_ON_Disconnect(Goose_RTC_t* pRTC, ble_evt_t * pBLE_Evt);
void Goose_RTC_ON_Connect(Goose_RTC_t* pRTC, ble_evt_t* pBLE_Evt);
void Goose_RTC_Set_Handler(Goose_RTC_t* pRTC, uint8_t new_state);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           ����@2017-04-19
* Function Name  :  Goose_RTC_Services_Init
* Description    :  Goose BLE RTC�����ʼ��    
* Input          :  Goose_RTC_t* 						pRTC
*					const Goose_RTC_ServiceInit_t* 		pRTC_Init
* Output         :  None
* Return         :  None
*******************************************************************************/
u32 Goose_RTC_Service_Init(Goose_RTC_t* pRTC, const Goose_RTC_ServiceInit_t* pRTC_Init)
{
	u32             Err_Code;
    ble_uuid128_t   RTC_Base_UUID = LBS_UUID_BASE;
    ble_uuid_t      BLE_UUID;

	// ��ʼ������ṹ��
    pRTC->Conn_Handle       = BLE_CONN_HANDLE_INVALID;
    pRTC->RTC_Write_Handler = pRTC_Init->RTC_Write_Handler;

	// ��ӷ���
    Err_Code = sd_ble_uuid_vs_add(&RTC_Base_UUID, &pRTC->UUID_Type);
//    VERIFY_SUCCESS(Err_Code);

    BLE_UUID.type = pRTC->UUID_Type;
    BLE_UUID.uuid = LBS_UUID_SERVICE;

    Err_Code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &BLE_UUID, &pRTC->Service_Handle);
//	VERIFY_SUCCESS(Err_Code);

	// �������
	Err_Code = RTC_Char_Add(pRTC, pRTC_Init);
//	VERIFY_SUCCESS(Err_Code);

	return NRF_SUCCESS;
    
}
// end of u32 Goose_RTC_Service_Init(Goose_RTC_t* pRTC, const Goose_RTC_ServiceInit_t* pRTC_Init)

/*******************************************************************************
*                           ����@2017-04-19
* Function Name  :  RTC_Char_Add
* Description    :  ����RTC����
* Input          :  Goose_RTC_t* 						pRTC
*					const Goose_RTC_ServiceInit_t* 		pRTC_Init
* Output         :  None
* Return         :  NRF_SUCCESS on success, otherwise an error code.
*******************************************************************************/
u32 RTC_Char_Add(Goose_RTC_t* pRTC, const Goose_RTC_ServiceInit_t* pRTC_Init) 
{
	u8 RTC_Buffer[16];
    // ��������Ȩ����
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          BLE_UUID;
    ble_gatts_attr_md_t attr_md;

    // �ͻ�������ֵ���������� ����notify
    ble_gatts_attr_md_t cccd_md;
    
    memset(&cccd_md, 0, sizeof(cccd_md));
    
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));


    char_md.char_props.notify       = 0;                            	// ����Ȩ��
    char_md.char_props.read         = 1;                            	// ��Ȩ��
    char_md.char_props.write        = 1;                            	// дȨ��
    char_md.p_char_user_desc        = NULL;            					// ����������
    char_md.p_char_pf               = NULL;                   			// ��ʽ
    char_md.p_user_desc_md          = NULL;
    char_md.p_cccd_md               = &cccd_md;                         // �ͻ�����������������Ȩ�� 0x2902 �鿴Notify֪ͨ
    char_md.p_sccd_md               = NULL;

    // �������ͺ�������
    BLE_UUID.type = pRTC->UUID_Type;
    BLE_UUID.uuid = LBS_UUID_BUTTON_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    // û�ж�дȨ��
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &BLE_UUID;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 8;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 16;
    attr_char_value.p_value   = RTC_Buffer;

    return sd_ble_gatts_characteristic_add(pRTC->Service_Handle,
                                           &char_md,
                                           &attr_char_value,
                                           &pRTC->RTC_Char_Handles);
                                           
} 
// end of u32 RTC_Char_Add(Goose_RTC_t* pRTC, const Goose_RTC_ServiceInit_t* pRTC_Init)

/*******************************************************************************
//*                           ����@2017-03-09
//* Function Name  :  Duck_Message_Characteristic_Add
//* Description    :  ����ͨѶ����
//* Input          :  Goose_RTC_t* pRTC �������
//* Output         :  None
//* Return         :  NRF_SUCCESS on success, otherwise an error code.
//*****************************************************************************/
//u32 Duck_Message_Characteristic_Add(Goose_RTC_t* pRTC) 
//{
//    // ��������Ȩ����
//    ble_gatts_char_md_t char_md;
//    ble_gatts_char_pf_t accel_pf;
//    ble_gatts_attr_t    attr_char_value;
//    ble_uuid_t          BLE_UUID;
//    ble_gatts_attr_md_t attr_md;
//    static char User_Desc[] = "Message";
//    ble_gatts_attr_md_t cccd_md;
//    
//    memset(&cccd_md, 0, sizeof(cccd_md));
//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
//    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
//     
//    // �������ݸ�ʽ
//    memset(&accel_pf, 0, sizeof(accel_pf));
//    accel_pf.format    = BLE_GATT_CPF_FORMAT_UTF8S;

//    memset(&char_md, 0, sizeof(char_md));

//    char_md.char_props.notify       = 1;                            // ����Ȩ��
//    char_md.char_props.read         = 0;                            // ��Ȩ��
//    char_md.char_props.write        = 1;                            // дȨ��
//    char_md.p_char_user_desc        = (u8 *) User_Desc;             // ����������
//    char_md.char_user_desc_size     = strlen(User_Desc);
//    char_md.char_user_desc_max_size = strlen(User_Desc);
//    char_md.p_char_pf               = NULL;                         // ��ʽ
//    char_md.p_user_desc_md          = NULL;
//    char_md.p_cccd_md               = &cccd_md;                         // �ͻ�����������������Ȩ�� 0x2902 �鿴Notify֪ͨ
//    char_md.p_sccd_md               = NULL;

//    // �������ͺ�������
//    BLE_UUID.type = pRTC->UUID_Type;
//    BLE_UUID.uuid = DUCK_MESSAGE_CHARACTERISTIC;

//    memset(&attr_md, 0, sizeof(attr_md));

//    // ֻд����
//    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

//    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
//    attr_md.rd_auth = 0;
//    attr_md.wr_auth = 0;
//    attr_md.vlen    = 1;

//    memset(&attr_char_value, 0, sizeof(attr_char_value));

//    attr_char_value.p_uuid    = &BLE_UUID;
//    attr_char_value.p_attr_md = &attr_md;
//    attr_char_value.init_offs = 0;
//    attr_char_value.max_len   = BLE_DUCK_MAX_DATA_LEN;

//    return sd_ble_gatts_characteristic_add(pRTC->Service_Handle,
//                                           &char_md,
//                                           &attr_char_value,
//                                           &pRTC->Message_Handle);
//                                           
//}// End of u32 Duck_Message_Characteristic_Add(Goose_RTC_t* pRTC) 

/*******************************************************************************
//*                           ����@2017-01-15
//* Function Name  :  Duck_Temp_Humi_Encode
//* Description    :  ����¶�����
//* Input          :  s16 sData    ��ʪ�ȶ�����
//*                   u8 * pBuffer  ����
//* Output         :  None
//* Return         :  NRF_SUCCESS on success, otherwise an error code.
//*****************************************************************************/
//u8 Duck_Temp_Humi_Encode(s16 sTemp, u16 usHumi, u8 * pBuffer)
//{
//    u8 ucLen = 0;

//    // 16����תASCII
//    ucLen            = sprintf((char*)pBuffer, "T=%d.%d H=%d.%d", sTemp/10 , sTemp % 10, usHumi / 10, usHumi % 10);

//    // ������Ϣ
//    if (Hall.Magnet_Near)
//    {
//        memcpy(&pBuffer[ucLen], " Near", 5);
//        ucLen += 5;
//    }
//    else
//    {
//        memcpy(&pBuffer[ucLen], " Far", 4);
//        ucLen += 4;
//    }
//    
//    pBuffer[ucLen++] = 0;
//    return ucLen;
//    
//}// End of u8 Duck_Temp_Humi_Encode(s16 sTemp, u16 usHumi, u8 * pBuffer))

/*******************************************************************************
*                           ����@2017-04-19
* Function Name  :  Goose_RTC_ON_BLE_Evt
* Description    :  Duck�¼�����
* Input          :  Goose_RTC_t* 	pRTC
*                   ble_evt_t* 		pBLE_Evt  BLE�¼�
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_RTC_ON_BLE_Evt(Goose_RTC_t* pRTC, ble_evt_t* pBLE_Evt)
{
    if ((pRTC == NULL) || (pBLE_Evt == NULL))
    {
        return;
    }

    switch (pBLE_Evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:

            Goose_RTC_ON_Connect(pRTC, pBLE_Evt);
            
            break;

        case BLE_GAP_EVT_DISCONNECTED:
        
            Goose_RTC_ON_Disconnect(pRTC, pBLE_Evt);
            
            break;

        case BLE_GATTS_EVT_WRITE:
        
            Goose_RTC_ON_Write(pRTC, pBLE_Evt);
            
            break;
        
        default:
            // No implementation needed.
            break;
    }
    
}
// End of void Goose_RTC_ON_BLE_Evt(Goose_RTC_t* pRTC, ble_evt_t * pBLE_Evt)

/*******************************************************************************
*                           ����@2017-04-19
* Function Name  :  Goose_RTC_ON_Write
* Description    :  BLE_GATTS_EVT_WRITE         �¼�����
* Input          :  Goose_RTC_t* 	pRTC 		�������
*                   ble_evt_t* 		pBLE_Evt  	BLE�¼�
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_RTC_ON_Write(Goose_RTC_t* pRTC, ble_evt_t * pBLE_Evt)
{
   ble_gatts_evt_write_t * p_evt_write = &pBLE_Evt->evt.gatts_evt.params.write;

   if ((p_evt_write->handle == pRTC->RTC_Char_Handles.value_handle) &&
        (pRTC->RTC_Write_Handler!= NULL))
    {
        pRTC->RTC_Write_Handler(pRTC, p_evt_write->data[0]);
    }

}// End of void Goose_RTC_ON_Write(Goose_RTC_t* pRTC, ble_evt_t * pBLE_Evt)

/*******************************************************************************
*                           ����@2017-04-19
* Function Name  :  Goose_RTC_ON_Disconnect
* Description    :  �Ͽ�����
* Input          :  Goose_RTC_t* pRTC �������
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_RTC_ON_Disconnect(Goose_RTC_t* pRTC, ble_evt_t * pBLE_Evt)
{
	UNUSED_PARAMETER(pBLE_Evt);
    pRTC->Conn_Handle            = BLE_CONN_HANDLE_INVALID;
//    pRTC->Temp_Humi_Listening_En = false;
//    pRTC->Message_Listening_En   = false;
   

}
// End of void void Goose_RTC_ON_Disconnect(Goose_RTC_t* pRTC)

/*******************************************************************************
*                           ����@2017-04-19
* Function Name  :  Goose_RTC_ON_Connect
* Description    :  ����
* Input          :  Goose_RTC_t* pRTC �������
*                   ble_evt_t * pBLE_Evt  BLE�¼�
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_RTC_ON_Connect(Goose_RTC_t* pRTC, ble_evt_t* pBLE_Evt)
{
    pRTC->Conn_Handle = pBLE_Evt->evt.gap_evt.conn_handle;

}
// End of void Goose_RTC_ON_Connect(Goose_RTC_t* pRTC, ble_evt_t* pBLE_Evt)

/*******************************************************************************
*                           ����@2017-04-19
* Function Name  :  Goose_RTC_Set_Handler
* Description    :  RTC ������
* Input          :  
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_RTC_Set_Handler(Goose_RTC_t* pRTC, uint8_t new_state)
{}
/******************* (C) COPYRIGHT 2017 ���� **************END OF FILE*********/
