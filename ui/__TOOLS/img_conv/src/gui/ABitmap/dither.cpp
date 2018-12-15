/*   Revise information
 *
 *   2001. 4. 29 : khyeo
 *   1. temp를 배열로 동적할당 한 것을 변수만 delete함.
 *      delete [] temp로 수정.
 *   2. floyd steinberg 에서 배열의 인덱스를 넘어서는 부분에 대한 예외처리중,
 *      인덱스 <= 최대값(갯수) 를 인덱스 < 최대값(댓수)
 */


//dither
// (target *, source *, width, height, targrtBpp, algorithm)
//
#include <mem.h>
#include "dither.h"


//j:int ditherFS1(UCHAR *target , UCHAR *source, int Width, int Height, UCHAR brtPoint, UCHAR degree)
void ditherFS1(UCHAR *target , UCHAR *source, int Width, int Height, UCHAR brtPoint, UCHAR degree)
{
   const int base=16;
   int sum,buf;
   short sharpMask[3][3]={   0,  -1,  0,
                            -1,   5,  -1,
                             0,  -1,   0};
   //short smoothMask[3][3]={  1,   1,   1,     1,   1,   1,     1,   1,   1};


  short *temp = new short[Width*Height];

  degree = degree==0?1:degree;
  degree = ( degree-1 ) % 100 + 1;
  //1<degree<100


  for(int y=0; y<Height; y++)
  {
   temp[y*Width+0]=source[y*Width+0]=target[y*Width+0]=255;
  }
  for(int y=0; y<Height; y++)
   temp[y*Width+(Width-1)]=source[y*Width+(Width-1)]=target[y*Width+(Width-1)]=255;
  for(int x=0; x<Width; x++)
   temp[x]=source[x]=source[x]=255;
  for(int x=0; x<Width; x++)
   temp[(Height-1)*Width+x]=source[(Height-1)*Width+x]=target[(Height-1)*Width+x]=255;


//Sharpening
  for(int y=1; y<Height-1; y++)
    for(int x=1; x<Width-1; x++)
    {
         sum=0;
         for(int yy=-1;yy<2;yy++)
          for(int xx=-1;xx<2;xx++)
           sum +=source[(y+yy)*Width+(x+xx)]*sharpMask[xx+1][yy+1];
         if(sum < 0) temp[y*Width+x]=0;
         else if(sum>255) temp[y*Width+x]=255;
         else temp[y*Width+x]=(UCHAR)sum;
     }

/*
// transfer to target image buffer
  for(int i=0;i<Width*Height;i++)
    target[i]=(UCHAR)temp[i];

// smoothing
  for(int y=1; y<Height-1; y++)
    for(int x=1; x<Width-1; x++)
    {
         sum=0;
         for(int yy=-1;yy<2;yy++)
          for(int xx=-1;xx<2;xx++)
           sum +=target[(y+yy)*Width+(x+xx)]*(smoothMask[xx+1][yy+1]/9);
         if(sum < 0) temp[y*Width+x]=0;
         else if(sum>255) temp[y*Width+x]=255;
         else temp[y*Width+x]=(UCHAR)sum;
     }
 */

//pre processing
/*


  +     ___--
  |   --       (x1,y1)
  |  /
  | /
  |/
  +----------

      255-x1
  y = ------  (x-x1) + y1
      255-y1
 */

//  for(int i=0;i<Width*Height;i++)
//     temp[i]=source[i];

  int x1 = brtPoint;
  int y1 = x1 + 40 +(degree-50);
  if(y1>255) y1=255;
  else if(y1<0) y1=0;

//  int x1=90;
//  int y1=73;
  for(int i=0;i<Width*Height;i++)
  {
      if(temp[i] < x1)
      {
        temp[i] = y1*temp[i]/x1;
        if(temp[i]>255) temp[i]=255;
      }
      else
      {
        temp[i] = (255-y1)*(temp[i]-x1)/(255-x1) + y1;
        if(temp[i]>255) temp[i]=255;
      }
  }

  //floyd steinberg dither
  const UCHAR THRESHOLD=128;

  int error;//for storing the error to diffusion

//floyd-steinber error difuusion dither

  UCHAR *current;

  for (int y=0; y<Height; y++)
  {
    for (int x=0; x<Width; x++)
    {
      if(temp[y*Width+x]<=THRESHOLD)
      {
        error=temp[y*Width+x];
        temp[y*Width+x]=0;
      }
      else
      {
        error=temp[y*Width+x]-255;
        temp[y*Width+x]=255;
      }
      //error diffusion
      if(y*Width+x+1 < Width*Height )
      {
        temp[y*Width+x+1] += error*7/base;
      }
      if((y+1)*Width+x-1 < Width*Height && (y+1)*Width+x-1 > 0)
      {
        temp[(y+1)*Width+x-1] += error*3/base;
      }
      if((y+1)*Width+x < Width*Height )
      {
        temp[(y+1)*Width+x]+=error*5/base  ;
      }
      if((y+1)*Width+x+1 < Width*Height )
      {
        temp[(y+1)*Width+x+1]+=error*1/base  ;
      }
    }

  }

 /*
 //Shrinking
   for(int y=1; y<Height-1; y++)
    for(int x=1; x<Width-1; x++)
    {
         if(temp[y*Width+x]==0)
         {
           sum=0;
           for(int yy=-1;yy<2;yy++)
            for(int xx=-1;xx<2;xx++)
              if(temp[(y+yy)*Width+(x+xx)]==255) sum++;

           if(sum >=8) target[y*Width+x]=255;
           else  target[y*Width+x]=0;
          }
         else target[y*Width+x]=255;
    }

 //Expansion
   for(int y=1; y<Height-1; y++)
    for(int x=1; x<Width-1; x++)
    {
         if(target[y*Width+x]==255)
         {
           sum=0;
           for(int yy=-1;yy<2;yy++)
            for(int xx=-1;xx<2;xx++)
              if(target[(y+yy)*Width+(x+xx)]==0) sum++;

           if(sum >=8) temp[y*Width+x]=0;
           else  temp[y*Width+x]=255;
          }
         else temp[y*Width+x]=0;
    }
  */
// transfer to target image buffer
  for(int i=0;i<Width*Height;i++)
    target[i]=(UCHAR)temp[i];

  delete [] temp;
}


