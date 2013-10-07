//
//keyboard functions
//
#include "stm32f30x.h"
#include "keyboard.h"
#include "hw_config.h"



#define LONG_KEYPRESS 4 //SHORT_KEYPRESS*LONG_KEYPRESS
#define SHORT_KEYPRESS 15



// Keyboard device driver

/* Private typedef -----------------------------------------------------------*/
#define KEYB_NOKEY 0x00

/* Private define ------------------------------------------------------------*/

#define KEYB_DEBOUNCE_CNT 1

/* Keyboard Interface pins  */  
#define CALIBRATION_PIN           GPIO_Pin_12
#define CALIBRATION_PORT          GPIOB
#define CALIBRATION_CLK           RCC_AHBPeriph_GPIOB

#define POWER_PIN                 GPIO_Pin_1
#define POWER_PORT                GPIOA
#define POWER_CLK                 RCC_AHBPeriph_GPIOA

/* Private macro -------------------------------------------------------------*/
   
/* Read key input bit */  
#define READ_KEY_CALIB()          GPIO_ReadInputDataBit(CALIBRATION_PORT, CALIBRATION_PIN)
#define READ_KEY_ONOFF()          GPIO_ReadInputDataBit(POWER_PORT, POWER_PIN)

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


// Key State structure
typedef struct KeyState_T_
{
	Uint16 KeyLock; // Key Locked Flag
	Uint16 KeyPressed; // Key Pressed Counter
} KeyState_T;

/****************************************************************/
// Function Prototypes
/****************************************************************/
void KeySetKeyLock(uint8_t indx);
Uint16 drv_Key_Get(Uint8 key);
void drv_Key_Scan_GPIO();
uint32_t INPReadBit(INP_TypeDef KEY);
static void KeyScanBut(INP_TypeDef INport, uint8_t indx);

//last pressed key
static Key_t LastKeyPressed;
static bool KeyPressed;
static uint16_t KeyCalibTickValue = 0,
                KeyONOFFTickValue = 0;

static uint8_t KeyCalibLongTickValue;//,KeyONOFFLongTickValue;

static uint16_t TickValue=SHORT_KEYPRESS;

static void (*KeyboardTask) (void) = 0;//NULL;


uint8_t KeyCnt[KEY_N];
KeyStateS KeyState;
uint8_t KeyEmulationModeOn = 0;

void KeyScan(void)
{ 
  // scan buttons
  KeyScanBut(INP_BUT_CALIB, KEYB_CALIBRATION_INDX);
  //KeyScanBut(INP_BUT_ONOFF, KEYB_POWER_INDX);
 
  KeyTranslate();
}


uint32_t INPReadBit(INP_TypeDef KEY)
{
  switch(KEY){
    case KEY_CALIB:
      return READ_KEY_CALIB();
    case KEY_ONOFF:
      return READ_KEY_ONOFF();    
    default:
      return 0;
  }
}

Key_t GetKeyPressed(void)    //wait for key pressed & return it; blocking
{
  uint32_t ticks;
  KeyStateS * key;
  uint32_t DevTicksRef10ms  = 0; 

  /* infinite loop */
  for(;;){
    ticks = DevTicks;
    if((ticks - DevTicksRef10ms) >= 1){
      KeyScan(); // keyboard scan
      key = KeyGet(); // get keyboard values
      DevTicksRef10ms = ticks;
      
      if(key->Pressed[KEYB_CALIBRATION_INDX]){
        KeyCalibTickValue++;
      }else if(key->Pressed[KEYB_POWER_INDX]){
        KeyONOFFTickValue++;	
      }else{  
        KeyPressed = false;
        TickValue=SHORT_KEYPRESS;
        
        //KeyONOFFLongTickValue = 0;
        KeyCalibLongTickValue = 0;

        KeyCalibTickValue = 0;
        //KeyONOFFTickValue = 0;
      }
      //------
      if (KeyCalibTickValue > TickValue){
        if (KeyCalibLongTickValue >= LONG_KEYPRESS){
          TickValue = LONG_KEYPRESS;
        }else{
          KeyCalibLongTickValue++;
        }
        KeyPressed = true;
        LastKeyPressed = KEY_CALIB;
        KeyCalibTickValue = 0;
      }
/*
      if (KeyONOFFTickValue > TickValue){
        if (KeyONOFFLongTickValue >= LONG_KEYPRESS){
          TickValue = LONG_KEYPRESS;
        }else{
          KeyONOFFLongTickValue++;
        }
        KeyPressed = true;
        LastKeyPressed = KEY_ONOFF;
        KeyONOFFTickValue = 0;
      }
*/     
      if (KeyONOFFTickValue > TickValue){
        KeyPressed = true;
        LastKeyPressed = KEY_ONOFF;
        KeyONOFFTickValue = 0;
      }
     
      if(KeyPressed)
      {
         KeyPressed = false;
         return LastKeyPressed;
      }
    }    
  }  
}

