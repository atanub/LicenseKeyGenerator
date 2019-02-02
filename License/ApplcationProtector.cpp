// ProtectApp.cpp : implementation file
//

#include "stdafx.h"
#include "ApplcationProtector.h"
#include "ProtectionDebugging.h"

#define HKEY_LOCAL_MACHINE_CUSTOM HKEY_CURRENT_USER


/////////////////////////////////////////////////////////////////////////////
// CApplcationProtector
const char* CApplcationProtector::m_pszRegistryRoot						= "Software\\Atanu Banik\\ARX Applications";
const char* CApplcationProtector::m_pszLogFileNames[]					= {"LicenseKeyIssuer.dlx", "LicenseKeyContourARX.dlx", "LicenseKeySurveyUtilARX.dlx"};
const char* CApplcationProtector::m_pszDebugLogFileNames[]				= {"LicenseKeyIssuer.txt", "LicenseKeyContourARX.txt", "LicenseKeySurveyUtilARX.txt"};
const char* CApplcationProtector::m_pszLicenseKeyTags[]					= {"LicenseKeyIssuer", "LicenseKeyContourARX", "LicenseKeySurveyUtilARX"};
const CLicenseManager::AppType	CApplcationProtector::m_eAppTypesAll[]	= {CLicenseManager::AppType::LicenseIssuer, CLicenseManager::AppType::ContourARX, CLicenseManager::AppType::SurveyUtilARX};


//
CApplcationProtector::CApplcationProtector(CLicenseManager::AppType eAppType, const CProtectionInterfaceBase* pUIface) : m_pUIface(pUIface), m_eAppType(eAppType)
{
	char szSysDir[MAX_PATH];

	GetSystemDirectory(szSysDir, MAX_PATH);
	sprintf(m_pszLogFilePath, "%s\\%s", szSysDir, GetLogFileName(eAppType));

	m_iNoOfTimesAccessed= 0;
	m_pRecLoggedData = 0L;
	m_bCanRunApp = false;
	m_pszLicenseKey = 0L;
}
//
CApplcationProtector::~CApplcationProtector()
{
	if(m_bCanRunApp)
	{
		UpdateLogFile();
	}
	delete []m_pRecLoggedData;
	delete []m_pszLicenseKey;
}

bool CApplcationProtector::UpdateLogFile(bool bIsInitializing/* = false*/)
{
	time_t timeToday;
	struct tm tmToday;
	bool bAppendToLog;

	bAppendToLog = true;
	time(&timeToday);
	tmToday = *(localtime(&timeToday));
	const PROTECTIONLOGDATA* pRecLatestData = GetLatestLogEntry();
	if(0L != pRecLatestData)
	{//Check if the last logged date is same as today's date
		struct tm tmLastAccessed;
		
		CProtectionDebugging::Log("Found entry in log file");
		tmLastAccessed = *(localtime(&(pRecLatestData->iTimeOfAccess)));
		if((tmLastAccessed.tm_mday == tmToday.tm_mday) && (tmLastAccessed.tm_mon == tmToday.tm_mon) && (tmLastAccessed.tm_year == tmToday.tm_year))
		{
			bAppendToLog = false;//Don't write multiple log entries for a single day
			CProtectionDebugging::Log("Log file not updated to prevent multiple log entries for a single day");
		}
		if(pRecLatestData->iTimeOfAccess > timeToday)
		{
			bAppendToLog = false;//User has changed system date & time
			CProtectionDebugging::Log("ERROR: User has changed system date & time!");
			CProtectionDebugging::Log("Log file not updated!");
		}

		if(bIsInitializing)
			bAppendToLog = true;
	}
	if(bAppendToLog && (0L != m_pRecLoggedData))
	{
		FILE* hFile;

		CProtectionDebugging::Log("Opening Log file...");
		hFile = fopen(m_pszLogFilePath, "wb");
		if(0L == hFile)
		{
			TRACE("\nFATAL ERROR: Failed to write Log file \"%s\" !", m_pszLogFilePath);
			CProtectionDebugging::Log("ERROR: Failed to open Log file!");
		}
		else
		{
			PROTECTIONLOGDATA Rec;
			char szTemp[_MAX_PATH];

			time(&(Rec.iTimeOfAccess));
			Rec.iApplicationType = (int)m_eAppType;
			//
			CProtectionDebugging::Log("Log file opened");
			if(!bIsInitializing)
			{
				m_iNoOfTimesAccessed++;
			}
			//
			fwrite(&m_iNoOfTimesAccessed, sizeof(m_iNoOfTimesAccessed), 1, hFile);
			if(!bIsInitializing)
			{
				fwrite(m_pRecLoggedData, sizeof(PROTECTIONLOGDATA), (m_iNoOfTimesAccessed - 1), hFile);
				fwrite(&Rec, sizeof(PROTECTIONLOGDATA), 1, hFile);
				
				sprintf(szTemp, "Application accessed %d# of times/days", (m_iNoOfTimesAccessed - 1));
				CProtectionDebugging::Log(szTemp);
			}
			else
			{
				fwrite(m_pRecLoggedData, sizeof(PROTECTIONLOGDATA), m_iNoOfTimesAccessed, hFile);
				sprintf(szTemp, "Application accessed %d# of times/days", m_iNoOfTimesAccessed);
				CProtectionDebugging::Log(szTemp);
			}
			fclose(hFile);
			CProtectionDebugging::Log("Log file closed");
		}
	}
	return bAppendToLog;
}
//
bool CApplcationProtector::ReadLogFile(const char* pszLogFile, unsigned long& iNoOfTimesAccessed, PROTECTIONLOGDATA** pRec)
{
	*pRec = 0L;
	{//
		FILE* hFile;

		hFile = fopen(pszLogFile, "rb");
		if(0L == hFile)
			return false;
		
		fread(&iNoOfTimesAccessed, sizeof(iNoOfTimesAccessed), 1, hFile);
		//
		*pRec = new PROTECTIONLOGDATA[iNoOfTimesAccessed];
		fread(*pRec, sizeof(PROTECTIONLOGDATA), iNoOfTimesAccessed, hFile);
		//
		fclose(hFile);
		return true;
	}
}

