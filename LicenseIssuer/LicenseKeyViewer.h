#if !defined(AFX_LICENSEKEYVIEWER_H__575B4B18_D536_4ABB_9316_804B88252D23__INCLUDED_)
#define AFX_LICENSEKEYVIEWER_H__575B4B18_D536_4ABB_9316_804B88252D23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LicenseKeyViewer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLicenseKeyViewer dialog

class CLicenseKeyViewer : public CDialog
{
// Construction
public:
	CLicenseKeyViewer(const char*, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLicenseKeyViewer)
	enum { IDD = IDD_LICENSE_KEY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLicenseKeyViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLicenseKeyViewer)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	const char* m_pszLicenseKey;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LICENSEKEYVIEWER_H__575B4B18_D536_4ABB_9316_804B88252D23__INCLUDED_)
