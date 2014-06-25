/******************** (C) COPYRIGHT 2011  **************************************
* File Name          : menu.h
* Author             : Valentinas
* Version            : V1.0.0
* Date               : 18/2/2013
* Description        : Meniu functions prototypes
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MENIU_H
#define __MENIU_H

/* Includes ------------------------------------------------------------------*/
#include "type.h"
/* Exported types ------------------------------------------------------------*/

/* Command definitions structure. */
typedef struct {//scmd {
   char val[8];
   bool (*m_Function)(void);
   //const struct scmd *m_SubMenu;
   //void (*func)(char *par);
} SCMD;

/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/



bool cmd_bootinfo(void);
bool cmd_erase_calibration(void);
bool cmd_run_calibration(void);
bool cmd_setboard_id(void);
bool cmd_help(void);
bool cmd_exit(void);

bool cmd_channal1(void);
bool cmd_channal2(void);
bool cmd_channal3(void);
bool cmd_channal4(void);
#endif  /*__MENIU_H*/

/******************* (C) COPYRIGHT ****************END OF FILE****/