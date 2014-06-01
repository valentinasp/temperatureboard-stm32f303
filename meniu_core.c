/*
* File Name          : meniu_core.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 18/2/2013
* Description        : Menu core
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_adc.h"
#include "stdlib.h"
#include "utils.h"
#include "type.h"
#include <stdio.h>
#include <ctype.h>
#include "string.h"
#include "menu_core.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static char in_line[80];
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static char *get_entry (char *cp, char **pNext);


void Menu(const SCMD* menu){
  char *sp,*next,*cp;
  int i;
  size_t sizeofarray = 0;
  //get size of array 
  while (menu[sizeofarray].m_Function!= NULL) {
    sizeofarray++;
  }
  for(;;){
    printf ("\r\nCmd> ");                     /* display prompt              */
    fflush (stdout);
  
    if (getline (in_line, sizeof (in_line)) == false) {
           continue;
        }
    sp = get_entry (&in_line[0], &next);
      if (*sp == 0) {
         continue;
      }

    for (cp = sp; *cp && *cp != ' '; cp++) {
       *cp = toupper (*cp);                 /* command to upper-case       */
    }
    for (i = 0; i < sizeofarray; i++) {
       if (strcmp (sp, (const char *)&menu[i].val)) {
          continue;
       }
       if(menu[i].m_Function())// (next);                  /* execute command function */
        break;
       else
         return;
    }
    if (i == sizeofarray) {
      printf ("\nCommand error\n");
    }
  }
}

static char *get_entry (char *cp, char **pNext) {
   char *sp, lfn = 0, sep_ch = ' ';

   if (cp == NULL) {                          /* skip NULL pointers          */
      *pNext = cp;
      return (cp);
   }

   for ( ; *cp == ' ' || *cp == '\"'; cp++) { /* skip blanks and starting  " */
      if (*cp == '\"') { sep_ch = '\"'; lfn = 1; }
      *cp = 0;
   }
 
   for (sp = cp; *sp != CR && *sp != LF; sp++) {
      if ( lfn && *sp == '\"') break;
      if (!lfn && *sp == ' ' ) break;
   }

   for ( ; *sp == sep_ch || *sp == CR || *sp == LF; sp++) {
      *sp = 0;
      if ( lfn && *sp == sep_ch) { sp ++; break; }
   }

   *pNext = (*sp) ? sp : NULL;                /* next entry                  */
   return (cp);
}