// ProcessHangAppDlg.h : header file
//

#if !defined(AFX_PROCESSHANGAPPDLG_H__99BB5354_2683_41FD_9D3C_317C0DE26545__INCLUDED_)
#define AFX_PROCESSHANGAPPDLG_H__99BB5354_2683_41FD_9D3C_317C0DE26545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CProcessHangAppDlg dialog

class CProcessHangAppDlg : public CDialog
{
// Construction
public:
	CProcessHangAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CProcessHangAppDlg)
	enum { IDD = IDD_PROCESSHANGAPP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessHangAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CProcessHangAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnHung();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSHANGAPPDLG_H__99BB5354_2683_41FD_9D3C_317C0DE26545__INCLUDED_)
