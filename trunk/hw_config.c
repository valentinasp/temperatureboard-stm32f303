/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_it.h"
#include "hw_config.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//ErrorStatus HSEStartUpStatus;
//int32_t DevTicks = 0;

/* Private function prototypes -----------------------------------------------*/
void NVIC_Config(void);


/** @defgroup STM32F303_LOW_LEVEL_Private_Variables
  * @{
  */ 
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {KEY_BUTTON_GPIO_PORT}; 

const uint16_t BUTTON_PIN[BUTTONn] = {KEY_BUTTON_PIN}; 

const uint32_t BUTTON_CLK[BUTTONn] = {KEY_BUTTON_GPIO_CLK};

const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {KEY_BUTTON_EXTI_LINE};

const uint8_t BUTTON_PORT_SOURCE[BUTTONn] = {KEY_BUTTON_EXTI_PORT_SOURCE};
								 
const uint8_t BUTTON_PIN_SOURCE[BUTTONn] = {KEY_BUTTON_EXTI_PIN_SOURCE}; 

const uint8_t BUTTON_IRQn[BUTTONn] = {KEY_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {EVAL_COM1}; 

GPIO_TypeDef* COM_TX_PORT[COMn] = {EVAL_COM1_TX_GPIO_PORT};
 
GPIO_TypeDef* COM_RX_PORT[COMn] = {EVAL_COM1_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {EVAL_COM1_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {EVAL_COM1_TX_GPIO_CLK};
 
const uint32_t COM_RX_PORT_CLK[COMn] = {EVAL_COM1_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {EVAL_COM1_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {EVAL_COM1_RX_PIN};
 
const uint16_t COM_TX_PIN_SOURCE[COMn] = {EVAL_COM1_TX_SOURCE};

const uint16_t COM_RX_PIN_SOURCE[COMn] = {EVAL_COM1_RX_SOURCE};
 
const uint8_t COM_TX_AF[COMn] = {EVAL_COM1_TX_AF};
 
const uint8_t COM_RX_AF[COMn] = {EVAL_COM1_RX_AF};



/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1  
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *   contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

  /* Enable USART clock */
  RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 

  /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);
  
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);
    
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStruct);
    
  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_AHBPeriphClockCmd(GPIO_CLK[Led], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
  GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];
}
/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4  
  * @retval None
  */
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BRR = GPIO_PIN[Led];
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4 
  * @retval None
  */
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4  
  * @retval None
  */
void STM_EVAL_LEDToggle(Led_TypeDef Led)
{
  GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}
/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter can be one of following parameters: 
  *     @arg BUTTON_KEY: Key Push Button 
  *     @arg BUTTON_RIGHT: Joystick Right Push Button 
  *     @arg BUTTON_LEFT: Joystick Left Push Button 
  *     @arg BUTTON_UP: Joystick Up Push Button 
  *     @arg BUTTON_DOWN: Joystick Down Push Button
  *     @arg BUTTON_SEL: Joystick Sel Push Button
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                     generation capability  
  * @retval None
  */
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* There is no Wakeup button on STM32303C_EVAL-EVAL, the Button value should
     be greater than 0. */
  if(Button > 0)
  {
    Button = (Button_TypeDef) (Button - 1);
    
    /* Enable the BUTTON Clock */
    RCC_AHBPeriphClockCmd(BUTTON_CLK[Button], ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Configure Button pin as input */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
    GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);


    if (Button_Mode == BUTTON_MODE_EXTI)
    {
      /* Connect Button EXTI Line to Button GPIO Pin */
      SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

      /* Configure Button EXTI line */
      EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
      EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    
      if(Button != BUTTON_KEY) 
      {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
      }
      else
      {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
      }
      EXTI_InitStructure.EXTI_LineCmd = ENABLE;
      EXTI_Init(&EXTI_InitStructure);

      /* Enable and set Button EXTI Interrupt to the lowest priority */
      NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

      NVIC_Init(&NVIC_InitStructure); 
    }
  }
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_KEY: Key Push Button 
  *     @arg BUTTON_RIGHT: Joystick Right Push Button 
  *     @arg BUTTON_LEFT: Joystick Left Push Button 
  *     @arg BUTTON_UP: Joystick Up Push Button 
  *     @arg BUTTON_DOWN: Joystick Down Push Button
  *     @arg BUTTON_SEL: Joystick Sel Push Button     
  * @retval - When Button > 0, the Button GPIO pin value is returned.
  *         - When Button = 0 , error code (0xFF) is returned.   
  */
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button)
{
  /* There is no Wakeup button on STM32303C_EVAL-EVAL, the Button value should
     be greater than 0. */
  if(Button > 0)
  {
    Button = (Button_TypeDef) (Button - 1);
    return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
  }
  else
  {
    return 0xFF; /* Error Code */
  }
}














