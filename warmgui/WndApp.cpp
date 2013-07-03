#include "StdAfx.h"
#include "utility.h"
#include "ChineseCodeLib.h"
#include "warmgui_summer.h"
#include "WarmguiConfig.h"
#include "WndApp.h"
#include "Window.h"

namespace WARMGUI {

CWndApp::CWndApp(void)
	: _hInstanceMutex(0)
	, _hInstance(0)
{
}


CWndApp::~CWndApp(void)
{
	if (_hInstanceMutex)
		CloseHandle(_hInstanceMutex);
}

int CWndApp::UniqueInstance(TCHAR * appname)
{
	//an instance exists already
	_hInstanceMutex = ::OpenMutexW(SYNCHRONIZE, TRUE, appname);
	if (_hInstanceMutex)
		return (-1);

	//create new instance
	_hInstanceMutex = ::CreateMutexW(0, FALSE, appname);
	if (!_hInstanceMutex)
		return (-2);

	return (0);
}

int CWndApp::InitialApp(HINSTANCE hInstance, int /*nCmdShow*/)
{
	_hInstance = hInstance;

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX), InitCtrls.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&InitCtrls);

    // Ignoring the return value because we want to continue running even in the
    // unlikely event that HeapSetInformation fails.
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
    CoInitialize(NULL);

	return (0);
}


void CWndApp::CleanupApp()
{
	CoUninitialize();
}

bool CWndApp::LoadConfigFile(const TCHAR* filepath)
{
	//the_app.LoadConfigure(curpath);
	char* file = new char[_tcslen(filepath) * 3 + 1];
	CChineseCodeLib::UnicodeToGB2312(file, _tcslen(filepath) * 3 + 1, (WCHAR*)filepath);
	try {
		_config.load(file);
		delete file;
		return true;
	} catch (Poco::PathNotFoundException& /*pnfe*/) {
	} catch (Poco::OpenFileException& /*ofe*/) {
	} catch (Poco::FileNotFoundException& /*fnfe*/) {
	}

	MessageBox(NULL, L"Open Config File Error", L"CWndApp", MB_OK);
	delete file;
	return false;
}


} //namespace WARMGUI
