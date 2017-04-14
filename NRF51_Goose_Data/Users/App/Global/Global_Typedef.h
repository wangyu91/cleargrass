/******************** (C) COPYRIGHT 2016 Â½³¬ **********************************
* File Name          :  Global_Typedef.h
* Author             :  Â½³¬
* CPU Type           :  nRF51802
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  12/25/2016
* Description        :  Global_Typedef header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_TYPEDEF_H
#define __GLOBAL_TYPEDEF_H
/* Includes ------------------------------------------------------------------*/
//#include "Global_Cfg.h"
//#include "Struct.h"
#include "nrf51.h"

/* Private define ------------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */


/* Private variables ---------------------------------------------------------*/



#endif /* __GLOBAL_TYPEDEF_H */

/******************* (C) COPYRIGHT 2016 Â½³¬ ********END OF FILE***************/

