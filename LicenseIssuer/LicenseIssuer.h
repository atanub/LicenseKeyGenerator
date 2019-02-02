// LicenseIssuer.h : main header file for the LICENSEISSUER application
//

#if !defined(AFX_LICENSEISSUER_H__1DA774CD_B8DF_4B27_A1F1_FA2E0DFDC2E3__INCLUDED_)
#define AFX_LICENSEISSUER_H__1DA774CD_B8DF_4B27_A1F1_FA2E0DFDC2E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// 
// CLicenseIssuerCmdLineInfo:
//
class CLicenseIssuerCmdLineInfo : public CCommandLineInfo
{
public:
	CLicenseIssuerCmdLineInfo();
public:
	BOOL IsResetRequested() const { return m_bResetFlag;}
	BOOL IsLicenseCheckRequired() const { return !m_bDoNotCheckLicense;}
	BOOL DebugProtectionProcess() const { return m_bDebugProtectionProcess;}
public:
	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );

protected:
	BOOL		m_bResetFlag;
	BOOL		m_bDoNotCheckLicense;
	BOOL		m_bDebugProtectionProcess;
};

/////////////////////////////////////////////////////////////////////////////
// CLicenseIssuerApp:
// See LicenseIssuer.cpp for the implementation of this class
//

class CLicenseIssuerApp : public CWinApp
{
public:
	CLicenseIssuerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLicenseIssuerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLicenseIssuerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CLicenseIssuerCmdLineInfo m_objCmdLineInfo;

};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LICENSEISSUER_H__1DA774CD_B8DF_4B27_A1F1_FA2E0DFDC2E3__INCLUDED_)