//Checks whether the license key supplied to this object is valid or not ...
CApplcationProtector::FaultType CApplcationProtector::Initialize(char** pszLicenseInfoForUser/* = 0L*/)
{
	bool bLicenseJustEntered;
	char szTemp[255];

	m_bCanRunApp = false;
	
	//
	if(0L != pszLicenseInfoForUser)
	{
		*pszLicenseInfoForUser = 0L;
		CProtectionDebugging::Log("Checking license info ...");
		CProtectionDebugging::Log(*pszLicenseInfoForUser);
	}
	//
	bLicenseJustEntered = false;
	//
	delete []m_pszLicenseKey;
	m_pszLicenseKey = 0L;
	//
	CProtectionDebugging::Log("Reading License Key From System Registry ...");
	if(!ReadLicenseKeyFromRegistry(m_pszLicenseKey, m_eAppType))
	{//Registry not set.......Ask user for License Key
		CProtectionDebugging::Log("License key not registered");
		if(!m_pUIface->GetLicenseKeyFromUser(m_pszLicenseKey))
		{
			return FaultType::UserCancelledActivation;//User did not supplied the key
		}
		CProtectionDebugging::Log("Just now user entered the license key");
		bLicenseJustEntered = true;
	}
	CProtectionDebugging::Log("Initializing 'License Manager' object");
	CLicenseManager objManager(m_eAppType, 0, 0);
	{//Validate License Key...

		CProtectionDebugging::Log("'License Manager' object validating license key...");
		CProtectionDebugging::Log(m_pszLicenseKey);
		const bool bIsValidLicense = objManager.CheckValidity(m_pszLicenseKey, true/*pop message*/);
		
		if(!bIsValidLicense)
		{
			CProtectionDebugging::Log("Not a valid license key...License might be expired!");
			TRACE("\nLicense expired!");
			//MessageBox(0L, "License has been expired", "License Manager", MB_ICONSTOP);
			CProtectionDebugging::Log("Reseting license related data");
			ResetLicenseRelatedData(m_eAppType);
			return FaultType::InvalidLicense;//License Expired/Not matching.....
		}
		if(bLicenseJustEntered)
		{
			//Write License Key Info to registry.....
			CProtectionDebugging::Log("Writing license key in System Registry");
			if(!WriteLicenseKeyInRegistry(m_pszLicenseKey, m_eAppType))
			{
				CProtectionDebugging::Log("Failed to write license key to System Registry");
				return FaultType::UpdRegistryFailed;//Failed to write in registry....
			}
			CProtectionDebugging::Log("Successfully written license key to System Registry");
			if(0L != m_pRecLoggedData)
			{
				delete []m_pRecLoggedData;
				m_pRecLoggedData = 0L;
			}
			{//Write Log file to prevent the user to change system date & time...
				time_t timeToday;
				m_iNoOfTimesAccessed = 1;
				m_pRecLoggedData = new PROTECTIONLOGDATA[1];
				
				time(&timeToday);
				(m_pRecLoggedData + 0)->iTimeOfAccess = timeToday;
				(m_pRecLoggedData + 0)->iApplicationType = m_eAppType;
				
				sprintf(szTemp, "Creating Log file [%s] to prevent the user to change system date & time...", m_pszLogFilePath);
				CProtectionDebugging::Log(szTemp);
				CProtectionDebugging::Log("Writing 1st Log entry");
				UpdateLogFile(true);
			}
		}
	}
	{//Read the log file....To check whether user has changed system date/time after installing this software...
		delete []m_pRecLoggedData;
		m_pRecLoggedData = 0L;

		sprintf(szTemp, "Reading the log file [%s]....To check whether user has changed system date/time after installing this software...", m_pszLogFilePath);
		CProtectionDebugging::Log(szTemp);
		if(!ReadLogFile(m_pszLogFilePath, m_iNoOfTimesAccessed, &m_pRecLoggedData))
		{
			CProtectionDebugging::Log("ERROR: Log file not found!");
			TRACE("\nLog file \"%s\" not found!", m_pszLogFilePath);
			return FaultType::NoLogFileExists;//Log file not found!
		}
		CProtectionDebugging::Log("Extracting the latest record from the Log file");
		const PROTECTIONLOGDATA* pRecLatestData = GetLatestLogEntry();
		if(0L != pRecLatestData)
		{
			//To restrict the user not to change system date & time..........
			time_t timeToday;

			time(&timeToday);
			if(pRecLatestData->iTimeOfAccess > timeToday) 
			{
				MessageBox(0L, "FATAL ERROR: System date & time changed\nPlease contact developer immediately", "License Manager", MB_ICONSTOP);
				TRACE("\nFATAL ERROR: System date & time changed by user");
				CProtectionDebugging::Log("FATAL ERROR: System date & time changed by user");
				return FaultType::InvalidLogFile;//user has changed system date & time..........
			}
		}
	}
	if(0L != pszLicenseInfoForUser)
	{//
		objManager.GetLicenseInfoForUser(m_pszLicenseKey, pszLicenseInfoForUser);
	}

	m_bCanRunApp = true;
	CProtectionDebugging::Log("License Key validated...Application can run.");
	return FaultType::Success;
}

