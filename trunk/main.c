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
#include "eeprom.h"
#include "serial.h"
#include "stdio.h"
#include "adc.h"   
#include "stm32f30x_it.h"

/** @addtogroup STM32F30x_StdPeriph_Examples
  * @{
  */

/** @addtogroup CAN_Networking
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY_PRESSED     0x00
#define KEY_NOT_PRESSED 0x01

#define TSK_IDLE          0
#define TSK_PWR_OFF       2
#define TSK_CLIBRATION    1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* CPAL local transfer structures */
CPAL_TransferTypeDef  sRxStructure, sTxStructure;
uint8_t tRxBuffer[MAX_BUFF_SIZE];
uint8_t tTxBuffer[256];
uint32_t BufferSize = MAX_BUFF_SIZE;
uint8_t Data[200];
int8_t drv_i2c_err = 0;

uint8_t KernelTask = TSK_IDLE;

CanTxMsg TxMessage = {0};
uint8_t KeyNumber = 0x0;

/* Values of Variable1, Variable2 and Variable3 */
uint16_t VarValue1, VarValue2, VarValue3;

/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};

RCC_ClocksTypeDef RCC_Clocks;
__IO uint32_t ActionState = ACTION_NONE;

/* Private function prototypes -----------------------------------------------*/
static void CAN_Config(void);
//void Delay(void);

void SysTick_Configuration(void);
void kernel(void);
/* Private functions ---------------------------------------------------------*/
static int8_t drv_i2c_EOT_Wait(void);
static int8_t drv_i2c_WriteBuffer(const uint8_t *Data, uint16_t DataLength);
static int8_t drv_i2c_ReadBuffer(uint8_t * buf, uint16_t len, uint16_t cmd);
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
  /* Test Virtual EEPROM */ 
#if 1  
  //uint16_t varvalue = 0;
  unsigned char SendBuffer[10];
  unsigned char ResiveBuffer[10];
  
  SysTick_Configuration();
  
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();

  /* EEPROM Init */
  EE_Init();
#if 0   
  /* Initialize variables to be used */
  VarValue1 = 0;
  VarValue2 = 0;
  VarValue3 = 0; 
  
  /* Store successively many values of the three variables in the EEPROM */
  /* Store 100 values of Variable1, Variable2 and Variable3 in EEPROM */
  for (varvalue = 0; varvalue < 100; varvalue++)
  {
    VarValue1 += 1;
    VarValue2 += 2;
    VarValue3 += 3;
             
    EE_WriteVariable(VirtAddVarTab[0], VarValue1);
    EE_WriteVariable(VirtAddVarTab[1], VarValue2);
    EE_WriteVariable(VirtAddVarTab[2], VarValue3);
  }
  /* read the last stored variables data*/
  EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
  EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]);
  EE_ReadVariable(VirtAddVarTab[2], &VarDataTab[2]);
#endif  
  SendBuffer[0] = 0x14;
  SendBuffer[1] = 0x25;
  SendBuffer[2] = 0x06;
  SendBuffer[3] = 0x07;
  SendBuffer[4] = 0x08;
  
  EE_Write_Buff(VirtAddVarTab[0],SendBuffer,5);  
  EE_Read_Buff(VirtAddVarTab[0],ResiveBuffer,5);
  
#endif
  ADC_Initialization();     
  /* Configures LED 1..4 */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDOff(LED2);
  
  init_serial();
//for(;;){  
  printf("Hello world!\n\r");
//}
  /* Configure Push button key */
 // STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO); 
   
  /* CAN configuration */
  CAN_Config();
  
  
/* Start CPAL communication configuration ***********************************/

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
  //I2C_DevStructure.CPAL_Mode = CPAL_MODE_SLAVE;
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
  
