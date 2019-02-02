#include "stdafx.h"
#include "LicenseIssuer.h"
#include "ProtectionInterface.h"
#include "LoginDlg.h"

//
// CProtectionInterface
/////////////////////////////////////////////////////////////////////////////
bool CProtectionInterface::GetLicenseKeyFromUser(char*& pszKey) const
{
	CLoginDlg Dlg;
	
	if(IDOK != Dlg.DoModal())
	{
		return false;
	}
	const char* pszLicenseKey = Dlg.GetLicenseKey();
	pszKey = new char[strlen(pszLicenseKey) + 1];
	strcpy(pszKey, pszLicenseKey);

	return true;
}