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
#include "i2c.h"
#include "keyboard.h"
//#include "ADCmeasurement.h"
#include "serial.h"//only for test
#include "utils.h"
#include "interpolation.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    idxValue0 = 0,
    idxValue100,
} PointsValues_t;

typedef enum {
    cp8procent = 0,
    cp10procent,
    cp14procent,
    cp18procent,
    cp24procent,
    cp30procent,
    cp50procent,
    cp70procent,
    cp90procent
} HCalibrationPoint_t;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
CalibrationTypeDef CalibrationValues[MAXTCHANNEL];

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
  ReadCalibration(&CalibrationValues[Channel5],Channel5);
  ReadCalibration(&CalibrationValues[Channel6],Channel6);
  
  for(size_t Channel=0;Channel<MAXTCHANNEL;Channel++){//set pints
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


void HCalibrationProcess(channel_t Channal){//(uint16_t* CalibValuesINDX,CalibrationTypeDef* CalibValues){
  uint32_t ticks;
  uint32_t DevTicksRef100ms = 0; 
  uint32_t DevTicksRef10ms  = 0; 
  uint32_t DevTicksRef500ms = 0;
  uint32_t DevTicksRef1s    = 0;
  uint32_t DevTicksRef2_5s  = 0;
  uint32_t DevTicksRef5s    = 0;
  char KeyPressed;
  unsigned int CurrADCValues[MAXHCHANNEL];
  float CurrVoltage = 0;
  //bool GeneratorVal = true;
  typedef enum {
    stWaitingForValue8,
    stWaitingForValue10,
    stWaitingForValue14,
    stWaitingForValue18,
    stWaitingForValue24,
    stWaitingForValue30,
    stWaitingForValue50,
    stWaitingForValue70,
    stWaitingForValue90,
    stSaveAndExit
  } states_t;
  states_t State=stWaitingForValue8;
  
  for(;;){
    //=======redraw calibration window heder==========
    printf("\x1b[2J");//terminal clear screen command
    switch (Channal)
    {
    case Channel1:
      printf("======= Selected channel: 1 ======\r\n");
      break;
    case Channel2:
      printf("======= Selected channel: 2 ======\r\n");
      break;
    case Channel3:
      printf("======= Selected channel: 3 ======\r\n");
      break;
    case Channel4:
      printf("======= Selected channel: 4 ======\r\n");
      break;
    } 
    switch (State){
     case stWaitingForValue8:
       printf("Press \"Enter\" to save 8'%' calibration value\r\n");
       break;
     case stWaitingForValue10:
       printf("Press \"Enter\" to save 10'%' calibration value\r\n");
       break;
     case stWaitingForValue14:
       printf("Press \"Enter\" to save 14'%' calibration value\r\n");
       break;
     case stWaitingForValue18:
       printf("Press \"Enter\" to save 18'%' calibration value\r\n");
       break;
     case stWaitingForValue24:
       printf("Press \"Enter\" to save 24'%' calibration value\r\n");
       break;
     case stWaitingForValue30:
       printf("Press \"Enter\" to save 30'%' calibration value\r\n");
       break;
     case stWaitingForValue50:
       printf("Press \"Enter\" to save 50'%' calibration value\r\n");
       break;
     case stWaitingForValue70:
       printf("Press \"Enter\" to save 70'%' calibration value\r\n");
       break;
     case stWaitingForValue90:
       printf("Press \"Enter\" to save 90'%' calibration value\r\n");
       break;
     default:
       //assert(false);
       break;
     }
    //=======redraw calibration window heder===========
    KeyPressed=0;
    do
    {
        ticks = DevTicks;
        if((ticks - DevTicksRef10ms) >= 10){
          KeyPressed=get_char();
          DevTicksRef10ms = ticks;     
        } 
        /* =============================================================
         100 msec process 
        ================================================================*/
        if((ticks - DevTicksRef100ms) >= 100){ // 100ms   
          DevTicksRef100ms = ticks;
          
        }
        
        /* =============================================================
         500 msec process 
        ================================================================*/
        if((ticks - DevTicksRef500ms) >= 500){ // 500 msec
          DevTicksRef500ms = ticks;
          
          //printf("\x1b[2J");//terminal clear screen command
          printf("\r%d  volt:%.3f",CurrADCValues[Channal],CurrVoltage);
        } 
        
        /* =============================================================
        1 sec process
        ================================================================*/
        if((ticks - DevTicksRef1s) >= 1000){ // 1s      
          DevTicksRef1s = ticks;
          
          //TestProg();      
        }
        /* =============================================================
        2,5 sec process
        ================================================================*/
        if((ticks - DevTicksRef2_5s) >= 2500){ // 2,5 s      
          DevTicksRef2_5s = ticks;
          
          //if(GeneratorVal){
            //GetADCValues(tempADCValues2,sizeof(tempADCValues2)/sizeof(tempADCValues2[0]));
          //}else{
            //GetADCValues(tempADCValues1,sizeof(tempADCValues1)/sizeof(tempADCValues1[0]));
          //}
          
          //CurrADCValues[Channal] = (tempADCValues1[Channal]+tempADCValues2[Channal])/2;
          CurrADCValues[Channal] = GetADCValue(Channal);
          CurrVoltage = (float)(CurrADCValues[Channal]*0.80586);
          CurrVoltage = CurrVoltage/1000;
          //SetGenerator(&GeneratorVal);
        }
        /* =============================================================
        5 sec process
        ================================================================*/
        if((ticks - DevTicksRef5s) >= 5000){ // 5s 
          DevTicksRef5s = ticks;
 
        }
        //==================================================================     
    }while(KeyPressed!=CR);
  
     switch (State)
     {  
        case stWaitingForValue8:
          SaveHCalibrationPoint(Channal,cp8procent);
          State=stWaitingForValue10;
          break;
        case stWaitingForValue10:
          SaveHCalibrationPoint(Channal,cp10procent);
          State=stWaitingForValue14;
          break;
        case stWaitingForValue14:
          SaveHCalibrationPoint(Channal,cp14procent);
          State=stWaitingForValue18;
          break;
        case stWaitingForValue18:
          SaveHCalibrationPoint(Channal,cp18procent);
          State=stWaitingForValue24;
          break;
        case stWaitingForValue24:
          SaveHCalibrationPoint(Channal,cp24procent);
          State=stWaitingForValue30;
          break;
        case stWaitingForValue30:
          SaveHCalibrationPoint(Channal,cp30procent);
          State=stWaitingForValue50;
          break;
        case stWaitingForValue50:
          SaveHCalibrationPoint(Channal,cp50procent);
          State=stWaitingForValue70;
          break;
        case stWaitingForValue70:
          SaveHCalibrationPoint(Channal,cp70procent);
          State=stWaitingForValue90;
          break;
        case stWaitingForValue90:
          SaveHCalibrationPoint(Channal,cp90procent);
          State=stSaveAndExit;
          break;
        default:
          break;
     }
     if(State==stSaveAndExit){
       WriteCalibration(&WriteCalibrationValues,Channal);
       return;
     }
  }
}



