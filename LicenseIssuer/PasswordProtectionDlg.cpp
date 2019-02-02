// PasswordProtectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "licenseissuer.h"
#include "PasswordProtectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswordProtectionDlg dialog


CPasswordProtectionDlg::CPasswordProtectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordProtectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswordProtectionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iNoOfTriesForPasswordInput = 0;
}


void CPasswordProtectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordProtectionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswordProtectionDlg, CDialog)
	//{{AFX_MSG_MAP(CPasswordProtectionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordProtectionDlg message handlers

void CPasswordProtectionDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strPassword, strInput;
	
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(strInput);
	VERIFY(strPassword.LoadString(IDS_STARTUP_PASSWORD));
	
	if(strPassword.Compare(strInput) != 0)
	{
		m_iNoOfTriesForPasswordInput++;
		
		if(m_iNoOfTriesForPasswordInput >= 3)
		{
			AfxMessageBox("Invalid Password Input! Exhausted maximum # of attempts.\nExiting from Application", MB_ICONSTOP);
			EndDialog(IDCANCEL);
			return;
		}
		AfxMessageBox("Invalid Password Input!", MB_ICONSTOP);
		return;
	}
	CDialog::OnOK();
}
