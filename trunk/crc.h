/******************** (C) COPYRIGHT 2011 Lifodas *******************************
* File Name          : crc.h
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/16/2011
* Description        : CRC Calculator
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRC_H
#define __CRC_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

//Function return calulated CRC value
//crc - start value
//ptr - sourse data
//len - calculation length
uint32_t calc_crc(uint32_t crc, uint32_t * ptr, uint32_t len);

uint8_t calc_crc8(uint8_t crc, uint8_t * ptr, uint32_t len);
/* External variables --------------------------------------------------------*/

#endif  /*__CRC_H*/

/******************* (C) COPYRIGHT 2011 Lifodas ****************END OF FILE****/
