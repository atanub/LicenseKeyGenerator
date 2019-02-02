// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D359A157_6E1B_4175_8D33_37E91003041E__INCLUDED_)
#define AFX_STDAFX_H__D359A157_6E1B_4175_8D33_37E91003041E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <winreg.h>

#ifdef _DEBUG
	#define TRACE              ::printf
#else
	#define TRACE              1 ? (void)0 : ::printf
#endif//_DEBUG

#endif // !defined(AFX_STDAFX_H__D359A157_6E1B_4175_8D33_37E91003041E__INCLUDED_)
