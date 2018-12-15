#include <vcl.h>
#pragma hdrstop

#include "ABITMAP.h"
#include "dither.h"


//***********************
// ������
//***********************
ABITMAP::ABITMAP()
{
    // �������� �ʱ�ȭ
    Width = 0;
    Height = 0;
    hMemDC = NULL;
    hBitmap = NULL;
    BPP = 0;
    IsSaveBitmap = FALSE;
    WidthAlign = 1;
    pBits = NULL;
    pPals = NULL;
}


//***********************
// �Ҹ���
//***********************
ABITMAP::~ABITMAP()
{
    // ���� ��Ʈ���� �����Ѵ�.
    DeleteBitmap();
}


//***********************
//  =  ������
//***********************
ABITMAP& ABITMAP::operator << ( const ABITMAP& src )
{
    SetBitmap( src.hMemDC, src.Width, src.Height,
               src.pBits, src.pPals, src.BPP );

    return( *this );
}


//********************************
// SetBitmap �޼ҵ�
// 16��Ʈ �÷��� ���, 5:6:5��带 ����.
//********************************
void ABITMAP::SetBitmap( HDC hDC, INT Width, INT Height, CHAR *lpBits, CHAR *lpPal, INT BitCount,
                                   INT HAlign )
{
    // ������ ��Ʈ���� �����Ѵ�.
    DeleteBitmap();

    BPP = BitCount;

    int ww = Width * BitCount/8;
    int tw = (ww + 0x01) & ~0x01;
    int hh = (Height >= 0)? Height:-Height;

    // ����, ���� ũ�⸦ ��Ʈ�Ѵ�.
    this->Width = Width;
    this->Height = hh;

    // DIB ����� ��Ʈ�Ѵ�.
    BITMAPINFO *bmi;
    if( BitCount <= 8 )
     bmi = (BITMAPINFO *)malloc( sizeof(BITMAPINFO) + sizeof(RGBQUAD)*(1<<BitCount) );
    else if( BitCount == 16 )
     bmi = (BITMAPINFO *)malloc( sizeof(BITMAPINFO) + sizeof(DWORD)*3 );  // for 5:6:5
    else
     bmi = (BITMAPINFO *)malloc( sizeof(BITMAPINFO) ); 
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = Width;
    bmi->bmiHeader.biHeight = Height;
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = BitCount;
    if( BitCount == 16 )
      bmi->bmiHeader.biCompression = BI_BITFIELDS; // for 5:6:5
    else
      bmi->bmiHeader.biCompression = BI_RGB;
    bmi->bmiHeader.biSizeImage = (tw * hh);
    bmi->bmiHeader.biXPelsPerMeter = 0;
    bmi->bmiHeader.biYPelsPerMeter = 0;
    bmi->bmiHeader.biClrUsed = 0;
    bmi->bmiHeader.biClrImportant = 0;

    // DIB ��� �ȷ�Ʈ�� ��Ʈ�Ѵ�.
    int cc = 0;
    if( BitCount <= 8 ){
      cc = 1 << BitCount;
      for( int i = 0; i < cc; i ++ ){
        bmi->bmiColors[i].rgbRed = lpPal[i * 3 + 0];
        bmi->bmiColors[i].rgbGreen = lpPal[i * 3 + 1];
        bmi->bmiColors[i].rgbBlue = lpPal[i * 3 + 2];
        bmi->bmiColors[i].rgbReserved = 0;
      }
    } else if( BitCount == 16 ){
      // for 5:6:5
      *(DWORD *)&bmi->bmiColors[0] = 0xF800;  // red mask
      *(DWORD *)&bmi->bmiColors[1] = 0x07E0;  // green mask
      *(DWORD *)&bmi->bmiColors[2] = 0x001F;  // blue mask
    }

    // ��Ʈ�� �迭 ����
    if( HAlign == 4 ){
      // ��Ʈ���� �����Ѵ�.
      hBitmap = CreateDIBitmap( hDC,
                  &bmi->bmiHeader, CBM_INIT, lpBits,
                  bmi, DIB_RGB_COLORS );
    } else {
      char *pp = (char *)malloc( tw * hh );
      char *tp = pp;
      for( int i = 0; i < hh; i ++ ){
        memcpy( tp, lpBits, ww );
        tp += tw;
        lpBits += ww;
      }

      // ��Ʈ���� �����Ѵ�.
      hBitmap = CreateDIBitmap( hDC,
                  &bmi->bmiHeader, CBM_INIT, pp,
                  bmi, DIB_RGB_COLORS );

      // �ӽ� ��Ʈ�� ����
      free( pp );
    }

    // DC�� �����Ѵ�.
    hMemDC = CreateCompatibleDC(hDC);

    // DC�� ��Ʈ���� �Ҵ��Ѵ�.
    SelectObject( hMemDC, hBitmap );

    if( IsSaveBitmap ){
      // ��Ʈ�� ����
      pBits = (char *)malloc( ww * hh );
      if( WidthAlign == HAlign ){
        memcpy( pBits, lpBits, ww * hh );
      }

      // �ȷ�Ʈ �迭 ����
      if( cc > 0 ){
        pPals = (char *)malloc( cc * 3 );
        memcpy( pPals, lpPal, cc * 3 );
      }
    }

    // �ӽ� ��� ���� �Ҹ�
    free( bmi );;
}


