#include "StdAfx.h"
#include "warmgui.h"

namespace WARMGUI {

CDxWinApp::CDxWinApp(void)
{
}


CDxWinApp::~CDxWinApp(void)
{
}

int CDxWinApp::InitialApp(HINSTANCE hInstance, int nCmdShow)
{
	if (CWndApp::InitialApp(hInstance, nCmdShow))
		return (-1);

	CDxFactorys* factorys = CDxFactorys::GetInstance();
	if (!factorys)
		return (-2);

	return (0);
}


void CDxWinApp::CleanupApp()
{
	CDxFactorys::GetInstance()->ReleaseResource();
	CWndApp::CleanupApp();
}

}//namespace WARMGUI