#if 0
//================================================================================== OLD =====================
/* Output Port parameters */
GPIO_TypeDef * OUTP_PORT[PORT_OUTP_N] = {
                                  PORT_OUTP_00_PORT, PORT_OUTP_01_PORT, PORT_OUTP_02_PORT, PORT_OUTP_03_PORT,
                                  PORT_OUTP_04_PORT, PORT_OUTP_05_PORT, PORT_OUTP_06_PORT, PORT_OUTP_07_PORT,
                                  PORT_OUTP_08_PORT, PORT_OUTP_09_PORT, PORT_OUTP_10_PORT,                               
};
const uint16_t OUTP_PIN[PORT_OUTP_N] = {
                                  PORT_OUTP_00_PIN, PORT_OUTP_01_PIN, PORT_OUTP_02_PIN, PORT_OUTP_03_PIN,
                                  PORT_OUTP_04_PIN, PORT_OUTP_05_PIN, PORT_OUTP_06_PIN, PORT_OUTP_07_PIN,
                                  PORT_OUTP_08_PIN, PORT_OUTP_09_PIN, PORT_OUTP_10_PIN,                                 
};
const uint32_t OUTP_CLK[PORT_OUTP_N] = {
                                  PORT_OUTP_00_CLK, PORT_OUTP_01_CLK, PORT_OUTP_02_CLK, PORT_OUTP_03_CLK,
                                  PORT_OUTP_04_CLK, PORT_OUTP_05_CLK, PORT_OUTP_06_CLK, PORT_OUTP_07_CLK,
                                  PORT_OUTP_08_CLK, PORT_OUTP_09_CLK, PORT_OUTP_10_CLK,                                 
};
const GPIOMode_TypeDef OUTP_MODE[PORT_OUTP_N] = {
                                  PORT_OUTP_00_MODE, PORT_OUTP_01_MODE, PORT_OUTP_02_MODE, PORT_OUTP_03_MODE,
                                  PORT_OUTP_04_MODE, PORT_OUTP_05_MODE, PORT_OUTP_06_MODE, PORT_OUTP_07_MODE,
                                  PORT_OUTP_08_MODE, PORT_OUTP_09_MODE, PORT_OUTP_10_MODE,                                 
};
const GPIOSpeed_TypeDef OUTP_SPEED[PORT_OUTP_N] = {
                                  PORT_OUTP_00_SPEED, PORT_OUTP_01_SPEED, PORT_OUTP_02_SPEED, PORT_OUTP_03_SPEED,
                                  PORT_OUTP_04_SPEED, PORT_OUTP_05_SPEED, PORT_OUTP_06_SPEED, PORT_OUTP_07_SPEED,
                                  PORT_OUTP_08_SPEED, PORT_OUTP_09_SPEED, PORT_OUTP_10_SPEED,
};
const BitAction OUTP_DEFAULT_VAL[PORT_OUTP_N] = {
                                  PORT_OUTP_00_DEFAULT_VAL, PORT_OUTP_01_DEFAULT_VAL, PORT_OUTP_02_DEFAULT_VAL, PORT_OUTP_03_DEFAULT_VAL,
                                  PORT_OUTP_04_DEFAULT_VAL, PORT_OUTP_05_DEFAULT_VAL, PORT_OUTP_06_DEFAULT_VAL, PORT_OUTP_07_DEFAULT_VAL,
                                  PORT_OUTP_08_DEFAULT_VAL, PORT_OUTP_09_DEFAULT_VAL, PORT_OUTP_10_DEFAULT_VAL,                                
};

