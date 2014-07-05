/******************** (C) COPYRIGHT 2013  **************************************
* File Name          : ADCmeasurement.h
* Author             : Valentinas
* Version            : V1.0.0
* Date               : 9/3/2013
* Description        : ADC module measurement functions prototypes
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADCMEASUREMENT_H
#define __ADCMEASUREMENT_H

/* Includes ------------------------------------------------------------------*/
#include "adc.h"
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
bool GetADCValues(unsigned int* Values,size_t ValuesNr);
unsigned int GetTSensorADCValue(size_t channel);

#ifdef __cplusplus
}
#endif

#endif  /*__ADCMEASUREMENT_H*/

/******************* (C) COPYRIGHT ****************END OF FILE****/