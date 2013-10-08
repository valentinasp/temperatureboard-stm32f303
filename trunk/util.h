/******************** (C) COPYRIGHT 2012 Lifodas *******************************
* File Name          : util.h
* Author             : vpa
* Version            : V1.0.0
* Date               : 02/02/2012
* Description        : helper functions
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_UTILITY_H
#define __I2C_UTILITY_H
/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define __SET_BIT(flag,bit) ( (flag)|=(1<<(bit)) )
#define __CLEAR_BIT(flag,bit) ( (flag)&=~(1<<(bit)) )
#define __TEST_BIT(flag,bit) ( ((flag)>>(bit))&1 )
/* Exported functions ------------------------------------------------------- */
int TERMINAL_IO( const char *format, ... );
/* External variables --------------------------------------------------------*/

#endif  /*__I2C_UTILITY_H*/

/******************* (C) COPYRIGHT 2012 Lifodas ****************END OF FILE****/
