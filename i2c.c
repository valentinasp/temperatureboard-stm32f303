/******************** (C) COPYRIGHT 2014 Valentinas Pavlovskis *****************
* File Name          : i2c.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 25/06/2014
* Description        : I2C functions
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"
#include <stdint.h>
#include "stdio.h"
#include <stdlib.h> 
#include "delay.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAX_BUFF_SIZE           200
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* CPAL local transfer structures */
CPAL_TransferTypeDef  sRxStructure, sTxStructure;
uint32_t BufferSize = MAX_BUFF_SIZE;
uint8_t tRxBuffer[MAX_BUFF_SIZE];
uint8_t tTxBuffer[MAX_BUFF_SIZE];
int8_t drv_i2c_err = 0;

uint8_t BoardsNr = 0;
uint8_t *BoardsArray;

uint8_t SelectedBoard = 0;
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static int8_t drv_i2c_EOT_Wait(void);
static int8_t drv_i2c_WriteBuffer(const uint8_t *Data, uint16_t DataLength);
static int8_t drv_i2c_ReadBuffer(uint8_t * buf, uint16_t len, uint16_t cmd);
/* Private functions ---------------------------------------------------------*/

void I2C_GetMagic(uint8_t* nr){
  tTxBuffer[0] = 0x00;
  tRxBuffer[0] = 0x00;
  drv_i2c_WriteBuffer(tTxBuffer,1); 
  drv_i2c_ReadBuffer(tRxBuffer,1,0x00);
  *nr = tRxBuffer[0];
}

void SelectBoardNr(uint8_t nr){
  SelectedBoard = nr;
}

void I2C_ScannBoards(void){
  uint8_t mnr = 0;
  BoardsNr = 0;
  
  for(size_t i=0;i<127;i++){
    SetBoardAddress (i);
    mnr = 0;
    Delay(5);
    //try get magic nr
    I2C_GetMagic(&mnr);
    if (mnr==0xAB){
      BoardsNr++;
      BoardsArray=realloc(BoardsArray,BoardsNr*sizeof(uint8_t));
      BoardsArray[BoardsNr-1]=i;
    }
  }
}

void I2C_PrintBoardsList(void){
  for(size_t i=0;i<BoardsNr;i++){
    printf("| %d               |       0x%02X          |   Humidity Board              |\r\n",i+1,BoardsArray[i]);
  }
}
uint8_t I2C_GetBoardsNr(void){
  return BoardsNr;
}

static int8_t drv_i2c_ReadBuffer(uint8_t * buf, uint16_t len, uint16_t cmd)
{
 /* If error return */
 if(drv_i2c_err){return 1;}

 /* Wait the end of transfer */
 drv_i2c_err = drv_i2c_EOT_Wait();

 if(!drv_i2c_err){
   sRxStructure.wNumData = len;
   sRxStructure.pbBuffer = buf;
   sRxStructure.wAddr2 = cmd;
   I2C_DevStructure.pCPAL_TransferRx = &sRxStructure;
   I2C_DevStructure.wCPAL_Options &= ~CPAL_OPT_NO_MEM_ADDR;
   I2C_DevStructure.wCPAL_Options &= ~CPAL_OPT_16BIT_REG;
   if(CPAL_I2C_Read(&I2C_DevStructure) != CPAL_PASS){
     /* I2C bus or peripheral is not able to start communication: Error management */
     drv_i2c_err = 1;
   }else{
     /* Wait the end of transfer */
     drv_i2c_err = drv_i2c_EOT_Wait();
   }
 }
 return drv_i2c_err;
}