/* Input Port parameters */
GPIO_TypeDef * INP_PORT[PORT_INP_N] = {
                                  PORT_INP_00_PORT, PORT_INP_01_PORT, PORT_INP_02_PORT, PORT_INP_03_PORT,
                                  PORT_INP_04_PORT, PORT_INP_05_PORT, PORT_INP_06_PORT,
};
const uint16_t INP_PIN[PORT_INP_N] = {
                                  PORT_INP_00_PIN, PORT_INP_01_PIN, PORT_INP_02_PIN, PORT_INP_03_PIN,  
                                  PORT_INP_04_PIN, PORT_INP_05_PIN, PORT_INP_06_PIN,                                     
};
const uint32_t INP_CLK[PORT_INP_N] = {
                                  PORT_INP_00_CLK, PORT_INP_01_CLK, PORT_INP_02_CLK, PORT_INP_03_CLK,
                                  PORT_INP_04_CLK, PORT_INP_05_CLK, PORT_INP_06_CLK,                                   
};
const GPIOMode_TypeDef INP_MODE[PORT_INP_N] = {
                                  PORT_INP_00_MODE, PORT_INP_01_MODE, PORT_INP_02_MODE, PORT_INP_03_MODE,
                                  PORT_INP_04_MODE, PORT_INP_05_MODE, PORT_INP_06_MODE,                                   
};


/* COM Port parameters */
USART_TypeDef* COM_USART[COM_N] = {PORT_COM1, PORT_COM2}; 
GPIO_TypeDef* COM_PORT[COM_N] = {PORT_COM1_GPIO, PORT_COM2_GPIO};
const uint32_t COM_USART_CLK[COM_N] = {PORT_COM1_CLK, PORT_COM2_CLK};
const uint32_t COM_PORT_CLK[COM_N] = {PORT_COM1_GPIO_CLK, PORT_COM2_GPIO_CLK};
const uint16_t COM_TX_PIN[COM_N] = {PORT_COM1_TxPin, PORT_COM2_TxPin};
const uint16_t COM_RX_PIN[COM_N] = {PORT_COM1_RxPin, PORT_COM2_RxPin};

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#ifdef CFG_USE_RUNTIME_PRINTF
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* CFG_USE_RUNTIME_PRINTF */
#endif /* __GNUC__ */

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Set_System_Init();
void Set_System(void);
void RTC_Configuration(void);
void NVIC_Configuration(void);
void SysTick_Configuration(void);
void PVD_Configuration(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void EXTI_Configuration(void);
void IWDG_Configuration(void);
void BOARD_OUTPInit(OUTP_TypeDef OUTport);
void BOARD_OUTPOn(OUTP_TypeDef OUTport);
void BOARD_OUTPOff(OUTP_TypeDef OUTport);
void BOARD_OUTPToggle(OUTP_TypeDef OUTport);
void BOARD_INPInit(INP_TypeDef INport);
uint32_t BOARD_INPReadBit(INP_TypeDef INport);
uint8_t BOARD_INPGetFlag(INP_TypeDef INport);
void BOARD_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
void BOARD_COM_PutChar(COM_TypeDef COM, uint8_t ch);
uint8_t BOARD_COM_GetChar(COM_TypeDef COM);
void BOARD_COM_PutStr(COM_TypeDef COM, const uint8_t *str);
void BOARD_COM_Printf(const char *str);
//PUTCHAR_PROTOTYPE
void Printf_Configuration();
void PowerOn_Check();
void BOARD_Power_Off();

/* Private functions ---------------------------------------------------------*/

void Set_System_Init()
{
  BOARD_INPInit(INP_BUT_ONOFF);
  BOARD_OUTPInit(OUTP_PWRSHDN);     
}

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  /* RCC configuration */
  RCC_Configuration();

  GPIO_Configuration();
  SysTick_Configuration();
  EXTI_Configuration();
  NVIC_Configuration();
  
  RTC_Configuration();
  
  #ifndef DEBUG
    IWDG_Configuration();
  #endif
}


