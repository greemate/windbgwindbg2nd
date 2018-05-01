// ThreadHangAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThreadHangApp.h"
#include "ThreadHangAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRITICAL_SECTION g_CriticalSection; 

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
// CThreadHangAppDlg dialog

CThreadHangAppDlg::CThreadHangAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThreadHangAppDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThreadHangAppDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreadHangAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThreadHangAppDlg)
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_ctrlOutput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CThreadHangAppDlg, CDialog)
	//{{AFX_MSG_MAP(CThreadHangAppDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_THREAD, OnBtnThread)
	ON_BN_CLICKED(IDC_BTN_DESTROY, OnBtnDestroy)
	ON_BN_CLICKED(IDC_BTN_SIGNAL, OnBtnSignal)
	ON_BN_CLICKED(IDC_BTN_NONSIGNAL, OnBtnNonsignal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThreadHangAppDlg message handlers

BOOL CThreadHangAppDlg::OnInitDialog()
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
	m_nThreadCount = 0;
	m_bEndFlag = FALSE;
	m_bSignal = FALSE;

	m_hLockEvent = CreateEvent(NULL, 
		TRUE,	// manual-reset
		TRUE,	// signaled
		_T("DEMO_THREADDEADLOCK"));

	// check m_hLockEvent == NULL

	InitializeCriticalSection(&g_CriticalSection); 

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CThreadHangAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CThreadHangAppDlg::OnPaint() 
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
HCURSOR CThreadHangAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// 에디트 창에 로그 메세지를 뿌려줌
void CThreadHangAppDlg::OutputMessage(LPCTSTR lpszMessage)
{
	m_ctrlOutput.SetSel(-1, -1);
	m_ctrlOutput.ReplaceSel(lpszMessage);
}

DWORD WINAPI ThreadProc( LPVOID lpParam )
{
	CThreadHangAppDlg* pDlg = (CThreadHangAppDlg*)lpParam;
	TCHAR* szOutputMessage = (TCHAR*)malloc(128);
	RtlZeroMemory(szOutputMessage, 128);
	sprintf(szOutputMessage, "TID %x has been created \n", GetCurrentThreadId());

	pDlg->OutputMessage(szOutputMessage);

	// 종료 플래그가 설정될때까지,
	while (pDlg->m_bEndFlag != TRUE)
	{
		DWORD dwWaitResult = 0;
		RtlZeroMemory(szOutputMessage, 128);
		sprintf(szOutputMessage, "[%x] : I'm alive! \n", GetCurrentThreadId());
		pDlg->OutputMessage(szOutputMessage);

		// wait
		//dwWaitResult = WaitForSingleObject(pDlg->m_hLockEvent, INFINITE);
		EnterCriticalSection( &g_CriticalSection );
	
		LeaveCriticalSection(&g_CriticalSection);

		Sleep(2000);

		
	}

	RtlZeroMemory(szOutputMessage, 128);
	sprintf(szOutputMessage, "TID %x ends. \n", GetCurrentThreadId());
	// OutputMessage 멤버함수의 SetSel 호출에서, 내부적으로 SendMessage를 하는데, 
	// 메인 다이얼로그의 OnBtnDestroy()에서 wait 함수로 이 스레드가 죽기를 기다리고 있어서 데드락이 걸린다. 
	// (스레드는 메인 다이얼로그로 보낸 SendMessage의 응답을 기다리고, 메인 다이얼로그는 스레드가 죽기를 기다림)
	// pDlg->OutputMessage(szOutputMessage);	// 커맨트를 풀면 OnBtnDestroy() 호출시에 데드락
	OutputDebugString(szOutputMessage);
	free(szOutputMessage);

	return 0;
}

CEdit	CThreadHangAppDlg::m_ctrlOutput;

void CThreadHangAppDlg::OnBtnThread() 
{
	// 스레드 생성
    m_hThread[m_nThreadCount] = CreateThread( 
			NULL,				// default security attributes
			0,					// use default stack size  
			ThreadProc,			// thread function 
			this,				// argument to thread function 
			0,					// use default creation flags 
			&m_dwThreadId[m_nThreadCount]);   // returns the thread identifier 
 
    if (m_hThread[m_nThreadCount] == NULL) 
    {
        ASSERT(FALSE);
		MessageBox("CreateThread failed!");
    }

	m_nThreadCount++;
}

void CThreadHangAppDlg::OnBtnDestroy() 
{
	// 스레드 종료 전역 플래그를 설정
	m_bEndFlag = TRUE;

    // 모든 스레드가 종료될때까지 기다린다
    WaitForMultipleObjects(m_nThreadCount, m_hThread, TRUE, INFINITE);

    // 모든 스레드 핸들을 닫는다
	for (unsigned nThreadIdx = 0 ; nThreadIdx < m_nThreadCount ; nThreadIdx++)
	{
		CloseHandle(m_hThread[nThreadIdx]);
	}

	DeleteCriticalSection(&g_CriticalSection);

}

void CThreadHangAppDlg::OnBtnSignal() 
{
	EnterCriticalSection( &g_CriticalSection );
}

void CThreadHangAppDlg::OnBtnNonsignal() 
{
	LeaveCriticalSection(&g_CriticalSection);

}
