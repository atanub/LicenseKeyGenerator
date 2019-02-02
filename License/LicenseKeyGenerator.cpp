#include "stdafx.h"
#include "LicenseKeyGenerator.h"
#include "ProtectionDebugging.h"

//
//
//CLicenseKeyGenerator
//

const int CLicenseKeyGenerator::m_iArraySwapSrcIndexes[] = {1, 3, 15, 17, 5, 7, 9, 11, 13, 19, 30};
const int CLicenseKeyGenerator::m_iArraySwapDesIndexes[] = {2, 18, 20, 4, 6, 8, 10, 12, 14, 16, 28};
const int CLicenseKeyGenerator::m_iArraySwapSrcIndexesForExpInfo[] = {1, 3, 5, 9, 11, 15, 13};
const int CLicenseKeyGenerator::m_iArraySwapDesIndexesForExpInfo[] = {8, 10, 4, 6, 2, 0, 16};
const unsigned int CLicenseKeyGenerator::m_iValueForLicenseMatching = (int)pow(2, 31) - 19;
const int CLicenseKeyGenerator::m_iMaxValidityInDays = 365;
const char CLicenseKeyGenerator::m_chKeySeparator = '-';

CLicenseKeyGenerator::CLicenseKeyGenerator(const unsigned int iHDDIdentifier, const int iMaxValidityInDays /* = -1*/) : m_iDeviceID(iHDDIdentifier), m_iValidityForNewLicenses((iMaxValidityInDays < 0) ? m_iMaxValidityInDays : iMaxValidityInDays)
{
	m_timeLicenseIssued = 0L;
	m_iValidity = 0;
}
CLicenseKeyGenerator::~CLicenseKeyGenerator()
{
}
char *CLicenseKeyGenerator::GenerateID(unsigned int *piLicenseKeyBeforeEncrypting /* = 0L*/) const
{
	unsigned int iLicenseKey;
	unsigned int iDeviceIDEx;

	iLicenseKey = 0;
	iDeviceIDEx = m_iDeviceID >> 3;

	iLicenseKey = labs(m_iValueForLicenseMatching - iDeviceIDEx);
	if (0L != piLicenseKeyBeforeEncrypting)
		*piLicenseKeyBeforeEncrypting = iLicenseKey;

	return EncryptLicenseKey(iLicenseKey, m_iValidityForNewLicenses);
}
bool CLicenseKeyGenerator::IsValidID(const char *pszParam, char **pszFeedback /*= 0L*/)
{
	struct tm timeIssueance;
	unsigned int iLicenseKey;
	int iValidity;
	time_t timeToday;
	char szTemp1[_MAX_PATH], szTemp2[_MAX_PATH];

	if (0L != pszFeedback)
	{
		*pszFeedback = 0L;
	}
	time(&timeToday);

	//Initialize
	m_timeLicenseIssued = 0L;
	m_iValidity = 0;

	iLicenseKey = DecryptLicenseKey(pszParam, &timeIssueance, iValidity);
	if (iLicenseKey <= 0)
	{
		if (0L != pszFeedback)
		{
			*pszFeedback = new char[64];
			sprintf(*pszFeedback, "Invalid license key provided");
		}
		return false;
	}

	strcpy(szTemp2, asctime(&timeIssueance));
	szTemp2[strlen(szTemp2) - 1] = '\0';
	sprintf(szTemp1, "License Issued on: %s Validity: %d days", szTemp2, iValidity);
	TRACE("\n%s", szTemp1);
	CProtectionDebugging::Log(szTemp1);

	const time_t timeExpiry = mktime(&timeIssueance) + iValidity * 24 * 3600;
	strcpy(szTemp2, asctime(localtime(&timeExpiry)));
	szTemp2[strlen(szTemp2) - 1] = '\0';
	sprintf(szTemp1, "License is valid till: %s", szTemp2);
	TRACE("\n%s", szTemp1);
	CProtectionDebugging::Log(szTemp1);

	{ //
		char *pszKey;
		unsigned int iLicenseKeyBeforeEncrypting;

		pszKey = GenerateID(&iLicenseKeyBeforeEncrypting);
		delete[] pszKey;
		//
		if ((iLicenseKey == iLicenseKeyBeforeEncrypting) && OverrideTimeBoundValidity(timeExpiry))
		{
			m_timeLicenseIssued = mktime(&timeIssueance);
			m_iValidity = iValidity;
			return true;
		}
		if (0L != pszFeedback)
		{
			*pszFeedback = new char[64];
			if (iLicenseKey != iLicenseKeyBeforeEncrypting)
			{
				sprintf(*pszFeedback, "Invalid license key provided");
			}
			else
			{
				if (!OverrideTimeBoundValidity(timeExpiry))
				{
					m_timeLicenseIssued = mktime(&timeIssueance);
					m_iValidity = iValidity;
					sprintf(*pszFeedback, "License expired on: %s", asctime(localtime(&timeExpiry)));
				}
			}
		}
	}
	return false;
}
bool CLicenseKeyGenerator::OverrideTimeBoundValidity(const time_t timeExpiry)
{
	time_t timeToday;

	time(&timeToday);
	return (timeToday <= timeExpiry);
}
bool CLicenseKeyGenerator::ExtractValidityInfo(const char *pszParam, time_t &timeLicenseIssued, int &iValidity)
{
	timeLicenseIssued = 0;
	iValidity = 0;

	const bool bFlag = this->IsValidID(pszParam);

	timeLicenseIssued = m_timeLicenseIssued;
	iValidity = m_iValidity;
	return bFlag;
}
unsigned int CLicenseKeyGenerator::DecryptLicenseKey(const char *pszParam, struct tm *ptimeIssueance, int &iValidity)
{
	//DUMMY Implementation
	return 1;
}
//Caller function is responsible for memory deallocation of the returned pointer
char *CLicenseKeyGenerator::EncryptLicenseKey(const unsigned int iLicenseKey, const int iValidityInDays)
{
	//DUMMY Implementation
	char *pszLicenseInfo = 0L;
	pszLicenseInfo = new char[1];
	strcpy(pszLicenseInfo, "");
	return pszLicenseInfo;
}
bool CLicenseKeyGenerator::EncryptExpirationInfoStr(char *pszParam, int iSize, int iValidity)
{
	//DUMMY Implementation
	sprintf(pszParam, "%s", "DUMMY");
	return true;
}

