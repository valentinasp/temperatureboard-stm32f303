/**
  ******************************************************************************
  * @file    CAN/CAN_Networking/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-October-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "interpolation.h"
#include "delay.h"
#include "eeprom.h"
#include "serial.h"
#include "stdio.h"
#include "adc.h" 
#include "menu_core.h" 
#include "menu.h"
#include "stm32f30x_it.h"
#include "i2c.h"
//#include "drv_can_open.h"
#include "can.h"
#include "drv_stm32_can.h"
#include "calibration.h"
#include "ADCmeasurement.h"
#include "math.h"


/** @addtogroup STM32F30x_StdPeriph_Examples
  * @{
  */

/** @addtogroup CAN_Networking
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
const SCMD MainMenu[] = {
   "INFO",   cmd_bootinfo,
   "ERASE",  cmd_erase_calibration,
   "RUNCAL", cmd_run_calibration,
   "VIEW",   cmd_view,
   "?",      cmd_help,
   "EXIT",   cmd_exit,
    NULL,    NULL
};
/* Private define ------------------------------------------------------------*/
//#define KEY_PRESSED     0x00
//#define KEY_NOT_PRESSED 0x01

#define TSK_IDLE          0
#define TSK_PWR_OFF       2
#define TSK_CLIBRATION    1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char MagicString[]={"labas"};

uint8_t Data[200];

uint8_t KernelTask = TSK_IDLE;

#ifdef USE_BUTTON 
  uint8_t KeyNumber = 0x0;
#endif

/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};

RCC_ClocksTypeDef RCC_Clocks;
__IO uint32_t ActionState = ACTION_NONE;

unsigned int ADCValues[6];
float Voltage[6];

uint8_t CurrI2CBoard = 0;

/* Private function prototypes -----------------------------------------------*/
void SysTick_Configuration(void);
void kernel(void);
void WWDG_Init(void);
void WWDG_CounterReset(void);
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f30x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f30x.c file
     */
  SysTick_Configuration();
  InitCalibration();
  //init interpolation
  InitInterpolationValues(MAXTCHANNEL,MAXPOINTS);  //init points
  LoadCalibrationData();
  
  ADC_Initialization(); 
  STM_HEATING_Init();
  /* Configures LED 1..4 */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDOff(LED2);
  
  init_serial();
 
#ifdef USE_BUTTON 
  /* Configure Push button key */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO); 
#endif
  
  InitCAN();
    
  InitI2C();
  Delay(1000);
  I2C_ScannBoards();
    
  //SetBoardAddress(0x01);
  //WWDG_Init();
  /* Infinite loop */
  CanOpenSendStartStatus();
  SystemStateChange(SYS_STATE_STOPPED);
  
  kernel();
  for(;;);
}