//j:int ditherFS2(UCHAR *target , UCHAR *source, int Width, int Height, UCHAR brtPoint, UCHAR degree)
void ditherFS2(UCHAR *target , UCHAR *source, int Width, int Height, UCHAR brtPoint, UCHAR degree)
{
  const int base=16;
   int sum,buf;
   short sharpMask[3][3]={  0,   -1,   0,
                           -1,    5,  -1,
                            0,   -1,   0};
                            
    //short smoothMask[3][3]={  1,   1,   1,    1,   1,   1,    1,   1,   1};


  short *temp = new short[Width*Height];

  degree = degree==0?1:degree;
  degree = ( degree-1 ) % 100 + 1;
  //1<degree<100

//Sharpening
  for(int y=0; y<Height; y++)
   temp[y*Width+0]=source[y*Width+0]=target[y*Width+0]=255;
  for(int y=0; y<Height; y++)
   temp[y*Width+(Width-1)]=source[y*Width+(Width-1)]=target[y*Width+(Width-1)]=255;
  for(int x=0; x<Width; x++)
   temp[x]=source[x]=source[x]=255;
  for(int x=0; x<Width; x++)
   temp[(Height-1)*Width+x]=source[(Height-1)*Width+x]=target[(Height-1)*Width+x]=255;

  for(int y=1; y<Height-1; y++)
    for(int x=1; x<Width-1; x++)
    {
         sum=0;
         for(int yy=-1;yy<2;yy++)
          for(int xx=-1;xx<2;xx++)
           sum +=source[(y+yy)*Width+(x+xx)]*sharpMask[xx+1][yy+1];
         if(sum < 0) temp[y*Width+x]=0;
         else if(sum>255) temp[y*Width+x]=255;
         else temp[y*Width+x]=(UCHAR)sum;
     }


/*
// transfer to target image buffer
  for(int i=0;i<Width*Height;i++)
    target[i]=(UCHAR)temp[i];

// smoothing
  for(int y=1; y<Height-1; y++)
    for(int x=1; x<Width-1; x++)
    {
         sum=0;
         for(int yy=-1;yy<2;yy++)
          for(int xx=-1;xx<2;xx++)
           sum +=target[(y+yy)*Width+(x+xx)]*(smoothMask[xx+1][yy+1]/9.);
         if(sum < 0) temp[y*Width+x]=0;
         else if(sum>255) temp[y*Width+x]=255;
         else temp[y*Width+x]=(UCHAR)sum;
     }

*/


//pre processing
//
//  +     ___--
//  |   --       (x1,y1)
//  |  /
//  | /
//  |/
//  +----------
//
//      255-x1
//  y = ------  (x-x1) + y1
//      255-y1
//

  int x1 = brtPoint;
  int y1 = x1 + 40 + (degree-50);
  if(y1>255) y1=255;
  else if(y1<0) y1=0;

//  int x1=90;
//  int y1=73;
  for(int i=0;i<Width*Height;i++)
  {
      if(temp[i] < x1)
      {
        temp[i] = y1*temp[i]/x1;
        if(temp[i]>255) temp[i]=255;
      }
      else
      {
        temp[i] = (255-y1)*(temp[i]-x1)/(255-x1) + y1;
        if(temp[i]>255) temp[i]=255;
      }
  }

//floyd steinberg dither
//
//    --- 255  :   LV3
//      -     TH2 :  210
//    --- 255*2/3  :  LV2
//      -     TH1 : 126
//    --- 255/3  :  LV1
//      -     TH0 : 42
//    --- 0  :  LV0
//
  const UCHAR TH0=42;
  const UCHAR TH1=126;
  const UCHAR TH2=210;

  const UCHAR LV0=0;
  const UCHAR LV1=255/3;
  const UCHAR LV2=255*2/3;
  const UCHAR LV3=255;

  int error;//for storing the error to diffusion

//floyd-steinber error difuusion dither

  UCHAR *current;

  int offset;
  for (int y=0; y<Height; y++)
  {
    offset=y*Width;
    for (int x=0; x<Width; x++)
    {
      if(temp[offset+x] <= TH0)
      {
        error=temp[offset+x];
        temp[offset+x]=LV0;
      }
      else if(temp[offset+x] <= TH1)
      {
        error=temp[offset+x]-LV1;
        temp[offset+x]=LV1;
      }
      else if(temp[offset+x] <= TH2)
      {
        error=temp[offset+x]-LV2;
        temp[offset+x]=LV2;
      }
      else
      {
        error=temp[offset+x]-LV3;
        temp[offset+x]=LV3;
      }
      //error diffusion
      if(offset+x+1 < Width*Height )
      {
        temp[offset+x+1] += error*7/base;
      }
      if(offset+Width+x-1 < Width*Height && offset+Width+x-1 > 0 )
      {
        temp[offset+Width+x-1] += error*3/base;
      }
      if(offset+Width+x < Width*Height )
      {
        temp[offset+Width+x]+=error*5/base  ;
      }
      if(offset+Width+x+1 < Width*Height )
      {
        temp[offset+Width+x+1]+=error*1/base  ;
      }
    }
  }

// transfer to target image buffer
  for(int i=0;i<Width*Height;i++)
    target[i]=(UCHAR)temp[i];

  delete [] temp;

}




