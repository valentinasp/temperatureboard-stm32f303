/*
* File Name          : calibration.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 14/12/2011
* Description        : Calibration
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "flash_if.h"
#include "calibration.h"
#include "crc.h"
#include "eeprom.h"
#include "keyboard.h"
//#include "ADCmeasurement.h"
//#include "serial.h"//only for test
//#include "utils.h"
#include "interpolation.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    idxValue0 = 0,
    idxValue100,
} PointsValues_t;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
CalibrationTypeDef CalibrationValues[MAXCHANNEL];

static CalibrationTypeDef WriteCalibrationValues;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void InitCalibration(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  /*!< Enable GPIO clocks */
  RCC_AHBPeriphClockCmd(sCALIB_ERRORLED_GPIO_CLK, ENABLE);
  //Init LEDs  
  GPIO_InitStructure.GPIO_Pin =  sCALIB_ERRORLED_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(sCALIB_ERRORLED_GPIO_PORT, &GPIO_InitStructure); 
  
  /* EEPROM Init */
  EE_Init();
}


void LoadCalibrationData(void){
  double ADCval[MAXPOINTS]={0,0};
  double Points[MAXPOINTS]={
                          0,    // 1
                          100,   // 2
  };
  ReadCalibration(&CalibrationValues[Channel1],Channel1);
  ReadCalibration(&CalibrationValues[Channel2],Channel2);
  ReadCalibration(&CalibrationValues[Channel3],Channel3);
  ReadCalibration(&CalibrationValues[Channel4],Channel4);
  
  for(size_t Channel=0;Channel<MAXCHANNEL;Channel++){//set pints
    ADCval[idxValue0] = CalibrationValues[Channel].cbCalibrValue0;
    ADCval[idxValue100]= CalibrationValues[Channel].cbCalibrValue100;
    SetInterpolationValues(ADCval,Points,Channel);        
  }
}

bool ReadCalibration(CalibrationTypeDef* CalibValues,unsigned int offset){
  unsigned int i = sizeof(*CalibValues);
  if(EE_Read_Buff(CALIBRATION_VALUES_AREA_BEG_ADDR+offset*sizeof(CalibrationTypeDef),
                         (unsigned char *)CalibValues, 
                         i)!=0) 
    return false;
  if((calc_crc(0x55555555, (uint32_t *)CalibValues, 4) == 0)
      &&(CalibValues->cbMagic == MAGIC))
    return true;
  else 
    return false;
}

bool WriteCalibration(CalibrationTypeDef* CalibValues,unsigned int offset){
  uint16_t err=0;    
  CalibValues->cbMagic= MAGIC; 
  CalibValues->cbCRC = calc_crc(0x55555555, (uint32_t *)CalibValues, 3);
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();
  err = EE_Write_Buff(CALIBRATION_VALUES_AREA_BEG_ADDR+offset*sizeof(CalibrationTypeDef),(unsigned char *)CalibValues, sizeof(CalibrationTypeDef));
  /* Lock the Flash Program Erase controller */
  FLASH_Lock();
  return (bool)err;
}


bool EraseCalibration(void){
  uint16_t err = 0;
  unsigned char TempValues[sizeof(CalibrationTypeDef)*4];//TODO: remove static 50 value
  for(int i=0;i<sizeof(CalibrationTypeDef);i++){
     TempValues[i]=0;
  }
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();
  err = EE_Write_Buff(CALIBRATION_VALUES_AREA_BEG_ADDR,TempValues, sizeof(CalibrationTypeDef)*4);
  /* Lock the Flash Program Erase controller */
  FLASH_Lock();
  return (bool)err;
}