//====================== Kernel loop ==============================
void kernel(void)
{
  uint32_t ticks;
  uint32_t DevTicksRef100ms = 0; 
  uint32_t DevTicksRef10ms  = 0; 
  uint32_t DevTicksRef500ms = 0;
  uint32_t DevTicksRef1s    = 0;
  uint32_t DevTicksRef2_5s  = 0;
  uint32_t DevTicksRef5s    = 0;
  uint32_t DevTicksRef10s   = 0;
  CAN_MESSAGE msg;
    
  /* infinite kernel loop */
  for(;;){
    ticks = DevTicks;
    WWDG_CounterReset();
    /* =============================================================
     10 msec process 
    ================================================================*/
    if((ticks - DevTicksRef10ms) >= 10){// 10ms 
#ifdef USE_BUTTON
      KeyScan(); // keyboard scan
      key = KeyGet(); // get keyboard values  
#endif
      DevTicksRef10ms = ticks;
#if 1     
      while(ReceiveCanMsg(&msg))  CanOpenProtocol(&msg);
      CanOpenTimer();
#endif
    } 
    /* =============================================================
     50 msec process 
    ================================================================*/
    if((ticks - DevTicksRef100ms) >= 50){ // 50ms  
#ifdef USE_BUTTON
      if (key->Pressed[KEYB_CALIBRATION_INDX] > 5 && !key->Lock[KEYB_CALIBRATION_INDX]){
        key->Lock[KEYB_CALIBRATION_INDX] = true;
        cmd_help();
        Menu(MainMenu);
      }
#endif
      
      if(IsMagicStr(MagicString)){
        cmd_help();
        Menu(MainMenu);
      }
      
      DevTicksRef100ms = ticks;
    }
    
    /* =============================================================
     500 msec process 
    ================================================================*/
    if((ticks - DevTicksRef500ms) >= 500){ // 500 msec
      DevTicksRef500ms = ticks;
      
      printf("\x1b[2J");//terminal clear screen command
      double temperature = 0;
      for(size_t Channel=0;Channel<MAXTCHANNEL;Channel++){
        GetValue(Channel,ADCValues[Channel],&temperature);
        if(temperature<-10 || temperature>200){
          printf("CH%d %d  volt:%.3f temperature: Error\r\n",Channel+1,ADCValues[Channel],Voltage[Channel]); 
        }else{
          printf("CH%d %d  volt:%.3f temperature: %.1f\r\n",Channel+1,ADCValues[Channel],Voltage[Channel],temperature);
          
          temperature *= 10;
        }
        //------ Temperature to CAN sending --------------------------------------------
          AnalogInput((unsigned short)Channel,(RECORD_TYPE)round(temperature)); 
          AnalogUnformatedInput((unsigned short)Channel,(RECORD_TYPE)ADCValues[Channel]);        
        //------------------------------------------------------------------------------
        
      }
      double TADCValue = ADC_GetChannelConversionValue(0); // Get ADC Value
      double InternalTemper = (1.43-((double)TADCValue*0.80586/1000))/0.0043+20;//in document +25
      printf("Internal temperature = %0.1f\r\n",InternalTemper);
      if(InternalTemper<10.0){
        STM_HEATING_On();
      }
      if(InternalTemper>45.0){
        STM_HEATING_Off();
      }
     
      printf("\r\nEnter '%s' string to enter the main menu",MagicString);
    } 
    
    /* =============================================================
    1 sec process
    ================================================================*/
    if((ticks - DevTicksRef1s) >= 1000){ // 1s      
      DevTicksRef1s = ticks;
 
#if 0     
     AnalogInput(0,GetHumidityValue(Channel1));
     AnalogUnformatedInput(kanalas,x);
#endif
     
      //TestProg();
      STM_EVAL_LEDToggle(LED1);
      STM_EVAL_LEDToggle(LED2);
    }
     /* =============================================================
    2,5 sec process
    ================================================================*/
    if((ticks - DevTicksRef2_5s) >= 2500){ // 2,5 s      
      DevTicksRef2_5s = ticks;

      GetADCValues(ADCValues,sizeof(ADCValues)/sizeof(ADCValues[0]));
      for(size_t ch=0;ch<MAXTCHANNEL;ch++){
        AnalogUnformatedInput(ch,ADCValues[ch]);// -> can table 
        Voltage[ch] = (float)(ADCValues[ch]*0.80586);
        Voltage[ch] = Voltage[ch]/1000;  
      }
      //Get via I2C data
      if(I2C_GetBoardsNr()>0){
        SelectBoardNr(CurrI2CBoard);
        
        for(size_t ch=0;ch<MAXHCHANNEL;ch++){
//--------------------------------------------------------------------------------------                    
          //AnalogInput(6 + ch,GetHumidityValue((channel_t)ch));
          //Delay(5);           
          //AnalogUnformatedInput(6 + ch,GetHADCValue((channel_t)ch));// -> can table 
          //Delay(5); 
            
          AnalogInput((MAXHCHANNEL*(size_t)CurrI2CBoard)+MAXTCHANNEL+ch,GetHumidityValue((channel_t)ch)); 
          Delay(5);
          AnalogUnformatedInput((MAXHCHANNEL*(size_t)CurrI2CBoard)+MAXTCHANNEL+ch,GetHADCValue((channel_t)ch));// -> can table 
          Delay(5); 
            
//------------------------------------------------------------------------------------------ 
        }
        //switch board
        CurrI2CBoard++;
        if(CurrI2CBoard>I2C_GetBoardsNr()-1){
          CurrI2CBoard=0;
        }
      }
    }    
    /* =============================================================
    5 sec process
    ================================================================*/
    if((ticks - DevTicksRef5s) >= 5000){ // 5s 

#if 0      
      CAN_MESSAGE msg;
      DevTicksRef5s = ticks;
      msg.Id= 0x134;
      msg.len = 3;
      msg.data[0] = 0x11;
      msg.data[1] = 0x22;
      msg.data[2] = 0x33;
       SendCanMsg(&msg);
       msg.data[2] = 0x44;
       SendCanMsg(&msg);
       msg.data[2] = 0x55;
       SendCanMsg(&msg);
       msg.data[2] = 0x66;
       SendCanMsg(&msg);
       msg.data[2] = 0x77;
       SendCanMsg(&msg);
      //STM_HEATING_Toggle();
      //TxMessage.Data[0] = ++KeyNumber;
      //CAN_Transmit(CANx, &TxMessage);
#endif 
    }
    //==================================================================   
    
    /* =============================================================
   10 sec process
    ================================================================*/
    if((ticks - DevTicksRef10s) >= 10000){ // 10s 
      DevTicksRef10s = ticks;
    }
    //================================================================== 
  }  
}
//====================== End kernel loop ==============================


