// LicenseManager.h: interface for the CLicenseManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LICENSEMANAGER_H__E19942DC_9EAE_413D_9173_87749B9F710B__INCLUDED_)
#define AFX_LICENSEMANAGER_H__E19942DC_9EAE_413D_9173_87749B9F710B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLicenseKeyGenerator;

class CLicenseManager  
{
public:
	typedef enum {LicenseIssuer = 0, ContourARX = 1, SurveyUtilARX = 2} AppType;
public:
	CLicenseManager(AppType eAppType, const unsigned int iDeviceID, const int iValidityInDays);
	virtual ~CLicenseManager();
public:
	char*	GenerateLicenseKey() const;
	bool	CheckValidity(const char*, bool bPopMessage = true) const;
	bool	ShowValidityInfoPriorToExpiry(const char*, int iShowWarningBefore = 15) const;
	bool	GetLicenseInfoForUser(const char* pszKey, char** pszLicenseInfoForUser) const;

	static unsigned long	ConvertVolumeSerialNo(const char* pszSlNoInHex);
	static bool				ReadLiceseInfo(const char* pszFile, char** pszLicenseInfo);
	static unsigned long	GetVolumeSerialNo(const char chDrive);
protected:
	unsigned long			m_iDeviceID;
	CLicenseKeyGenerator*	m_pLicenseGenerator;

};

#endif // !defined(AFX_LICENSEMANAGER_H__E19942DC_9EAE_413D_9173_87749B9F710B__INCLUDED_)
