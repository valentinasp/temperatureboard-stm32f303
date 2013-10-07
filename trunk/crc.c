/******************** (C) COPYRIGHT 2011 Lifodas *******************************
* File Name          : crc.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 14/12/2011
* Description        : CRC calculator
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "crc.h"
#include "stdint.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

uint32_t calc_crc(uint32_t crc, uint32_t * ptr, uint32_t len)
{
  uint32_t i;
  for(i=0; i < len; i++){
      crc ^= *ptr++;
  }
  return crc;
}

uint8_t calc_crc8(uint8_t crc, uint8_t * ptr, uint32_t len)
{
  uint32_t i;
  for(i=0; i < len; i++){
      crc ^= *ptr++;
  }
  return crc;
}
