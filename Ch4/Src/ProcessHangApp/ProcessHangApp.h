// ProcessHangApp.h : main header file for the PROCESSHANGAPP application
//

#if !defined(AFX_PROCESSHANGAPP_H__C54D21E9_D2F4_4500_A9F8_DE3C012A9FDA__INCLUDED_)
#define AFX_PROCESSHANGAPP_H__C54D21E9_D2F4_4500_A9F8_DE3C012A9FDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CProcessHangAppApp:
// See ProcessHangApp.cpp for the implementation of this class
//

class CProcessHangAppApp : public CWinApp
{
public:
	CProcessHangAppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessHangAppApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CProcessHangAppApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSHANGAPP_H__C54D21E9_D2F4_4500_A9F8_DE3C012A9FDA__INCLUDED_)
