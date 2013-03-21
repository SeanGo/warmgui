#include "StdAfx.h"
#include "qks_incs.h"
#include "resource.h"


RtAnalyseWnd::RtAnalyseWnd(void)
{
}


RtAnalyseWnd::~RtAnalyseWnd(void)
{
}


int RtAnalyseWnd::OnCreate(LPCREATESTRUCT cs)
{
	if (CDxWindow::OnCreate(cs))
		return (-1);

	SetWindowLong(_hwnd, GWL_STYLE, WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW);
	//SetWindowLong(_hwnd, GWL_STYLE, WS_VISIBLE | WS_CLIPCHILDREN);

	//RECT rect2 = {0, 0, 0, 0};
	////_cmdwnd.Init(_hwnd);
	////_cmdwnd.CrateCmdWnd(rect2, this);

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(_hwnd, 0, 0, cx, cy, true);
	SetFocus(_hwnd);

	return (0);
}

int RtAnalyseWnd::OnCommand(WORD nCmdId, WORD /*nSource*/, HWND /*hwnd*/)
{
	switch(nCmdId) {
	case ID_VIEW_TOOLBAR:
		break;
	}
	return (0);
}


const int HEIGHT_ATIELIA_TOP_MARGEN = 5;
const int WARMGUI_TOOLBAR_HEIGHT = 60;

