#if !defined(AFX_PASSWORDPROTECTIONDLG_H__83089767_3772_4826_9C86_569E03E2D588__INCLUDED_)
#define AFX_PASSWORDPROTECTIONDLG_H__83089767_3772_4826_9C86_569E03E2D588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasswordProtectionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPasswordProtectionDlg dialog

class CPasswordProtectionDlg : public CDialog
{
// Construction
public:
	CPasswordProtectionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPasswordProtectionDlg)
	enum { IDD = IDD_DIALOG_PASSWORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasswordProtectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasswordProtectionDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int		m_iNoOfTriesForPasswordInput;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORDPROTECTIONDLG_H__83089767_3772_4826_9C86_569E03E2D588__INCLUDED_)
