// QKSpectrum.cpp
//

#include "stdafx.h"
#include "qks_incs.h"
#include "network_inc.h"

#include "Test1.h"
#include "Test2.h"
#include "Test3.h"
#include "resource.h"

#define MAX_LOADSTRING 100

#define _HAVE_NETWORK_ 1
#define _realtime_calculate_ 0
#define __recive_data_from_america_server__ 0

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

    qksapp app;
    app.InitialApp(hInstance, nCmdShow);

	TCHAR szTitle[MAX_LOADSTRING] = L"MyFirstWindow";
	TCHAR szClassName[MAX_LOADSTRING] = L"WARMGUIWNDCLASS";

	CTest3 mainwnd;
	mainwnd.SetConfig(app.GetConfig());
	BOOL r = mainwnd.InitInstance(hInstance, hPrevInstance, lpCmdLine, nCmdShow, szClassName, szTitle);
	if (r) {
    	mainwnd.LoadAccelerators(IDC_QKSPECTRUM);
		mainwnd.RunMessageLoop();
    }

	app.CleanupApp();

    return (r) ? 0 : -1;
}





// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
