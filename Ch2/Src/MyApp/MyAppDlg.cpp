// MyAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyApp.h"
#include "MyAppDlg.h"

#include "SCM.h"
#include "IoCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


DWORD g_dwGlobal = 0x5678;

char g_szBuffer[16];


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
// CMyAppDlg dialog

CMyAppDlg::CMyAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyAppDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyAppDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyAppDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyAppDlg, CDialog)
	//{{AFX_MSG_MAP(CMyAppDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_MUTEX1, OnButtonMutex1)
	ON_BN_CLICKED(IDC_BUTTON_MUTEX2, OnButtonMutex2)
	ON_BN_CLICKED(IDC_BUTTON_EVENT1, OnButtonEvent1)
	ON_BN_CLICKED(IDC_BUTTON_EVENT2, OnButtonEvent2)
	ON_BN_CLICKED(IDC_BUTTON_IOVERIFY, OnButtonIoverify)
	ON_BN_CLICKED(IDC_BUTTON_ERES1, OnButtonEres1)
	ON_BN_CLICKED(IDC_BUTTON_ERES2, OnButtonEres2)
	ON_BN_CLICKED(IDC_BUTTON_BUGCHECK_0XA, OnButtonBugcheck0xa)
	ON_BN_CLICKED(IDC_BUTTON_BUGCHECK_0X50, OnButtonBugcheck0x50)
	ON_BN_CLICKED(IDC_BUTTON_BUGCHECK_0XBE, OnButtonBugcheck0xbe)
	ON_BN_CLICKED(IDC_BUTTON_BUGCHECK_0XD6, OnButtonBugcheck0xd6)
	ON_BN_CLICKED(IDC_BUTTON_BUGCHECK_0XC4, OnButtonBugcheck0xc4)
	ON_BN_CLICKED(IDC_BUTTON_BUGCHECK_0X8E, OnButtonBugcheck0x8e)
	ON_BN_CLICKED(IDC_BUTTON_BUGCHECK_0X7F, OnButtonBugcheck0x7f)
	ON_BN_CLICKED(IDC_BUTTON1, OnBreakPoint)
	ON_BN_CLICKED(IDC_BUTTON_USER_CRASH, OnButtonUserCrash)
	ON_BN_CLICKED(IDC_BUTTON_HANG, OnButtonHang)
	ON_BN_CLICKED(IDC_BUTTON_KERNELHANG, OnButtonKernelhang)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyAppDlg message handlers

BOOL CMyAppDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyAppDlg::OnPaint() 
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
HCURSOR CMyAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyAppDlg::DisplayError(PCHAR pStr, DWORD dwError)
{
	char szBuf[256];

	wsprintf( szBuf, "%s(%d)", pStr, dwError );
	AfxMessageBox( szBuf );
//	::MessageBox( NULL, szBuf, "Message", MB_OK );
}

#define DRIVER_NAME			"MyDrv"

DWORD RegisterDriver(PCHAR pPath, PCHAR pName)
{
	DWORD dwError;
	CHAR szPathName[512];
	DWORD dwLen;

	lstrcpy(szPathName, pPath);

	dwLen = lstrlen(szPathName);
	if (szPathName[dwLen-1] != '\\')
		lstrcat(szPathName, "\\");

	lstrcat(szPathName, pName);
	lstrcat(szPathName, ".sys");

	dwError = RegisterLegacyDriver(szPathName, pName, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL);

	if (dwError == ERROR_SERVICE_EXISTS)
		dwError = ERROR_SUCCESS;

	return dwError;
}

DWORD GetModulePath(PCHAR pPathName)
{
	DWORD dwError = ERROR_SUCCESS;
	char *pEnd;

	if (GetModuleFileName( AfxGetInstanceHandle(), pPathName, 512 ) == 0)
	{
		dwError = GetLastError();

		return dwError;
	}
	
	pEnd = strrchr(pPathName, '\\');
	if (pEnd == NULL)
	{
		return ERROR_INVALID_DATA;
	}

	*pEnd = 0;

	return ERROR_SUCCESS;
}

void CMyAppDlg::OnButtonStart() 
{
	DWORD dwError = ERROR_SUCCESS;
	char szPathName[512];

	dwError = GetModulePath( szPathName );
	if (dwError != ERROR_SUCCESS)
	{
		DisplayError( "GetModulePath", dwError );
		return ;
	}

	dwError = RegisterDriver( szPathName, DRIVER_NAME );
	if (dwError != ERROR_SUCCESS)
	{
		DisplayError( "RegisterDriver Error", dwError );
		return ;
	}	
	
	dwError = StartLegacyDriver( DRIVER_NAME );
	if (dwError != ERROR_SUCCESS)
	{
		DeregisterLegacyDriver( DRIVER_NAME );
		DisplayError( "StartDrvier Error", dwError );
		return ;
	}

	DisplayError( "Start", 0 );
}

