// LabAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LabApp.h"
#include "LabAppDlg.h"

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
// CLabAppDlg dialog

CLabAppDlg::CLabAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLabAppDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLabAppDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLabAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLabAppDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLabAppDlg, CDialog)
	//{{AFX_MSG_MAP(CLabAppDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_NULLPTRACCESS_BTN, OnNullptraccessBtn)
	ON_BN_CLICKED(IDC_BUFFEROVERFLOW_BTN, OnBufferoverflowBtn)
	ON_BN_CLICKED(IDC_FREEDHEAPACCESS_BTN, OnFreedheapaccessBtn)
	ON_BN_CLICKED(IDC_HANG_BTN, OnHangBtn)
	ON_BN_CLICKED(IDC_THREADHANG_BTN, OnThreadhangBtn)
	ON_BN_CLICKED(IDC_MEMORYLEAK_BTN, OnMemoryleakBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLabAppDlg message handlers

BOOL CLabAppDlg::OnInitDialog()
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

void CLabAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLabAppDlg::OnPaint() 
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
HCURSOR CLabAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

struct SomeStructWhichNotInitialized
{
	PVOID MemoryBuffer;
};

struct MYStruct
{
	PCHAR pBuffer;
};

void NullAccessFunction(MYStruct* pMyStruct)
{
	strcpy( pMyStruct->pBuffer, "test" );
}

// NULL 포인터 엑세스 demo
void CLabAppDlg::OnNullptraccessBtn() 
{
	MYStruct stStruct;
	stStruct.pBuffer = NULL;

	NullAccessFunction(&stStruct);
}

void CLabAppDlg::OnBufferoverflowBtn() 
{
	char szLocalBuffer[10] = {0};

	strcpy(szLocalBuffer, "strcpyIsDeprecatedSoYouShouldUseSafeStringFunctions");
	
}

// 해제한 메모리 사용함
void CLabAppDlg::OnFreedheapaccessBtn() 
{
	SomeStructWhichNotInitialized* pInitializedStruct = 
		(SomeStructWhichNotInitialized*)malloc(sizeof(SomeStructWhichNotInitialized));

	free(pInitializedStruct);

	pInitializedStruct->MemoryBuffer = (PVOID)0xffffffff;
}

void CLabAppDlg::OnHangBtn() 
{
	while (1)
	{
		// do nothing
		;
	}
}


///////////////////////////////////////////////////////////////////////
// Thread Hang
///////////////////////////////////////////////////////////////////////
DWORD __stdcall ThreadFunc( LPVOID lpParam ) 
{
	while (1)
	{
		Sleep(100000);
	}

	return 0;
}


DWORD __stdcall ThreadFunc2( LPVOID lpParam ) 
{
	ULONG	ulIndex = 0;

	while (1)
	{
		// do nothing, just shows infinite loop
		ulIndex++;
	}

	return 0;
}

void CLabAppDlg::OnThreadhangBtn() 
{
	HANDLE	hThread = NULL;
	ULONG	ulIndex = 0;
	ULONG	ulHangIndex = 0;

    srand( (unsigned)time( NULL ) );
	ulHangIndex = rand() % 5;

	for (ulIndex = 0; ulIndex < 5; ulIndex++)
	{
		if (ulIndex == ulHangIndex) 
		{
			hThread = CreateThread(NULL, 0, ThreadFunc2, NULL, 0, NULL);
		}
		else
		{
			hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
		}

		if ( hThread == NULL) 
		{
			ASSERT(FALSE);
			// CreateTherad failed.
		}
		else
		{
			CloseHandle(hThread);
		}
	}
}

void CLabAppDlg::OnMemoryleakBtn() 
{
	PVOID	pBuffer = malloc(100);
}
