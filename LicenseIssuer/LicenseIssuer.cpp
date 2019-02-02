// LicenseIssuer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LicenseIssuer.h"
#include "LicenseIssuerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 
// CLicenseIssuerCmdLineInfo:
//
CLicenseIssuerCmdLineInfo::CLicenseIssuerCmdLineInfo() : CCommandLineInfo()
{
	m_bResetFlag = FALSE;
	m_bDoNotCheckLicense = FALSE;
	m_bDebugProtectionProcess = FALSE;
}
void CLicenseIssuerCmdLineInfo::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
{
	if(!m_bResetFlag)
		m_bResetFlag = (0 == strcmpi(lpszParam, "Reset"));

	if(!m_bDebugProtectionProcess)
		m_bDebugProtectionProcess = (0 == strcmpi(lpszParam, "debug"));
}

/////////////////////////////////////////////////////////////////////////////
// CLicenseIssuerApp

BEGIN_MESSAGE_MAP(CLicenseIssuerApp, CWinApp)
	//{{AFX_MSG_MAP(CLicenseIssuerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLicenseIssuerApp construction

CLicenseIssuerApp::CLicenseIssuerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLicenseIssuerApp object

CLicenseIssuerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLicenseIssuerApp initialization

BOOL CLicenseIssuerApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	this->ParseCommandLine(m_objCmdLineInfo);

	{//Set up logger for Protection Process debugging
		char szModuleName[_MAX_PATH];
		CFileFind objFileFind;
		CString strLogFile;
	
		::GetModuleFileName(this->m_hInstance, szModuleName, _MAX_PATH);
		objFileFind.FindFile(szModuleName);
		objFileFind.FindNextFile();
		strLogFile.Format("%s\\%s", objFileFind.GetRoot(), CApplcationProtector::GetDebugLogFileName(CLicenseManager::AppType::LicenseIssuer));
		CProtectionDebugging::StartDebugLog(strLogFile, m_objCmdLineInfo.DebugProtectionProcess());
	}
	BOOL bIsForInternalUse;

	bIsForInternalUse = !m_objCmdLineInfo.IsLicenseCheckRequired();
	CProtectionDebugging::Log((bIsForInternalUse) ? "Protection Type: Not Protected, for internal use ONLY" : "Protection Type: Protected");

	CProtectionDebugging::Log("Launching User Interface");
	CLicenseIssuerDlg dlg(m_objCmdLineInfo);
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	
	CProtectionDebugging::EndDebugLog();
	return FALSE;
}

