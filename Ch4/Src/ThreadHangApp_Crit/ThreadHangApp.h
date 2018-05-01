// ThreadHangApp.h : main header file for the THREADHANGAPP application
//

#if !defined(AFX_THREADHANGAPP_H__70AA5E92_1E42_42C5_922C_C2A5B33F5E38__INCLUDED_)
#define AFX_THREADHANGAPP_H__70AA5E92_1E42_42C5_922C_C2A5B33F5E38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CThreadHangAppApp:
// See ThreadHangApp.cpp for the implementation of this class
//

class CThreadHangAppApp : public CWinApp
{
public:
	CThreadHangAppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadHangAppApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CThreadHangAppApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREADHANGAPP_H__70AA5E92_1E42_42C5_922C_C2A5B33F5E38__INCLUDED_)
