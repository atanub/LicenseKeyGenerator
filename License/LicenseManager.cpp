// LicenseManager.cpp: implementation of the CLicenseManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LicenseManager.h"
#include "LicenseKeyGenerator.h"
#include "ProtectionDebugging.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLicenseManager::CLicenseManager(AppType eAppType, const unsigned int iDeviceID, const int iValidityInDays)
{
	m_iDeviceID = (iDeviceID <= 0) ? GetVolumeSerialNo('C') : iDeviceID;
	m_pLicenseGenerator = 0L;
	if(m_iDeviceID > 0)
	{
		switch(eAppType)
		{
		case LicenseIssuer:
			m_pLicenseGenerator = new CLicenseKeyGeneratorManager(m_iDeviceID, iValidityInDays);
			break;
		case ContourARX:
			m_pLicenseGenerator = new CLicenseKeyGeneratorContour(m_iDeviceID, iValidityInDays);
			break;
		case SurveyUtilARX:
			m_pLicenseGenerator = new CLicenseKeyGeneratorSurvey(m_iDeviceID, iValidityInDays);
			break;
		default:
			break;
		}
	}
}

CLicenseManager::~CLicenseManager()
{
	delete m_pLicenseGenerator;
}
char* CLicenseManager::GenerateLicenseKey() const
{
	if(m_iDeviceID <= 0)
		return 0L;

	return m_pLicenseGenerator->GenerateID();
}
bool CLicenseManager::CheckValidity(const char* pszKey, bool bPopMessage /*= true*/) const
{
	if(m_iDeviceID <= 0)
		return false;
	
	char* pszFeedback;
	const bool bIsValid = m_pLicenseGenerator->IsValidID(pszKey, &pszFeedback);
	if(!bIsValid && (0L != pszFeedback))
	{
		MessageBox(0L, pszFeedback, "License Manager", MB_ICONSTOP);
		CProtectionDebugging::Log(pszFeedback);
		delete []pszFeedback;
		return false;
	}
	delete []pszFeedback;

	if(bIsValid && bPopMessage)
	{
		ShowValidityInfoPriorToExpiry(pszKey);
	}
	return bIsValid;
}
bool CLicenseManager::ShowValidityInfoPriorToExpiry(const char* pszKey, int iShowWarningBefore/* = 15*/) const
{
	if(m_iDeviceID <= 0)
		return false;
	
	time_t timeToday, timeLicenseIssued;
	int iValidity;

	if(!m_pLicenseGenerator->ExtractValidityInfo(pszKey, timeLicenseIssued, iValidity))
	{
		MessageBox(0L, "License expired, Please renew as soon as possible.", "License Manager", MB_ICONSTOP);
		return false;
	}
	if(iValidity < 0)
	{//License will never expire !
		CProtectionDebugging::Log("License Type: \"License will NEVER expire\"");
		return true;
	}

	time(&timeToday);
	const time_t timeExpiry = timeLicenseIssued + iValidity * 24 * 3600;
	const long iNoOfDaysToExpire = (timeExpiry - timeToday) / (24 * 3600);

	if(iNoOfDaysToExpire <= iShowWarningBefore)
	{
		char szMsg[_MAX_PATH];

		sprintf(szMsg, "License about to be expired, %d more days remaining\r\nPlease renew as soon as possible", iNoOfDaysToExpire);
		MessageBox(0L, szMsg, "License Manager", MB_ICONSTOP);
		
		sprintf(szMsg, "Warning: License is about to be expired, %d more days remaining", iNoOfDaysToExpire);
		CProtectionDebugging::Log(szMsg);
	}
	return true;
}
bool CLicenseManager::GetLicenseInfoForUser(const char* pszKey, char** pszLicenseInfoForUser) const
{
	*pszLicenseInfoForUser = 0L;
	if(m_iDeviceID <= 0)
		return false;
	
	time_t timeToday, timeLicenseIssued;
	int iValidity;

	*pszLicenseInfoForUser = new char[512];

	if(!m_pLicenseGenerator->ExtractValidityInfo(pszKey, timeLicenseIssued, iValidity))
	{
		sprintf(*pszLicenseInfoForUser, "License expired, renew as soon as possible");
		return false;
	}
	time(&timeToday);
	const time_t timeExpiry = timeLicenseIssued + iValidity * 24 * 3600;
	const long iNoOfDaysToExpire = (timeExpiry - timeToday) / (24 * 3600);
	char szDate[64];
	
	strcpy(szDate, asctime(localtime(&timeLicenseIssued)));
	if(iValidity < 0)
	{//License will never expire!
		sprintf(*pszLicenseInfoForUser, "License Issued on:%s\r\nThis License will NEVER expire", szDate);
		return true;
	}
	sprintf(*pszLicenseInfoForUser, "License Issued on:%s\r\nValidity: %d days\r\nExpire on: %s", szDate, iValidity, asctime(localtime(&timeExpiry)));
	return true;
}

