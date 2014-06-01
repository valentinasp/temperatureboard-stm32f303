/**
  ******************************************************************************
  * @file    adc.c
  * @author  
  * @version V0.0.1
  * @date    2012-11-16
  * @brief   This file contains all the functions 
  *          low level I/O functions for adc module
  ******************************************************************************
  */ 

/*******************************************************************/
#include "stm32f30x_adc.h"
#include "stm32f30x_dma.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_misc.h"
#include "stm32f30x.h"
#include "adc.h"  
/*******************************************************************/


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_ADDRESS    ((uint32_t)0x40012458)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/*******************************************************************/
GPIO_InitTypeDef  GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure; 
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
uint32_t ADC_Result;
/*******************************************************************/

ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
GPIO_InitTypeDef ADC_GPIO_InitStructure;

typedef enum 
{ 
  adc_channel_0 = 0,
  adc_channel_1,
  adc_channel_2,
  adc_channel_3,
  adc_channel_4,
  adc_channel_5,
  adc_channel_6 //Internel temperature
} ADC_Channels;

__IO uint16_t ADC_ConvertedValue_Tbl[7];

/* Private function prototypes -----------------------------------------------*/
void ADC_DMA_Config(void);
void ADC_RCC_Configuration(void);
void ADC_Initialization(void);



//void ADC_init(void){
//  ADC_RCC_Configuration();
  /* Set internal voltage regulator to 1.8V */
  //PWR_VoltageScalingConfig(PWR_VoltageScaling_Range1);
  /* Wait Until the Voltage Regulator is ready */
  //while (PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET);
  
  /* ADC1 channel0,channel1,channel2,channel3 configuration using DMA1 channel1 */
//  ADC_DMA_Config();
//}

void ADC_RCC_Configuration(void)
{  
  /* Enable HSI Clock */
  RCC_HSICmd(ENABLE);
  
  //RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  RCC_HSEConfig(RCC_HSE_OFF);  
    
  /* Enable ADC clock & SYSCFG */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_SYSCFG, ENABLE);

}


/**
  * @brief  Configure the ADC1 channel18 using DMA channel1.
  * @param  None
  * @retval None
  */
void ADC_DMA_Config(void)
{
  /*------------------------ DMA1 configuration ------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* DMA1 channel1 configuration */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue_Tbl[adc_channel_0];
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 4;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /*----------------- ADC1 configuration with DMA enabled --------------------*/
  /* Enable the HSI oscillator */
  RCC_HSICmd(ENABLE);

  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  /* Configure PA.4 (ADC Channel18) in analog mode */
  ADC_GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
  ADC_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  ADC_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &ADC_GPIO_InitStructure);//???

  /* Check that HSI oscillator is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

  /* Enable ADC1 clock */
 // RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* ADC1 configuration */
  //ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  //ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  //ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //ADC_InitStructure.ADC_NbrOfConversion = 4;
  //ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel0 configuration */ 
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_384Cycles);
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_384Cycles);
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_384Cycles);
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_384Cycles);

  /* Enable the request after last transfer for DMA Circular mode */
  //ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Wait until the ADC1 is ready */
  //while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
  {
  }

  /* Start ADC1 Software Conversion */ 
  //ADC_SoftwareStartConv(ADC1);
}



void EnableADC1(void){
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Wait until the ADC1 is ready */
  //while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
  {
  }

  /* Start ADC1 Software Conversion */ 
  //ADC_SoftwareStartConv(ADC1);
}

void DisableADC1(void){
  /* Diseble ADC1 DMA */
  ADC_DMACmd(ADC1, DISABLE);
  
  /* Disable ADC1 */
  ADC_Cmd(ADC1, DISABLE);
}

/**
  * @brief  Returns the last ADC conversion result data for selected channel.  
  * @param  Channalx: where x can be 1 to select the Channel1 peripheral.
  * @retval The Data conversion value.
  */
