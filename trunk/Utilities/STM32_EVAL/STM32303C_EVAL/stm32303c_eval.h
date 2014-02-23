/**
  ******************************************************************************
  * @file    stm32303c_eval.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    23-October-2012
  * @brief   This file contains definitions for STM32303C_EVAL's Leds, push-buttons
  *          and COM ports hardware resources.
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32303C_EVAL_H
#define __STM32303C_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "stm32_eval_legacy.h"
#include "hw_config.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup STM32303C_EVAL
  * @{
  */
      
/** @addtogroup STM32303C_EVAL_LOW_LEVEL
  * @{
  */ 

/** @defgroup STM32303C_EVAL_LOW_LEVEL_Exported_Types
  * @{
  */






typedef enum 
{ 
  JOY_NONE = 0,
  JOY_SEL = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOYState_TypeDef;

/**
  * @}
  */ 

/** @defgroup STM32303C_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** 
  * @brief  Define for STM32303C_EVAL board  
  */ 
#if !defined (USE_STM32303C_EVAL)
 #define USE_STM32303C_EVAL
#endif

/** @addtogroup STM32303C_EVAL_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             2

#define LED1_PIN                         GPIO_Pin_8
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOB
  
#define LED2_PIN                         GPIO_Pin_9
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOB

/**
  * @}
  */ 

/** @addtogroup STM32303C_EVAL_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          6

/**
 * @brief Key push-button
 */
#define KEY_BUTTON_PIN                   GPIO_Pin_6
#define KEY_BUTTON_GPIO_PORT             GPIOE
#define KEY_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOE
#define KEY_BUTTON_EXTI_LINE             EXTI_Line6
#define KEY_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOE
#define KEY_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource6
#define KEY_BUTTON_EXTI_IRQn             EXTI9_5_IRQn

/**
 * @brief Joystick Right push-button
 */
#define RIGHT_BUTTON_PIN                 GPIO_Pin_6
#define RIGHT_BUTTON_GPIO_PORT           GPIOD
#define RIGHT_BUTTON_GPIO_CLK            RCC_AHBPeriph_GPIOD
#define RIGHT_BUTTON_EXTI_LINE           EXTI_Line6
#define RIGHT_BUTTON_EXTI_PORT_SOURCE    EXTI_PortSourceGPIOD
#define RIGHT_BUTTON_EXTI_PIN_SOURCE     EXTI_PinSource6
#define RIGHT_BUTTON_EXTI_IRQn           EXTI9_5_IRQn

/**
 * @brief Joystick Left push-button
 */
#define LEFT_BUTTON_PIN                  GPIO_Pin_5
#define LEFT_BUTTON_GPIO_PORT            GPIOB
#define LEFT_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define LEFT_BUTTON_EXTI_LINE            EXTI_Line5
#define LEFT_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOB
#define LEFT_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource5
#define LEFT_BUTTON_EXTI_IRQn            EXTI9_5_IRQn  

/**
 * @brief Joystick Up push-button
 */
#define UP_BUTTON_PIN                    GPIO_Pin_7
#define UP_BUTTON_GPIO_PORT              GPIOE
#define UP_BUTTON_GPIO_CLK               RCC_AHBPeriph_GPIOE
#define UP_BUTTON_EXTI_LINE              EXTI_Line7
#define UP_BUTTON_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOE
#define UP_BUTTON_EXTI_PIN_SOURCE        EXTI_PinSource7
#define UP_BUTTON_EXTI_IRQn              EXTI9_5_IRQn  

/**
 * @brief Joystick Down push-button
 */  
#define DOWN_BUTTON_PIN                  GPIO_Pin_5
#define DOWN_BUTTON_GPIO_PORT            GPIOD
#define DOWN_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOD
#define DOWN_BUTTON_EXTI_LINE            EXTI_Line5
#define DOWN_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOD
#define DOWN_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource5
#define DOWN_BUTTON_EXTI_IRQn            EXTI9_5_IRQn  

/**
 * @brief Joystick Sel push-button
 */
#define SEL_BUTTON_PIN                   GPIO_Pin_13
#define SEL_BUTTON_GPIO_PORT             GPIOC
#define SEL_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOC
#define SEL_BUTTON_EXTI_LINE             EXTI_Line13
#define SEL_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOC
#define SEL_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource13
#define SEL_BUTTON_EXTI_IRQn             EXTI15_10_IRQn 

/**
  * @}
  */ 


/** @addtogroup STM32303C_EVAL_LOW_LEVEL_COM
  * @{
  */

/**
 * @brief Definition for COM port1, connected to USART1
 */ 


/**
  * @}
  */

/** @addtogroup STM32303C_EVAL_LOW_LEVEL_SD_SPI
  * @{
  */
/**
  * @brief  SD SPI Interface pins
  */
#define SD_SPI                           SPI2
#define SD_SPI_CLK                       RCC_APB1Periph_SPI2

#define SD_SPI_SCK_PIN                   GPIO_Pin_9                  /* PF.09 */
#define SD_SPI_SCK_GPIO_PORT             GPIOF                       /* GPIOF */
#define SD_SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOF
#define SD_SPI_SCK_SOURCE                GPIO_PinSource9
#define SD_SPI_SCK_AF                    GPIO_AF_5

#define SD_SPI_MISO_PIN                  GPIO_Pin_14                  /* PB.14 */
#define SD_SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOB */
#define SD_SPI_MISO_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SD_SPI_MISO_SOURCE               GPIO_PinSource14
#define SD_SPI_MISO_AF                   GPIO_AF_5

#define SD_SPI_MOSI_PIN                  GPIO_Pin_15                  /* PB.15 */
#define SD_SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOB */
#define SD_SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SD_SPI_MOSI_SOURCE               GPIO_PinSource15
#define SD_SPI_MOSI_AF                   GPIO_AF_5

#define SD_CS_PIN                        GPIO_Pin_15                  /* PE.15 */
#define SD_CS_GPIO_PORT                  GPIOE                       /* GPIOE */
#define SD_CS_GPIO_CLK                   RCC_AHBPeriph_GPIOF

   
#define SD_DETECT_PIN                    GPIO_Pin_6                 /* PC.06 */
#define SD_DETECT_EXTI_LINE              EXTI_Line6
#define SD_DETECT_EXTI_PIN_SOURCE        EXTI_PinSource6
#define SD_DETECT_GPIO_PORT              GPIOC                       /* GPIOC */
#define SD_DETECT_GPIO_CLK               RCC_AHBPeriph_GPIOC
#define SD_DETECT_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOC
#define SD_DETECT_EXTI_IRQn              EXTI9_5_IRQn

/**
  * @}
  */

/** @addtogroup STM32303C_EVAL_LOW_LEVEL_sEE_SPI
  * @{
  */
/**
  * @brief  sEE SPI Interface pins
  */
#define sEE_SPI                          SPI2
#define sEE_SPI_CLK                      RCC_APB1Periph_SPI2
   
#define sEE_SPI_SCK_PIN                  GPIO_Pin_9                   /* PF.09 */
#define sEE_SPI_SCK_GPIO_PORT            GPIOF                        /* GPIOF */
#define sEE_SPI_SCK_GPIO_CLK             RCC_AHBPeriph_GPIOF
#define sEE_SPI_SCK_SOURCE               GPIO_PinSource9
#define sEE_SPI_SCK_AF                   GPIO_AF_5

#define sEE_SPI_MISO_PIN                 GPIO_Pin_14                  /* PB.14 */
#define sEE_SPI_MISO_GPIO_PORT           GPIOB                        /* GPIOB */
#define sEE_SPI_MISO_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define sEE_SPI_MISO_SOURCE              GPIO_PinSource14
#define sEE_SPI_MISO_AF                  GPIO_AF_5

#define sEE_SPI_MOSI_PIN                 GPIO_Pin_15                  /* PB.15 */
#define sEE_SPI_MOSI_GPIO_PORT           GPIOB                        /* GPIOB */
#define sEE_SPI_MOSI_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define sEE_SPI_MOSI_SOURCE              GPIO_PinSource15
#define sEE_SPI_MOSI_AF                  GPIO_AF_5

#define sEE_SPI_CS_PIN                   GPIO_Pin_7                  /* PD.07 */
#define sEE_SPI_CS_GPIO_PORT             GPIOD                       /* GPIOD */
#define sEE_SPI_CS_GPIO_CLK              RCC_AHBPeriph_GPIOD

/**
  * @brief  sEE SPI Interface Type
  */
#define sEE_M95M01 

/**
  * @}
  */
  
/** @addtogroup STM32303C_EVAL_LOW_LEVEL_TSENSOR_I2C
  * @{
  */
/**
  * @brief  TS751 Temperature Sensor I2C Interface pins
  */
#define TS751_I2C                        I2C2
#define TS751_I2C_CLK                    RCC_APB1Periph_I2C2

#define TS751_I2C_SCL_PIN                GPIO_Pin_6                  /* PF.06 */
#define TS751_I2C_SCL_GPIO_PORT          GPIOF                       /* GPIOF */
#define TS751_I2C_SCL_GPIO_CLK           RCC_AHBPeriph_GPIOF
#define TS751_I2C_SCL_SOURCE             GPIO_PinSource6
#define TS751_I2C_SCL_AF                 GPIO_AF_4

#define TS751_I2C_SDA_PIN                GPIO_Pin_10                  /* PA.10 */
#define TS751_I2C_SDA_GPIO_PORT          GPIOA                       /* GPIOA */
#define TS751_I2C_SDA_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define TS751_I2C_SDA_SOURCE             GPIO_PinSource10
#define TS751_I2C_SDA_AF                 GPIO_AF_4

#define TS751_I2C_SMBUSALERT_PIN         GPIO_Pin_8                  /* PA.08 */
#define TS751_I2C_SMBUSALERT_GPIO_PORT   GPIOA                       /* GPIOA */
#define TS751_I2C_SMBUSALERT_GPIO_CLK    RCC_AHBPeriph_GPIOA
#define TS751_I2C_SMBUSALERT_SOURCE      GPIO_PinSource8
#define TS751_I2C_SMBUSALERT_AF          GPIO_AF_4

/**
  * @}
  */
   
/** @addtogroup STM32303C_EVAL_LOW_LEVEL_I2C_EE
  * @{
  */
/**
  * @brief  I2C EEPROM Interface pins
  */  
#define sEE_I2C                          I2C2
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C2
   
#define sEE_I2C_SCL_PIN                  GPIO_Pin_6                  /* PF.06 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOF                       /* GPIOF */
#define sEE_I2C_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOF
#define sEE_I2C_SCL_SOURCE               GPIO_PinSource6
#define sEE_I2C_SCL_AF                   GPIO_AF_4

#define sEE_I2C_SDA_PIN                  GPIO_Pin_10                  /* PA.10 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOA                       /* GPIOA */
#define sEE_I2C_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOA
#define sEE_I2C_SDA_SOURCE               GPIO_PinSource10
#define sEE_I2C_SDA_AF                   GPIO_AF_4

/**
  * @}
  */

/** @defgroup STM32303C_EVAL_LOW_LEVEL_Exported_Functions
  * @{
  */

//void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void); 
void TS751_LowLevel_DeInit(void);
void TS751_LowLevel_Init(void);
void sEE_SPI_LowLevel_DeInit(void);
void sEE_SPI_LowLevel_Init(void);
void sEE_I2C_LowLevel_DeInit(void);
void sEE_I2C_LowLevel_Init(void); 

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32303C_EVAL_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
