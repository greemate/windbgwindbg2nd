// LabApp.h : main header file for the LABAPP application
//

#if !defined(AFX_LABAPP_H__AD340E53_F77A_42C5_BD25_8E113188A4E2__INCLUDED_)
#define AFX_LABAPP_H__AD340E53_F77A_42C5_BD25_8E113188A4E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLabAppApp:
// See LabApp.cpp for the implementation of this class
//

class CLabAppApp : public CWinApp
{
public:
	CLabAppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabAppApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLabAppApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LABAPP_H__AD340E53_F77A_42C5_BD25_8E113188A4E2__INCLUDED_)
