#include "StdAfx.h"
#include "warmgui_summer.h"
#include "TestDispatcher_summer.h"
#include "SummerApp.h"


CSummerApp::CSummerApp(void)
    : dispatcher_1(0)
    , dispatcher_2(0)
    , dispatcher_3(0)
    , dispatcher_4(0)
{

}


CSummerApp::~CSummerApp(void)
{
    SafeDelete(dispatcher_1);
    SafeDelete(dispatcher_2);
    SafeDelete(dispatcher_3);
    SafeDelete(dispatcher_4);
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

    dispatcher_1 = new CTestDispatcher_summer("ok1", 1);
    dispatcher_2 = new CTestDispatcher_summer("ok2", 2);
    dispatcher_3 = new CTestDispatcher_summer("ok3", 3);
    dispatcher_4 = new CTestDispatcher_summer("ok4", 4);

	return (0);
}


void CSummerApp::CleanupApp()
{
    dispatcher_1->stop();
    dispatcher_2->stop();
    dispatcher_3->stop();
    dispatcher_4->stop();

    Sleep(1000);

	WARMGUI::CDxFactorys::GetInstance()->ReleaseResource();
	CWndApp::CleanupApp();
}

void CSummerApp::regester_graph(WARMGUI::IDataGraph_summer* graph)
{
    dispatcher_1->reg_data_graph(graph);
    dispatcher_2->reg_data_graph(graph);
    dispatcher_3->reg_data_graph(graph);
    dispatcher_4->reg_data_graph(graph);
}

void CSummerApp::dispathcer_start()
{
    dispatcher_4->start();
    dispatcher_3->start();
    dispatcher_2->start();
    dispatcher_1->start();
}

