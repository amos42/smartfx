//============================================================================
//
// Header  : Symbian2Win32.h
// Function: Basic Type Definition for Symbian
//

#ifndef _SYMBIAN_2_WIN32_H_
#define _SYMBIAN_2_WIN32_H_

#include "PlatformDefine.h"

#if _IS_SYMBIAN_

#include <e32std.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>

///////////////////////////////////////////////////////////////
// Base Type Definition
///////////////////////////////////////////////////////////////
typedef TBool	BOOL;	
typedef TUint	UINT;
typedef TUint64	ULONGLONG;	
typedef TUint32	DWORD, ULONG;
typedef TUint16	WORD, USHORT;
typedef TUint8	BYTE, UCHAR;
typedef TInt	INT;	
typedef TInt64  LONGLONG;
typedef TInt32	LONG;	
typedef TInt16	SHORT;	
typedef TInt8	CHAR;	
typedef TReal32	FLOAT;	
typedef TReal64	DOUBLE;	
typedef TAny	VOID;

typedef FLOAT	*PFLOAT;
typedef BOOL	*PBOOL;
typedef BOOL	*LPBOOL;
typedef BYTE	*PBYTE;
typedef BYTE	*LPBYTE;
typedef int		*PINT;
typedef int		*LPINT;
typedef WORD	*PWORD;
typedef WORD	*LPWORD;
typedef long	*LPLONG;
typedef DWORD	*PDWORD;
typedef DWORD	*LPDWORD;

typedef ULONG	*PULONG;
typedef USHORT	*PUSHORT;
typedef UCHAR	*PUCHAR;

typedef TAny		*LPVOID;
typedef const TAny	*LPCVOID;

typedef char		*LPSTR, *PSTR;
typedef const char	*LPCSTR, *PCSTR;

typedef wchar_t		WCHAR;
typedef WCHAR		*LPWSTR, *PWSTR;
typedef const WCHAR	*LPCWSTR, *PCWSTR;

 
typedef DWORD		COLORREF;
typedef DWORD		*LPCOLORREF;

#define RGB(r,g,b)  ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define GetRValue(rgb)      ((BYTE)(rgb))
#define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      ((BYTE)((rgb)>>16))

typedef struct tagRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    };
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER;

typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *PRECT;

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT;

typedef struct tagSIZE
{
    LONG        cx;
    LONG        cy;
} SIZE, *PSIZE;

#define MAX_PATH	256
#define _MAX_PATH	256

#ifndef NULL
#define NULL    0
#endif

#ifndef FALSE
#define FALSE	EFalse
#endif

#ifndef TRUE
#define TRUE	ETrue
#endif

#ifndef CONST
#define CONST	const
#endif

#define CALLBACK    __stdcall
#define __cdecl		


#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


#define MINCHAR     0x80        
#define MAXCHAR     0x7f        
#define MINSHORT    0x8000      
#define MAXSHORT    0x7fff      
#define MINLONG     0x80000000  
#define MAXLONG     0x7fffffff  
#define MAXBYTE     0xff        
#define MAXWORD     0xffff      
#define MAXDWORD    0xffffffff  


#define INFINITE	0xFFFFFFFF  // Infinite timeout


///////////////////////////////////////////////////////////////
// TCHAR Definition
///////////////////////////////////////////////////////////////

#define _stricmp	strcasecmp
#define _strnicmp	strncasecmp
#define _snprintf	snprintf
#define _vsnprintf	vsnprintf

#define _wcsnicmp	wcsnicmp
#define _snwprintf	snwprintf
#define _vsnwprintf	vsnwprintf

#ifdef _UNICODE

typedef wchar_t			TCHAR, *LPTSTR;
typedef const wchar_t	*LPCTSTR;		

#define _T(x)		L##x
#define TEXT(x)		L##x

#define _tcscat     wcscat
#define _tcschr     wcschr
#define _tcscpy     wcscpy
#define _tcscspn    wcscspn
#define _tcslen     wcslen
#define _tcsncat    wcsncat
#define _tcsncpy    wcsncpy
#define _tcspbrk    wcspbrk
#define _tcsrchr    wcsrchr
#define _tcsspn     wcsspn
#define _tcsstr     wcsstr
#define _tcstok		_wcstok//wcstok			// 지원 X => Symbian용 직접 구현하여 사용 가능

#define _tcscmp     wcscmp
#define _tcsicmp    _wcsicmp
#define _tcsnccmp   wcsncmp
#define _tcsncmp    wcsncmp
#define _tcsncicmp  _wcsnicmp
#define _tcsnicmp   _wcsnicmp

#define _tcslwr     _wcslwr
#define _tcsupr     _wcsupr

#define _tcstod     wcstod
#define _tcstol     wcstol
#define _tcstoul    wcstoul

#define _itot       UNDEFINED//_itow		// 지원 X (_sntprintf 사용)
#define _ltot       UNDEFINED//_ltow		// 지원 X (_sntprintf 사용)
#define _ultot      UNDEFINED//_ultow		// 지원 X (_sntprintf 사용)
#define _ttoi       _wtoi					// 지원 X => Symbian용 직접 구현하여 사용 가능		
#define _ttol       _wtol					// 지원 X => Symbian용 직접 구현하여 사용 가능

#define _istalnum   iswalnum
#define _istalpha   iswalpha
#define _istascii   UNDEFINED				// 지원 X
#define _istcntrl   iswcntrl
#define _istdigit   iswdigit
#define _istgraph   iswgraph
#define _istlower   iswlower
#define _istprint   iswprint
#define _istpunct   iswpunct
#define _istspace   iswspace
#define _istupper   iswupper
#define _istxdigit  iswxdigit

