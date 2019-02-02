// LicenseIssuerDlg.h : header file
//

#if !defined(AFX_LICENSEISSUERDLG_H__C5550289_5B36_4BD5_9C4C_48C4330380E9__INCLUDED_)
#define AFX_LICENSEISSUERDLG_H__C5550289_5B36_4BD5_9C4C_48C4330380E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CApplcationProtector;
class CProtectionInterface;

/////////////////////////////////////////////////////////////////////////////
// CLicenseIssuerDlg dialog

class CLicenseIssuerDlg : public CDialog
{
// Construction
public:
	CLicenseIssuerDlg(const CLicenseIssuerCmdLineInfo& objCmdLineInfo, CWnd* pParent = NULL);	// standard constructor
	~CLicenseIssuerDlg();
// Dialog Data
	//{{AFX_DATA(CLicenseIssuerDlg)
	enum { IDD = IDD_LICENSEISSUER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLicenseIssuerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLicenseIssuerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CApplcationProtector*				m_pApplcationProtector;
	CProtectionInterface*				m_pProtectionInterface;
	const CLicenseIssuerCmdLineInfo&	m_objCmdLineInfo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LICENSEISSUERDLG_H__C5550289_5B36_4BD5_9C4C_48C4330380E9__INCLUDED_)
