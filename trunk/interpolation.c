/******************** (C) COPYRIGHT 2013  **************************************
* File Name          : interpolation.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 10/3/2013
* Description        : Interpoliation (lagranz) functons
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "interpolation.h"
#include "stm32f30x_it.h"
#include <stdlib.h>     /* malloc, free, rand */
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ARRAYSIZE 2
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static size_t MaxPoints=0;
static size_t MaxChannelNr=0;
static double* arr_x; // ADC values array
static double* arr_y; // points values array
/* Private function prototypes -----------------------------------------------*/
static double chislZnamLag(double* arr_x, double* arr_y, int n, double x_isk, int i);
static double Lagranz(double* arr_x, double* arr_y, int n, double x_isk);
/* Exported functions ------------------------------------------------------- */

bool InitInterpolationValues(size_t CurrMaxChannelNr,size_t CurrMaxPoints){
  if((arr_x = (double*)malloc(CurrMaxChannelNr*CurrMaxPoints*sizeof(double)))==NULL){
    return false;
  }else if((arr_y = (double*)malloc(CurrMaxChannelNr*CurrMaxPoints*sizeof(double)))==NULL){
    return false;
  }else{
    MaxPoints=CurrMaxPoints;
    MaxChannelNr=CurrMaxChannelNr;
    return true;
  }
}

bool DeinitInterpolationValues(void){
  free(arr_x);
  free(arr_y);
  return true;
}

bool SetInterpolationValues(double* ADCvalues,double* PointsValues,size_t ChannelNr){
  if(ChannelNr>=MaxChannelNr)
    return false;
  memcpy(&arr_x[ChannelNr*MaxPoints],ADCvalues,sizeof(double)*MaxPoints);
  memcpy(&arr_y[ChannelNr*MaxPoints],PointsValues,sizeof(double)*MaxPoints);
  return true;
}

bool GetValue(size_t ChannelNr,double CurrADCValue,double* CalculatedValue){
  if(ChannelNr>MaxChannelNr)
    return false;
  *CalculatedValue=Lagranz(&arr_x[ChannelNr*MaxPoints],&arr_y[ChannelNr*MaxPoints], MaxPoints, CurrADCValue);
  return true;
}


/* Private functions ---------------------------------------------------------*/
static double chislZnamLag(double* arr_x, double* arr_y, int n, double x_isk, int i)
{
   double Chesl = 1, Znam = 1;

   for (int k = 0; k!= n; k++ )
   {
      if ( k == i ) continue;

      Chesl *= (x_isk - arr_x[k]);
   }

   for(int k= 0; k!= n;k++)
   {
      if (arr_x [i]== arr_x[k]) continue;
      Znam *= arr_x[i] - arr_x[k];
   }

   return Chesl/Znam;
 }

// arr_x array off acp values
// arr_y array off points values (example temperature 0 and  100)
// n number off pars
// x_isk current acp value
static double Lagranz(double* arr_x, double* arr_y, int n, double x_isk)
{
   double result = 0.0;

   for (int i = 0; i != n; i++)
   {
      result += arr_y[i]*chislZnamLag(arr_x,arr_y,n,x_isk,i);
   }

   return result;
 }
/* Exported functions --------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/