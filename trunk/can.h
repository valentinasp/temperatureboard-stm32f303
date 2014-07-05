/******************** (C) COPYRIGHT 2013  **************************************
* File Name          : can.h
* Author             : vpa
* Version            : V1.0.0
* Date               : 05/07/2014
* Description        : can config functions prototypes
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H
#define __CAN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"

#ifdef __cplusplus
 extern "C" {
#endif
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialisation the CAN.
  * @param  None
  * @retval None
  */
void InitCAN(void);


#ifdef __cplusplus
}
#endif

#endif  /*__CAN_H*/

/******************* (C) COPYRIGHT ****************END OF FILE****/