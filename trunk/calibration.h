/******************** (C) COPYRIGHT 2011  **************************************
* File Name          : calibration.h
* Author             : Valentinas
* Version            : V2.0.0
* Date               : 12/16/2011
* Description        : Calibracion functions prototypes
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CALIBRATION_H
#define __CALIBRATION_H

/* Includes ------------------------------------------------------------------*/
//#include "startup.h"
#include "type.h"
//#include "util.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/
#define MAGIC 0xCA123CA
#define CALIBRATION_VALUES_AREA_BEG_ADDR	0x08080000
   
#define sCALIB_ERRORLED_PIN                     GPIO_Pin_9
#define sCALIB_ERRORLED_GPIO_PORT               GPIOB
#define sCALIB_ERRORLED_GPIO_CLK                RCC_AHBPeriph_GPIOB 

#define sGENERATOR_LED_PIN                      GPIO_Pin_8
#define sGENERATOR_LED_GPIO_PORT                GPIOB
#define sGENERATOR_LED_GPIO_CLK                 RCC_AHBPeriph_GPIOB
//Generator pin definition
#define sGENERATOR_PIN                          GPIO_Pin_0
#define sGENERATOR_GPIO_PORT                    GPIOB
#define sGENERATOR_GPIO_CLK                     RCC_AHBPeriph_GPIOB
/* Exported macro ------------------------------------------------------------*/
   
/* Data LED: Data LED off*/ 
#define CALIB_DATA_LED_LOW()   GPIO_ResetBits(sCALIB_DATALED_GPIO_PORT, sCALIB_DATALED_PIN)
/* Data LED: Data LED on */
#define CALIB_DATA_LED_HIGH()  GPIO_SetBits(sCALIB_DATALED_GPIO_PORT, sCALIB_DATALED_PIN)

/* Generator: Generator pin off*/ 
#define GENERATOR_LOW()         GPIO_ResetBits(sGENERATOR_GPIO_PORT, sGENERATOR_PIN)
/* Generator: Generator pin on */
#define GENERATOR_HIGH()        GPIO_SetBits(sGENERATOR_GPIO_PORT, sGENERATOR_PIN)

/* Generator LED: Generator LED off*/ 
#define GENERATOR_LED_LOW()     GPIO_ResetBits(sGENERATOR_LED_GPIO_PORT, sGENERATOR_LED_PIN)
/* Generator LED: Generator LED on */
#define GENERATOR_LED_HIGH()    GPIO_SetBits(sGENERATOR_LED_GPIO_PORT, sGENERATOR_LED_PIN)  

/* Read sCalibration start bit */   
#define IOCALIB_PIN()          GPIO_ReadInputDataBit(CALIBRATION_PORT, CALIBRATION_PIN)

#define MAXCHANNEL 4
#define MAXPOINTS 9 //8,10,14,18,24,30,50,70,90 pecents

#pragma pack (push, 1)
typedef struct{
  volatile unsigned long cbMagic;//Magic number used for validate image
  volatile unsigned long cbCalibrValue8; // 8 percent
  volatile unsigned long cbCalibrValue10;// 10 percent
  volatile unsigned long cbCalibrValue14;// 14 percent
  volatile unsigned long cbCalibrValue18;// 18 percent
  volatile unsigned long cbCalibrValue24;// 24 percent
  volatile unsigned long cbCalibrValue30;// 30 percent
  volatile unsigned long cbCalibrValue50;// 50 percent
  volatile unsigned long cbCalibrValue70;// 70 percent
  volatile unsigned long cbCalibrValue90;// 90 percent
  volatile unsigned long cbCRC;//Examine cabibration values crc
} CalibrationTypeDef;
#pragma pack (pop)

typedef enum {
              Channel1=0,
              Channel2,
              Channel3,
              Channel4
} channel_t;

/* Exported functions ------------------------------------------------------- */
void InitCalibration(void);
void InitGenerator(void);
void CalibrationProcess(channel_t Channal);
void LoadCalibrationData(void);

//void CalibrationProcess(uint16_t* CalibValuesINDX,CalibrationTypeDef* CalibValues);

bool ReadCalibration(CalibrationTypeDef* CalibValues,unsigned int offset);
bool WriteCalibration(CalibrationTypeDef* CalibValues,unsigned int offset);
bool EraseCalibration(void);

// Function set and invert generator value
void SetGenerator(bool *value);

/* External variables --------------------------------------------------------*/
extern CalibrationTypeDef CalibrationValues[4];


#endif  /*__CALIBRATION_H*/

/******************* (C) COPYRIGHT ****************END OF FILE****/
