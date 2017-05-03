/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  BLE_Init.c
* Author             :  王宇
* CPU Type           :  nRF51822
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  19/04/2017
* Description        :  BLE初始化
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "BLE_Init.h"
#include "nrf51.h"
#include "nrf_delay.h"
#include "app_gpiote.h"
#include "softdevice_handler_appsh.h"
#include "Goose_RTC_Service.h"
#include "ble_stack_handler_types.h"

/* Private variables ---------------------------------------------------------*/
ble_advdata_service_data_t      m_service_data[1]; 
static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;          /**< Handle of the current connection. */

Goose_RTC_t						m_Goose_RTC;

/* Private function prototypes -----------------------------------------------*/
static void services_init(void);
void 		gap_params_init(void);
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt);
static void conn_params_error_handler(uint32_t nrf_error);
void 		conn_params_init(void);
static void on_adv_evt(ble_adv_evt_t ble_adv_evt);
static void on_ble_evt(ble_evt_t * p_ble_evt);
static void advertising_init(void);
static void ble_evt_dispatch(ble_evt_t * p_ble_evt);
static void advertising_start(void);
void 		BLE_Stack_Init(void);
void 		BLE_Init(void);                                                    // BLE初始化

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           王宇@2017-04-19
* Function Name  :  services_init
* Description    :  服务初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
static void services_init(void)
{
    uint32_t       			Err_Code;
    Goose_RTC_ServiceInit_t RTC_Init;
    
    memset(&RTC_Init, 0, sizeof(RTC_Init));
    
    RTC_Init.RTC_Write_Handler = Goose_RTC_Set_Handler;

    Err_Code = Goose_RTC_Service_Init(&m_Goose_RTC, &RTC_Init);
    APP_ERROR_CHECK(Err_Code);
}
// end of static void services_init(void)

/*******************************************************************************
*                           王宇@2017-04-19
* Function Name  :  gap_params_init
* Description    :  GAP 参数初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}
// end of void gap_params_init(void)

/*******************************************************************************
*                           王宇@2017-04-19
* Function Name  :  on_conn_params_evt & conn_params_error_handler
* Description    :  断开连接 回调函数和错误处理函数
* Input          :  ----
* Output         :  None
* Return         :  None
*******************************************************************************/
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/*******************************************************************************
*                           王宇@2017-04-19
* Function Name  :  conn_params_init
* Description    :  连接参数初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}
// end of void conn_params_init(void)

/*******************************************************************************
*                           王宇@2017-04-19
* Function Name  :  on_adv_evt
* Description    :  广播事件处理
* Input          :  ble_adv_evt_t 	ble_adv_evt		
* Output         :  None
* Return         :  None
*******************************************************************************/
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
//            NRF_LOG_INFO("Fast advertising\r\n");
            break;

        case BLE_ADV_EVT_IDLE:
            break;

        default:
            break;
    }
}
// end of static void on_adv_evt(ble_adv_evt_t ble_adv_evt)

/*******************************************************************************
*                           王宇@2017-04-19
* Function Name  :  on_ble_evt
* Description    :  ble事件处理
* Input          :  ble_evt_t* 	p_ble_evt		
* Output         :  None
* Return         :  None
*******************************************************************************/
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break; // BLE_GAP_EVT_CONNECTED

        case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            break; // BLE_GAP_EVT_DISCONNECTED

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GAP_EVT_SEC_PARAMS_REQUEST

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTS_EVT_SYS_ATTR_MISSING

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTC_EVT_TIMEOUT

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTS_EVT_TIMEOUT

        case BLE_EVT_USER_MEM_REQUEST:
            err_code = sd_ble_user_mem_reply(p_ble_evt->evt.gattc_evt.conn_handle, NULL);
            APP_ERROR_CHECK(err_code);
            break; // BLE_EVT_USER_MEM_REQUEST

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
        {
            ble_gatts_evt_rw_authorize_request_t  req;
            ble_gatts_rw_authorize_reply_params_t auth_reply;

            req = p_ble_evt->evt.gatts_evt.params.authorize_request;

            if (req.type != BLE_GATTS_AUTHORIZE_TYPE_INVALID)
            {
                if ((req.request.write.op == BLE_GATTS_OP_PREP_WRITE_REQ)     ||
                    (req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_NOW) ||
                    (req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL))
                {
                    if (req.type == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
                    {
                        auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
                    }
                    else
                    {
                        auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_READ;
                    }
                    auth_reply.params.write.gatt_status = APP_FEATURE_NOT_SUPPORTED;
                    err_code = sd_ble_gatts_rw_authorize_reply(p_ble_evt->evt.gatts_evt.conn_handle,
                                                               &auth_reply);
                    APP_ERROR_CHECK(err_code);
                }
            }
        } break; // BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST

#if (NRF_SD_BLE_API_VERSION == 3)
        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
            err_code = sd_ble_gatts_exchange_mtu_reply(p_ble_evt->evt.gatts_evt.conn_handle,
                                                       NRF_BLE_MAX_MTU_SIZE);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST
#endif

        default:
            // No implementation needed.
            break;
    }
}
// end of static void on_ble_evt(ble_evt_t * p_ble_evt)

