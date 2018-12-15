#ifndef __ABITMAP_H_
#define __ABITMAP_H_


class ABITMAP {
public:
    ABITMAP();
    ~ABITMAP();

public:
    INT     Width, Height;
    HDC     hMemDC;
    HBITMAP hBitmap;
    INT     BPP;
    BOOL    IsSaveBitmap;
    INT     WidthAlign;
    CHAR   *pBits;
    CHAR   *pPals;

public:
    ABITMAP& operator << ( const ABITMAP& src );

public:
    void SetBitmap( HDC hDC, INT Width, INT Height, CHAR *lpBits, CHAR *lpPal, INT BitCount,
                             INT HAlign = 1 );
    void SetBitmap( HDC hDC, INT Width=0, INT Height=0,
                             INT SrcX=0, INT SrcY=0, INT SrcWidth=0, INT SrcHeight=0 );
    void SetBitmapFromWnd( HWND hWnd, INT Width = 0, INT Height = 0,
                                      INT SrcX = 0, INT SrcY = 0, INT SrcWidth = 0, INT SrcHeight = 0 );
    void SetBitmapFromWnd( HWND hWnd, INT Width, INT Height, RECT &Rect );
    
    void ChangeGray( HDC hDC );
    void Change256( HDC hDC );
    void ChangeGray2Bitmap( HDC hDC, INT BPP, INT Bright = 60 );
    void DeleteBitmap();
    void PutBitmap( HDC hDC, INT X = 0, INT Y = 0, INT Width = 0, INT Height = 0 );
    void PutBitmap( HDC hDC, SIZE &size );
    BOOL SaveToFile( LPSTR FileName );
};


#endif

