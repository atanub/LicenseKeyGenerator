// ProtectApp.cpp : implementation file
//

#include "stdafx.h"
#include "ProtectionDebugging.h"

//
// CProtectionDebugging
/////////////////////////////////////////////////////////////////////////////
CProtectionDebugging* CProtectionDebugging::m_pObjLogUtil = 0L;
CProtectionDebugging::CProtectionDebugging(const char* pszFile, const bool bLoggingEnabled/* = false*/) : m_bLoggingEnabled(bLoggingEnabled)
{
	m_pszLogFile = new char[strlen(pszFile) + 1];
	strcpy(m_pszLogFile, pszFile);
	m_pFileStream = 0L;

	m_pFileStream = fopen(m_pszLogFile, "at");
}
CProtectionDebugging::~CProtectionDebugging()
{
	if(0L != m_pszLogFile)
		delete []m_pszLogFile;

	if(0L != m_pFileStream)
	{
		fclose(m_pFileStream);
	}
}
void CProtectionDebugging::StartDebugLog(const char* pszFile, const bool bLoggingEnabled/* = false*/)
{
	if(0L != m_pObjLogUtil)
	{
		EndDebugLog();
		Log("Logging Ended - Terminated Abnormally", true);
	}
	m_pObjLogUtil = new CProtectionDebugging(pszFile, bLoggingEnabled);
	Log("Logging Started ***", true);
}
void CProtectionDebugging::EndDebugLog()
{
	if(0L != m_pObjLogUtil)
	{
		Log("Logging Ended");
		delete m_pObjLogUtil;
		m_pObjLogUtil = 0L;
	}
}
void CProtectionDebugging::Log(const char* pszText, bool bIsLoggingJustStarted)
{
	if(0L == m_pObjLogUtil)
	{
		TRACE("\nFATAL ERROR: StartDebugLog() MUST be called prior to call this function!");
		return;
	}
	if(!m_pObjLogUtil->m_bLoggingEnabled)
		return;
	{//Log
		time_t timeToday;
		char szTime[128], szDate[128];

		time(&timeToday);
		_strtime( szTime );
		_strdate( szDate );

		if(bIsLoggingJustStarted)
			fprintf(m_pObjLogUtil->m_pFileStream, "\n");

		fprintf(m_pObjLogUtil->m_pFileStream, "[%s %s] Protection Log: %s\n", szTime, szDate, pszText);
		fflush(m_pObjLogUtil->m_pFileStream);
	}
}
