#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>


// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <errno.h>
#include <string.h>
#include <tchar.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <malloc.h>
#include <memory.h>

#include <initguid.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <new.h>      // for placement new
#include <shlobj.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>
#include <limits.h>

//C++ Header Files
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>

using namespace std;


//DirectX Header Files
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dwmapi.h>



#include <strsafe.h>



// CRT's memory leak detection
#if defined(DEBUG) || defined(_DEBUG)
#include <crtdbg.h>
#endif

// Direct3D9 includes
#include <d3d9.h>
#include <d3dx9.h>

// Direct3D11 includes
#include <d3dcommon.h>
#include <dxgi.h>
#include <d3d10_1.h>
#include <d3d10.h>
#include <d3dx10.h>

// XInput includes
#include <xinput.h>

// HRESULT translation for Direct3D and other APIs 
#include <dxerr.h>


#ifdef _DEBUG
#	define _CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
#	define CheckMemoryLeak _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF)
#endif 


#pragma warning(disable : 4100)
#pragma warning(disable : 4512)