bool CApplcationProtector::ReadLicenseKeyFromRegistry(char*& pszLicenseKey, CLicenseManager::AppType eAppType)
{
	HKEY hKey;
	long lRet;
	DWORD cbData;
	char szAuthCodeRegDB[_MAX_PATH];

	pszLicenseKey = 0L;
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE_CUSTOM, m_pszRegistryRoot, 0, KEY_QUERY_VALUE, &hKey);
	if(lRet != ERROR_SUCCESS)
		return false; 

	//Read License Key
    cbData = sizeof(szAuthCodeRegDB);

	lRet = RegQueryValueEx(hKey, (LPCSTR)GetLicenseTag(eAppType), NULL, NULL, (unsigned char*)szAuthCodeRegDB, &cbData);
    if(lRet != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return false; 
    }
	szAuthCodeRegDB[cbData] = '\0';
	RegCloseKey(hKey);

	//Check 
	pszLicenseKey = new char[strlen(szAuthCodeRegDB) + 1];
	strcpy(pszLicenseKey, szAuthCodeRegDB);
	return true;
}
bool CApplcationProtector::WriteLicenseKeyInRegistry(const char* pszLicenseKey, CLicenseManager::AppType eAppType)
{
	HKEY hKeyRoot;
	long lRet;

	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE_CUSTOM, m_pszRegistryRoot, 0, KEY_QUERY_VALUE|KEY_CREATE_SUB_KEY|KEY_SET_VALUE, &hKeyRoot);
	if(lRet != ERROR_SUCCESS)
	{//Create Key.....
		DWORD dwDisposition;

		if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_LOCAL_MACHINE_CUSTOM, m_pszRegistryRoot, 0L, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0L, &hKeyRoot, &dwDisposition))
		{
			return false; 
		}
	}

	if(RegSetValueEx(hKeyRoot, GetLicenseTag(eAppType), 0, REG_SZ, (LPBYTE)(LPCTSTR)pszLicenseKey, strlen(pszLicenseKey)) == ERROR_SUCCESS)
	{
		RegCloseKey(hKeyRoot);
		return true;
	}
	RegCloseKey(hKeyRoot);
	return false;
}