//********************************
// SetBitmap �޼ҵ�
//  ! ũ�⸦ �־��� �� ���� ����.
//    ����� ��� !
//********************************
void ABITMAP::SetBitmap( HDC hDC, INT Width, INT Height,
                                   INT SrcX, INT SrcY, INT SrcWidth, INT SrcHeight )
{
    // ���� ��Ʈ���� ������.
    DeleteBitmap();

    // ����Ʈ �Ķ���� ó��
    if( SrcWidth  == 0 ) SrcWidth  = Width;
    if( SrcHeight == 0 ) SrcHeight = Height;

    // �ȷ�Ʈ ��ġ (�߿�!)
    HPALETTE hpal = (HPALETTE)GetCurrentObject( hDC, OBJ_PAL );
//    SelectPalette( dc, hpal, FALSE );

    // �ӽ÷� ������ ��Ʈ�� ����
    HDC dc = CreateCompatibleDC( hDC );
    HBITMAP hbm = CreateCompatibleBitmap( hDC, Width, Height );
    SelectObject( dc, hbm );
    SetStretchBltMode( dc, COLORONCOLOR );
    StretchBlt( dc, 0, 0, Width, Height, hDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY );

    // �̹��� ũ��� ��� ũ�⸦ ��´�.
    unsigned int iImageSize, iInfoSize;
    GetDIBSizes( hbm, iInfoSize, iImageSize ); // Call by Reference

    // ����� ũ�⿡ ���� �޸𸮸� �Ҵ��Ѵ�.
    LPBITMAPINFO bmi = (LPBITMAPINFO)malloc( iInfoSize );
    char *pp = (char *)malloc( iImageSize );

    // �̹����� ����� ��´�.
    GetDIB( hbm, hpal, bmi, pBits );
    if( bmi->bmiHeader.biBitCount <= 8 )
      GetDIBColorTable( hDC, 0, (1 << bmi->bmiHeader.biBitCount), bmi->bmiColors );

    // �̹����� ����� �������, �̳��� ����
    DeleteDC( dc );
    DeleteObject( hbm );

    // ��Ʈ�� �����͸� ��Ʈ�Ѵ�.
//    SetDIBits( hDC, hbm,
//               0, Height, pBits, bmi, DIB_RGB_COLORS );

    // ��Ʈ���� �����Ѵ�.
    hBitmap = CreateDIBitmap( hDC,
                              &bmi->bmiHeader, CBM_INIT, pp,
                              bmi, DIB_RGB_COLORS );

    // �������� ��Ʈ
    this->Width = bmi->bmiHeader.biWidth;
    this->Height = bmi->bmiHeader.biHeight;
    this->BPP = bmi->bmiHeader.biBitCount;

    if( IsSaveBitmap ){
      int ww = Width * BPP / 8;
      int hh = (Height > 0)? Height:-Height;

      int mask = (1 << WidthAlign) - 1;
      int tw = (ww + mask) & ~mask;
      this->pBits = (char *)malloc( tw * hh );

      if( WidthAlign == 4 ){
        memcpy( pBits, pp, tw * hh );
      } else {
        mask = (1 << 4) - 1;
        int tw2 = (ww + mask) & ~mask;
        char *sp = pp;
        char *tp = pBits;
        for( int i = 0; i < hh; i ++ ){
          memcpy( tp, sp, ww );
          sp += tw;
          tp += tw2;
        }
      }

      // DIB ��� �ȷ�Ʈ�� ��Ʈ�Ѵ�.
      if( this->BPP <= 8 ){
        int cc = 1 << this->BPP;
        pPals = (char *)malloc( cc * 3 );
        for( int i = 0; i < cc; i ++ ){
          pPals[i*3 + 0] = bmi->bmiColors[i].rgbRed;
          pPals[i*3 + 1] = bmi->bmiColors[i].rgbGreen;
          pPals[i*3 + 2] = bmi->bmiColors[i].rgbBlue;
        }
      }
    }

    // �ӽ� ��Ʈ�� ����
    free( pp );

    // �ӽ� ��� �޸𸮸� �����Ѵ�.
    free( bmi );

    // DC�� �����Ѵ�.
    hMemDC = CreateCompatibleDC( hDC );

    // DC�� ��Ʈ���� �Ҵ��Ѵ�.
    SelectObject( hMemDC, hBitmap );
}


