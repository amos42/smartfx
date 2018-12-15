// Test_ResConv.h : main header file for the TEST_RESCONV application
//

#if !defined(AFX_TEST_RESCONV_H__281D43AA_36EB_41BB_93C9_02226667A3B4__INCLUDED_)
#define AFX_TEST_RESCONV_H__281D43AA_36EB_41BB_93C9_02226667A3B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTest_ResConvApp:
// See Test_ResConv.cpp for the implementation of this class
//

class CTest_ResConvApp : public CWinApp
{
public:
	CTest_ResConvApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_ResConvApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTest_ResConvApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_RESCONV_H__281D43AA_36EB_41BB_93C9_02226667A3B4__INCLUDED_)
