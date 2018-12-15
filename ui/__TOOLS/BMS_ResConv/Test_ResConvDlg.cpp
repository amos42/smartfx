// Test_ResConvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test_ResConv.h"
#include "Test_ResConvDlg.h"
#include "BMSWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_ResConvDlg dialog

CTest_ResConvDlg::CTest_ResConvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTest_ResConvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTest_ResConvDlg)
	m_strInputPath = _T("");
	m_strOutputPath = _T("");
	m_strHeaderFile = _T("");
	m_bShowLog = FALSE;
	m_dwId = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strTestPath =  _T("");
	m_strLogFile = _T("");
	m_bAuto = FALSE;
}

void CTest_ResConvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTest_ResConvDlg)
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInputPath);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutputPath);
	DDX_Text(pDX, IDC_EDIT_HEADER, m_strHeaderFile);
	DDX_Check(pDX, IDC_CHECK_SHOW, m_bShowLog);
	DDX_Text(pDX, IDC_EDIT_ID, m_dwId);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTest_ResConvDlg, CDialog)
	//{{AFX_MSG_MAP(CTest_ResConvDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INPUT, OnButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, OnButtonOutput)
	ON_BN_CLICKED(IDC_BUTTON_FILTER, OnButtonFilter)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_HEADER, OnButtonHeader)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_ResConvDlg message handlers