/**
* @brief  Configures the SysTick to generate an interrupt each 250 ms.
* @param  None
* @retval None
*/
void SysTick_Configuration(void)
{
  DevTicks = 0;
  /* SysTick end of count event each 10ms */
  //RCC_GetClocksFreq(&RCC_Clocks);
  //SysTick_Config(RCC_Clocks.HCLK_Frequency / 10000);
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
  //SysTick_Config(32000000/10000);
}




/**
  * @brief  Turn ON/OFF the dedicated led
  * @param  Ledstatus: Led number from 1 to 4.
  * @retval None
  */
void LED_Display(uint8_t Ledstatus)
{
  /* Turn off all leds */
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  
  switch(Ledstatus)
  {
    case(1): 
      STM_EVAL_LEDOn(LED1);
      break;
   
    case(2): 
      STM_EVAL_LEDOn(LED2);
      break;
 
    default:
      break;
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif
/**
  * @}
  */

/**
  * @}
  */
void WWDG_Init(void){
  
  /* WWDG configuration */
  /* Enable WWDG clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

  /* WWDG clock counter = (PCLK1/4096)/8 = 244 Hz (~4 ms)  */
  WWDG_SetPrescaler(WWDG_Prescaler_8);

  /* Set Window value to 65 */
  WWDG_SetWindowValue(127);

  /* Enable WWDG and set counter value to 127, WWDG timeout = ~4 ms * 64 = 262 ms */
  WWDG_Enable(127);
  
//  /* WWDG configuration */
//  /* Enable WWDG clock */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
//
//  /* WWDG clock counter = (PCLK1 (36MHz)/4096)/8 = 1098Hz (~910 us)  */
//  WWDG_SetPrescaler(WWDG_Prescaler_8);
//
//  /* Set Window value to 80; WWDG counter should be refreshed only when the counter
//    is below 80 (and greater than 64) otherwise a reset will be generated */
//  WWDG_SetWindowValue(80);
//  /* Enable WWDG and set counter value to 127, WWDG timeout = ~910 us * 64 = 58.24 ms 
//    In this case the refresh window is: ~910 * (127-80) = 42.7ms < refresh window < ~910 * 64 = 58.2ms
//  */
//  WWDG_Enable(127);
  /* Clear EWI flag */
  WWDG_ClearFlag();
  /* Enable EW interrupt */
  WWDG_EnableIT();
}

void WWDG_CounterReset(void){
    /* Update WWDG counter */
    WWDG_SetCounter(127);
    /* Clear EWI flag */
    WWDG_ClearFlag();
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
