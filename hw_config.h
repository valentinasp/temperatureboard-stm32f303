/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : hw_config.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Define the Device Mode Type */
typedef enum 
{
  DevMode_USB = 0,
  DevMode_PROBE = 1,
  DevMode_SUPPORT = 2
} DevMode_TypeDef;

#define PORT_INP_N    7

/* Define the Input Port Type */
typedef enum 
{
  INP_BUT_CALIB    = 0,
  INP_BUT_ONOFF
} INP_TypeDef;

#define PORT_OUTP_N    11

/* Define the Output Port Type */
typedef enum 
{
  OUTP_PWRSHDN     = 0,
  OUTP_MAIN_ON,
  OUTP_OMAP_RSTn,
  OUTP_OMAP_ON,
  OUTP_BL_CTRL,
  OUTP_VFL_ON,
  OUTP_BL_ON,
  OUTP_OMAP_IN0,
  OUTP_OMAP_IN1,  
  OUTP_OMAP_IN2,    
  OUTP_LED,
} OUTP_TypeDef;

/* Define the COM ports Type connected to UART */
typedef enum 
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#ifdef IMAGE1
#define PROGRAMM_AREA_BEG_ADDR	0x08006000
#else
  #define PROGRAMM_AREA_BEG_ADDR	0x08002400
#endif

#define APP_NVIC_OFFSET (PROGRAMM_AREA_BEG_ADDR ^ 0x08000000)


/* Define the Input Ports---------------------------------------------------- */
// clibration button
#define PORT_INP_00_PORT    GPIOB
#define PORT_INP_00_CLK     RCC_APB2Periph_GPIOB
#define PORT_INP_00_PIN     GPIO_Pin_12
#define PORT_INP_00_MODE    GPIO_Mode_IN_FLOATING
// reset button
#define PORT_INP_01_PORT    GPIOA
#define PORT_INP_01_CLK     RCC_APB2Periph_GPIOA
#define PORT_INP_01_PIN     GPIO_Pin_3
#define PORT_INP_01_MODE    GPIO_Mode_IN_FLOATING

/* Define the Output Ports--------------------------------------------------- */
// power shutdown
#define PORT_OUTP_00_PORT        GPIOA
#define PORT_OUTP_00_PIN         GPIO_Pin_7
#define PORT_OUTP_00_CLK         RCC_APB2Periph_GPIOA
#define PORT_OUTP_00_MODE        GPIO_Mode_Out_PP
#define PORT_OUTP_00_SPEED       GPIO_Speed_2MHz
#define PORT_OUTP_00_DEFAULT_VAL Bit_RESET
// main on
#define PORT_OUTP_01_PORT        GPIOA
#define PORT_OUTP_01_PIN         GPIO_Pin_8
#define PORT_OUTP_01_CLK         RCC_APB2Periph_GPIOA
#define PORT_OUTP_01_MODE        GPIO_Mode_Out_PP
#define PORT_OUTP_01_SPEED       GPIO_Speed_2MHz
#define PORT_OUTP_01_DEFAULT_VAL Bit_RESET

/* Define the COM ports connected to UART ------------------------------------*/

#define COM_N                     1

/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define PORT_COM1                   USART1
#define PORT_COM1_GPIO              GPIOA
#define PORT_COM1_CLK               RCC_APB2Periph_USART1
#define PORT_COM1_GPIO_CLK          RCC_APB2Periph_GPIOA
#define PORT_COM1_RxPin             GPIO_Pin_10
#define PORT_COM1_TxPin             GPIO_Pin_9

/**
 * @brief Definition for COM port2, connected to USART2
 */ 
#define PORT_COM2                   USART2
#define PORT_COM2_GPIO              GPIOA
#define PORT_COM2_CLK               RCC_APB1Periph_USART2
#define PORT_COM2_GPIO_CLK          RCC_APB2Periph_GPIOA
#define PORT_COM2_RxPin             GPIO_Pin_3
#define PORT_COM2_TxPin             GPIO_Pin_2

/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/

#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
