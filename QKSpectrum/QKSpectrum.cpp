// QKSpectrum.cpp
//

#include "stdafx.h"
#include "qks_incs.h"


#include "resource.h"

#define MAX_LOADSTRING 100

#define _HAVE_NETWORK_ 1
#define _realtime_calculate_ 0
#define __recive_data_from_america_server__ 0

CEuclidView * wnd_euclid  = 0;
CAnalystView* wnd_analyst = 0;

qksapp the_app;

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

	CTickDataView mainwnd;
	mainwnd.SetConfig(the_app.GetConfig());

	BOOL r = mainwnd.InitInstance(hInstance, hPrevInstance, lpCmdLine, nCmdShow, szClassName, szTitle);
	if (r) {
    	mainwnd.LoadAccelerators(IDC_QKSPECTRUM);
		mainwnd.RunMessageLoop();
    }

    if (wnd_euclid) {
        wnd_euclid->Close();
        SafeDelete(wnd_euclid);
    }
    
    if (wnd_analyst) {
        wnd_analyst->Close();
        SafeDelete(wnd_analyst);
    }

	the_app.CleanupApp();

    //return (r) ? 0 : -1;
    return (0);
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


int ShowEuclidView(CWindow* parent_window)
{
    RECT rect;
    rect.left = rect.top = 200, rect.right = rect.bottom = 600;

    if (!wnd_euclid) {
        wnd_euclid = new CEuclidView();
        wnd_euclid->SetConfig(the_app.GetConfig());
    }

    if (!wnd_euclid->GetSafeHwnd()) {
        wnd_euclid->LoadAccelerators(IDC_QKSPECTRUM);
        wnd_euclid->Create(
            L"EuclidWindowClass",
            L"EuclidView",
            WS_VISIBLE | WS_CLIPCHILDREN,
            rect,
            parent_window);
    }
    return (0);
}

int ShowAnalystView(CWindow* parent_window)
{
    RECT rect;
    rect.left = rect.top = 200, rect.right = rect.bottom = 600;

    if (!wnd_analyst) {
        wnd_analyst = new CAnalystView();
        wnd_analyst->SetConfig(the_app.GetConfig());
    }

    if (!wnd_analyst->GetSafeHwnd()) {
        wnd_analyst->LoadAccelerators(IDC_QKSPECTRUM);
        wnd_analyst->Create(
            L"EuclidWindowClass",
            L"EuclidView",
            WS_VISIBLE | WS_CLIPCHILDREN,
            rect,
            parent_window);
    }
    return (0);
}
