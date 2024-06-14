// rcnUtilities.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
//#include <afxdllx.h>
//#include "mfcutilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*

static AFX_EXTENSION_MODULE RcnUtilitiesDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("RCNUTILITIES.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(RcnUtilitiesDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(RcnUtilitiesDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("RCNUTILITIES.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(RcnUtilitiesDLL);
	}
	return 1;   // ok
}

///////////////////////////////////////////////////////////////////////////////////
*/
CTime			ConvertTime;//used for time conversions
int	ConvertTime64ToHHMMSS(_int64 timetoconvert)
{
	ConvertTime = (long)timetoconvert;//this inits the CTIME struct for conversion
	//return out time in our hhmmss format
	return ConvertTime.GetHour()*10000 + ConvertTime.GetMinute()*100 + ConvertTime.GetSecond();
}

///////////////////////////////////////////////////////////////////////////////////

int	ConvertTime64ToYYYYMMDD(_int64 timetoconvert)
{
	ConvertTime = (long)timetoconvert;//this inits the CTIME struct for conversion
	//return out time in our hhmmss format
	return ConvertTime.GetYear()*10000 + ConvertTime.GetMonth()*100 + ConvertTime.GetDay();
}


