// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__905508D7_0C39_4E55_B776_0FB47AEF28FE__INCLUDED_)
#define AFX_STDAFX_H__905508D7_0C39_4E55_B776_0FB47AEF28FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <stdio.h>

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#ifdef _DEBUG
	#define TRACE              ::printf
#else
	#define TRACE              1 ? (void)0 : ::printf
#endif//_DEBUG

#endif // !defined(AFX_STDAFX_H__905508D7_0C39_4E55_B776_0FB47AEF28FE__INCLUDED_)
