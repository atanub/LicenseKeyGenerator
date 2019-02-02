#if !defined(AFX_PROTECTAPP_H__A40DFD0F_DC9E_11D3_8811_0080C8047EF6__INCLUDED_)
#define AFX_PROTECTAPP_H__A40DFD0F_DC9E_11D3_8811_0080C8047EF6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ApplcationProtector.h : header file
//
#include "LicenseManager.h"

typedef struct tagPROTECTIONLOGDATA
{
	long	iTimeOfAccess;
	int		iApplicationType;
} PROTECTIONLOGDATA;

//
// CProtectionInterfaceBase
/////////////////////////////////////////////////////////////////////////////
class CProtectionInterfaceBase
{
protected:
	CProtectionInterfaceBase() {;}
public:
	~CProtectionInterfaceBase() {;}
public:
	virtual bool GetLicenseKeyFromUser(char*&) const = 0;

};

//
// CApplcationProtector
/////////////////////////////////////////////////////////////////////////////
class CApplcationProtector  
{
public:
	typedef enum {Success, InvalidRegistry, UpdRegistryFailed, InvalidLicense, NoLogFileExists, InvalidLogFile, UpdLogFailed, UserCancelledActivation, Unknown} FaultType;
// Construction
public:
	CApplcationProtector(CLicenseManager::AppType eAppType, const CProtectionInterfaceBase*);
	virtual ~CApplcationProtector();

public:
	FaultType Initialize(char** pszLicenseInfoForUser = 0L);
	bool ResetLicenseRelatedData(CLicenseManager::AppType eAppType, const bool bCalledByUser = false);//To be called when user changed system date....

public:
	static const char*	GetDebugLogFileName(CLicenseManager::AppType eAppType);
protected:
	static bool			ReadLogFile(const char* pszLogFile, unsigned long& iNoOfTimesAccessed, PROTECTIONLOGDATA** pRec);
	static bool			ReadLicenseKeyFromRegistry(char*& pszLicenseKey, CLicenseManager::AppType eAppType);
	static bool			WriteLicenseKeyInRegistry(const char* pszLicenseKey, CLicenseManager::AppType eAppType);
	static const char*	GetLicenseTag(CLicenseManager::AppType eAppType);
	static const char*	GetLogFileName(CLicenseManager::AppType eAppType);

protected:
	bool UpdateLogFile(bool bIsInitializing = false);
	const PROTECTIONLOGDATA* GetLatestLogEntry() const;

protected:
	const CLicenseManager::AppType	m_eAppType;
	const CProtectionInterfaceBase*	m_pUIface;
	char*							m_pszLicenseKey;
	char							m_pszLogFilePath[MAX_PATH];
	//Log file Info...
	unsigned long			m_iNoOfTimesAccessed;
	PROTECTIONLOGDATA*		m_pRecLoggedData;
	//
	bool					m_bCanRunApp;
private :
	static const char*						m_pszDebugLogFileNames[];
	static const char*						m_pszLogFileNames[];
	static const char*						m_pszRegistryRoot;
	static const char*						m_pszLicenseKeyTags[];
	static const CLicenseManager::AppType	m_eAppTypesAll[];
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROTECTAPP_H__A40DFD0F_DC9E_11D3_8811_0080C8047EF6__INCLUDED_)
