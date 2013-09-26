/*
* File Name          : calibration.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 14/12/2011
* Description        : Calibration
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include "calibration.h"
#include "crc.h"
#include "eeprom.h"
#include "keyboard.h"
#include "ADCmeasurement.h"
#include "serial.h"//only for test
#include "utils.h"
#include "interpolation.h"
//#include "util.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    idxValue8 = 0,
    idxValue10,
    idxValue14,
    idxValue18,
    idxValue24,
    idxValue30,
    idxValue50,
    idxValue70,
    idxValue90
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
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(sCALIB_ERRORLED_GPIO_PORT, &GPIO_InitStructure);  
}


void LoadCalibrationData(void){
  double ADCval[MAXPOINTS]={0,0,0,0,0,0,0,0,0};
  double Points[MAXPOINTS]={
                          8,    // 1
                          10,   // 2
                          14,   // 3
                          18,   // 4
                          24,   // 5
                          30,   // 6
                          50,   // 7
                          70,   // 8
                          90    // 9
  };
  ReadCalibration(&CalibrationValues[Channel1],Channel1);
  ReadCalibration(&CalibrationValues[Channel2],Channel2);
  ReadCalibration(&CalibrationValues[Channel3],Channel3);
  ReadCalibration(&CalibrationValues[Channel4],Channel4);
  
  for(size_t Channel=0;Channel<MAXCHANNEL;Channel++){//set pints
    ADCval[idxValue8] = CalibrationValues[Channel].cbCalibrValue8;
    ADCval[idxValue10]= CalibrationValues[Channel].cbCalibrValue10;
    ADCval[idxValue14]= CalibrationValues[Channel].cbCalibrValue14;
    ADCval[idxValue18]= CalibrationValues[Channel].cbCalibrValue18;
    ADCval[idxValue24]= CalibrationValues[Channel].cbCalibrValue24;
    ADCval[idxValue30]= CalibrationValues[Channel].cbCalibrValue30;
    ADCval[idxValue50]= CalibrationValues[Channel].cbCalibrValue50;
    ADCval[idxValue70]= CalibrationValues[Channel].cbCalibrValue70;
    ADCval[idxValue90]= CalibrationValues[Channel].cbCalibrValue90;
    SetInterpolationValues(ADCval,Points,Channel);        
  }
}

bool ReadCalibration(CalibrationTypeDef* CalibValues,unsigned int offset){
  unsigned int i = sizeof(*CalibValues);
  if(EEPROM_If_Read_Buff (CALIBRATION_VALUES_AREA_BEG_ADDR+offset*sizeof(CalibrationTypeDef),
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
  DATA_EEPROM_Unlock();
  err = EEPROM_If_Write_Buff (CALIBRATION_VALUES_AREA_BEG_ADDR+offset*sizeof(CalibrationTypeDef),(unsigned char *)CalibValues, sizeof(CalibrationTypeDef));
  DATA_EEPROM_Lock();
  return (bool)err;
}


bool EraseCalibration(void){
  uint16_t err = 0;
  unsigned char TempValues[sizeof(CalibrationTypeDef)*4];//TODO: remove static 50 value
  for(int i=0;i<sizeof(CalibrationTypeDef);i++){
     TempValues[i]=0;
  }
  DATA_EEPROM_Unlock();
  err = EEPROM_If_Write_Buff (CALIBRATION_VALUES_AREA_BEG_ADDR,TempValues, sizeof(CalibrationTypeDef)*4);
  DATA_EEPROM_Lock();
  return (bool)err;
}