/**
  * @brief  Configures RTC clock source and prescaler.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{ 
   /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable the LSI OSC */
  RCC_LSICmd(ENABLE);
  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_ALR, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32768);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

/**
  * @brief  Configures NVIC and Vector Table base location.
  * @param  None
  * @retval : None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;  
  
  /* OMAP Ready signal */  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  
  NVIC_Init(&NVIC_InitStructure);    
  
  /* PWR but signal */  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;  
  NVIC_Init(&NVIC_InitStructure);  

  /* VFL but signal */  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;  
  NVIC_Init(&NVIC_InitStructure);    
  
  NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
  
  /* Set the Vector Table base location at 0x08000000 */ 
  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); 
}

void NVIC_Control_Button_Int(FunctionalState state)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = state; 
  
  /* Connect Button EXTI Line to pwr but */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;  
  EXTI_Init(&EXTI_InitStructure);
  
  /* Connect Button EXTI Line to vfl but */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;  
  EXTI_Init(&EXTI_InitStructure);

  /* Connect Button EXTI Line to omap ready but */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
  EXTI_InitStructure.EXTI_Line = EXTI_Line12; 
  EXTI_Init(&EXTI_InitStructure);
}

void NVIC_Control_Charger_Int(FunctionalState state)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Configure EXTI line */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = state; 
  
  /* Connect EXTI Line to  VC_CHARGING */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;  
  EXTI_Init(&EXTI_InitStructure);
  
  /* Connect EXTI Line to  VC_CHARGER_ON */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;  
  EXTI_Init(&EXTI_InitStructure);
}
/**
* @brief  Configures the SysTick to generate an interrupt each 250 ms.
* @param  None
* @retval None
*/
void SysTick_Configuration(void)
{
  uint32_t enc;
  DevTicks = 0;
    /* SysTick interrupt each 10 ms with SysTick Clock equal to 8/8 MHz */
    if(SysTick_Config((8000000/8) / 1000))
    { 
      /* Capture error */ 
      while (1);
    }

  /* Select AHB clock(HCLK) divided by 8 as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  /* Encode the priority for an interrupt */
  /* group = 3, priority=2, subpriority=1 */
  enc = NVIC_EncodePriority(3, 0, 0);

  /* Set SysTick Preemption Priority */
  NVIC_SetPriority(SysTick_IRQn, enc);
}

 
void PVD_Configuration(void)
{
 
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;  
  
  /* Enable PWR and BKP clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);  

  /* Set the Vector Table base location at 0x08000000 */ 
  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

  /* Configure EXTI Line16(PVD Output) to generate an interrupt on rising and
  falling edges */
  EXTI_ClearITPendingBit(EXTI_Line16);
  EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);  
  
    /* Enable the PVD Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
   
  /* Configure the PVD Level to 2.5V required level by ADC*/
  PWR_PVDLevelConfig(PWR_PVDLevel_2V5);
  
  /* Enable the PVD Output */
  PWR_PVDCmd(ENABLE);   
}




/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval : None
  */
