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
/* Exported functions ------------------------------------------------------- */
void SetBoardAddress (uint32_t Addr);
void I2C_GetMagic(uint8_t* nr);

void I2C_ScannBoards(void);
void I2C_PrintBoardsList(void);
uint8_t I2C_GetBoardsNr(void);
void SelectBoardNr(uint8_t nr);
/* External variables --------------------------------------------------------*/



#endif  /*__I2C_H*/

/******************* (C) COPYRIGHT 2011 Valentina Pavlovskis ****END OF FILE****/