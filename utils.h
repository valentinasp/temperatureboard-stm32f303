#ifndef __UTILS_H
#define __UTILS_H

#include "util.h"
#include "utils.h"
#include "type.h"

/*---------------------------------------------------------------------------*/
#define CNTLQ       0x11
#define CNTLS       0x13
#define DEL         0x7F
#define BACKSPACE   0x08
#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B




extern int getline (char *lp, unsigned int n);
extern bool getmagicline (char *lp, unsigned int n);
//extern int strtoul(char *s);
extern int WaitEventWithTimeout( int ( *event ) ( void ) , int cond , unsigned long ms );
/*---------------------------------------------------------------------------*/



//#define CMD_COUNT   (sizeof (cmd) / sizeof (cmd[0]))


#endif

