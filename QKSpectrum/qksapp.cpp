#include "StdAfx.h"
#include "qks_incs.h"
#include "network_inc.h"


qksapp::qksapp(void)
{
}


qksapp::~qksapp(void)
{
}

int qksapp::InitialApp(HINSTANCE hInstance, int nCmdShow)
{
	TCHAR curpath[MAX_PATH];
	////::GetCurrentDirectoryW(MAX_PATH, curpath);	////something is wired
	GetModuleFileNameW(NULL, curpath, MAX_PATH);
	curpath[_tcslen(curpath) - 3] = L'\0';
	_tcscat_s(curpath, MAX_PATH, L"xml");
    if (!LoadConfigFile(curpath))
		return (-1);

    if (CDxWinApp::InitialApp(hInstance, nCmdShow))
		return (-2);

 	Poco::Net::initializeNetwork();
	Poco::Net::initializeSSL();
    CEuclidConfig::GetInstance()->GetSng1Config(_config.getString("sng-config-file"));


    return (0);
}


void qksapp::CleanupApp()
{
	Poco::Net::uninitializeSSL();
	Poco::Net::uninitializeNetwork();

    CDxWinApp::CleanupApp();
}