/*
int ditherFS2(UCHAR *target , UCHAR *source, int Width, int Height, UCHAR brtPoint, UCHAR degree)
{
  const int base=16;

  short *temp = new short[Width*Height];

  degree = degree==0?1:degree;
  degree = ( degree-1 ) % 100 + 1;
  //1<degree<100

//pre processing
//
//  +     ___--
//  |   --       (x1,y1)
//  |  /
//  | /
//  |/
//  +----------
//
//      255-x1
//  y = ------  (x-x1) + y1
//      255-y1


  for(int i=0;i<Width*Height;i++)
    temp[i]=source[i];


  int x1 = brtPoint;
  int y1 = x1 + 40 + (degree-50);
  if(y1>255) y1=255;
  else if(y1<0) y1=0;

//  int x1=90;
//  int y1=73;
  for(int i=0;i<Width*Height;i++)
  {
      if(temp[i] < x1)
      {
        temp[i] = y1*temp[i]/x1;
        if(temp[i]>255) temp[i]=255;
      }
      else
      {
        temp[i] = (255-y1)*(temp[i]-x1)/(255-x1) + y1;
        if(temp[i]>255) temp[i]=255;
      }
  }

  //floyd steinberg dither
//
//    --- 255  :   LV3
//      -     TH2 :  210
//    --- 255*2/3  :  LV2
//      -     TH1 : 126
//    --- 255/3  :  LV1
//      -     TH0 : 42
//    --- 0  :  LV0

  const UCHAR TH0=42;
  const UCHAR TH1=126;
  const UCHAR TH2=210;

  const UCHAR LV0=0;
  const UCHAR LV1=255/3;
  const UCHAR LV2=255*2/3;
  const UCHAR LV3=255;

  int error;//for storing the error to diffusion

//floyd-steinber error difuusion dither

  UCHAR *current;

  int offset;
  for (int y=0; y<Height; y++)
  {
    offset=y*Width;
    for (int x=0; x<Width; x++)
    {
      if(temp[offset+x] <= TH0)
      {
        error=temp[offset+x];
        temp[offset+x]=LV0;
      }
      else if(temp[offset+x] <= TH1)
      {
        error=temp[offset+x]-LV1;
        temp[offset+x]=LV1;
      }
      else if(temp[offset+x] <= TH2)
      {
        error=temp[offset+x]-LV2;
        temp[offset+x]=LV2;
      }
      else
      {
        error=temp[offset+x]-LV3;
        temp[offset+x]=LV3;
      }
      //error diffusion
      if(offset+x+1 <= Width*Height )
      {
        temp[offset+x+1] += error*7/base;
      }
      if(offset+Width+x-1 <= Width*Height )
      {
        temp[offset+Width+x-1] += error*3/base;
      }
      if(offset+Width+x <= Width*Height )
      {
        temp[offset+Width+x]+=error*5/base  ;
      }
      if(offset+Width+x+1 <= Width*Height )
      {
        temp[offset+Width+x+1]+=error*1/base  ;
      }
    }

  }



// transfer to target image buffer
  for(int i=0;i<Width*Height;i++)
    target[i]=(UCHAR)temp[i];

  delete [] temp;

}
*/

void dither(UCHAR *target , UCHAR *source, int Width, int Height, int targetBpp, int brt)
{
    //j:int ret;

    if(targetBpp==1)// && algorithm==FloydSteinberg)
        //j:ret=ditherFS1(target, source, Width, Height, 120, brt);
        ditherFS1(target, source, Width, Height, 120, brt);
    else if(targetBpp==2)// && algorithm==FloydSteinberg)
        //j:ret=ditherFS2(target, source, Width, Height, 100, brt);
        ditherFS2(target, source, Width, Height, 100, brt);
}