bool CLicenseKeyGenerator::DecryptExpirationInfoStr(const char *pszParam, struct tm *ptimeIssueance, int &iValidity)
{
	//DUMMY Implementation
	iValidity = 179;
	return true;
}

//Caller function is responsible for memory deallocation of the returned pointer
char *CLicenseKeyGenerator::InsertRandomCharAfterEachOddDigit(const char *pszParam)
{
	int i, j, k;
	char *pszResult;

	const int iMinValue = 65;
	'A';
	const int iMaxValue = 90;
	'Z';
	int iResultStrSize = 4 * strlen(pszParam) + 1; //
	int *piArrayRandomNumbers;

	pszResult = new char[iResultStrSize];
	memset(pszResult, 0, iResultStrSize);
	//
	piArrayRandomNumbers = new int[iResultStrSize];
	memset(piArrayRandomNumbers, 0, sizeof(int) * iResultStrSize);
	GenerateRandomNoInBetween(iMinValue, iMaxValue, piArrayRandomNumbers, iResultStrSize);

	for (i = 0, j = 0, k = 0; i < (int)strlen(pszParam); i++)
	{
		*(pszResult + j++) = *(pszParam + i);
		*(pszResult + j++) = (char)(*(piArrayRandomNumbers + k++)); //Random #
		*(pszResult + j++) = (char)(*(piArrayRandomNumbers + k++)); //Random #
		*(pszResult + j++) = (char)(*(piArrayRandomNumbers + k++)); //Random #
	}
	delete[] piArrayRandomNumbers;
	return pszResult;
}
//Caller function is responsible for memory deallocation of the returned pointer
char *CLicenseKeyGenerator::StripRandomCharsFromEncryptedKey(const char *pszParam)
{
	char *pszResult;
	int i, j;

	pszResult = new char[strlen(pszParam)];
	memset(pszResult, 0, strlen(pszParam));
	//
	for (i = 0, j = 0; i < (int)strlen(pszParam);)
	{
		*(pszResult + j++) = *(pszParam + i);
		i = i + 4;
	}
	return pszResult;
}
void CLicenseKeyGenerator::GenerateRandomNoInBetween(const int iMinValue, const int iMaxValue, int *piArrayResult, int iArraySize)
{ //
	int i;

	srand((unsigned)time(NULL));
	for (i = 0; i < iArraySize; i++)
	{
		*(piArrayResult + i) = rand() % (iMaxValue - iMinValue + 1) + iMinValue;
	}
}

