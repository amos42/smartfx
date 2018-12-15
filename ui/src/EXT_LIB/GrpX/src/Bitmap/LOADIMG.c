/*#############################################################################
-------------------------------------------------------------------------------

                                  LOADIMG.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Loading Image pool
-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"
#include <string.h>


typedef struct {
    char            Header[20];
    long            Ver;
    unsigned short  wAttr;
    char            BPP, PaletteBPP;
    short           Width, Height;
    short           LogX, LogY;
    char		 clTransColor[3];
    char            nMaskBPP;
    char            Reserved[64 - (20+4+(2+1+1)+2*2+2*2+(3+1))];
} IMG_HEADER;


/**************************************
  리소스로부터 이미지를 로드한다.
  8비트 리소스로부터 16비트 이미지 생성
***************************************/
atGRPX_BITMAP * atGRPX_CreateBitmapFromStreamEx( atLPGRPXMNG lpGrpX, const atCHAR *lpStream, atINT nBPP, atINT nPalBPP, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
    char *res;
    IMG_HEADER *ih;
    atGRPX_BITMAP *img;
	unsigned char *palette;

    if( lpStream == atNULL ) return atNULL;
    res = (char *)lpStream;

    ih = (IMG_HEADER *)res;
    res += sizeof(IMG_HEADER);
    if( strcmp( ih->Header, "Amos Image File;" ) ) return atNULL;

#ifdef __BIG_ENDIAN_
    REVERSE( ih->Ver );
    REVERSE( ih->wAttr );
    REVERSE( ih->BPP );
    REVERSE( ih->PaletteBPP );
    REVERSE( ih->Width );
    REVERSE( ih->Height );
    REVERSE( ih->LogX );
    REVERSE( ih->LogY );
#endif

    if( ih->Ver != 2 ) return atNULL;

    img = atGRPX_CreateBlankBitmapEx( lpGrpX, ih->Width, ih->Height, nBPP, nPalBPP, fnAllocator, fnDeallocator );
    if( img == atNULL ){
      return atNULL;
    }
    
	if( (ih->BPP <= 8) && (ih->PaletteBPP > 0) ){
		palette = (unsigned char*)res;
		res += (1 << ih->BPP) * ((ih->PaletteBPP == 15)? 2 : ih->PaletteBPP/8);
	}

    if( ih->BPP == nBPP ){
      memcpy( img->lpBits, res, img->nWidth * img->nHeight * (ih->BPP/8) );
      res += img->nWidth * img->nHeight * (ih->BPP/8);
    } else {
      if( (nBPP == 16) && (ih->BPP == 8) ){
        int i, j, w, h;
        unsigned char *pal;
        atGRPX_PIXEL vv;
        atGRPX_PIXEL *p;
        w = ih->Width;
        h = ih->Height;
        p = img->lpBits;
        
        for( i = 0; i < h; i ++ ){
          for( j = 0; j < w; j ++ ){
			if(ih->PaletteBPP == 24){
				pal = &palette[*res++ * 3];
				vv = MAKE565( pal[0], pal[1], pal[2] );
			} else {
				vv = palette[ *res++ * 2];
			}	
			*p++ = vv;
          }
        }  
      }
    }

    atGRPX_SetBitmapLogPos( img, ih->LogX, ih->LogY );
	
    if( ih->wAttr & 0x8000 ){
		atGRPX_COLOR cl = atGRPX_GetRGB( ih->clTransColor[1], ih->clTransColor[2], ih->clTransColor[3]);
        atGRPX_SetBitmapTransColor( img, cl );
	}
	
    if( ih->wAttr & 0x4000 ){
	 atLPBYTE p;
	if( fnAllocator )
	    p = (atLPBYTE)fnAllocator( img->nWidth * img->nHeight );
	else
	    p = (atLPBYTE)atGRPX_AllocMem( lpGrpX, img->nWidth * img->nHeight );
	if( p != atNULL ){
         memcpy( p, res, img->nWidth * img->nHeight );
    	  atGRPX_SetBitmapAlphaChannel( img, p, atTRUE );
	}
    }

    return( img );
}


atGRPX_BITMAP * atGRPX_CreateBitmapFromStream( atLPGRPXMNG lpGrpX, const atCHAR *lpStream, atINT nBPP, atINT nPalBPP )
{
	return atGRPX_CreateBitmapFromStreamEx( lpGrpX, lpStream, nBPP, nPalBPP, atNULL, atNULL );
}