BOOL CTest_ResConvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	char lpExt[_MAX_PATH];	
	_splitpath(m_strInputPath, NULL, NULL, NULL, lpExt);
	if (stricmp(lpExt, ".bst")==0)
	{
		GetDlgItem(IDC_BUTTON_OUTPUT)->EnableWindow(FALSE);

		if (m_bAuto)
		{
			OnButtonFilter();
			//::SendMessage(GetSafeHwnd(), WM_CLOSE, NULL, NULL);
			exit(0);
		}
	}
	else
		GetDlgItem(IDC_BUTTON_OUTPUT)->EnableWindow(TRUE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTest_ResConvDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTest_ResConvDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTest_ResConvDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTest_ResConvDlg::UpdatePath() 
{
	if (GetSafeHwnd()) UpdateData(TRUE);

	char lpBuffer[_MAX_PATH];
	char lpDrive[_MAX_PATH];	
	char lpPath[_MAX_PATH];	
	char lpName[_MAX_PATH];	
	char lpExt[_MAX_PATH];	
	
	_splitpath(m_strInputPath, lpDrive, lpPath, lpName, lpExt);

	_makepath(lpBuffer, lpDrive, lpPath, lpName, "bms");
	m_strOutputPath = lpBuffer;

	_makepath(lpBuffer, lpDrive, lpPath, "language", ".h");
	m_strHeaderFile = lpBuffer;

	_makepath(lpBuffer, lpDrive, lpPath, lpName, ".txt");
	if (stricmp(m_strInputPath, lpBuffer))
		m_strLogFile = lpBuffer;
	else
		m_strLogFile.Empty();

	m_strTestPath = m_strOutputPath; 

	if (stricmp(lpExt, ".bst")==0)
	{
		FILE* hFile = fopen(m_strInputPath, "rt");

		DWORD dwCount = 0;
		
		char lpBuffer[_MAX_PATH];		// Reading buffer
		char* lpParameter;
		CString strFilename;

		char lpBMSPath[_MAX_PATH] = {0, };	
		char lpTXTPath[_MAX_PATH] = {0, };
		char lpDEFPath[_MAX_PATH] = {0, };

		while(!feof(hFile))
		{
			// Reading buffer
			memset(lpBuffer, 0x00, _MAX_PATH);
			fgets(lpBuffer, _MAX_PATH, hFile);
		
			// 주석 처리
			if ((lpParameter = strstr(lpBuffer, "//"))) *lpParameter = NULL;

			if ((lpParameter = strstr(lpBuffer, "bms_path:")))
			{
				strFilename = lpParameter+strlen("bms_path:");
				strFilename.TrimLeft();
				strFilename.TrimRight();
				strcpy(lpBMSPath, strFilename.GetBuffer(strFilename.GetLength()));
			}
			else if ((lpParameter = strstr(lpBuffer, "txt_path:")))
			{
				strFilename = lpParameter+strlen("txt_path:");
				strFilename.TrimLeft();
				strFilename.TrimRight();
				strcpy(lpTXTPath, strFilename.GetBuffer(strFilename.GetLength()));
			}
			else if ((lpParameter = strstr(lpBuffer, "def_path:")))
			{
				strFilename = lpParameter+strlen("def_path:");
				strFilename.TrimLeft();
				strFilename.TrimRight();
				strcpy(lpDEFPath, strFilename.GetBuffer(strFilename.GetLength()));
			}
		}

		fclose(hFile);

		m_strOutputPath = lpBMSPath;
		m_strLogFile = lpTXTPath;
		m_strHeaderFile = lpDEFPath;

		if (GetSafeHwnd()) GetDlgItem(IDC_BUTTON_OUTPUT)->EnableWindow(FALSE);
	}
	else
	{
		if (GetSafeHwnd()) GetDlgItem(IDC_BUTTON_OUTPUT)->EnableWindow(TRUE);
	}

	if (GetSafeHwnd()) UpdateData(FALSE);
}

void CTest_ResConvDlg::OnButtonInput() 
{
	CFileDialog dlg(TRUE, _T("*.bst"), NULL, OFN_HIDEREADONLY,
		_T("Script Files (*.bst)|*.bst|Res Files (*.rc;*.txt;*.h)|*.rc;*.txt;*.h|All Files (*.*)|*.*||"), this);
	dlg.m_ofn.lpstrTitle = "Import Res File";
	
	if (dlg.DoModal() != IDOK) return;

	UpdateData(TRUE);
	m_strInputPath = dlg.GetPathName();
	UpdateData(FALSE);

	UpdatePath();
}

void CTest_ResConvDlg::OnButtonHeader() 
{
	CFileDialog dlg(FALSE, _T("*.h"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Header Files (*.h)|*.h|All Files (*.*)|*.*||"), this);
	dlg.m_ofn.lpstrTitle = "Include Header File";

	if (dlg.DoModal() != IDOK) return;

	UpdateData(TRUE);
	m_strHeaderFile = dlg.GetPathName();
	UpdateData(FALSE);
}

void CTest_ResConvDlg::OnButtonOutput() 
{
	char lpExt[_MAX_PATH];	
	_splitpath(m_strInputPath, NULL, NULL, NULL, lpExt);
	if (stricmp(lpExt, ".bst")==0) return;

	CFileDialog dlg(FALSE, _T("*.bms"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("MBS Files (*.bms)|*.bms|All Files (*.*)|*.*||"), this);
	dlg.m_ofn.lpstrTitle = "Export MBS File";

	if (dlg.DoModal() != IDOK) return;

	UpdateData(TRUE);
	m_strOutputPath = dlg.GetPathName();
	UpdateData(FALSE);
}

void CTest_ResConvDlg::OnButtonFilter() 
{
	UpdateData(TRUE);

	if (m_strInputPath == m_strOutputPath)
	{
		MessageBox("Output filename is invalidate!", "Error", MB_OK|MB_ICONERROR);
		return;
	}

	BeginWaitCursor();

	long lFlag = 0;

	char lpExt[_MAX_PATH];	
	_splitpath(m_strInputPath, NULL, NULL, NULL, lpExt);
	if (stricmp(lpExt, ".bst")==0)
	{
		lFlag = BuildScript(m_strInputPath.GetBuffer(m_strInputPath.GetLength()));
	}
	else
	{
		char lpszInput[_MAX_PATH] = {0,};
		char lpszOutput[_MAX_PATH] = {0,};
		char lpszHeader[_MAX_PATH] = {0,};
		char lpszLog[_MAX_PATH] = {0,};

		strcpy(lpszInput, m_strInputPath.GetBuffer(m_strInputPath.GetLength()));
		strcpy(lpszOutput, m_strOutputPath.GetBuffer(m_strOutputPath.GetLength()));
		strcpy(lpszHeader, m_strHeaderFile.GetBuffer(m_strHeaderFile.GetLength()));
		strcpy(lpszLog, m_strLogFile.GetBuffer(m_strLogFile.GetLength()));
		
		if (stricmp(lpszInput, lpszOutput)==0) lpszOutput[0] = NULL;
		if (stricmp(lpszInput, lpszHeader)==0) lpszHeader[0] = NULL;
		if (stricmp(lpszInput, lpszLog)==0) lpszLog[0] = NULL;

		lFlag = BuildFile(lpszInput, lpszOutput, lpszHeader, lpszLog, m_bShowLog);
	}

	if (lFlag==0)
		MessageBox("Building Complete!", "Complete", MB_OK|MB_ICONINFORMATION);

	EndWaitCursor();
}

long CTest_ResConvDlg::BuildScript(char* lpszInput)
{
	FILE* hFile = fopen(lpszInput, "rt");

	DWORD dwCount = 0;
	
	char lpBuffer[_MAX_PATH];		// Reading buffer
	char* lpParameter;
	CString strFilename;

	char lpBMSPath[_MAX_PATH] = {0, };	
	char lpTXTPath[_MAX_PATH] = {0, };
	char lpDEFPath[_MAX_PATH*10] = {0, };

	while(!feof(hFile))
	{
		// Reading buffer
		memset(lpBuffer, 0x00, _MAX_PATH);
		fgets(lpBuffer, _MAX_PATH, hFile);
	
		// 주석 처리
		if ((lpParameter = strstr(lpBuffer, "//"))) *lpParameter = NULL;

		if ((lpParameter = strstr(lpBuffer, "bms_path:")))
		{
			strFilename = lpParameter+strlen("bms_path:");
			strFilename.TrimLeft();
			strFilename.TrimRight();
			strcpy(lpBMSPath, strFilename.GetBuffer(strFilename.GetLength()));

			continue;
		}
		else if ((lpParameter = strstr(lpBuffer, "txt_path:")))
		{
			strFilename = lpParameter+strlen("txt_path:");
			strFilename.TrimLeft();
			strFilename.TrimRight();
			strcpy(lpTXTPath, strFilename.GetBuffer(strFilename.GetLength()));

			continue;
		}
		else if ((lpParameter = strstr(lpBuffer, "def_path:")))
		{
			strFilename = lpParameter+strlen("def_path:");
			strFilename.TrimLeft();
			strFilename.TrimRight();
			strcpy(lpDEFPath, strFilename.GetBuffer(strFilename.GetLength()));

			continue;
		}

		strFilename = lpBuffer;
		strFilename.TrimLeft();
		strFilename.TrimRight();

		if (strFilename.IsEmpty()) continue;

		char lpDrive[_MAX_PATH];	
		char lpPath[_MAX_PATH];	
		char lpName[_MAX_PATH];	
		char lpExt[_MAX_PATH];	
		_splitpath(strFilename, lpDrive, lpPath, lpName, lpExt);
		if (!lpDrive[0] && !lpPath[0])
		{
			_splitpath(m_strInputPath, lpDrive, lpPath, NULL, NULL);
			_makepath(lpBuffer, lpDrive, lpPath, lpName, lpExt);
			strFilename = lpBuffer;
			lpDrive[0] = NULL;
			lpPath[0] = NULL;
		}

		char lpszInput[_MAX_PATH] = {0,};
		char lpszOutput[_MAX_PATH] = {0,};
		char lpszHeader[_MAX_PATH] = {0,};
		char lpszLog[_MAX_PATH] = {0,};

		strcpy(lpszInput, strFilename.GetBuffer(strFilename.GetLength()));

		_makepath(lpBuffer, lpDrive, lpBMSPath, lpName, "bms");
		strcpy(lpszOutput, lpBuffer);
		if (stricmp(lpszInput, lpBuffer)==0)
		{
			ASSERT(0);
			continue;
		}

		DWORD dwLen = strlen(lpDEFPath);
		DWORD st, ed;
		char tFilename[_MAX_PATH];
		
		for (st=0; st<dwLen-1; st++)
		{
			if (lpDEFPath[st] == ';' || lpDEFPath[st] == ' ') continue;

			for (ed=st+1; ed<dwLen; ed++)
			{
				if (ed == dwLen-1)
					ed++;
				else if (lpDEFPath[ed] != ';')
					continue;

				strncpy(tFilename, lpDEFPath+st, ed-st);
				tFilename[ed-st] = NULL;

				for (long i=(ed-st)-1; i >=0; i--)
				{
					if (tFilename[i] != ' ') break;
					tFilename[i] = NULL;
				}

				st = ed;

				_makepath(lpBuffer, lpDrive, tFilename, NULL, NULL);
				lpBuffer[strlen(lpBuffer)-1] = NULL;
				if (stricmp(lpszInput, lpBuffer))
				{
					strcat(lpszHeader, lpBuffer);
					strcat(lpszHeader, ";");
				}

				break;
			}
		}

		_makepath(lpBuffer, lpDrive, lpTXTPath, lpName, ".txt");
		if (stricmp(lpExt, ".txt") && stricmp(lpszInput, lpBuffer))
			strcpy(lpszLog, lpBuffer);

		BuildFile(lpszInput, lpszOutput, lpszHeader, lpszLog, FALSE);
		
		if (dwCount == 0)
			m_strTestPath = lpszOutput;
	
		dwCount++;
	}

	fclose(hFile);

	return 0;
}

long CTest_ResConvDlg::BuildFile(char* lpszInput, char* lpszOutput, char* lpszHeader, char* lpszLog, BOOL bShowLog)
{
	// binary로 읽어서 처리한다.
	// 최초 2BYTE로 유니코드 인지 비교하고.. (0xFFFE)
	// "IDS_"를 만나면 ' '가 나올 때 까지가 ID
	// "..." 사이의 값을 스트링으로 처리한다.
	// 메모리 상에 풀리스트를 만들어 넣고 일괄 저장한다.

	UpdateData(TRUE);

	CBMSWriter writer;

	if (!writer.ReadFile(lpszInput))
	{
		MessageBox("Script parsing Failure!", lpszInput, MB_OK|MB_ICONERROR);
		return 1;
	}

	if (!writer.MergeIndex(lpszHeader))
	{
		MessageBox("Index merging Failure!", lpszInput, MB_OK|MB_ICONERROR);
		return 1;
	}

	if (!writer.BuildFile(lpszOutput))
	{
		MessageBox("File building Failure!", lpszOutput, MB_OK|MB_ICONERROR);
		return 2;
	}

	//if (lpszHeader && lpszHeader[0])
	//	writer.MakeHeaderFile(lpszOutput, lpszHeader);
	
	if (lpszLog && lpszLog[0])
	{
		writer.MakeLogFile(lpszOutput, lpszLog);

		if (bShowLog)
			WinExec(CString("notepad.exe ")+CString(lpszLog), SW_SHOW);
	}

	return 0;
}

void CTest_ResConvDlg::OnButtonSearch() 
{
	UpdateData(TRUE);
	
	CBMSReader reader;
	wchar_t szValue[BMS_MAX_STRING];
	char szFilename[_MAX_PATH] = "c:/result.txt";

	if (reader.Open(m_strTestPath.GetBuffer(m_strTestPath.GetLength())))
	{
		if (reader.GetString(m_dwId, szValue, BMS_MAX_STRING))
		{
			if (szValue[0] == 0)
				wcscpy(szValue, L"[NULL]");

			FILE* hFile = fopen(szFilename, "wb");
			WORD tag = 0xFEFF;
			fwrite(&tag, sizeof(tag), 1, hFile);
			fputws(szValue, hFile);
			fclose(hFile);

			WinExec(CString("notepad.exe ")+CString(szFilename), SW_SHOW);
			return;
		}
	}

	MessageBox("Search Failure!", "Message", MB_OK|MB_ICONERROR);
}
