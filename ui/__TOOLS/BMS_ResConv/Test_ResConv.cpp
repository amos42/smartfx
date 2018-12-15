// Test_ResConv.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Test_ResConv.h"
#include "Test_ResConvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTest_ResConvApp

BEGIN_MESSAGE_MAP(CTest_ResConvApp, CWinApp)
	//{{AFX_MSG_MAP(CTest_ResConvApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_ResConvApp construction

CTest_ResConvApp::CTest_ResConvApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTest_ResConvApp object

CTest_ResConvApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTest_ResConvApp initialization

BOOL CTest_ResConvApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	CTest_ResConvDlg dlg;

	CFileFind ff;
	if (!cmdInfo.m_strFileName.IsEmpty() && ff.FindFile(cmdInfo.m_strFileName))
	{
		dlg.m_bAuto = cmdInfo.m_bRunAutomated;
		//dlg.m_bShowLog = !dlg.m_bAuto;
		dlg.m_strInputPath = cmdInfo.m_strFileName;
		dlg.UpdatePath();
	}

	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
