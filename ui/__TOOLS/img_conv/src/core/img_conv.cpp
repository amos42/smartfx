#include <stdio.h>
#include <string.h>
#include <windows.h>


#define AMOS_IMAGE_FILE_VER 2


/*
  ret code :
   0  : success
   -1 : Invalide parameter
   -2 : not supported version or format
   -3 : source file open error
   -4 : source file is no BMP format
   -5 : not support source BPP
   -6 : not support destination BPP
   -7 : target file open error
*/
   

#define MAKE555(r,g,b) ((WORD)(((((WORD)((r) & 0xF8)) << (7))) | \
                       ((((WORD)((g) & 0xF8)) << (2))) | \
                       (((WORD)((b) & 0xF8)) >> (3))))
#define MAKE565(r,g,b) ((WORD)(((((WORD)((r) & 0xF8)) << (5+6-3))) | \
                       ((((WORD)((g) & 0xFC)) << (5-2))) | \
                       (((WORD)((b) & 0xF8)) >> 3)))



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

typedef struct {
    char    Header[20];
    long    Ver;
    long    BPP;
    short   Width, Height;
    char    Reserved[64-(20+4+4+2*2)];
} MSK_HEADER;



static int conv_img_8bpp_to_8bpp( char *src, char *des, unsigned char *pal, unsigned char *pal2, int bpp, int des_bpp, int pal_bpp, int *sp_d, int *dp_d )
{
    unsigned char v, v2;
    unsigned char r, g, b;
    
    *des = *src;    

    *sp_d = 8/bpp;
    *dp_d = 8/bpp;
    
    if( pal_bpp == 24 ){
        memcpy( pal2, pal, 256*3 );
    } else if( pal_bpp == 15 ){
        for( int i = 0; i < 256; i++ ){
            *((short *)pal2) = MAKE555( pal[i*3+0], pal[i*3+1], pal[i*3+2] ); 
			pal2 += sizeof(short);
        }        
    } else if( pal_bpp == 16 ){
        for( int i = 0; i < 256; i++ ){
            *((short *)pal2) = MAKE565( pal[i*3+0], pal[i*3+1], pal[i*3+2] ); 
			pal2 += sizeof(short);
        }     
    }   
                      
    return 0;
}

static int conv_img_8bpp_to_16bpp( char *src, char *des, unsigned char *pal, int s_bpp, int des_bpp, int *sp_d, int *dp_d )
{
    unsigned char v, v2;
    unsigned char r, g, b;
    
    v = *src;    
    for( int i = 0; i < 8/s_bpp; i ++ ){
      v2 = (v >> ((8/s_bpp)-1-i)*s_bpp) & ((1<<s_bpp)-1);
      r = pal[v2*3+0]; g = pal[v2*3+1]; b = pal[v2*3+2];
      *((unsigned short *)des) = (des_bpp==15)? MAKE555(r,g,b) : MAKE565(r,g,b);
	  des += sizeof(unsigned short);
    }

    *sp_d = 8/s_bpp;
    *dp_d = 8/s_bpp;
                      
    return 0;
}

static int conv_img_8bpp_to_24bpp( char *src, char *des, unsigned char *pal, int s_bpp, int des_bpp, int *sp_d, int *dp_d )
{
    unsigned char v, v2;
    unsigned char r, g, b;
    
    v = *src;    
    for( int i = 0; i < 8/s_bpp; i ++ ){
      v2 = (v >> ((8/s_bpp)-1-i)*s_bpp) & ((1<<s_bpp)-1);
      r = pal[v2*3+0]; g = pal[v2*3+1]; b = pal[v2*3+2];
      *des++ = r;
      *des++ = g;
      *des++ = b;
    }

    *sp_d = 8/s_bpp;
    *dp_d = 8/s_bpp;
                      
    return 0;
}

static int conv_img_8bpp_to_32bpp( char *src, char *des, unsigned char *pal, int s_bpp, int des_bpp, int *sp_d, int *dp_d )
{
    unsigned char v, v2;
    unsigned char r, g, b;
    
    v = *src;    
    for( int i = 0; i < 8/s_bpp; i ++ ){
      v2 = (v >> ((8/s_bpp)-1-i)*s_bpp) & ((1<<s_bpp)-1);
      r = pal[v2*3+0]; g = pal[v2*3+1]; b = pal[v2*3+2];
      *des++ = r;
      *des++ = g;
      *des++ = b;
      *des++ = 0xff;
    }

    *sp_d = 8/s_bpp;
    *dp_d = 8/s_bpp;
                      
    return 0;
}