bool CApplcationProtector::ResetLicenseRelatedData(CLicenseManager::AppType eAppType, const bool bCalledByUser/* = false*/)
{
	HKEY hKeyRoot;
	long lRet;
	bool bRetVal;
	
	bRetVal = false;
	CProtectionDebugging::Log("Opening System Registry Key...");
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE_CUSTOM, m_pszRegistryRoot, 0, KEY_QUERY_VALUE|KEY_CREATE_SUB_KEY|KEY_SET_VALUE, &hKeyRoot);
	if(lRet != ERROR_SUCCESS)
	{
		CProtectionDebugging::Log("Failed to open System Registry key !");
		return false; 
	}

	CProtectionDebugging::Log("Deleting System Registry Key...");
	if(RegDeleteValue(hKeyRoot, GetLicenseTag(eAppType)) == ERROR_SUCCESS)
	{
		bRetVal = true;
		CProtectionDebugging::Log("Registry Key deletion succeded");
	}
	CProtectionDebugging::Log("Closing System Registry Key...");
	RegCloseKey(hKeyRoot);
	if(bCalledByUser)
	{
		CProtectionDebugging::Log("User asked for 'Reset'...the log file not deleted");
		return bRetVal;//Don't delete the log file !!!
	}
	CProtectionDebugging::Log("Deleting the log file");
	bRetVal = bRetVal && DeleteFile(m_pszLogFilePath);
	CProtectionDebugging::Log((bRetVal) ? "Log file deletion  succeded":"Log file deletion FAILED");
	return bRetVal;
}
const char* CApplcationProtector::GetLicenseTag(CLicenseManager::AppType eAppType)
{
	int i;

	for(i = 0; i < sizeof(m_pszLicenseKeyTags)/sizeof(char*); i++)
	{
		if(eAppType == m_eAppTypesAll[i])
			return m_pszLicenseKeyTags[i];
	}
	return 0L;
}
const char* CApplcationProtector::GetLogFileName(CLicenseManager::AppType eAppType)
{
	int i;

	for(i = 0; i < sizeof(m_pszLicenseKeyTags)/sizeof(char*); i++)
	{
		if(eAppType == m_eAppTypesAll[i])
			return m_pszLogFileNames[i];
	}
	return 0L;
}
const char* CApplcationProtector::GetDebugLogFileName(CLicenseManager::AppType eAppType)
{
	int i;

	for(i = 0; i < sizeof(m_pszLicenseKeyTags)/sizeof(char*); i++)
	{
		if(eAppType == m_eAppTypesAll[i])
			return m_pszDebugLogFileNames[i];
	}
	return 0L;
}
const PROTECTIONLOGDATA* CApplcationProtector::GetLatestLogEntry() const
{
	if((0L == m_pRecLoggedData) || (m_iNoOfTimesAccessed < 1))
		return 0L;

	unsigned int i;
	for(i = 0; i < m_iNoOfTimesAccessed; i++)
	{//Log
		char szTemp[_MAX_PATH], szDate[128];
		
		strcpy(szDate, ctime(&((m_pRecLoggedData + i)->iTimeOfAccess)) );
		szDate[strlen(szDate) - 1] = '\0';

		sprintf(szTemp, "--- Application access #%5d of %5d, Date: [%s]", (i + 1), m_iNoOfTimesAccessed, szDate);
		CProtectionDebugging::Log(szTemp);
	}

	return (m_pRecLoggedData + m_iNoOfTimesAccessed - 1);
}