#define _totupper   towupper
#define _totlower   towlower

#define _tprintf    wprintf
#define _ftprintf   fwprintf
#define _stprintf   UNDEFINED//swprintf		// 지원 X (_sntprintf 사용) => Max Count 포함
#define _sntprintf  _snwprintf
#define _vtprintf   vwprintf
#define _vftprintf  vfwprintf
#define _vstprintf  UNDEFINED//vswprintf	// 지원 X (_vsntprintf 사용) => Max Count 포함
#define _vsntprintf _vsnwprintf
#define _tscanf     wscanf
#define _ftscanf    fwscanf
#define _stscanf    swscanf

#else //_UNICODE

typedef char		TCHAR, *LPTSTR;
typedef const char	*LPCTSTR;		

#define _T(x)		x
#define TEXT(x)		x

#define _tcscat     strcat
#define _tcschr     strchr
#define _tcscpy     strcpy
#define _tcscspn    strcspn
#define _tcslen     strlen
#define _tcsncat    strncat
#define _tcsncpy    strncpy
#define _tcspbrk    strpbrk
#define _tcsrchr    strrchr
#define _tcsspn     strspn
#define _tcsstr     strstr
#define _tcstok     strtok

#define _tcscmp     strcmp
#define _tcsicmp    _stricmp
#define _tcsnccmp   strncmp
#define _tcsncmp    strncmp
#define _tcsncicmp  _strnicmp
#define _tcsnicmp   _strnicmp

#define _tcslwr     _strlwr					// 지원 X => Symbian용 직접 구현하여 사용 가능
#define _tcsupr     _strupr					// 지원 X => Symbian용 직접 구현하여 사용 가능

#define _tcstod     strtod
#define _tcstol     strtol
#define _tcstoul    strtoul

#define _itot       UNDEFINED//_itoa		// 지원 X (sprintf 사용)
#define _ltot       UNDEFINED//_ltoa		// 지원 X (sprintf 사용)
#define _ultot      UNDEFINED//_ultoa		// 지원 X (sprintf 사용)
#define _ttoi       atoi
#define _ttol       atol

#define _istalnum   isalnum
#define _istalpha   isalpha
#define _istascii   isascii
#define _istcntrl   iscntrl
#define _istdigit   isdigit
#define _istgraph   isgraph
#define _istlower   islower
#define _istprint   isprint
#define _istpunct   ispunct
#define _istspace   isspace
#define _istupper   isupper
#define _istxdigit  isxdigit

#define _totupper   toupper
#define _totlower   tolower

#define _tprintf    printf
#define _ftprintf   fprintf
#define _stprintf   sprintf
#define _sntprintf  _snprintf
#define _vtprintf   vprintf
#define _vftprintf  vfprintf
#define _vstprintf  vsprintf
#define _vsntprintf _vsnprintf
#define _tscanf     scanf
#define _ftscanf    fscanf
#define _stscanf    sscanf

#endif //_UNICODE


///////////////////////////////////////////////////////////////
// String Function Implementation For Symbian
///////////////////////////////////////////////////////////////

inline wchar_t* _wcstok(wchar_t * aSrc, const wchar_t * aDel)
{
	static wchar_t* tRet;
	return wcstok(aSrc, aDel, &tRet );
//	return NULL; //some strange thing happens....for stl..?
}

inline TInt _wtoi(const wchar_t* aData)
{
	TPtrC testPtr((TText16*)aData);
	
	TLex lex(testPtr);
	
	TInt dataValue = 0;
	lex.Val(dataValue);	
	
	return dataValue;
}

inline TInt32 _wtol(const wchar_t* aData)
{
	TPtrC testPtr((TText16*)aData);
	
	TLex lex(testPtr);
	
	TInt32 dataValue = 0;
	lex.Val(dataValue);	
	
	return dataValue;
}

inline char* _strupr(char* pSrc)
{
	char* pTmp= pSrc;
	while (*pTmp)
		*pTmp++= toupper(*pTmp);
	return pSrc;
}

inline char* _strlwr(char* pSrc)
{
	char* pTmp= pSrc;
	while (*pTmp)
		*pTmp++= tolower(*pTmp);
	return pSrc;
}


///////////////////////////////////////////////////////////////
// Temporary Start
///////////////////////////////////////////////////////////////
typedef void*   HANDLE;
#define INVALID_HANDLE_VALUE	MAXDWORD

//#define _wfopen     wfopen
#ifdef _UNICODE
#define _tfopen     wfopen
#else //_UNICODE
#define _tfopen     fopen
#endif //_UNICODE

#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2

/* wave data block header */
typedef struct wavehdr_tag {
    BYTE*       lpData;                 /* pointer to locked data buffer */
    DWORD       dwBufferLength;         /* length of data buffer */
    DWORD       dwBytesRecorded;        /* used for input only */
    DWORD*   	dwUser;                 /* for client's use */
    DWORD       dwFlags;                /* assorted flags (see defines) */
    DWORD       dwLoops;                /* loop control counter */
    struct wavehdr_tag *lpNext;     /* reserved for driver */
    DWORD*   	reserved;               /* reserved for driver */
} WAVEHDR, *PWAVEHDR;

typedef void* HWND; //21win : 2008.08.28
///////////////////////////////////////////////////////////////
// Temporary End
///////////////////////////////////////////////////////////////

//#elif defined(_AFX)
//	#include <afx.h>
#elif _IS_WIN_CE_ || _IS_PC_ || _IS_WINMOBILE_
	#include <windows.h>
	#include <assert.h>			
#endif //_IS_SYMBIAN_

#endif //_SYMBIAN_2_WIN32_H_