static int conv_img_24bpp_to_8bpp( char *src, char *des, int des_bpp, int *sp_d, int *dp_d )
{
    
    return 0;
}

static int conv_img_24bpp_to_16bpp( char *src, char *des, int des_bpp, int *sp_d, int *dp_d )
{
    char r, g, b;
    
    b = *src++;
    g = *src++;
    r = *src;
    
    *(unsigned short *)des = (des_bpp==15)? MAKE555(r,g,b) : MAKE565(r,g,b);

    *sp_d = 1;
    *dp_d = 1;
                      
    return 0;
}

static int conv_img_24bpp_to_24bpp( char *src, char *des, int des_bpp, int *sp_d, int *dp_d )
{
    char r, g, b;
    
    b = *src++;
    g = *src++;
    r = *src;

    *des++ = r;
    *des++ = g;
    *des = b;
                      
    *sp_d = 1;
    *dp_d = 1;
                      
    return 0;
}

static int conv_img_24bpp_to_32bpp( char *src, char *des, int des_bpp, int *sp_d, int *dp_d )
{
    char r, g, b;
    
    b = *src++;
    g = *src++;
    r = *src;

    *des++ = r;
    *des++ = g;
    *des++ = b;
    *des = 0xff;
                      
    *sp_d = 1;
    *dp_d = 1;
                      
    return 0;
}

static int conv_img_32bpp_to_8bpp( char *src, char *des, int des_bpp, int *sp_d, int *dp_d )
{
    return 0;
}

static int conv_img_32bpp_to_16bpp( char *src, char *des, int des_bpp, int *sp_d, int *dp_d )
{
    char r, g, b;
    
    b = *src++;
    g = *src++;
    r = *src;
    
    *(short *)des = (des_bpp==15)? MAKE555(r,g,b) : MAKE565(r,g,b);

    *sp_d = 1;
    *dp_d = 1;
                      
    return 0;
}

static int conv_img_32bpp_to_24bpp( char *src, char *des, int des_bpp, int *sp_d, int *dp_d )
{
    char a, r, g, b;
    
    b = *src++;
    g = *src++;
    r = *src;
    
    *des++ = r;
    *des++ = g;
    *des = b;
                      
    *sp_d = 1;
    *dp_d = 1;
                      
    return 0;
}


static int conv_img_32bpp_to_32bpp( char *src, char *des, int des_bpp, int *sp_d, int *dp_d )
{
    char a, r, g, b;
    
    b = *src++;
    g = *src++;
    r = *src++;
    a = *src;
    
    *des++ = r;
    *des++ = g;
    *des++ = b;
    *des = a;
                      
    *sp_d = 1;
    *dp_d = 1;
                      
    return 0;
}


