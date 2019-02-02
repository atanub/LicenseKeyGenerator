// LicenseKeyViewer.cpp : implementation file
//

#include "stdafx.h"
#include "LicenseIssuer.h"
#include "LicenseKeyViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLicenseKeyViewer dialog


CLicenseKeyViewer::CLicenseKeyViewer(const char* pszLicenseKey, CWnd* pParent /*=NULL*/)
	: CDialog(CLicenseKeyViewer::IDD, pParent), m_pszLicenseKey(pszLicenseKey)
{
	//{{AFX_DATA_INIT(CLicenseKeyViewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLicenseKeyViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLicenseKeyViewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLicenseKeyViewer, CDialog)
	//{{AFX_MSG_MAP(CLicenseKeyViewer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLicenseKeyViewer message handlers

void CLicenseKeyViewer::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CLicenseKeyViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT1)->SetWindowText(m_pszLicenseKey);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