/*******************************************************************************
*                           王宇@2017-04-19
* Function Name  :  advertising_init
* Description    :  广播初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
static void advertising_init(void)
{
    uint32_t               err_code;
    ble_advdata_t          advdata;
    ble_advdata_t          scanrsp;
    ble_adv_modes_config_t options;

    // Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = false;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = m_adv_uuids;

    memset(&options, 0, sizeof(options));
    options.ble_adv_fast_enabled  = true;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

    err_code = ble_advertising_init(&advdata, &scanrsp, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);
}
// end of static void advertising_init(void)

/*******************************************************************************
*                           王宇@2017-04-19
* Function Name  :  ble_evt_dispatch
* Description    :  事件分发函数
* Input          :  ble_evt_t*		p_ble_evt
* Output         :  None
* Return         :  None
*******************************************************************************/
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_params_on_ble_evt(p_ble_evt);
    on_ble_evt(p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);

    Goose_RTC_ON_BLE_Evt(&m_Goose_RTC, p_ble_evt);
}

/*******************************************************************************
*                           王宇@2017-04-19
* Function Name  :  advertising_start
* Description    :  广播开始
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
static void advertising_start(void)
{
//    uint32_t err_code = ble_advertising_start(BLE_ADV_MODE_FAST);

//    APP_ERROR_CHECK(err_code);

//    bsp_board_led_on(ADVERTISING_LED_PIN);

	uint32_t             err_code;
    ble_gap_adv_params_t adv_params;

    // Start advertising
    memset(&adv_params, 0, sizeof(adv_params));

    adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    adv_params.p_peer_addr = NULL;
    adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    adv_params.interval    = APP_ADV_INTERVAL;
    adv_params.timeout     = APP_ADV_TIMEOUT_IN_SECONDS;

    err_code = sd_ble_gap_adv_start(&adv_params);
    APP_ERROR_CHECK(err_code);


}
// end of static void advertising_start(void)
/*******************************************************************************
*                           王宇@2014-04-19
* Function Name  :  BLE_Stack_Init
* Description    :  BLE栈初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void BLE_Stack_Init(void)
{
    uint32_t err_code;

    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, softdevice_evt_schedule);
//    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT, PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
#if (NRF_SD_BLE_API_VERSION == 3)
    ble_enable_params.gatt_enable_params.att_mtu = NRF_BLE_MAX_MTU_SIZE;
#endif
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);
    
}// End of void BLE_Stack_Init(void)
/*******************************************************************************
*                           王宇@2017-04-19
* Function Name  :  BLE_Init
* Description    :  BLE初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void BLE_Init(void)
{
    BLE_Stack_Init();
    gap_params_init(); 
    services_init();
    advertising_init(); 
    conn_params_init();
    advertising_start();

}// End of void BLE_Init(void)
/*******************************************************************************
//*                           陈风定@2017-3-2
//* Function Name  :  tem_send_handler
//* Description    :  温度发送至手机APP
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*****************************************************************************/
//void tem_send_handler(void *params)
//{
//        u8 Temp = 0;
//        uint8_t arry[20];
//        Get_Temerature(&Temp); 
//        sprintf(arry, "tem:%d %d", (char)(Temp),Set_Temp);
//        ble_tems_string_send(&m_tems, arry, 20);
//}
/*******************************************************************************
//*                           陈风定@2017-2-14
//* Function Name  :  BLE_Task_Create
//* Description    :  创建Button任务
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*****************************************************************************/
//void BLE_Task_Create(void)
//{
//    u32 err_code = NRF_SUCCESS;
//    // 配置参数 周期模式运行
//    BLE_Task.Run_Mode        = APP_TIMER_MODE_REPEATED;
//    BLE_Task.Timeout_handler = tem_send_handler;
//    BLE_Task.Period          = 1000;

//    BLE_Task.p_ID = &BLE_Task.ID;
//    err_code = app_timer_create(&BLE_Task.p_ID,
//                                 BLE_Task.Run_Mode,
//                                 BLE_Task.Timeout_handler);
//    
//    if (err_code != NRF_SUCCESS)
//    {
//         NRF_LOG_ERROR("BLE create failed!\r\n"); 
//    }
//    else
//    {
//      Task_Timer_Start(&BLE_Task, NULL);
//    }
//}// End of void Button_Task_Create(void)
/******************* (C) COPYRIGHT 2017 王宇 **************END OF FILE*********/
