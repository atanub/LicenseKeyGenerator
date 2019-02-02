// LicenseIssuerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LicenseIssuer.h"
#include "LicenseIssuerDlg.h"
#include "LicenseKeyViewer.h"
#include "ProtectionInterface.h"
#include "AboutDlg.h"
#include "PasswordProtectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLicenseIssuerDlg dialog
CLicenseIssuerDlg::CLicenseIssuerDlg(const CLicenseIssuerCmdLineInfo& objCmdLineInfo, CWnd* pParent /*=NULL*/)
	: CDialog(CLicenseIssuerDlg::IDD, pParent), m_objCmdLineInfo(objCmdLineInfo)
{
	//{{AFX_DATA_INIT(CLicenseIssuerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pProtectionInterface = new CProtectionInterface();
	m_pApplcationProtector = new CApplcationProtector(CLicenseManager::AppType::LicenseIssuer, m_pProtectionInterface);
}
CLicenseIssuerDlg::~CLicenseIssuerDlg()
{
	delete m_pProtectionInterface;
	delete m_pApplcationProtector;
}

void CLicenseIssuerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLicenseIssuerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLicenseIssuerDlg, CDialog)
	//{{AFX_MSG_MAP(CLicenseIssuerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLicenseIssuerDlg message handlers

BOOL CLicenseIssuerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	if(m_objCmdLineInfo.IsLicenseCheckRequired())
	{//Check for License........
		{//
			CPasswordProtectionDlg Dlg;

			if(Dlg.DoModal() != IDOK)
			{
				EndDialog(IDCANCEL);
				return TRUE;
			}
		}
		{//22-Sep-2006 ---- Disable License Key Checking Mechanism 
			//
			//CProtectionDebugging::Log("Initializing Protector object");
			//if(!m_pApplcationProtector->Initialize())
			//{
			//	CProtectionDebugging::Log("Protector object initialization failed !");
			//	EndDialog(IDCANCEL);
			//	return TRUE;
			//}
			//CProtectionDebugging::Log("Protector object initialization succeded");
		}
	}
	{//
		const char* pszArrayApp[] = {"License Issuer", "Contour ARX", "Survey Utilities ARX"};
		const int iTypes[] = {CLicenseManager::AppType::LicenseIssuer, CLicenseManager::AppType::ContourARX, CLicenseManager::AppType::SurveyUtilARX};
		int i, iIndex;
		CComboBox* pCombo;
		CString strTmp;

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
		for(i = 0; i < sizeof(iTypes)/sizeof(int); i++)
		{
			if((iTypes[i] == CLicenseManager::AppType::LicenseIssuer) && (m_objCmdLineInfo.IsLicenseCheckRequired()))
			{
				continue;
			}
			iIndex = pCombo->AddString(pszArrayApp[i]);
			pCombo->SetItemData(iIndex, iTypes[i]);
		}
		const unsigned long	iSlNo = CLicenseManager::GetVolumeSerialNo('C');
		strTmp.Format("%x", iSlNo);
		GetDlgItem(IDC_EDIT1)->SetWindowText(strTmp);
		GetDlgItem(IDC_EDIT2)->SetWindowText("15");
	}
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetLimitText(3);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLicenseIssuerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout(m_objCmdLineInfo, m_pApplcationProtector);
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLicenseIssuerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLicenseIssuerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLicenseIssuerDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strVolSlNoInHex, strValidity, strMsg;
	int i;
	CLicenseManager::AppType eType;
	
	{//
		i = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
		if(i < 0)
		{
			AfxMessageBox("Invalid Application selected", MB_ICONSTOP);
			GetDlgItem(IDC_COMBO1)->SetFocus();
			return;
		}
		eType = (CLicenseManager::AppType) ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetItemData(i);
	}
	GetDlgItem(IDC_EDIT1)->GetWindowText(strVolSlNoInHex);
	const unsigned int iHDDIdentifier = CLicenseManager::ConvertVolumeSerialNo(strVolSlNoInHex);
	if(iHDDIdentifier <= 0)
	{
		AfxMessageBox("Invalid Volume Serial Number provided", MB_ICONSTOP);
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetSel(0, -1);
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetFocus();
		return;
	}
	GetDlgItem(IDC_EDIT2)->GetWindowText(strValidity);
	const int iValidity = atoi(strValidity);
	if(CLicenseManager::AppType::LicenseIssuer == eType)
	{
		if(iValidity > 999)
		{
			AfxMessageBox("Invalid License Validity Period entered", MB_ICONSTOP);
			((CEdit*)GetDlgItem(IDC_EDIT2))->SetSel(0, -1);
			((CEdit*)GetDlgItem(IDC_EDIT2))->SetFocus();
			return;
		}
	}
	else
	{
		if((iValidity <= 0) || (iValidity > 180))
		{
			AfxMessageBox("Invalid License Validity Period entered. Maximum validity period is 180 Days.", MB_ICONSTOP);
			((CEdit*)GetDlgItem(IDC_EDIT2))->SetSel(0, -1);
			((CEdit*)GetDlgItem(IDC_EDIT2))->SetFocus();
			return;
		}
	}

	{//
		CLicenseManager objManager(eType, iHDDIdentifier, iValidity);
		char* pszKey;

		pszKey = objManager.GenerateLicenseKey();
		if(0L != pszKey)
		{
			CLicenseKeyViewer DlgViewer(pszKey, this);
			
			if(IDOK == DlgViewer.DoModal())
			{//
				DWORD dwFlags;
				const CString strConstFOpenFilter = CString("License Key Files (*.TXT)|*.TXT||");

				dwFlags = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;
				CFileDialog Dlg(FALSE, "txt", 0L, dwFlags, strConstFOpenFilter);
				if(Dlg.DoModal() == IDOK)
				{
					FILE* hFile;

					hFile = fopen(Dlg.GetPathName(), "w");
					if(0L == hFile)
					{
						strMsg.Format("FATAL ERROR: Failed to write file \"%s\" !", Dlg.GetPathName());
						AfxMessageBox(strMsg, MB_ICONSTOP);
					}
					else
					{
						strMsg.Format("License key successfully saved in file: \"%s\".", Dlg.GetPathName());
						fprintf(hFile, "%s", pszKey);
						fclose(hFile);
						AfxMessageBox(strMsg, MB_ICONINFORMATION);
					}
				}
			}
		}
		delete []pszKey;
	}
	//CDialog::OnOK();
}