//********************************
// SetBitmapFromWnd �޼ҵ�
// ������ �ڵ�κ��� ��Ʈ�� ��Ʈ
//********************************
void ABITMAP::SetBitmapFromWnd( HWND hWnd, INT Width, INT Height,
                                            INT SrcX, INT SrcY, INT SrcWidth, INT SrcHeight )
{
    HDC hdc = GetDC( hWnd );

    // ����Ʈ �Ķ���� ó��
    if( Width == 0 || Height == 0 ){
      RECT rr;
      GetClientRect( hWnd, &rr );
      Width  = rr.right - rr.left;
      Height = rr.bottom - rr.top;      
    }
      
    SetBitmap( hdc, Width, Height, SrcX, SrcY, SrcWidth, SrcHeight );

    ReleaseDC( hWnd, hdc );
}


//********************************
// SetBitmapFromWnd �޼ҵ�
// ������ �ڵ�κ��� ��Ʈ�� ��Ʈ
//********************************
void ABITMAP::SetBitmapFromWnd( HWND hWnd, INT Width, INT Height, RECT &Rect )
{
    SetBitmapFromWnd( hWnd, Width, Height,
                            Rect.left, Rect.top,
                            Rect.right-Rect.left+1, Rect.bottom-Rect.top+1 );
}


//********************************
// Change256 �޼ҵ�
//********************************
void ABITMAP::Change256( HDC hDC )
{
    char pal[256*3];

    // 256 �ȷ�Ʈ ����
    for( int i = 0; i < 256; i ++ ){
      pal[i*3+0] = i & 0xE0;
      pal[i*3+1] = (i << 3) & 0xE0;
      pal[i*3+2] = i << 6;
    }

    char *pp = (char *)malloc( Width * Height );

    int v;
    int r, g, b;
    for( int y = 0; y < Height; y ++ ){
      for( int x = 0; x < Width; x ++ ){
        switch( BPP ){
          case 1  :
          case 2  :
          case 4  :
          case 8  : // ��~ �����Ƽ� ����.
                    break;
          case 15 :
          case 16 : v = *(WORD *)(pBits + ((x + y*Width) * BPP/8));
                    b = (v >> 10);
                    g = (v >> 5) & 0x1F;
                    r = v & 0x1F;
                    v = (b >> 3) | (g & 0x1C) | (r << 3);
                    break;
          case 24 :
          case 32 : b = (unsigned char)pBits[(x + y*Width) * BPP/8  + 0];
                    g = (unsigned char)pBits[(x + y*Width) * BPP/8  + 1];
                    r = (unsigned char)pBits[(x + y*Width) * BPP/8  + 2];
                    v = (b >> 6) | ((g >> 3) & 0x1C) | (r & 0xE0);
                    break;
        }

        pp[x + (Height-y-1)*Width] = v;
      }
    }

    SetBitmap( hDC, Width, Height, pp, pal, 8 );

    free( pp );
}

