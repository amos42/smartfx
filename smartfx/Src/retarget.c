/******************************************************************************/
/* RETARGET.C: 'Retarget' layer for target-dependent low level functions      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include <stdio.h>
#include <rt_misc.h>
#include "usart.h"

#pragma import(__use_no_semihosting_swi)


//extern int  sendchar(int ch);  /* in serial.c */


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
FILE __stderr;

int fputc(int ch, FILE *f) {
  //return (sendchar(ch));
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
	return ch;  
}

int fgetc (FILE *fp)  {
  return (0);
}


int fclose(FILE* f) {
return 0;
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}
int fseek (FILE *fp, long nPos, int nMode)  {
  return (0);
}


int fflush (FILE *pStream)  {
  return (0);
}


void _ttywrch(int ch) {
  putchar(ch);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
