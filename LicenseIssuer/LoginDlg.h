#if !defined(AFX_LOGINDLG_H__AB511564_BA9C_421A_A5B4_0A6C613923A1__INCLUDED_)
#define AFX_LOGINDLG_H__AB511564_BA9C_421A_A5B4_0A6C613923A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

class CLoginDlg : public CDialog
{
// Construction
public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
public:
	const char* GetLicenseKey() const { return m_strLicenseKey;}

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_LICENSE_KEY_SELECTION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
	virtual void OnOK();
	afx_msg void OnButtonFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CString m_strLicenseKey;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__AB511564_BA9C_421A_A5B4_0A6C613923A1__INCLUDED_)