void RCC_Configuration(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();


#ifdef HSE_PLL_ON
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  
  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    
    /* Set the Flash wait states */
    #ifdef HSE_PLL_ON_72MHz /* 2 wait states */
    FLASH_SetLatency(FLASH_Latency_2);
    #endif
    
    #ifdef HSE_PLL_ON_48MHz /* 1 wait state */ 
    FLASH_SetLatency(FLASH_Latency_1);
    #endif
    
    #ifdef HSE_PLL_ON_36MHz /* 1 wait state */
    FLASH_SetLatency(FLASH_Latency_1);
    #endif
    
    #ifdef HSE_PLL_ON_24MHz /* 0 wait state */
    FLASH_SetLatency(FLASH_Latency_0);
    #endif
    
    #ifdef HSE_PLL_ON_16MHz /* 0 wait state */
    FLASH_SetLatency(FLASH_Latency_0);
    #endif
    
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    #ifdef HSE_PLL_ON_72MHz
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    #endif
    
    #ifdef HSE_PLL_ON_48MHz
    /* PLLCLK = 8MHz * 6 = 48 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
    #endif
    
    #ifdef HSE_PLL_ON_36MHz
    /* PLLCLK = 8MHz / 2 * 9 = 36 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);
    #endif
    
    #ifdef HSE_PLL_ON_24MHz

    // for value line devices
    /* PLLCLK = (8MHz/2) * 6 = 24 MHz */
    RCC_PREDIV1Config(RCC_PREDIV1_Source_HSE, RCC_PREDIV1_Div2);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);

    /* PLLCLK = 8MHz * 3 = 24 MHz */    
//    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_3);
    #endif
    
    #ifdef HSE_PLL_ON_16MHz
    /* PLLCLK = 8MHz * 2 = 16 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_2);
    #endif
    
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
#endif /* HSE_PLL_ON */
    
#ifdef HSE_PLL_OFF
  
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  
 if(HSEStartUpStatus == SUCCESS)
  {
    /* Disable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Disable);
  
    /* Flash 0 wait state */
    FLASH_SetLatency(FLASH_Latency_0);
    
    #ifdef HSE_PLL_OFF_8MHz
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    #endif 

    #ifdef HSE_PLL_OFF_4MHz
    /* HCLK = SYSCLK/2 = 4MHz */
    RCC_HCLKConfig(RCC_SYSCLK_Div2); 
    #endif 
    
    #ifdef HSE_PLL_OFF_2MHz
    /* HCLK = SYSCLK/4 = 2MHz */
    RCC_HCLKConfig(RCC_SYSCLK_Div4); 
    #endif 
    
    #ifdef HSE_PLL_OFF_1MHz
    /* HCLK = SYSCLK/8 = 1MHz */
    RCC_HCLKConfig(RCC_SYSCLK_Div8); 
    #endif 
    
    #ifdef HSE_PLL_OFF_500KHz
    /* HCLK = SYSCLK/16 = 500KHz */
    RCC_HCLKConfig(RCC_SYSCLK_Div16); 
    #endif 
    
    #ifdef HSE_PLL_OFF_125KHz
    /* HCLK = SYSCLK/64 = 125KHz */
    RCC_HCLKConfig(RCC_SYSCLK_Div64); 
    #endif 
    
    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2); 

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);
  
    /* Select HSE as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);

    /* Wait till HSE is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x04)
    {
    }  
  }
#endif /* HSE_PLL_OFF */ 
 
 
#ifdef HSI_PLL_ON
 
  /* Enable Prefetch Buffer */
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
 
  /* Set the Flash wait states */
  #ifdef HSI_PLL_ON_64MHz /* 2 wait states */
  FLASH_SetLatency(FLASH_Latency_2);
  #endif
    
  #ifdef HSI_PLL_ON_48MHz /* 1 wait state */ 
  FLASH_SetLatency(FLASH_Latency_1);
  #endif
    
  #ifdef HSI_PLL_ON_36MHz /* 1 wait state */
  FLASH_SetLatency(FLASH_Latency_1);
  #endif
    
  #ifdef HSI_PLL_ON_24MHz /* 0 wait state */
  FLASH_SetLatency(FLASH_Latency_0);
  #endif
    
  #ifdef HSI_PLL_ON_16MHz /* 0 wait state */
  FLASH_SetLatency(FLASH_Latency_0);
  #endif
    
  /* HCLK = SYSCLK */
  RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
  /* PCLK2 = HCLK/2 */
  RCC_PCLK2Config(RCC_HCLK_Div2); 

  /* PCLK1 = HCLK/4 */
  RCC_PCLK1Config(RCC_HCLK_Div4);  
  
  #ifdef HSI_PLL_ON_64MHz
  /* PLLCLK = 8MHz/2 * 16 = 64 MHz */
  RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
  #endif
    
  #ifdef HSI_PLL_ON_48MHz
  /* PLLCLK = 8MHz/2 * 12 = 48 MHz */
  RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
  #endif
    
  #ifdef HSI_PLL_ON_36MHz
  /* PLLCLK = 8MHz/2 * 9 = 36 MHz */
  RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_9);
  #endif
    
  #ifdef HSI_PLL_ON_24MHz
  /* PLLCLK = 8MHz/2 * 6 = 24 MHz */
  RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_6);
  #endif
    
  #ifdef HSI_PLL_ON_16MHz
  /* PLLCLK = 8MHz/2 * 4 = 16 MHz */
  RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_4);
  #endif
    
  /* Enable PLL */ 
  RCC_PLLCmd(ENABLE);

  /* Wait till PLL is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {
  }

  /* Select PLL as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

  /* Wait till PLL is used as system clock source */
  while(RCC_GetSYSCLKSource() != 0x08)
  {
  }
  
