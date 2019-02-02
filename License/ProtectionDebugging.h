#if !defined(_PROTECTIONDEBUGGING_H)
#define _PROTECTIONDEBUGGING_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProtectionDebugging.h : header file
//
#include "LicenseManager.h"

//
// CProtectionDebugging
/////////////////////////////////////////////////////////////////////////////
class CProtectionDebugging
{
protected:
	CProtectionDebugging(const char* pszFile, const bool bLoggingEnabled = false);
public:
	~CProtectionDebugging();
public:
	static void StartDebugLog(const char* pszFile, const bool bLoggingEnabled = false);
	static void EndDebugLog();
	static void Log(const char* pszText, bool bIsLoggingJustStarted = false);
protected:
	char*			m_pszLogFile;
	FILE*			m_pFileStream;
	const bool		m_bLoggingEnabled;
protected:
	static	CProtectionDebugging* m_pObjLogUtil;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_PROTECTIONDEBUGGING_H)