unsigned long CLicenseManager::GetVolumeSerialNo(const char chDrive)
{
	char szDrives[_MAX_PATH];
	char* pszDrives;
	int i;
	
	if(0 == GetLogicalDriveStrings(_MAX_PATH, szDrives))
	{
		//assert(false);
		return 0;
	}
	for(i = 0, pszDrives = szDrives; ; i += 4, pszDrives = pszDrives + 4)
	{
		if((i > 0) && (szDrives[i] == '\0') && (szDrives[i - 1] == '\0'))
		{
			break;
		}
		if(tolower(chDrive) == tolower(*(pszDrives + 0)))
		{//
			unsigned long iVolumeSerialNumber, iMaxCompLen, iFileSystemFlags;
			
			if(GetVolumeInformation(pszDrives, 0L, 0, &iVolumeSerialNumber, &iMaxCompLen, &iFileSystemFlags, 0L, 0))
			{
				TRACE("\nSerial # of \"%s\": %u", pszDrives, iVolumeSerialNumber);
				return iVolumeSerialNumber;
			}
		}
	}
	return 0;
}
unsigned long CLicenseManager::ConvertVolumeSerialNo(const char* pszSlNoInHex)
{
	char* pszSlNo;
	int i, j;
	unsigned long iValToRet;
	
	pszSlNo = new char[strlen(pszSlNoInHex) + 1];
	memset(pszSlNo, 0, strlen(pszSlNoInHex) + 1);
	for(i = 0, j = 0; i < (int)strlen(pszSlNoInHex); i++)
	{
		const char chVal = toupper(*(pszSlNoInHex + i));
		
		if(((chVal >= '0') && (chVal <= '9')) || ((chVal >= 'A') && (chVal <= 'F')))
		{
			*(pszSlNo + j) = chVal;
			j++;
		}
	}
	iValToRet = 0;
	for(i = (int)strlen(pszSlNo) - 1; i >= 0; i--)
	{
		const char chVal = toupper(*(pszSlNo + i));
		unsigned long iDigit;
		char szTxt[2];
		
		szTxt[1] = '\0';
		if(isalpha(chVal))
		{
			iDigit = 10 + (chVal - 'A');
		}
		else
		{
			szTxt[0] = chVal;
			iDigit = atoi(szTxt);
		}
		iValToRet += iDigit * (unsigned long)pow(16, (strlen(pszSlNo) - (i + 1)));
	}
	delete []pszSlNo;
	return iValToRet;
}

//
bool CLicenseManager::ReadLiceseInfo(const char* pszFile, char** pszLicenseKey)
{
	*pszLicenseKey = 0L;
	{//
		FILE* hFile;

		hFile = fopen(pszFile, "r");
		if(0L == hFile)
			return false;
		const int iFileSize = 512;//Determine file size in bytes here....
		
		//
		*pszLicenseKey = new char[iFileSize];
		fgets(*pszLicenseKey, iFileSize, hFile);
		//
		fclose(hFile);
		return true;
	}
}
