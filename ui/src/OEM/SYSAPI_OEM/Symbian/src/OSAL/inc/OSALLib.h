#ifndef __OSALLIB_HEADER_FILE_H__
#define __OSALLIB_HEADER_FILE_H__

# if _MSC_VER > 1000
#  pragma once
# endif // _MSC_VER > 1000

#if defined(_WINDOWS) || defined(_LIB)
	# if _MSC_VER > 1000
	#  pragma once
	# endif // _MSC_VER > 1000

	#ifdef _AFXDLL
	# define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

	# include <afx.h>
	# include <afxwin.h>         // MFC core and standard components
	# include <afxext.h>         // MFC extensions
	#else
	#include <WINDOWS.H>
	#endif

	#ifdef _UNICODE
		#include <TCHAR.h>
		#include <WCHAR.h>
	#endif
#else
	#ifdef _UNICODE
		#include <WCHAR.h>
	#endif
#endif

#if _IS_PC_ && _MSC_VER > 1200
	#pragma warning(disable : 4996)
#endif

#if _IS_PC_ && _MSC_VER > 1200
	#include "stdafx.h"
#endif

#include <ASSERT.H>

//#include "define.h"
//#include "alpha_unicode.h"
//#include "HeapManager.h"

#endif//#define __OSALLIB_HEADER_FILE_H__
