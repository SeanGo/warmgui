#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"
#include "SummerApp.h"
#include "SummerView.h"


CSummerApp::CSummerApp(void)
    : _ctp_disp(0)
{

}


CSummerApp::~CSummerApp(void)
{
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

    _ctp_disp = new CCtpmdDispatcher_summer("ctp-disp");
    _ctp_disp->set_config(&_config, "");
    _dispatchers.push_back(_ctp_disp);

	return (0);
}


void CSummerApp::CleanupApp()
{
    _dispatchers.stop();

    Sleep(1000);

	WARMGUI::CDxFactorys::GetInstance()->ReleaseResource();
	CWndApp::CleanupApp();
}
