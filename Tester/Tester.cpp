	// License.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <LicenseKeyGenerator.h>
#include <LicenseManager.h>
#include <ApplcationProtector.h>
#include "ProtectionInterface.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	const unsigned int iTest = 1161171670;
	/*
	*/
	{//
		CLicenseKeyGeneratorManager objManager(iTest, 11);
		char* pszKey;

		pszKey = objManager.GenerateID();
		TRACE("\n[%s]<<Key", pszKey);
		
		if(objManager.IsValidID(pszKey))
		{
			TRACE("<<is a valid Key");
		}
		else
		{
			TRACE("<<is an INVALID Key");
		}
		
		delete []pszKey;
	}
	{//
		CLicenseKeyGeneratorContour objManager(iTest, 11);
		char* pszKey;

		pszKey = objManager.GenerateID();
		TRACE("\n[%s]<<Key<<Contour", pszKey);
		
		if(objManager.IsValidID(pszKey))
		{
			TRACE("<<is a valid Key<<Contour");
		}
		else
		{
			TRACE("<<is an INVALID Key<<Contour");
		}
		
		delete []pszKey;
	}
	{//
		CLicenseKeyGeneratorSurvey objManager(iTest, 11);
		char* pszKey;

		pszKey = objManager.GenerateID();
		TRACE("\n[%s]<<Key<<Survey", pszKey);
		
		if(objManager.IsValidID(pszKey))
		{
			TRACE("<<is a valid Key<<Survey");
		}
		else
		{
			TRACE("<<is an INVALID Key<<Survey");
		}
		
		delete []pszKey;
	}
	/*
	*/
	{
		CProtectionInterface objCProtectionInterface;
		//typedef enum {LicenseIssuer = 0, ContourARX = 1, SurveyUtilARX = 2} AppType;
		CApplcationProtector objProtectorA(CLicenseManager::AppType::LicenseIssuer, &objCProtectionInterface);
		CApplcationProtector objProtectorB(CLicenseManager::AppType::ContourARX, &objCProtectionInterface);
		CApplcationProtector objProtectorC(CLicenseManager::AppType::SurveyUtilARX, &objCProtectionInterface);

		objProtectorA.Initialize();
		objProtectorB.Initialize();
		objProtectorC.Initialize();
		//
		/*
			char* pszKey;

			objCProtectionInterface.GetLicenseKeyFromUser(pszKey);
			unsigned long i = CLicenseManager::ConvertVolumeSerialNo(pszKey);
			TRACE("\n%s = %u", pszKey, i);
			delete []pszKey;
		*/
	}

	return 0;
}
bool CProtectionInterface::GetLicenseKeyFromUser(char*& pszKey) const
{
	pszKey = new char[512];

	printf("\nEnter License Key:");
	if(scanf("%s", pszKey) <= 0)
		return false;
	
	return true;
}

