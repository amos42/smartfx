// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__ATORNADO_CE4A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__ATORNADO_CE4A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

//#include <windows.h>

// C RunTime Header Files
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>

//#include "define.h"

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
		#include <windows.h>
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

#include <ASSERT.H>

#include "define.h"


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__ATORNADO_CE4A9FD_11D0_BFD1_444553540000__INCLUDED_)
