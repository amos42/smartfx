#ifndef _DITHER_H
#define _DITHER_H

#include <vcl.h>


enum ALGO {FloydSteinberg, Floyd2, Jarvis};

void dither(UCHAR *target , UCHAR *source, int Width, int Height, int targetBpp, int brt);


#endif