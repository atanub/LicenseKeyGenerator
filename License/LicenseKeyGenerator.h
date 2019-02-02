#ifndef _LICENSEKEYGENERATOR_H
#define _LICENSEKEYGENERATOR_H
//
//CLicenseKeyGenerator
//
class CLicenseKeyGenerator
{
protected:
	CLicenseKeyGenerator(const unsigned int iHDDIdentifier, const int iMaxValidityInDays = -1);
public:
	virtual ~CLicenseKeyGenerator();
public:
	virtual char*	GenerateID(unsigned int* iLicenseKeyBeforeEncrypting = 0L) const;
	virtual bool	IsValidID(const char*, char** = 0L);
	virtual bool	ExtractValidityInfo(const char* pszParam, time_t& timeLicenseIssued, int& iValidity);
protected:
	virtual bool	OverrideTimeBoundValidity(const time_t timeExpiry);


#ifdef _DEBUG
	static void		Test(int iDeviceID);
#endif//_DEBUG

protected:
	static void				DecimalToBinaryString(unsigned int iParam, char* pszBits, int iSize);
	static void				SwapCharacters(char* pszParam, const int iArraySwapSrcIndexes[], const int iArraySwapDesIndexes[], int iArraySize);
	static unsigned int		BinaryToDecimalValue(const char* pszParam);
	
	static unsigned int		DecryptLicenseKey(const char* pszParam, struct tm* ptimeIssueance, int& iValidity);
	static char*			EncryptLicenseKey(const unsigned int iLicenseKey, const int iValidityInDays);
	static char*			InsertRandomCharAfterEachOddDigit(const char* pszBits);
	static void				GenerateRandomNoInBetween(const int iMinValue, const int iMaxValue, int* piArrayResult, int iArraySize);
	static char*			StripRandomCharsFromEncryptedKey(const char* pszParam);

	static bool	EncryptExpirationInfoStr(char*, int, int);
	static bool DecryptExpirationInfoStr(const char* pszParam, struct tm* ptimeIssueance, int& iValidity);
	static int  ExtractNConvertString(const char* pszParam, const int iStartIndex, const int iEndIndex, char* pszResult);
protected:
	static const unsigned	int m_iValueForLicenseMatching;
	static const			int m_iArraySwapSrcIndexes[];
	static const			int m_iArraySwapDesIndexes[];
	static const			int m_iArraySwapSrcIndexesForExpInfo[];
	static const			int m_iArraySwapDesIndexesForExpInfo[];
	static const			int m_iMaxValidityInDays;
	static const			char m_chKeySeparator;
protected:
	const unsigned int	m_iDeviceID;
	const int			m_iValidityForNewLicenses;
	time_t				m_timeLicenseIssued;
	int					m_iValidity;
};
//
//CLicenseKeyGeneratorContour
//
class CLicenseKeyGeneratorContour : public CLicenseKeyGenerator
{
public:
	CLicenseKeyGeneratorContour(const unsigned int iHDDIdentifier, const int iMaxValidityInDays = -1);
	virtual ~CLicenseKeyGeneratorContour() {;}
public:
	virtual char*	GenerateID(unsigned int* iLicenseKeyBeforeEncrypting = 0L) const;
};
//
//CLicenseKeyGeneratorSurvey
//
class CLicenseKeyGeneratorSurvey : public CLicenseKeyGenerator
{
public:
	CLicenseKeyGeneratorSurvey(const unsigned int iHDDIdentifier, const int iMaxValidityInDays = -1);
	virtual ~CLicenseKeyGeneratorSurvey() {;}
public:
	virtual char*	GenerateID(unsigned int* iLicenseKeyBeforeEncrypting = 0L) const;
};
//
//CLicenseKeyGeneratorManager
//
class CLicenseKeyGeneratorManager : public CLicenseKeyGenerator
{
public:
	CLicenseKeyGeneratorManager(const unsigned int iHDDIdentifier, const int iMaxValidityInDays = -1);
	virtual ~CLicenseKeyGeneratorManager() {;}
public:
	virtual char*	GenerateID(unsigned int* iLicenseKeyBeforeEncrypting = 0L) const;
	virtual bool	ExtractValidityInfo(const char* pszParam, time_t& timeLicenseIssued, int& iValidity);
protected:
	virtual bool	OverrideTimeBoundValidity(const time_t timeExpiry);
};

#endif// _LICENSEKEYGENERATOR_H