void CMyAppDlg::OnButtonStop() 
{
	DWORD dwError = ERROR_SUCCESS;
	
	dwError = StopLegacyDriver( DRIVER_NAME );
	if (dwError != ERROR_SUCCESS)
	{
		DisplayError( "StopDriver Error", dwError );
		return ;
	}

	dwError = DeregisterLegacyDriver( DRIVER_NAME );
	if (dwError != ERROR_SUCCESS)
	{
		DisplayError( "DeregisterDriver Error", dwError );
		return ;
	}

	DisplayError( "Stop", 0 );
}

void CMyAppDlg::OnOK() 
{
	StopLegacyDriver( DRIVER_NAME );
	DeregisterLegacyDriver( DRIVER_NAME );
	
	CDialog::OnOK();
}

void CMyAppDlg::OnCancel() 
{
	StopLegacyDriver( DRIVER_NAME );
	DeregisterLegacyDriver( DRIVER_NAME );
	
	CDialog::OnCancel();
}

DWORD CMyAppDlg::ControlDriver(DWORD dwCode, PVOID pInputBuffer, DWORD dwInputBufferSize,
					PVOID pOutputBuffer, DWORD dwOutputBufferSize)
{
	DWORD dwError = ERROR_SUCCESS;
	DWORD dwReturned;
	HANDLE hDevice;
	CHAR szName[30];

	wsprintf(szName, "\\\\.\\%s", DRIVER_NAME);

	hDevice = CreateFile(szName, GENERIC_READ, 0, NULL,
						 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (hDevice == INVALID_HANDLE_VALUE) 
	{
		dwError = GetLastError();
		return dwError;
	}

	if (!DeviceIoControl(hDevice, dwCode, pInputBuffer, dwInputBufferSize, 
						pOutputBuffer, dwOutputBufferSize, &dwReturned, NULL))
	{
		dwError = GetLastError();
	}

	CloseHandle(hDevice);

	return dwError;
}

void CMyAppDlg::Result(DWORD dwError)
{
	if (dwError != ERROR_SUCCESS)
		DisplayError( "Error Code ", dwError );
	else
		DisplayError( "Success ", 0 );
}

void CMyAppDlg::OnButtonMutex1() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_MUTEX1, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonMutex2() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_MUTEX2, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonEvent1() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_EVENT1, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonEvent2() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_EVENT2, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonIoverify() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_IOVERIFY, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonEres1() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_ERESOURCE1, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonEres2() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_ERESOURCE2, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonBugcheck0xa() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_BUGCHECK_0xA, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonBugcheck0x50() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_BUGCHECK_0x50, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonBugcheck0xbe() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_BUGCHECK_0xBE, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonBugcheck0xd6() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_BUGCHECK_0xD6, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonBugcheck0xc4() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_BUGCHECK_0xC4, NULL, 0, NULL, 0 );

	Result( dwError );
}


void CMyAppDlg::OnButtonBugcheck0x8e() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_BUGCHECK_0x8E, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnButtonBugcheck0x7f() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_BUGCHECK_0x7F, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::OnBreakPoint() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_BREAKPOINT, NULL, 0, NULL, 0 );

	Result( dwError );
}

void CMyAppDlg::MyStrCpy(PCHAR pDest, PCHAR pSrc)
{
	DWORD dwSrcLen, i;

	dwSrcLen = (DWORD)strlen( pSrc );
	
	for (i = 0; i < dwSrcLen; i++)
	{
		pDest[i] = pSrc[i];
	}

	pDest[i] = 0;
}

void CMyAppDlg::OnButtonUserCrash() 
{
	int i;
	char *pBuffer[2] = { g_szBuffer, NULL };

	for (i = 0; i < 2; i++)
	{
		MyStrCpy( pBuffer[i], "UserCrash" );
	}
}

void CMyAppDlg::OnButtonHang() 
{
	DWORD dwCount;
	char szMsg[200];
	
	for (dwCount = 100; dwCount >= 0; dwCount--)
	{
		MyStrCpy( szMsg, "OnButtonHang" );
	}
}

void CMyAppDlg::OnButtonKernelhang() 
{
	DWORD dwError = ControlDriver( MYDRV_IOCTL_HANG, NULL, 0, NULL, 0 );

	Result( dwError );
}
