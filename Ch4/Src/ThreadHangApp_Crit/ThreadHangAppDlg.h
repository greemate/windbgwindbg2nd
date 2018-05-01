// ThreadHangAppDlg.h : header file
//

#if !defined(AFX_THREADHANGAPPDLG_H__082DCC44_1B81_4961_A430_2EF77D78C8F5__INCLUDED_)
#define AFX_THREADHANGAPPDLG_H__082DCC44_1B81_4961_A430_2EF77D78C8F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CThreadHangAppDlg dialog

#define MAX_THREAD	10

class CThreadHangAppDlg : public CDialog
{
// Construction
public:
	CThreadHangAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CThreadHangAppDlg)
	enum { IDD = IDD_THREADHANGAPP_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadHangAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:	
	HANDLE	m_hThread[MAX_THREAD];
	DWORD	m_dwThreadId[MAX_THREAD];
	DWORD	m_nThreadCount;
	HANDLE	m_hLockEvent;
	static	void OutputMessage(LPCTSTR lpszMessage);
	static CEdit	m_ctrlOutput;
	BOOL	m_bEndFlag;
	BOOL	m_bSignal;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CThreadHangAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnThread();
	afx_msg void OnBtnDestroy();
	afx_msg void OnBtnSignal();
	afx_msg void OnBtnNonsignal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREADHANGAPPDLG_H__082DCC44_1B81_4961_A430_2EF77D78C8F5__INCLUDED_)
