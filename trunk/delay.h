/******************** (C) COPYRIGHT 2013  **************************************
* File Name          : delay.h
* Author             : Valentinas
* Version            : V1.0.0
* Date               : 9/3/2013
* Description        : Delay functions prototypes
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H

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
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime);

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void);

#ifdef __cplusplus
}
#endif

#endif  /*__DELAY_H*/

/******************* (C) COPYRIGHT ****************END OF FILE****/