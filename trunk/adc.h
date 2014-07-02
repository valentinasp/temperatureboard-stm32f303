/**
  ******************************************************************************
  * @file    adc.h
  * @author  
  * @version V0.0.1
  * @date    2012-11-16
  * @brief   This file contains all the functions prototypes 
  *          low level I/O functions for adc module
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>     /* qsort */  
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
   typedef enum 
{ 
  adc_channel_0 = 0,
  adc_channel_1,
  adc_channel_2,
  adc_channel_3,
  adc_channel_4,
  adc_channel_5,
  adc_channel_6 //Internel temperature
} ADC_Channels;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//void ADC_init(void);
void DisableADC1(void);
void EnableADC1(void);
void ADC_Initialization(void);

/**
  * @brief  Returns the last ADC conversion result data for selected channel.  
  * @param  Channalx: where x can be 1 to select the Channel1 peripheral.
  * @retval The Data conversion value.
  */
uint16_t ADC_GetChannelConversionValue(size_t Channalx);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */


//ADC module

