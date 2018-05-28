// MyAppDlg.h : header file
//

#if !defined(AFX_MYAPPDLG_H__B2FD72CA_CA37_4619_AB56_456FC55C5748__INCLUDED_)
#define AFX_MYAPPDLG_H__B2FD72CA_CA37_4619_AB56_456FC55C5748__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyAppDlg dialog

class CMyAppDlg : public CDialog
{
// Construction
public:
	void Result(DWORD dwError);
	void DisplayError(PCHAR pStr, DWORD dwError);
	DWORD ControlDriver(DWORD dwCode, PVOID pInputBuffer, DWORD dwInputBufferSize, PVOID pOutputBuffer, DWORD dwOutputBufferSize);
	void MyStrCpy(PCHAR pDest, PCHAR pSrc);

	CMyAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyAppDlg)
	enum { IDD = IDD_MYAPP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonMutex1();
	afx_msg void OnButtonMutex2();
	afx_msg void OnButtonEvent1();
	afx_msg void OnButtonEvent2();
	afx_msg void OnButtonIoverify();
	afx_msg void OnButtonEres1();
	afx_msg void OnButtonEres2();
	afx_msg void OnButtonBugcheck0xa();
	afx_msg void OnButtonBugcheck0x50();
	afx_msg void OnButtonBugcheck0xbe();
	afx_msg void OnButtonBugcheck0xd6();
	afx_msg void OnButtonBugcheck0xc4();
	afx_msg void OnButtonBugcheck0x3b();
	afx_msg void OnButtonBugcheck0x7f();
	afx_msg void OnBreakPoint();
	afx_msg void OnButtonUserCrash();
	afx_msg void OnButtonHang();
	afx_msg void OnButtonKernelhang();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYAPPDLG_H__B2FD72CA_CA37_4619_AB56_456FC55C5748__INCLUDED_)
