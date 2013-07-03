// warmgui.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "warmgui_summer.h"

HINSTANCE ghDllInstance = 0;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID /*lpReserved*/
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
#	    ifdef _DEBUG
    	    CheckMemoryLeak;
#   	endif
		ghDllInstance = hModule;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


const COLORALPHA DEFAULT_COLOR_ALPHA = D2D1::ColorF(.0f, .0f, .0f, -1.0f);

bool operator== (const COLORALPHA& a, const COLORALPHA& b)
{
    return (a.a == b.a && a.b == b.b && a.g == b.g && a.r == b.r);
}


//--------------------------------------------------------------------------------------
// Outputs to the debug stream a formatted Unicode string with a variable-argument list.
//--------------------------------------------------------------------------------------
VOID WINAPI _MyTraceW( LPCWSTR strMsg, ... )
{
#if defined(DEBUG) || defined(_DEBUG)
    WCHAR strBuffer[512];

    va_list args;
    va_start(args, strMsg);
    vswprintf_s( strBuffer, 512, strMsg, args );
    strBuffer[511] = L'\0';
    va_end(args);

#ifdef _UNICODE
	OutputDebugStringW( strBuffer );
#else
	OutputDebugStringA( strBuffer );
#endif //_UNICODE

#else
    UNREFERENCED_PARAMETER( strMsg );
#endif
}


//--------------------------------------------------------------------------------------
// Outputs to the debug stream a formatted MBCS string with a variable-argument list.
//--------------------------------------------------------------------------------------
VOID WINAPI _MyTraceA( LPCSTR strMsg, ... )
{
#if defined(DEBUG) || defined(_DEBUG)
    CHAR strBuffer[512];

    va_list args;
    va_start(args, strMsg);
    vsprintf_s( strBuffer, 512, strMsg, args );
    strBuffer[511] = '\0';
    va_end(args);

    OutputDebugStringA( strBuffer );
#else
    UNREFERENCED_PARAMETER( strMsg );
#endif
}


namespace WARMGUI {

/* static (global) variables that are specified as exported by getopt() */
TCHAR* toptarg = NULL; /* pointer to the start of the option argument */
static int    toptind = 1; /* number of the next argv[] to be evaluated */
static int    topterr = 1; /* non-zero if a question mark should be returned
when a non-valid option character is detected */

/* handle possible future character set concerns by putting this in a macro */
#define _next_char(X) (TCHAR)(*(X+1))

int gettopt(int argc, TCHAR *argv[], TCHAR *opstring)
{
	static TCHAR *pIndexPosition = NULL; /* place inside current argv string */
	TCHAR *pArgString = NULL;            /* where to start from next */
	TCHAR *pOptString;                   /* the string in our program */


	if (pIndexPosition != NULL) {
		/* we last left off inside an argv string */
		if (*(++pIndexPosition)) {
		/* there is more to come in the most recent argv */
			pArgString = pIndexPosition;
		}
	}

	if (pArgString == NULL) {
		/* we didn't leave off in the middle of an argv string */
		if (toptind >= argc) {
			/* more command-line arguments than the argument count */
			pIndexPosition = NULL; /* not in the middle of anything */
			return -1; /* used up all command-line arguments */
		}

		/*---------------------------------------------------------------------
		* If the next argv[] is not an option, there can be no more options.
		*-------------------------------------------------------------------*/
		pArgString = argv[toptind++]; /* set this to the next argument ptr */

		if ((_T('/') != *pArgString) && /* doesn't start with a slash or a dash? */
				(_T('-') != *pArgString)) {
			--toptind; /* point to current arg once we're done */
			toptarg = NULL; /* no argument follows the option */
			pIndexPosition = NULL; /* not in the middle of anything */
			return -1; /* used up all the command-line flags */
		}

		/* check for special end-of-flags markers */
		if ((_tcscmp(pArgString, _T("-")) == 0) ||
				(_tcscmp(pArgString, _T("--")) == 0)) {
			toptarg = NULL; /* no argument follows the option */
			pIndexPosition = NULL; /* not in the middle of anything */
			return -1; /* encountered the special flag */
		}

		pArgString++; /* look past the / or - */
	}

	if (_T(':') == *pArgString) { /* is it a colon? */
		/*---------------------------------------------------------------------
		* Rare case: if topterr is non-zero, return a question mark;
		* otherwise, just return the colon we're on.
		*-------------------------------------------------------------------*/
		return (topterr ? (int)_T('?') : (int)_T(':'));
	} else if ((pOptString = _tcschr(opstring, *pArgString)) == 0) {
		/*---------------------------------------------------------------------
		* The letter on the command-line wasn't any good.
		*-------------------------------------------------------------------*/
		toptarg = NULL; /* no argument follows the option */
		pIndexPosition = NULL; /* not in the middle of anything */
		return (topterr ? (int)_T('?') : (int)*pArgString);
	} else {
		/*---------------------------------------------------------------------
		* The letter on the command-line matches one we expect to see
		*-------------------------------------------------------------------*/
		if (_T(':') == _next_char(pOptString)) { /* is the next letter a colon? */
			/* It is a colon. Look for an argument string. */
			if (_T('\0') != _next_char(pArgString)) { /* argument in this argv? */
			toptarg = &pArgString[1]; /* Yes, it is */
			} else {
				/*-------------------------------------------------------------
				* The argument string must be in the next argv.
				* But, what if there is none (bad input from the user)?
				* In that case, return the letter, and toptarg as NULL.
				*-----------------------------------------------------------*/
				if (toptind < argc)
					toptarg = argv[toptind++];
				else {
					toptarg = NULL;
					return (topterr ? (int)_T('?') : (int)*pArgString);
				}
			}
			pIndexPosition = NULL; /* not in the middle of anything */
		} else {
			/* it's not a colon, so just return the letter */
			toptarg = NULL; /* no argument follows the option */
			pIndexPosition = pArgString; /* point to the letter we're on */
		}
		return (int)*pArgString; /* return the letter that matched */
	}
}

inline bool pt_in_rect(RECT& rect, int x, int y)
{
	return (x >= rect.left && x <= rect.right
		 && y >= rect.top  && y <= rect.bottom);
}

inline bool pt_in_rect(RECT& rect, POINT_u pt)
{
	return (pt.x >= rect.left && pt.x <= rect.right
		 && pt.y >= rect.top  && pt.y <= rect.bottom);
}

}