#endif /* HSI_PLL_ON */
 
#ifdef HSI_PLL_OFF
  
  /* Disable Prefetch Buffer */
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Disable);
  
  /* Flash 0 wait state */
  FLASH_SetLatency(FLASH_Latency_0);
  
  #ifdef HSI_PLL_OFF_8MHz
  /* HCLK = SYSCLK */
  RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  #endif 

  #ifdef HSI_PLL_OFF_4MHz
  /* HCLK = SYSCLK/2 = 4MHz */
  RCC_HCLKConfig(RCC_SYSCLK_Div2); 
  #endif 
    
  #ifdef HSI_PLL_OFF_2MHz
  /* HCLK = SYSCLK/4 = 2MHz */
  RCC_HCLKConfig(RCC_SYSCLK_Div4); 
  #endif 
    
  #ifdef HSI_PLL_OFF_1MHz
  /* HCLK = SYSCLK/8 = 1MHz */
  RCC_HCLKConfig(RCC_SYSCLK_Div8); 
  #endif 
   
  #ifdef HSI_PLL_OFF_500KHz
  /* HCLK = SYSCLK/16 = 500KHz */
  RCC_HCLKConfig(RCC_SYSCLK_Div16); 
  #endif 
    
  #ifdef HSI_PLL_OFF_125KHz
  /* HCLK = SYSCLK/64 = 125KHz */
  RCC_HCLKConfig(RCC_SYSCLK_Div64); 
  #endif 
    
  /* PCLK2 = HCLK/2 */
  RCC_PCLK2Config(RCC_HCLK_Div2); 

  /* PCLK1 = HCLK/2 */
  RCC_PCLK1Config(RCC_HCLK_Div2);
  
  /* Select HSI as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

  /* Wait till HSI is used as system clock source */
   while(RCC_GetSYSCLKSource() != 0x00)
  {
  }    
#endif /* HSI_PLL_OFF */ 
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval : None
  */
void GPIO_Configuration(void)
{ 
  /* Initialize Output Port pins */
  BOARD_OUTPInit(OUTP_MAIN_ON);    
  BOARD_OUTPInit(OUTP_OMAP_ON);  
  BOARD_OUTPInit(OUTP_VFL_ON);   
  BOARD_OUTPInit(OUTP_BL_CTRL);     
  BOARD_OUTPInit(OUTP_BL_ON);       
  BOARD_OUTPInit(OUTP_OMAP_RSTn); 
  BOARD_OUTPInit(OUTP_OMAP_IN0); 
  BOARD_OUTPInit(OUTP_OMAP_IN1); 
  BOARD_OUTPInit(OUTP_OMAP_IN2);    
  BOARD_OUTPInit(OUTP_LED);    
  
  /* Initialize Input Port pins */  
  BOARD_INPInit(INP_OMAP_READY);         
  BOARD_INPInit(INP_OMAP_OUT0);         
  BOARD_INPInit(INP_OMAP_OUT1);           
}


