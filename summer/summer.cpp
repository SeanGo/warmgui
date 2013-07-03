#include "stdafx.h"
#include "summer.h"
#include "SummerAtelier.h"
#include "SummerApp.h"
#include "SummerView.h"

//#include "TestSummerCanvas.h"
//#include "TestDispatcher_summer.h"

CSummerApp the_app;
#include "resource.h"

#define MAX_LOADSTRING 100

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
#ifdef _DEBUG
	CheckMemoryLeak;
#endif //_DEBUG

    the_app.InitialApp(hInstance, nCmdShow);

    TCHAR szTitle[MAX_LOADSTRING] = L"MyFirstWindow";
	TCHAR szClassName[MAX_LOADSTRING] = L"WARMGUIWNDCLASS";

	CSummerView mainwnd;

	BOOL r = mainwnd.InitInstance(hInstance, hPrevInstance, lpCmdLine, nCmdShow, szClassName, szTitle);
	if (r) {
    	mainwnd.LoadAccelerators(IDC_SUMMER);
		mainwnd.RunMessageLoop();
    }

	the_app.CleanupApp();

    //return (r) ? 0 : -1;
    return (0);
}


