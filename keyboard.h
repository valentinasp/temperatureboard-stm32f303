//
//keyboard functions
//
#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdint.h>

#include "type.h"
#include "stdio.h"
#include "stm32f30x_it.h"
//#include "drv_keyboard.h"

#define KEY_N          2

#define KEYB_CALIBRATION_INDX   0
#define KEYB_POWER_INDX         1


typedef enum 
{
  KEY_CALIB,
  KEY_ONOFF
} Key_t;

typedef struct KeyStateS_{
  uint8_t Code;
  uint8_t Lock[KEY_N];    
  uint16_t Pressed[KEY_N];
} KeyStateS;


Key_t GetKeyPressed(void);    //wait for key pressed & return it; blocking
bool IsKeyPressed(void);   //check do we have key pressed; non-blocking, analog to kbhit()
bool IsSDCardPresence(void);   //check presence of SD card

void KeyInit(void);
KeyStateS * KeyGet(void);
void KeyScan(void);
void KeyTranslate(void);

int KeyToBit(uint8_t keyCode);
void KeyEmulationModeSet(uint8_t new_mode);
void KeyEmulateCode(uint8_t keyCode);

#endif   //KEYBOARD_H_
