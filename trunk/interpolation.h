/******************** (C) COPYRIGHT 2013  **************************************
* File Name          : interpolation.h
* Author             : Valentinas
* Version            : V1.0.0
* Date               : 9/3/2013
* Description        : Lagranz interpoliation functions prototypes
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H

/* Includes ------------------------------------------------------------------*/
#include <cstring>
#include "stm32f30x.h"
#include "type.h"

#ifdef __cplusplus
 extern "C" {
#endif
   

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

bool InitInterpolationValues(size_t CurrMaxChannelNr,size_t CurrMaxPoints);
bool DeinitInterpolationValues(void);
bool SetInterpolationValues(double* ADCvalues,double* PointsValues,size_t ChannelNr);

bool GetValue(size_t ChannelNr,double CurrADCValue,double* CalculatedValue); 
   

#ifdef __cplusplus
}
#endif

#endif  /*__DELAY_H*/

/******************* (C) COPYRIGHT ****************END OF FILE****/