int conv_img_file( char *src_filename, char *img_filename, char *msk_filename, int des_bpp, int des_fmt_type )
{
    int ret = 0;
    
    FILE *fp = fopen( src_filename, "rb" );
    if( fp == NULL ){
      return -3;
    }

    BITMAPFILEHEADER bfh;
    fread( &bfh, sizeof(BITMAPFILEHEADER), 1, fp );
    if( bfh.bfType != MAKEWORD('B','M') ){
      fclose( fp );
      return -4;
    }

    BITMAPINFOHEADER bih;
    fread( &bih, sizeof(BITMAPINFOHEADER), 1, fp );

    int s_bpp = bih.biBitCount;
    int pal_bpp;
    if( des_bpp == 6 ){
      des_bpp = 8;
      pal_bpp = 15;
    } else if( des_bpp == 7 ){
      des_bpp = 8;
      pal_bpp = 16;
    } else if( des_bpp == 8 ){
      des_bpp = 8;
      pal_bpp = 24;    
    } else {
      pal_bpp = 0;
    }
      
    RGBQUAD rgbquad[256];
    BYTE pal[256*3];
    BYTE pal2[256*3];
    if( s_bpp <= 8 ){
      int cnt = (bih.biClrUsed > 0)? bih.biClrUsed : (1 << bih.biBitCount);
      fread( rgbquad, cnt*sizeof(RGBQUAD), 1, fp );
      for( int i = 0; i < cnt; i ++ ){
        pal[i*3+0] = rgbquad[i].rgbRed;
        pal[i*3+1] = rgbquad[i].rgbGreen;
        pal[i*3+2] = rgbquad[i].rgbBlue;
      }
    }

    int twidth = bih.biWidth * bih.biBitCount/8;
    twidth = (twidth + 3) & ~0x3;
    int theight = ( bih.biHeight >= 0 )? bih.biHeight : -bih.biHeight;
    bih.biSizeImage = twidth * theight;

    char *bits = new char[ bih.biSizeImage ];
    fread( bits, bih.biSizeImage, 1, fp );

    fclose( fp );

    BYTE *tgt = new BYTE[ bih.biWidth * bih.biHeight * ((des_bpp==15)?2:des_bpp/8) ];
    char *t = (char *)tgt;
    BYTE *tgt2 = new BYTE[ bih.biWidth * bih.biHeight ];
    char *t2 = (char *)tgt2;
    BYTE v, v2, r, g, b;
    int sp_i, dp_i;

    char *sp = (char *)bits + (twidth * (theight-1));

    for( int y = 0; y < theight; y ++ ){
      for( int x = 0; x < bih.biWidth; x ++ ){
        switch( s_bpp ){
//          case 1  :
//          case 2  :
//          case 4  :
          case 8  : switch( des_bpp ){
                      case 8: conv_img_8bpp_to_8bpp( sp, t, pal, pal2, s_bpp, des_bpp, pal_bpp, &sp_i, &dp_i );
                              break;
                      case 15: 
                      case 16: conv_img_8bpp_to_16bpp( sp, t, pal, s_bpp, des_bpp, &sp_i, &dp_i );
                              break;
                      case 24: conv_img_8bpp_to_24bpp( sp, t, pal, s_bpp, des_bpp, &sp_i, &dp_i );
                              break;
                      case 32: conv_img_8bpp_to_32bpp( sp, t, pal, s_bpp, des_bpp, &sp_i, &dp_i );
                              break;
                      default: ret = -6;
                              goto exitloop;
//                              break;
                    }
                    break;
          case 16 : //ShowMessage( "16비트 컬러임. 제작자에게 문의 바람." );
                    ret = -4;
                    goto exitloop;
//                    break;
          case 24 : switch( des_bpp ){
                      case 15: 
                      case 16: conv_img_24bpp_to_16bpp( sp, t, des_bpp, &sp_i, &dp_i );
                              break;
                      case 24: conv_img_24bpp_to_24bpp( sp, t, des_bpp, &sp_i, &dp_i );
                              break;
                      case 32: conv_img_24bpp_to_32bpp( sp, t, des_bpp, &sp_i, &dp_i );
                              break;
                      default: ret = -6;
                              goto exitloop;
//                              break;
                    }
                    break;
          case 32 : switch( des_bpp ){
                      case 15: 
                      case 16: conv_img_32bpp_to_16bpp( sp, t, des_bpp, &sp_i, &dp_i );
                              break;
                      case 24: conv_img_32bpp_to_24bpp( sp, t, des_bpp, &sp_i, &dp_i );
                              break;
                      case 32: conv_img_32bpp_to_32bpp( sp, t, des_bpp, &sp_i, &dp_i );
                              break;
                      default: ret = -6;
                              goto exitloop;
//                              break;
                    }
                    *t2++ = sp[3];
                    break;
          default : //ShowMessage( "지원 하지 않는 포맷 : " + IntToStr(bpp) + " bit color" );
                    ret = -5;
                    goto exitloop;
//                    break;
        }
        
        t += dp_i * des_bpp / 8;
        sp += sp_i * s_bpp / 8;
        x += dp_i - 1;
      }
      
      sp -= twidth + bih.biWidth * s_bpp / 8;
    }

exitloop:

    delete [] bits;
    
    if( ret == 0 ){
        if( des_fmt_type == 0 ){
          fp = fopen( img_filename, "wt" );
          if( fp != NULL ){
              fprintf( fp, "/* Image Ver = %d, BPP = %d, PalBPP = %d */\n", AMOS_IMAGE_FILE_VER, des_bpp, pal_bpp );
              fprintf( fp, "unsigned char pBitmap[ %d*%d + %d*%d*%d ] = {\n", (des_bpp == 8)? 256 : 0, (pal_bpp==15 || pal_bpp==16)?2:((pal_bpp==24)?3:0), bih.biWidth, bih.biHeight, (des_bpp==15)?2:des_bpp/8 );
              if( des_bpp == 8 ){
                for( int i = 0; i < 256; i++ ){
                  if( pal_bpp == 24 )
                    fprintf( fp, "    0x%02X, 0x%02X, 0x%02X,\n", pal[i*3+0], pal[i*3+1], pal[i*3+2] );
                  else if( pal_bpp == 15 || pal_bpp == 16 )
                    fprintf( fp, "    0x%02X, 0x%02X,\n", pal[i*2+0], pal[i*2+1] );
                }
              }
              fprintf( fp, "    " );
              for( int i = 0; i < theight; i ++ ){
                for( int j = 0; j < bih.biWidth * ((des_bpp==15)?2:des_bpp/8); j ++ ){
                  fprintf( fp, "0x%02X", tgt[i*bih.biWidth + j] );
                  if( (i < theight-1) || (j < (bih.biWidth * ((des_bpp==15)?2:des_bpp/8))-1) ) fprintf(fp, ", ");
                }
                if( i < theight-1 ) fprintf(fp, "\n    ");
              }
              fprintf( fp, "\n};\n" );
              fclose( fp );
          } else {
              ret = -7;
          }
        } else if( des_fmt_type == 1 ){
          fp = fopen( img_filename, "wb" );
          if( fp != NULL ){
              if( des_bpp == 8 ){
                  if( pal_bpp == 24 )
                    fwrite(pal, 256*3, 1, fp);
                  else if( pal_bpp == 15 || pal_bpp == 16 )
                    fwrite(pal2, 256*2, 1, fp);
              }
              fwrite(tgt, bih.biWidth*theight*((des_bpp==15)?2:des_bpp/8), 1, fp);
              fclose( fp );
          } else {
              ret = -7;
          }
        } else if( (des_fmt_type == 2) || (des_fmt_type == 3) ){
          fp = fopen( img_filename, "wb" );
          if( fp != NULL ){
              IMG_HEADER hdr;
              memset( &hdr, 0, sizeof(IMG_HEADER) );
              strcpy( hdr.Header, "Amos Image File;" );
              hdr.Ver        = AMOS_IMAGE_FILE_VER;
		if( des_fmt_type == 3 ){
              	hdr.wAttr      = (s_bpp == 32)? 0x4000  : 0x0000;
			hdr.nMaskBPP = 8;
		} else {
              	hdr.wAttr      = 0x0000;
			hdr.nMaskBPP = 0;
		}
              hdr.BPP        = des_bpp;
              hdr.PaletteBPP = pal_bpp;
              hdr.Width = bih.biWidth;
              hdr.Height = theight;
              hdr.LogX = hdr.LogY = 0;
              fwrite( &hdr, sizeof(IMG_HEADER), 1, fp );
              if( des_bpp == 8 ){
                  if( pal_bpp == 24 )
                    fwrite(pal, 256*3, 1, fp);
                  else if( pal_bpp == 15 || pal_bpp == 16 )
                    fwrite(pal2, 256*2, 1, fp);
              }
              fwrite(tgt, bih.biWidth*theight*((des_bpp==15)?2:des_bpp/8), 1, fp);
              if( (des_fmt_type == 3) && (s_bpp == 32) ){
                fwrite(tgt2, bih.biWidth*theight, 1, fp);
              }
              fclose( fp );			  
          } else {
              ret = -7;
          }
        }
        
        if( ret == 0 ){
            if( s_bpp == 32 ){
              if( des_fmt_type == 0 ){
                fp = fopen( msk_filename, "wt" );
                fprintf( fp, "unsigned char pBitmapMask[ %d * %d ] = {\n", bih.biWidth, bih.biHeight );
                fprintf( fp, "    " );
                for( int i = 0; i < theight; i ++ ){
                  for( int j = 0; j < bih.biWidth; j ++ ){
                    fprintf( fp, "0x%02X", tgt2[i*bih.biWidth + j] );
                    if( (i < theight-1) || (j < bih.biWidth-1) ) fprintf(fp, ", ");
                  }
                  if( i < theight-1 ) fprintf(fp, "\n    ");
                }
                fprintf( fp, "\n};\n" );
                fclose( fp );
              } else if( des_fmt_type == 1 ){
                fp = fopen( msk_filename, "wb" );
                fwrite(tgt2, bih.biWidth*theight, 1, fp);
                fclose( fp );
              } else if( des_fmt_type == 2 ){
                MSK_HEADER hdr2;
                memset( &hdr2, 0, sizeof(IMG_HEADER) );
                strcpy( hdr2.Header, "Amos Image Mask;" );
                hdr2.Ver = 1;
                hdr2.BPP = 8;
                hdr2.Width = bih.biWidth;
                hdr2.Height = theight;
                fp = fopen( msk_filename, "wb" );
                fwrite( &hdr2, sizeof(MSK_HEADER), 1, fp );
                fwrite(tgt2, bih.biWidth*theight, 1, fp);
                fclose( fp );
              } else if( des_fmt_type == 3 ){
                // nothing
              }
            }
        }
    }    
    
    delete [] tgt2;      
    delete [] tgt;
    
    return ret;
}



