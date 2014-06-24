#include <stdio.h>
#include <ctype.h>
//#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "serial.h"


//int strtoul(char *s);
//extern int  getkey (void);

/*----------------------------------------------------------------------------
 *      Line Editor
 *---------------------------------------------------------------------------*/
signed int getline (char *lp, unsigned int n) {
   unsigned int cnt = 0;
   char c;

   do {
      c = getkey ();
      //c = get_char();
      switch (c) {
         case CNTLQ:                       /* ignore Control S/Q             */
         case CNTLS:
            break;;
         case BACKSPACE:
         case DEL:
            if (cnt == 0) {
               break;
            }
            cnt--;                         /* decrement count                */
            lp--;                          /* and line pointer               */
            putchar (0x08);                /* echo backspace                 */
            putchar (' ');
            putchar (0x08);
            fflush (stdout);
            break;
         case ESC:
            *lp = 0;                       /* ESC - stop editing line        */
            return (false);
         case CR:                          /* CR - done, stop editing line   */
            *lp = c;
            lp++;                          /* increment line pointer         */
            cnt++;                         /* and count                      */
            c = LF;
         default:
            putchar (*lp = c);             /* echo and store character       */
            fflush (stdout);
            lp++;                          /* increment line pointer         */
            cnt++;                         /* and count                      */
            break;
      }
   } while (cnt < n - 2  &&  c != LF);     /* check limit and CR             */
   *lp = 0;                                /* mark end of string             */
   return (true);
}

bool getmagicline (char *lp, unsigned int n) {
   char *str_buffer;
   char c;

   c = get_char();
   
   if(c){
     if(isprint(c)){
       str_buffer = (char*) malloc (n+1);
       memcpy(str_buffer,&lp[1],n-1);
       str_buffer[n-1] = c;
       memcpy(lp,str_buffer,n);
       free(str_buffer);
       return (true);
     }else{
        return (false);
     }
   }else{
     return (false);
   }
}

/* string -> signed int */
/*int strtoul(char *s)
{
	int ret;
	int radix = 10;
	int negative = 0;
	int i;

	ret = 0;
	if(*s == '-') 
	{
		negative = 1;
		s++;
	}
	else if(*s == '0')
	{
		s++;
		if(*s == 'x')
		{
			s++;
			radix = 0x10;
		}
	}

	while (*s) {
		if (*s >= '0' && *s <= '9')
			i = *s - '0';
		else if (*s >= 'a' && *s <= 'f')
			i = *s - 'a' + 0xa;
		else if (*s >= 'A' && *s <= 'F')
			i = *s - 'A' + 0xa;
		else
			break;
		if(i >= radix) break;
		ret = (ret * radix) + i;
		s++;
	}

	return negative?(-ret):ret;
}
*/

int WaitEventWithTimeout( int ( *event ) ( void ) , int cond , unsigned long ms )
{
	unsigned long i ;
	int result;

	while ( ms-- )
	{
		result = ( *event ) ();
		if ( cond )
		{
			if ( result )
				return 0;
		}
		else
		{
			if ( !result )
				return 0;
		}

		for ( i = 0; i < 5000; )
			i++ ;
	}

	return -1;
}



/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