//  delay(5000); /* Delay 5000 ms */
  
 //==========================
  sRxStructure.wNumData = 0; /* Number of data to be read */
  sRxStructure.pbBuffer = (uint8_t*)Data; /* Transmit buffer */
  sRxStructure.wAddr1 = 0x30; /*Command register address*/
  sRxStructure.wAddr2 = 0x00; /*Command register address*/
  I2C1_DevStructure.pCPAL_TransferRx = &sRxStructure;
  I2C1_DevStructure.wCPAL_Options &= ~CPAL_OPT_NO_MEM_ADDR;
  I2C1_DevStructure.wCPAL_Options &= ~CPAL_OPT_16BIT_REG;//8 bit
  
  
  /* Infinite loop */
  kernel();
  for(;;);
  
  while(1)
  {
//    while(STM_EVAL_PBGetState(BUTTON_KEY) == KEY_PRESSED)
//    {
//      if(KeyNumber == 0x4) 
//      {
//        KeyNumber = 0x00;
//      }
//      else
//      {
        LED_Display(++KeyNumber);
        TxMessage.Data[0] = KeyNumber;
        CAN_Transmit(CANx, &TxMessage);
        //Delay();
        
//        while(STM_EVAL_PBGetState(BUTTON_KEY) != KEY_NOT_PRESSED)
//        {
//        }
//      }
//    }
  }
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
 
  //__enable_irq(); 

  /* infinite kernel loop */
  for(;;){
    ticks = DevTicks;
    if((ticks - DevTicksRef10ms) >= 10){
      //KeyScan(); // keyboard scan
      //key = KeyGet(); // get keyboard values
      
      DevTicksRef10ms = ticks;     
    } 
    /* =============================================================
     50 msec process 
    ================================================================*/
    if((ticks - DevTicksRef100ms) >= 50){ // 50ms   
      //if (key->Pressed[KEYB_CALIBRATION_INDX] > 5 && !key->Lock[KEYB_CALIBRATION_INDX]){
        //key->Lock[KEYB_CALIBRATION_INDX] = true;
        //RunCalibrationProcess();
        //CalibrationProcess(&CalibINDX,&CValues);
        //cmd_help();
        //Menu(MainMenu);
      //}
      
      DevTicksRef100ms = ticks;
    }
    
    /* =============================================================
     500 msec process 
    ================================================================*/
    if((ticks - DevTicksRef500ms) >= 500){ // 500 msec
      DevTicksRef500ms = ticks;
      
      printf("\x1b[2J");//terminal clear screen command
      //double Humidity = 0;
      //for(size_t Channel=0;Channel<MAXCHANNEL;Channel++){
      //  GetValue(Channel,ADCValues[Channel],&Humidity);
      //  printf("CH%d %d  volt:%.3f humidity: %.1f\r\n",Channel+1,ADCValues[Channel],Voltage[Channel],Humidity);
      printf("ADC Value0 = %d\r\n",ADC_GetChannelConversionValue(0));
      printf("ADC Value1 = %d\r\n",ADC_GetChannelConversionValue(1));
      printf("ADC Value2 = %d\r\n",ADC_GetChannelConversionValue(2));
      
      printf("ADC Value3 = %d\r\n",ADC_GetChannelConversionValue(3));
      printf("ADC Value4 = %d\r\n",ADC_GetChannelConversionValue(4));
      printf("ADC Value5 = %d\r\n",ADC_GetChannelConversionValue(5));
      //}
      //
    } 
    
    /* =============================================================
    1 sec process
    ================================================================*/
    if((ticks - DevTicksRef1s) >= 1000){ // 1s      
      DevTicksRef1s = ticks;
      
      //TestProg();
      STM_EVAL_LEDToggle(LED1);
      STM_EVAL_LEDToggle(LED2);
      
      //Read redister
      tTxBuffer[0] = 0x00;
      drv_i2c_WriteBuffer(tTxBuffer,1); 
      drv_i2c_ReadBuffer(tRxBuffer,1,0x00);

      //Read redister
      tTxBuffer[0] = 0x10;
      drv_i2c_WriteBuffer(tTxBuffer,1); 
      drv_i2c_ReadBuffer(tRxBuffer,4,0x10); 
    }
     /* =============================================================
    2,5 sec process
    ================================================================*/
    if((ticks - DevTicksRef2_5s) >= 2500){ // 2,5 s      
      DevTicksRef2_5s = ticks;
      /*
      if(GeneratorVal){
        GetADCValues(tempADCValues2,sizeof(tempADCValues2)/sizeof(tempADCValues2[0]));
      }else{
        GetADCValues(tempADCValues1,sizeof(tempADCValues1)/sizeof(tempADCValues1[0]));
      }
      for(size_t ch=0;ch<MAXCHANNEL;ch++){
        ADCValues[ch] = (tempADCValues1[ch]+tempADCValues2[ch])/2;
        Voltage[ch] = (float)(ADCValues[ch]*0.80586);
        Voltage[ch] = Voltage[ch]/1000;  
      }
        
      SetGenerator(&GeneratorVal);
      */
    }
    /* =============================================================
    5 sec process
    ================================================================*/
    if((ticks - DevTicksRef5s) >= 5000){ // 5s 
      DevTicksRef5s = ticks;
      
      
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
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
static void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  NVIC_InitTypeDef  NVIC_InitStructure;
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(CAN_GPIO_CLK, ENABLE);

  /* Connect CAN pins to AF7 */
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

  /* NVIC configuration *******************************************************/
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CANx);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate = 1MBps (CAN clocked at 36 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 2;
  CAN_Init(CANx, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Transmit Structure preparation */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
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

/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
/*void Delay(void)
{
  uint16_t nTime = 0x0000;

  for(nTime = 0; nTime <0xFFF; nTime++)
  {
  }
}
*/
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
//   I2C_DevStructure.wCPAL_Options &= ~CPAL_OPT_NO_MEM_ADDR; /* send command right after address */
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
    //I2C1_DevStructure.wCPAL_Options |= CPAL_OPT_I2C_NOSTOP;
    //I2C1_DevStructure.wCPAL_Options |= CPAL_OPT_I2C_NOSTOP_MODE;


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

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
