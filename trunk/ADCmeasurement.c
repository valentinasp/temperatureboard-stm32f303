/******************** (C) COPYRIGHT 2013  **************************************
* File Name          : ADCmeasurement.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 9/3/2013
* Description        : ADC module measurement functons
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "ADCmeasurement.h"
#include "delay.h"
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* qsort */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TEMP_VALUES_SIZE 100
#define CHANNELS_NR 6
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned int tempValues[CHANNELS_NR][TEMP_VALUES_SIZE];
/* Private function prototypes -----------------------------------------------*/
int comp(const void *i, const void *j);
int floatcomp(const void* elem1, const void* elem2);
//float findMode(float *array, int elems);
unsigned int findMode(unsigned int *array, int elems);

/* Private functions ---------------------------------------------------------*/
int comp(const void *i, const void *j)
{
  return *(unsigned int *)i - *(unsigned int *)j;
}

int floatcomp(const void* elem1, const void* elem2)
{
    if(*(const float*)elem1 < *(const float*)elem2)
        return -1;
    return *(const float*)elem1 > *(const float*)elem2;
}

//float findMode(float *array, int elems)
unsigned int findMode(unsigned int *array, int elems)
{
       //float lastnum = array[0];
       unsigned int lastnum = array[0];
       int curlen = 1;
       //float mode = array[0];
       unsigned int mode = array[0];
       int modelen = 1;
       for(int i = 1; i < elems; i++)
       {
           if (array[i] == lastnum)
               curlen++;
           else
           {
               if (curlen > modelen)
               {
                   modelen = curlen;
                   mode = lastnum;
               }
               lastnum = array[i];
               curlen = 1;
           }
       }

    return mode;
}

/* Exported functions --------------------------------------------------------*/

bool GetADCValues(unsigned int* Values,size_t ValuesNr){
  if(ValuesNr>CHANNELS_NR)
    return false; 
  
  for(size_t i=0;i<TEMP_VALUES_SIZE;i++)
  {
    for(size_t ch=0;ch<CHANNELS_NR;ch++){
      tempValues[ch][i] = ADC_GetChannelConversionValue(ch);
    }
  }
  
  for(size_t ch=0;ch<CHANNELS_NR;ch++){
    Values[ch]=findMode(tempValues[ch], TEMP_VALUES_SIZE);
  }
  
  return true;
  //qsort(tempValues, TEMP_VALUES_SIZE, sizeof(unsigned int), comp);
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

