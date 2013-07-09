#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"
#include "SummerApp.h"
#include "ZitPredictView.h"
#include "SummerView.h"


CSummerApp::CSummerApp(void)
{

}


CSummerApp::~CSummerApp(void)
{
}

int CSummerApp::InitialApp(HINSTANCE hInstance, int nCmdShow)
{
	HANDLE hProcess = GetCurrentProcess();
	BOOL bPriorityBoost;
	BOOL bResult = GetProcessPriorityBoost (hProcess, &bPriorityBoost);
	DWORD dwPriority = GetPriorityClass(hProcess);
	if (dwPriority != REALTIME_PRIORITY_CLASS) {
		bResult = SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
		if (!bResult)
            return (-4);
	}

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

    CCtpmdDispatcher_summer* _ctp_disp = new CCtpmdDispatcher_summer("ctp-disp");
    _ctp_disp->set_config(&_config, "");
    _dispatchers.push_back(_ctp_disp);

    CDaysCtpmmdCalculator* _approx_cal = new CDaysCtpmmdCalculator("days-approx");
    _approx_cal->SetConfig(&_config, "");
    _dispatchers.reg_cal(_approx_cal, "ctp-disp");
    _calculators.push_back(_approx_cal);

    CZitPredictCalculator* _zit_prdct = new CZitPredictCalculator("zit-prdct");
    _zit_prdct->SetConfig(&_config, "");
    _dispatchers.reg_cal(_zit_prdct, "ctp-disp");
    _calculators.push_back(_zit_prdct);

    //_calculators.start();
	return (0);
}


void CSummerApp::CleanupApp()
{
    _calculators.stop();
    Sleep(1000);

    _dispatchers.stop();
    Sleep(1000);

	WARMGUI::CDxFactorys::GetInstance()->ReleaseResource();
	CWndApp::CleanupApp();
}