bool IsKeyPressed(void)   //check do we have key pressed; non-blocking, analog to kbhit()
{
   KeyPressed = false;

   if (KeyboardTask != NULL) {
      KeyboardTask();
   }

   if(drv_Key_Get(KEY_ONOFF)){
		KeyPressed = true;
   }
   else if(drv_Key_Get(KEY_CALIB)){
  		KeyPressed = true;
   } 

   return KeyPressed;
}




static void KeyScanBut(INP_TypeDef INport, uint8_t indx)
{
  if (indx > KEY_N - 1) return;
  
  if (KeyEmulationModeOn) {
    if (KeyState.Code & (0x01 << indx)){
      KeyState.Pressed[indx]++;
    }
    else{
      KeyState.Pressed[indx] = 0;
    }
    return;
  }
  
  
  if (INPReadBit(INport)){//(BOARD_INPReadBit(INport)){
    KeyCnt[indx]++;
    if (KeyCnt[indx] >= KEYB_DEBOUNCE_CNT){
      KeyCnt[indx] = KEYB_DEBOUNCE_CNT;
      KeyState.Pressed[indx]++;
      KeyState.Code |= (0x01 << indx);
    }
  }else{
    KeyState.Pressed[indx] = 0;
    if (KeyCnt[indx] > 0){
      KeyCnt[indx]--;
    }
    if (KeyCnt[indx] == 0){
      KeyState.Code &= ~(0x01 << indx);
    }
  }
}

/****************************************************************/
// Local variables
/****************************************************************/

// Keyboard State
//KeyState_T KeyState[KEY_N] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};

/****************************************************************/
// Functions
/****************************************************************/
void KeyInit(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  /*!< Enable GPIO clocks */
  RCC_AHBPeriphClockCmd(CALIBRATION_CLK, ENABLE);
  //Init Calibration Button
  GPIO_InitStructure.GPIO_Pin =  CALIBRATION_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CALIBRATION_PORT, &GPIO_InitStructure);
}

KeyStateS * KeyGet(void)
{
  return &KeyState;
}

void KeyTranslate(void)
{
  int i;
  // remove button lock if key released
  for (i = 0; i < KEY_N; i++){
    if ((KeyState.Code & (1 << i)) == KEYB_NOKEY && (!KeyCnt[i] || KeyEmulationModeOn)){
      KeyState.Lock[i] = 0;
    }
  }  
}

void KeySetKeyLock(uint8_t indx)
{
  KeyState.Lock[indx] = 1;
}

int KeyToBit(uint8_t keyCode)
{
  int i;
  for(i = 0; i < KEY_N; i++){
    if (keyCode & (1 << i)){
      return i+1;
    }
  }
  return 0;
}

void KeyEmulationModeSet(uint8_t new_mode)
{
  if (new_mode == RESET){
    KeyEmulationModeOn = RESET;
  }
  else{
    KeyEmulationModeOn = SET;
  }
}

void KeyEmulateCode(uint8_t keyCode)
{
  if (KeyEmulationModeOn){
    KeyState.Code = keyCode;
  }
}


