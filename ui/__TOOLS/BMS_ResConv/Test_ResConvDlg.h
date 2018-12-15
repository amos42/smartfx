// Test_ResConvDlg.h : header file
//

#if !defined(AFX_TEST_RESCONVDLG_H__F65D2A36_3DD1_458C_86A0_C725F2C70B93__INCLUDED_)
#define AFX_TEST_RESCONVDLG_H__F65D2A36_3DD1_458C_86A0_C725F2C70B93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTest_ResConvDlg dialog

class CTest_ResConvDlg : public CDialog
{
// Construction
public:
	CTest_ResConvDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTest_ResConvDlg)
	enum { IDD = IDD_TEST_RESCONV_DIALOG };
	CString	m_strInputPath;
	CString	m_strOutputPath;
	CString	m_strHeaderFile;
	BOOL	m_bShowLog;
	DWORD	m_dwId;
	//}}AFX_DATA
	
	CString	m_strTestPath;

public:
	CString	m_strLogFile;
	BOOL m_bAuto;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_ResConvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
public:
	// Generated message map functions
	//{{AFX_MSG(CTest_ResConvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	afx_msg void OnButtonFilter();
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonHeader();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void UpdatePath();
	long BuildScript(char* lpszInput);
	long BuildFile(char* lpszInput, char* lpszOutput, char* lpszHeader, char* lpszLog, BOOL bShowLog);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_RESCONVDLG_H__F65D2A36_3DD1_458C_86A0_C725F2C70B93__INCLUDED_)