void CLicenseKeyGenerator::DecimalToBinaryString(unsigned int iParam, char *pszBits, int iSize)
{
	int i;

	memset(pszBits, 0, iSize);
	for (i = 0; i < 32; i++)
	{
		const unsigned long iBit = (unsigned long)pow(2, 32 - i - 1);
		*(pszBits + i) = ((iParam & iBit) ? '1' : '0');
	}
	//itoa(iParam, pszBits, 2);
	//	{//Query for each bit value
	//		char szBits[64];
	//		TRACE("\n%s = %d", szBits, iParam);
	//		int iTest;
	//
	//		TRACE("\n");
	//		for(i = 0, iTest = iParam; i < 32; i++)
	//		{
	//			iTest = iParam >> (32 - i - 1);
	//			TRACE("%d", (iTest & 1));
	//		}
	//		TRACE("<<%d\n", iParam);
	//	}
}
void CLicenseKeyGenerator::SwapCharacters(char *pszParam, const int iArraySwapSrcIndexes[], const int iArraySwapDesIndexes[], int iArraySize)
{
	int i;

	//assert(strlen(pszParam) == 32);
	//assert(sizeof(m_iArraySwapSrcIndexes) == sizeof(m_iArraySwapDesIndexes));
	for (i = 0; i < iArraySize; i++)
	{
		char chSrc, chDest;

		chSrc = *(pszParam + *(iArraySwapSrcIndexes + i));
		chDest = *(pszParam + *(iArraySwapDesIndexes + i));
		*(pszParam + *(iArraySwapSrcIndexes + i)) = chDest;
		*(pszParam + *(iArraySwapDesIndexes + i)) = chSrc;
	}
}
unsigned int CLicenseKeyGenerator::BinaryToDecimalValue(const char *pszParam)
{
	unsigned int iRetValue;
	int i;

	//assert(strlen(pszParam) == 32);
	for (i = (strlen(pszParam) - 1), iRetValue = 0; i >= 0; i--)
	{
		const int iPosition = 32 - i - 1;
		char szTxt[2];

		szTxt[1] = '\0';
		szTxt[0] = *(pszParam + i);
		const int iDigit = atoi(szTxt);
		iRetValue += iDigit * (unsigned long)pow(2, iPosition);
	}
	return iRetValue;
}

int CLicenseKeyGenerator::ExtractNConvertString(const char *pszParam, const int iStartIndex, const int iEndIndex, char *pszResult)
{
	if (((int)strlen(pszParam) > iStartIndex) && ((int)strlen(pszParam) >= iEndIndex))
	{
		int i;
		char szTemp[32];

		memset(szTemp, 0, sizeof(szTemp));
		for (i = iStartIndex; i < iEndIndex; i++)
		{
			szTemp[i - iStartIndex] = *(pszParam + i);
		}
		strcpy(pszResult, szTemp);
		return atoi(szTemp);
	}
	return -1; //Failure
}

