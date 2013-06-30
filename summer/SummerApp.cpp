#include "StdAfx.h"
#include "warmgui_summer.h"
#include "TestDispatcher_summer.h"
#include "SummerApp.h"


CSummerApp::CSummerApp(void)
    : dispatcher(0)
{

}


CSummerApp::~CSummerApp(void)
{
    SafeDelete(dispatcher);
}

int CSummerApp::InitialApp(HINSTANCE hInstance, int nCmdShow)
{

	TCHAR curpath[MAX_PATH];
	////::GetCurrentDirectoryW(MAX_PATH, curpath);	////something is wired
	GetModuleFileNameW(NULL, curpath, MAX_PATH);
	curpath[_tcslen(curpath) - 3] = L'\0';
	_tcscat_s(curpath, MAX_PATH, L"xml");
    if (!LoadConfigFile(curpath))
		return (-1);

    if (WARMGUI::CWndApp::InitialApp(hInstance, nCmdShow))
		return (-2);

	WARMGUI::CDxFactorys* factorys = WARMGUI::CDxFactorys::GetInstance();
	if (!factorys)
		return (-3);

    dispatcher = new CTestDispatcher_summer("ok");
	return (0);
}


void CSummerApp::CleanupApp()
{
    dispatcher->stop();

    Sleep(1000);

	WARMGUI::CDxFactorys::GetInstance()->ReleaseResource();
	CWndApp::CleanupApp();
}

