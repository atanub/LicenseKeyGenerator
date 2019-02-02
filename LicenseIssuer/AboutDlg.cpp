// LicenseIssuer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LicenseIssuer.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg
CAboutDlg::CAboutDlg(const CLicenseIssuerCmdLineInfo& objCmdLineInfo, CApplcationProtector* pProtectionInterface) 
: CDialog(CAboutDlg::IDD), m_pAppProtector(pProtectionInterface), m_objCmdLineInfo(objCmdLineInfo)
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
	//ON_BN_CLICKED(IDC_BUTTON_ME, OnDeveloper)
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	const CString strLicenceInfoFmt("This Product is licensed to :\r\t%s,\r\t%s");
	
	CDialog::OnInitDialog();
	CString strLicenceInfo, strTmp, strToCompany, strAboutMeButtonCaption, strToCompanyAddress, strLicenseInfoForUser;

	strToCompany.LoadString(IDS_CLIENT_NAME);
	strToCompanyAddress.LoadString(IDS_CLIENT_ADDRESS);
	strLicenceInfo.Format(strLicenceInfoFmt, strToCompany, strToCompanyAddress);

	if(m_objCmdLineInfo.IsLicenseCheckRequired())
	{
		char* pszLicenseInfoForUser;
	
		m_pAppProtector->Initialize(&pszLicenseInfoForUser);
		if(0L == pszLicenseInfoForUser)
		{
			AfxMessageBox("License Info Invalid, About box can not be shown !", MB_ICONSTOP);
			EndDialog(IDCANCEL);
			return FALSE;
		}

		strLicenseInfoForUser = "";
		if(0L != pszLicenseInfoForUser)
		{
			strLicenseInfoForUser = pszLicenseInfoForUser;
		}
		delete []pszLicenseInfoForUser;
	}
	else
	{
		strLicenseInfoForUser.Format("This application is running for \"Internal Usage\" ONLY!");
	}
	// TODO: Add extra initialization here
	strTmp.Format("%s\r\nProtection Process Debugging: %s\r\nReset Licensing Info: %s", strLicenseInfoForUser, (m_objCmdLineInfo.DebugProtectionProcess()) ? "ON":"OFF", (m_objCmdLineInfo.IsResetRequested()) ? "ON":"OFF");
	GetDlgItem(IDC_EDIT_DISP_INSTALL_INFO)->SetWindowText(strTmp);

	GetDlgItem(IDC_EDIT_DISP_INSTALL_INFO)->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(IDC_STATIC_LICENSE)->SetWindowText(strLicenceInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CAboutDlg::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	if((MK_CONTROL & nFlags) && (MK_SHIFT & nFlags))
	{
		if(IDYES == AfxMessageBox("License info is about to be removed. Are you sure you want to reset License Info ?", MB_YESNO|MB_ICONQUESTION))
		{
			if(m_pAppProtector->ResetLicenseRelatedData(CLicenseManager::LicenseIssuer, true))
			{
				AfxMessageBox("License info has been removed. Please restart the application", MB_ICONINFORMATION);
				PostMessage(WM_CLOSE);
				GetParent()->PostMessage(WM_CLOSE);
				return;
			}
		}

		/*
		if(GetDlgItem(IDC_EDIT_DISP_INSTALL_INFO)-> IsWindowVisible())
			GetDlgItem(IDC_EDIT_DISP_INSTALL_INFO)->ShowWindow(SW_HIDE);
		else
			GetDlgItem(IDC_EDIT_DISP_INSTALL_INFO)->ShowWindow(SW_SHOW);
		*/
	}
	
	CDialog::OnRButtonDblClk(nFlags, point);
}
void CAboutDlg::OnDeveloper()
{
}
