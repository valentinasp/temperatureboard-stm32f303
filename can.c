/******************** (C) COPYRIGHT 2013  **************************************
* File Name          : can.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 05/07/2014
* Description        : can config functons
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "stm32f30x_it.h"
#include "drv_stm32_can.h"
#include "drv_can_open.h"
#include "i2c.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//CAN Baudrate
//#define CAN_1000 
//#define CAN_500 
//#define CAN_250 
#define CAN_125
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CanTxMsg TxMessage = {0};
/* Private function prototypes -----------------------------------------------*/
static void CAN_Config(void);
/* Exported functions prototypes ---------------------------------------------*/
extern void InitCAN(void);

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Initialisation the CAN.
  * @param  None
  * @retval None
  */
void InitCAN(void){
  /* CAN configuration */
  CAN_Config();
  Init_RecordTable(0, 0, (I2C_GetBoardsNr()*MAXHCHANNEL+MAXTCHANNEL), 0);
  SystemStateChange(0);
}

/* Private functions ---------------------------------------------------------*/

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

#ifdef CAN_1000
  /* CAN Baudrate = 1MBps (CAN clocked at 36 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 2;
#endif

#ifdef CAN_500  
  /* Baudrate = 500 Kbps (CAN clocked with 36Mhz)*/
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 4;
#endif
  
#ifdef CAN_250  
  /* Baudrate = 250 Kbps (CAN clocked with 36Mhz)*/
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 8;
#endif
  
#ifdef CAN_125  
  /* CAN Baudrate = 125kbps (CAN clocked at 36 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 16;
#endif
    
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
  Init_CanIrq();
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/