int main( int argc, char *argv[] )
{
    int img_version;
    char *src_filename, *img_filename, *msk_filename;
    int len;
    char img_filename_arr[256], msk_filename_arr[256];
    int des_bpp;
    int des_fmt_type;

    if( argc < 1+4 ){
        printf( "[err] Invalide parameter\n" );
        printf( "Usage : img_conv <version> <src_filename> <img_filename> <msk_filename> [des_bpp=16] [des_fmt_type=2]\n" );
        printf( "> version :\n"
                " 1 - not support\n"
                " 2 - default\n" );
        printf( "> img_filename, msk_filename :\n"
                " * - same img_filename with default file ext\n" );
        printf( "> des_bpp :\n"
                " 6 - 8bit pixel, 15bit Palette\n"
                " 7 - 8bit pixel, 16bit Palette\n"
                " 8 - 8bit pixel, 24bit Palette\n"
                " 15 - 15bit pixel - 5:5:5\n"
                " 16 - 16bit pixel - 5:6:5\n"
                " 24 - 24bit pixel\n"
                " 32 - 32bit pixel (with alpha)\n" );
        printf( "> des_fmt_type :\n"
                " 0 - Text\n"
                " 1 - Binary\n"
                " 2 - Binary (Header)\n"
                " 3 - Binary (Header, AlphaChannel)\n" );
        return -1;
    }

    img_version = atoi(argv[1]);
    if( img_version != AMOS_IMAGE_FILE_VER ){
        printf( "[err] only support version <2>\n" );
        return -2;
    }

    src_filename = argv[2];
    img_filename = argv[3];
    msk_filename = argv[4];
    if( argc < 1+4+1 ){
        des_bpp = 16;
    } else {
        des_bpp = atoi( argv[5] );
    }
    if( argc < 1+4+2 ){
        des_fmt_type = 2;
    } else {
        des_fmt_type = atoi( argv[6] );
    }

    if( (des_fmt_type < 0) || (des_fmt_type > 3) ){
        printf( "[err] not support file format\n" );
        return -2;
    }
	
    len = strlen(src_filename);
    if( len < 5 ) return -1;
        
    if( strcmp( img_filename, "*" ) == 0 ){
        strcpy( img_filename_arr, src_filename );
        if( des_fmt_type == 0 )
          strcpy( &img_filename_arr[len-3], "img.h" );
        else if( des_fmt_type == 1 )
          strcpy( &img_filename_arr[len-3], "img.bin" );
        else if( (des_fmt_type == 2) || (des_fmt_type == 3) )
          strcpy( &img_filename_arr[len-3], "img" );
        else 
          strcpy( img_filename_arr, "" );
        img_filename = img_filename_arr;
    }
    if( strcmp( msk_filename, "*" ) == 0 ){
        strcpy( msk_filename_arr, src_filename );
        if( des_fmt_type == 0 )
          strcpy( &msk_filename_arr[len-3], "msk.h" );
        else if( des_fmt_type == 1 )
          strcpy( &msk_filename_arr[len-3], "msk.bin" );
        else if( des_fmt_type == 2 )
          strcpy( &msk_filename_arr[len-3], "msk" );
        else 
          strcpy( msk_filename_arr, "" );
        msk_filename = msk_filename_arr;
    }
            
    printf( "[msg] converting...\n" );
    printf( " >img_file_version : %d\n", img_version );
    printf( " >src_filename : %s\n", src_filename );
    printf( " >img_filename : %s\n", img_filename );
    printf( " >msk_filename : %s\n", msk_filename );
    printf( " >des_bpp : %d\n", des_bpp );
    printf( " >des_fmt_type : %d\n", des_fmt_type );
    
    int r = conv_img_file( src_filename, img_filename, msk_filename, des_bpp, des_fmt_type );
        
    if( r == 0 ){
        printf( "[suc] Success\n" );
    } else {
        printf( "[err] Fail (err=%d)\n", r );
    }
    
    return r;
}

