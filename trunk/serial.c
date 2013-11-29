/*
* File Name          : serial.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 18/2/2013
* Description        : Srial port functons
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "serial.h"
#include "stm32f30x.h"
#include "stm32f30x_it.h"
#include "stm32f30x_usart.h"
#include "hw_config.h"

#include "stdio.h"
#include "stdlib.h"
#include "type.h"
#include "util.h"
#include "utils.h"


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
/* Private function prototypes -----------------------------------------------*/



/* Private functions ---------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *       init_serial:  Initialize Serial Interface
 *---------------------------------------------------------------------------*/
void init_serial (void) {
  /* Initialize the serial interface */

  /* NVIC configuration */
  NVIC_Config();
  
  /* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);

  /* Enable the EVAL_COMX Transmoit interrupt: this interrupt is generated when the 
     EVAL_COM1 transmit data register is empty */  
  USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);
  
  /* Wait until EVAL_COMX send the TxBuffer */
  while(TxCounter < NbrOfDataToTransfer)
  {}
  
  /* The software must wait until TC=1. The TC flag remains cleared during all data
     transfers and it is set by hardware at the last frame’s end of transmission*/
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}
 
  /* Enable the EVAL_COMX Receive interrupt: this interrupt is generated when the 
     EVAL_COMX receive data register is not empty */
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
}




/*----------------------------------------------------------------------------
 *       sendchar:  Write a character to Serial Port
 *---------------------------------------------------------------------------*/
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET)
  {}

  return ch;
}
/*----------------------------------------------------------------------------
 *       getkey:  Read a character from Serial Port
 *---------------------------------------------------------------------------*/
int getkey (void) {
  while(RxCounter < NbrOfDataToRead)
  {}
  RxCounter = 0;
  return RxBuffer[0];
}
/*----------------------------------------------------------------------------
 *       get_char:  Read a character from Serial Port not stoped function
 *---------------------------------------------------------------------------*/
int get_char (void) {
  if(RxCounter)
  {
    RxCounter = 0;
    return RxBuffer[0];
  }else{
    return 0;
  }
}
/*----------------------------------------------------------------------------
 *       RxDataReady:  if U1RBR contains valid data reurn 1, otherwise 0
 *---------------------------------------------------------------------------*/
/*
int RxDataReady( void )
{
	//return ( U1LSR & 0x01 );	//Receive data ready
	//return ( U0LSR & 0x01 );	//Receive data ready
  return 0;
}
*/
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/




