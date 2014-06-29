/******************** (C) COPYRIGHT 2014 Valentina Pavlovskis ******************
* File Name          : i2c.h
* Author             : vpa
* Version            : V1.0.0
* Date               : 25/06/2014
* Description        : I2C functions
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "stm32303c_eval.h"
#include "stm32f30x_i2c_cpal.h"
#include "calibration.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define OWN_ADDRESS             0x68
#define I2C_TIMING              0x40110405 
#define I2C_DevStructure        I2C1_DevStructure

#define ACTION_NONE             0xFF
#define ACTION_DISABLED         0xFD
#define ACTION_PENDING          0xFE
#define ACTION_PERIODIC         0xFC

#define STATE_OFF               0
#define STATE_ON                1

/* Registers */
#define REG_MAGICREG            (0x00)  // Mmagic string
#define REG_VERREG              (0x01)	// Version register
#define REG_STAUSREG            (0x02)  // STM32 Status Register
#define REG_IDREG               (0x03)  // 
    
#define REG_CONTROLREG          (0x04)  
#define REG_CONTROLREG_CH       0x30
#define REG_CONTROLREG_POINT    0x0F
   
#define REG_RESERVED2           (0x05)         
#define REG_RESERVED3           (0x06)  
#define REG_RESERVED4           (0x07)  
    
#define REG_ADC0VALUEREG        (0x08)      
#define REG_ADC1VALUEREG        (0x0a)      
#define REG_ADC2VALUEREG        (0x0c)      
#define REG_ADC3VALUEREG        (0x0e)  
    
#define REG_HUMIDITY0REG        (0x10)     
#define REG_HUMIDITY1REG        (0x12)  
#define REG_HUMIDITY2REG        (0x14)      
#define REG_HUMIDITY3REG        (0x16) 
/* Exported functions ------------------------------------------------------- */
void SetBoardAddress (uint32_t Addr);
void I2C_GetMagic(uint8_t* nr);

void I2C_ScannBoards(void);
void I2C_PrintBoardsList(void);
uint8_t I2C_GetBoardsNr(void);
void SelectBoardNr(uint8_t nr);

void SaveHCalibrationPoint(uint8_t channel,uint8_t cpoint);
uint16_t GetADCValue(channel_t Channal);
/* External variables --------------------------------------------------------*/



#endif  /*__I2C_H*/

/******************* (C) COPYRIGHT 2011 Valentina Pavlovskis ****END OF FILE****/