static int8_t drv_i2c_WriteBuffer(const uint8_t *Data, uint16_t DataLength)
{
  /* If error return */
  if(drv_i2c_err){return 1;}
  
  if(!drv_i2c_err){
    I2C_DevStructure.wCPAL_Options |= CPAL_OPT_NO_MEM_ADDR;
    I2C_DevStructure.wCPAL_Options &= ~CPAL_OPT_16BIT_REG;
    
    sTxStructure.wNumData = DataLength; /* Number of data to be written */
    sTxStructure.pbBuffer = (uint8_t*)Data; /* Power Monitor buffer */
    I2C_DevStructure.pCPAL_TransferTx = &sTxStructure;

    /* Wait the end of transfer */
    drv_i2c_err = drv_i2c_EOT_Wait();
  
    if(CPAL_I2C_Write(&I2C_DevStructure) != CPAL_PASS){
      /* I2C bus or peripheral is not able to start communication: Error management */
      drv_i2c_err = 1;
    }
    else{
      /* Wait the end of transfer */
      drv_i2c_err = drv_i2c_EOT_Wait();
    }
    
  }

  return drv_i2c_err;
}

/**
  * @brief  Wait End-Of-Transfer of I2C driver .
  * @param  None
  * @retval : error
  */
static int8_t drv_i2c_EOT_Wait(void)
{
  /* If error return */
  if(drv_i2c_err){return drv_i2c_err;}

  /* Wait the end of transfer */
  while(I2C_DevStructure.CPAL_State != CPAL_STATE_READY){
    if(I2C_DevStructure.wCPAL_DevError != CPAL_I2C_ERR_NONE){
      drv_i2c_err = 1;
      break;
    }
  }
  return drv_i2c_err;
}

void InitCPAL(void){
  
  /* Initialize local Reception structures */
  sRxStructure.wNumData = BufferSize;       /* Maximum Number of data to be received */
  sRxStructure.pbBuffer = 0;//tRxBuffer;        /* Common Rx buffer for all received data */
  sRxStructure.wAddr1 = 0x30;//0;                  /* Not needed */
  sRxStructure.wAddr2 = 0;                  /* Not needed */
  
  /* Initialize local Transmission structures */
  sTxStructure.wNumData = 0;//BufferSize;       /* Maximum Number of data to be received */
  sTxStructure.pbBuffer = &tTxBuffer[0];     /* Common Rx buffer for all received data */
  sTxStructure.wAddr1 = 0x30;//OWN_ADDRESS;        /* The own board address */
  sTxStructure.wAddr2 = 0;//0;                  /* Not needed */
  
  /* Set HSI as I2C clock source */
  RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);
  
  /* Configure the device structure */
  CPAL_I2C_StructInit(&I2C_DevStructure);      /* Set all fields to default values */
  I2C_DevStructure.CPAL_Mode = CPAL_MODE_MASTER;
  I2C_DevStructure.wCPAL_Options =  CPAL_OPT_NO_MEM_ADDR ;
#ifdef CPAL_I2C_DMA_PROGMODEL
  I2C_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_DMA;
#elif defined (CPAL_I2C_IT_PROGMODEL)
  I2C_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
#else
 #error "Please select one of the programming model (in stm32f30x_i2c_cpal_conf.h)"
#endif
  I2C_DevStructure.pCPAL_I2C_Struct->I2C_Timing = I2C_TIMING;
  I2C_DevStructure.pCPAL_I2C_Struct->I2C_OwnAddress1 = OWN_ADDRESS;
  I2C_DevStructure.pCPAL_TransferRx = &sRxStructure;
  I2C_DevStructure.pCPAL_TransferTx = &sTxStructure;
  
  /* Initialize CPAL device with the selected parameters */
  CPAL_I2C_Init(&I2C_DevStructure);
  
  sRxStructure.wNumData = 0; /* Number of data to be read */
  sRxStructure.pbBuffer = &tRxBuffer[0];//(uint8_t*)Data; /* Transmit buffer */
  sRxStructure.wAddr1 = 0x30; /*Command register address*/
  sRxStructure.wAddr2 = 0x00; /*Command register address*/
  I2C1_DevStructure.pCPAL_TransferRx = &sRxStructure;
  I2C1_DevStructure.wCPAL_Options &= ~CPAL_OPT_NO_MEM_ADDR;
  I2C1_DevStructure.wCPAL_Options &= ~CPAL_OPT_16BIT_REG;//8 bit
  
}

void SetBoardAddress (uint32_t Addr){
  sTxStructure.wAddr1=Addr;
  sRxStructure.wAddr1=Addr;
}