/**
  * @brief  Configures EXTI Lines.
  * @param  None
  * @retval : None
  */
void EXTI_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  
  /* Configure EXTI Line17(RTC Alarm) to generate an interrupt on rising edge */
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

/**
  * @brief  Configures IWDG watchdog.
  * @param  None
  * @retval : None
  */
void IWDG_Configuration(void)
{
  /* IWDG timeout equal to sec (the timeout may varies due to LSI frequency dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG counter clock: 40KHz(LSI) / 256 = 156.25 Hz */
  IWDG_SetPrescaler(IWDG_Prescaler_256);

  /* Set counter reload value to max ~26 s */
  IWDG_SetReload(0xFFF);

  /* Reload IWDG counter */
  IWDG_ReloadCounter();

  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
}

/**
  * @brief  Configures Output Port GPIO.
  * @param  OUTport: Specifies the Port to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg OUTP_xxx
  * @retval None
  */
void BOARD_OUTPInit(OUTP_TypeDef OUTport)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO Clock */
  RCC_APB2PeriphClockCmd(OUTP_CLK[OUTport], ENABLE);

  /* Write default pin value */
  GPIO_WriteBit(OUTP_PORT[OUTport], OUTP_PIN[OUTport], OUTP_DEFAULT_VAL[OUTport]);

  /* Configure the GPIO pin */
  GPIO_InitStructure.GPIO_Pin = OUTP_PIN[OUTport];
  GPIO_InitStructure.GPIO_Mode = OUTP_MODE[OUTport]; 
  GPIO_InitStructure.GPIO_Speed = OUTP_SPEED[OUTport];

  GPIO_Init(OUTP_PORT[OUTport], &GPIO_InitStructure);
}

/**
  * @brief  Turns selected Port On.
  * @param  OUTport: Specifies the Port to be used. 
  *   This parameter can be one of following parameters:
  *     @arg OUTP_xxx
  * @retval None
  */
void BOARD_OUTPOn(OUTP_TypeDef OUTport)
{
  OUTP_PORT[OUTport]->BSRR = OUTP_PIN[OUTport]; 
}

/**
  * @brief  Turns selected Port Off.
  * @param  OUTport: Specifies the Port to be used. 
  *   This parameter can be one of following parameters:
  *     @arg OUTP_xxx
  * @retval None
  */
void BOARD_OUTPOff(OUTP_TypeDef OUTport)
{
  OUTP_PORT[OUTport]->BRR = OUTP_PIN[OUTport];
}

/**
  * @brief  Toggles the selected Port.
  * @param  OUTport: Specifies the Port to be used. 
  *   This parameter can be one of following parameters:
  *     @arg OUTP_xxx
  * @retval None
  */
void BOARD_OUTPToggle(OUTP_TypeDef OUTport)
{
  OUTP_PORT[OUTport]->ODR ^= OUTP_PIN[OUTport];
}

/**
  * @brief  Configures Button GPIO.
  * @param  INport: Specifies the Port to be configured.
  *   This parameter can be one of following parameters:
  *     @arg INP_xxx
  * @retval None
  */
void BOARD_INPInit(INP_TypeDef INport)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable Button GPIO clock */
  RCC_APB2PeriphClockCmd(INP_CLK[INport], ENABLE);
  
  /* Configure Button pin as input floating */
  GPIO_InitStructure.GPIO_Pin = INP_PIN[INport];
  GPIO_InitStructure.GPIO_Mode = INP_MODE[INport];
  GPIO_Init(INP_PORT[INport], &GPIO_InitStructure);
}

/**
  * @brief  Reads the specified input port pin.
  * @param  INport: Specifies the Port to be used.
  *   This parameter can be one of following parameters:
  *     @arg INP_xxx
  * @retval The GPIO pin value.
  */
uint32_t BOARD_INPReadBit(INP_TypeDef INport)
{
  return (INP_PORT[INport]->IDR & INP_PIN[INport]);
}