//********************************
// ChangeGray �޼ҵ�
//********************************
void ABITMAP::ChangeGray( HDC hDC )
{
    char pal[256*3];

    // �׷��� ���� �ȷ�Ʈ ����
    for( int i = 0; i < 256; i ++ ){
      pal[i*3+0] = pal[i*3+1] = pal[i*3+2] = i;
    }

    char *pp = (char *)malloc( Width * Height );

    int v;
    for( int y = 0; y < Height; y ++ ){
      for( int x = 0; x < Width; x ++ ){
        switch( BPP ){
          case 1  :
          case 2  :
          case 4  :
          case 8  : // ��~ �����Ƽ� ����.
                    break;
          case 15 :
          case 16 : v = *(WORD *)(pBits + ((x + y*Width) * BPP/8));
                    v = (v >> 10) * 30 + ((v >> 5) & 0x1F) * 59 + (v & 0x1F) * 11;
                    v = v * (256/32) / 100;
                    break;
          case 24 :
          case 32 : v = (unsigned char)pBits[(x + y*Width) * BPP/8  + 0] * 30
                      + (unsigned char)pBits[(x + y*Width) * BPP/8  + 1] * 59
                      + (unsigned char)pBits[(x + y*Width) * BPP/8  + 2] * 11;
                    v /= 100;
                    break;
        }

        pp[x + (Height-y-1)*Width] = v;
      }
    }

    SetBitmap( hDC, Width, Height, pp, pal, 8 );

    free( pp );
}


//********************************
// ChangeGrayBitmap �޼ҵ�
//********************************
void ABITMAP::ChangeGray2Bitmap( HDC hDC, INT BPP, INT Bright )
{
    if( this->BPP != 8 ) return;

    char pal[256*3];
    // �׷��� ���� �ȷ�Ʈ ����
    for( int i = 0; i < 256; i ++ ){
      pal[i*3+0] = pal[i*3+1] = pal[i*3+2] = i;
    }
    BYTE *temp = new BYTE[(Width+2) * Height + 10]; //for safe

  dither( temp, pBits, Width, Height, BPP, Bright ); //j: �̰� ��¡.. -_-;

    SetBitmap( hDC, Width, Height, (char *)temp, pal, 8 );
    
    delete [] temp;
}


//********************************
// DeleteBitmap �޼ҵ�
//********************************
void ABITMAP::DeleteBitmap(void)
{
    // ���� ��Ʈ���� �����Ǿ��ٸ�
    if( hBitmap ){
      // DC�� ����
      DeleteDC( hMemDC );
      // ��Ʈ���� ����
      DeleteObject( hBitmap );

      // ��Ʈ�� �����Ϳ� �ȷ�Ʈ ������ ����
      if( pBits ){
        free( pBits );
        pBits = NULL;
      }
      if( pPals ){
        free( pPals );
        pPals = NULL;
      }  

      // �������� �ʱ�ȭ
      hMemDC = 0;
      hBitmap = 0;
      Width = 0;
      Height = 0;
      BPP = 0;
    }
}


//***********************
// PutBitmap �޼ҵ�
//***********************
void ABITMAP::PutBitmap( HDC hDC, INT X, INT Y, INT Width, INT Height )
{
    if( Width == 0 || Height == 0 ){
      // ��Ʈ���� �����Ѵ�.
      BitBlt( hDC, X, Y, this->Width, this->Height, hMemDC, 0, 0, SRCCOPY );
    } else {
      // ��Ʈ���� �÷��� �����Ѵ�.
      SetStretchBltMode( hDC, COLORONCOLOR );
      StretchBlt( hDC, X, Y, Width, Height, hMemDC, 0, 0, this->Width, this->Height, SRCCOPY );
    }
}


//***********************
// PutBitmap �޼ҵ�
//***********************
void ABITMAP::PutBitmap( HDC hDC, SIZE &size )
{
    PutBitmap( hDC, 0, 0, size.cx, size.cy );
}


//***********************
// SaveFile �޼ҵ�
//***********************
BOOL ABITMAP::SaveToFile( LPSTR FileName )
{
/*  
    FILE *fp;
    
    fp = fopen( FileName. "wb" );
    if( fp == NULL ) return( FALSE );
    
    PutBitmap( hDC, 0, 0, size.cx, size.cy );
*/
    return( TRUE );    
}