uint16_t ADC_GetChannelConversionValue(size_t Channalx)
{
  /* Return the selected ADC conversion value */
  return (uint16_t) ADC_ConvertedValue_Tbl[Channalx];
}


/*******************************************************************/
void ADC_Initialization(void)
{
    // Включаем тактирование DMA1, ADC12 и GPIOA
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	
 
    // Настраиваем пин на работу в режиме аналогового входа
    GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    // Настройки DMA
//============================================================
    DMA_DeInit(DMA1_Channel1);
    // Данные будем брать из регистра данных ADC1
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
    // Переправлять данные будем в переменную ADC_Result
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue_Tbl[adc_channel_0];
    // Передача данных из периферии в память
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    // Размер буфера
    DMA_InitStructure.DMA_BufferSize = 4;
    // Адрес источника данных не инкрементируем - он всегда один и 
    // тот же
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // Аналогично, и с памятью
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    // Настройки размера данных
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    // Включаем первый канал DMA1
    DMA_Cmd(DMA1_Channel1, ENABLE);
 //---------------------------------------------------------
    
    DMA_DeInit(DMA2_Channel1);
    // Данные будем брать из регистра данных ADC1
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC2->DR);
    // Переправлять данные будем в переменную ADC_Result
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue_Tbl[adc_channel_4];
    // Размер буфера
    DMA_InitStructure.DMA_BufferSize = 3;
    
    DMA_Init(DMA2_Channel1, &DMA_InitStructure);
    // Включаем первый канал DMA1
    DMA_Cmd(DMA2_Channel1, ENABLE);
//============================================================

    // Настраиваем тактирование АЦП
    RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);
 
    ADC_StructInit(&ADC_InitStructure);
 
    // Калибровка АЦП
//============================================================    
    ADC_VoltageRegulatorCmd(ADC1, ENABLE);
    ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
    ADC_StartCalibration(ADC1);
 //---------------------------------------------------------
    ADC_VoltageRegulatorCmd(ADC2, ENABLE);
    ADC_SelectCalibrationMode(ADC2, ADC_CalibrationMode_Single);
    ADC_StartCalibration(ADC2);
//============================================================
    
    // Настраиваем непрерывные преобразования  
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                                                                    
    ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;                    
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;             
    ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;                  
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;          
    ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
    ADC_CommonInit(ADC2, &ADC_CommonInitStructure);
 
 // Включаем работу ДМА через АЦП
    ADC_DMACmd(ADC1, ENABLE);
    ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
 
    ADC_DMACmd(ADC2, ENABLE);
    ADC_DMAConfig(ADC2, ADC_DMAMode_Circular);
    
    while(ADC_GetCalibrationStatus(ADC1) != RESET );
    while(ADC_GetCalibrationStatus(ADC2) != RESET );
    
    // Продолжается настройка АЦП
    ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 
    ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;         
    ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;   
    ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;  
    ADC_InitStructure.ADC_NbrOfRegChannel = 4;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    ADC_InitStructure.ADC_NbrOfRegChannel = 3;
    ADC_Init(ADC2, &ADC_InitStructure);
 
    ADC_TempSensorCmd(ADC1, ENABLE);
    // Включаем третий канал первого модуля АЦП 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_181Cycles5 );//ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_601Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_601Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_601Cycles5 );
    
    ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_601Cycles5);//ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 2, ADC_SampleTime_601Cycles5);
    ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 3, ADC_SampleTime_601Cycles5);
    
      //ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_384Cycles);
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_384Cycles);
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_384Cycles);
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_384Cycles);
 
    // Наконец-то включаем АЦП
    ADC_Cmd(ADC1, ENABLE);
    ADC_Cmd(ADC2, ENABLE);
 
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));  
    while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_RDY));  
 
    ADC_StartConversion(ADC1); 
    ADC_StartConversion(ADC2);  
 
//    ADC_Result = ADC_GetConversionValue(ADC1);
}
 
/*******************************************************************/