/**
  * @brief  Returns the selected Pin flag set or not.
  * @param  INport: Specifies the Port to be used.
  *   This parameter can be one of following parameters:
  *     @arg INP_xxx
  * @retval The Button GPIO pin flag (SET or RESET).
  */
uint8_t BOARD_INPGetFlag(INP_TypeDef INport)
{
  FlagStatus bitstatus;
  if(INP_PORT[INport]->IDR & INP_PIN[INport])
  {
    bitstatus = SET;
  }else{
    bitstatus = RESET;
  }
  /* Return the flag status */
  return bitstatus;
}

#if 0
/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1
  *     @arg COM2  
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *   contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void BOARD_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(COM_PORT_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable UART clock */
  if (COM == COM1)
  {
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
  }
  else
  {
    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(COM_PORT[COM], &GPIO_InitStructure);
    
  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(COM_PORT[COM], &GPIO_InitStructure);

  /* Reset USART */
  USART_DeInit(COM_USART[COM]);

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStruct);
    
  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
}


/**
  * @brief  Put char to the COM Port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1
  *     @arg COM2  
  * @param  ch: character
  * @retval None
  */
void BOARD_COM_PutChar(COM_TypeDef COM, uint8_t ch)
{
  /* Write a character to the USART */
  USART_SendData(COM_USART[COM], ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(COM_USART[COM], USART_FLAG_TC) == RESET){}
}


/**
  * @brief  Get char from the COM Port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1
  *     @arg COM2  
  * @retval character
  */
uint8_t BOARD_COM_GetChar(COM_TypeDef COM)
{
  uint8_t ch;

  /* Loop until RXNE = 1 */
  while(USART_GetFlagStatus(COM_USART[COM], USART_FLAG_RXNE) == RESET){}
  ch = (USART_ReceiveData(COM_USART[COM]));
  
  return ch;
}


/**
  * @brief  Put string to the COM Port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1
  *     @arg COM2  
  * @param  str: string
  * @retval None
  */
void BOARD_COM_PutStr(COM_TypeDef COM, const uint8_t *str)
{
  while(*str){
    BOARD_COM_PutChar(COM, *str++);
  }
}

/**
  * @brief  Put string to the standart output.
  * @param  str: string
  * @retval None
  */
void BOARD_COM_Printf(const char *str)
{
  while(*str){
    BOARD_COM_PutChar(COM1, *str++);
  }
}
#endif

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
#ifdef CFG_USE_RUNTIME_PRINTF
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(PORT_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(PORT_COM1, USART_FLAG_TC) == RESET){}
  
  return ch;
}
#endif /* CFG_USE_RUNTIME_PRINTF */

/**
  * @brief  Configures USART for C library printf function.
  * @param  None
  * @retval None
  */
void Printf_Configuration()
{
  USART_InitTypeDef USART_InitStructure;
  /* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the middle 
        - USART LastBit: The clock pulse of the last data bit is not output to 
                         the SCLK pin
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  BOARD_COMInit(COM1, &USART_InitStructure);

  /* Wait until transmiter ready */
  while (USART_GetFlagStatus(PORT_COM1, USART_FLAG_TC) == RESET){}
}

void PowerOn_Check()
{
  int i;
  long j;
  
  // check if not battery low
  //if (BOARD_INPReadBit(INP_LOBAT)){
  if (1){
    // check ON/Off held at least 5 * 100 ms = 0.5 sec
    for(i = 0; i < 5; i++){
      if (!BOARD_INPGetFlag(INP_BUT_ONOFF)){
        BOARD_OUTPOn(OUTP_PWRSHDN); // turn off
      }      
      //for(j=0; j < 250000; j++); // ~100 msec delay
      for(j=0; j < 130000; j++); // ~100 msec delay
    }
  }
  else{
    BOARD_OUTPOn(OUTP_PWRSHDN); // turn off
  }
}

void BOARD_Power_Off()
{ 
  BOARD_OUTPOn(OUTP_PWRSHDN); // turn off
}

#endif