//
//CLicenseKeyGeneratorContour
//
CLicenseKeyGeneratorContour::CLicenseKeyGeneratorContour(const unsigned int iHDDIdentifier, const int iMaxValidityInDays /* = -1*/)
	: CLicenseKeyGenerator(iHDDIdentifier, iMaxValidityInDays)
{
}
char *CLicenseKeyGeneratorContour::GenerateID(unsigned int *piLicenseKeyBeforeEncrypting /* = 0L*/) const
{
	unsigned int iLicenseKey;
	unsigned int iDeviceIDEx;

	iLicenseKey = 0;
	iDeviceIDEx = m_iDeviceID >> 3;

	iLicenseKey = labs(m_iValueForLicenseMatching - iDeviceIDEx) + 3;
	if (0L != piLicenseKeyBeforeEncrypting)
		*piLicenseKeyBeforeEncrypting = iLicenseKey;

	return EncryptLicenseKey(iLicenseKey, m_iValidityForNewLicenses);
}

//
//CLicenseKeyGeneratorSurvey
//
CLicenseKeyGeneratorSurvey::CLicenseKeyGeneratorSurvey(const unsigned int iHDDIdentifier, const int iMaxValidityInDays /* = -1*/)
	: CLicenseKeyGenerator(iHDDIdentifier, iMaxValidityInDays)
{
}
char *CLicenseKeyGeneratorSurvey::GenerateID(unsigned int *piLicenseKeyBeforeEncrypting /* = 0L*/) const
{
	unsigned int iLicenseKey;
	unsigned int iDeviceIDEx;

	iLicenseKey = 0;
	iDeviceIDEx = m_iDeviceID >> 4;

	iLicenseKey = labs(m_iValueForLicenseMatching - iDeviceIDEx) + 2;
	if (0L != piLicenseKeyBeforeEncrypting)
		*piLicenseKeyBeforeEncrypting = iLicenseKey;

	return EncryptLicenseKey(iLicenseKey, m_iValidityForNewLicenses);
}

//
//CLicenseKeyGeneratorManager
//
CLicenseKeyGeneratorManager::CLicenseKeyGeneratorManager(const unsigned int iHDDIdentifier, const int iMaxValidityInDays /* = -1*/)
	: CLicenseKeyGenerator(iHDDIdentifier, iMaxValidityInDays)
{
}
char *CLicenseKeyGeneratorManager::GenerateID(unsigned int *piLicenseKeyBeforeEncrypting /* = 0L*/) const
{
	unsigned int iLicenseKey;
	unsigned int iDeviceIDEx;

	iLicenseKey = 0;
	iDeviceIDEx = m_iDeviceID >> 5;

	iLicenseKey = labs(m_iValueForLicenseMatching - iDeviceIDEx) + 1;
	if (0L != piLicenseKeyBeforeEncrypting)
		*piLicenseKeyBeforeEncrypting = iLicenseKey;

	return EncryptLicenseKey(iLicenseKey, m_iValidityForNewLicenses);
}
bool CLicenseKeyGeneratorManager::OverrideTimeBoundValidity(const time_t timeExpiry)
{
	time_t timeToday;

	time(&timeToday);
	return (timeToday <= timeExpiry);
}
bool CLicenseKeyGeneratorManager::ExtractValidityInfo(const char *pszParam, time_t &timeLicenseIssued, int &iValidity)
{
	timeLicenseIssued = 0;
	iValidity = 0;

	const bool bFlag = IsValidID(pszParam);

	timeLicenseIssued = m_timeLicenseIssued;
	iValidity = m_iValidity;
	if (bFlag && (this->m_iValidity >= 999))
	{
		iValidity = -1; //-ve validity means - life time validity
	}
	return bFlag;
}

#ifdef _DEBUG
void CLicenseKeyGenerator::Test(int iDeviceID)
{
	CLicenseKeyGenerator obj(iDeviceID);
	char *pszKey;

	pszKey = obj.GenerateID();
	TRACE("\nLicense Key for Device [%ld] is [%s]", iDeviceID, pszKey);
	if (obj.IsValidID(pszKey))
	{
		TRACE("\nVerified: License Key for Device [%ld] is [%s]", iDeviceID, pszKey);
	}
	delete[] pszKey;
}
#endif //_DEBUG
