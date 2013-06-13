#pragma once

#ifndef WINVER                  // Specifies that the minimum required platform is Windows Vista.
#define WINVER 0x0600           // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows 98.
#define _WIN32_WINDOWS 0x0600   // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE               // Specifies that the minimum required platform is Internet Explorer 7.0.
#define _WIN32_IE 0x0800        // Change this to the appropriate value to target other versions of IE.
#endif

#include <SDKDDKVer.h>

//windows header files
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



#ifdef _DEBUG
 #define _CRTDBG_MAP_ALLOC
 #include <stdlib.h> 
 #include <crtdbg.h>
 #define CheckMemoryLeak _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF)
#endif 

