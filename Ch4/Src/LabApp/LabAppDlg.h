// LabAppDlg.h : header file
//

#if !defined(AFX_LABAPPDLG_H__A1211BB0_9D07_471A_9156_9D111EBF9A8E__INCLUDED_)
#define AFX_LABAPPDLG_H__A1211BB0_9D07_471A_9156_9D111EBF9A8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLabAppDlg dialog

class CLabAppDlg : public CDialog
{
// Construction
public:
	CLabAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLabAppDlg)
	enum { IDD = IDD_LABAPP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLabAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNullptraccessBtn();
	afx_msg void OnBufferoverflowBtn();
	afx_msg void OnFreedheapaccessBtn();
	afx_msg void OnHangBtn();
	afx_msg void OnThreadhangBtn();
	afx_msg void OnMemoryleakBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LABAPPDLG_H__A1211BB0_9D07_471A_9156_9D111EBF9A8E__INCLUDED_)
