#include "StdAfx.h"
#include "warmgui_incs.h"

namespace WARMGUI {


#define TOOLBAR_HEIGHT 60

#define __TRACE_INFO__ 0
#if (__TRACE_INFO__)
#	define DEBUG_TRACE MYTRACE
#else
#	define DEBUG_TRACE(x, ...)
#endif //__TRACE_INFO__

CDxWindow::CDxWindow(void)
	: _atelier(0)
	, _rectClient(NULL_RECT)
	, _config(0)
{
}


CDxWindow::~CDxWindow(void)
{
    if (_atelier) delete _atelier;
}


int CDxWindow::OnCreate(LPCREATESTRUCT /*cs*/)
{
	if (InitAtelier())
		return (-1);

	return 0;
}

inline RECT * CDxWindow::GetClientRect()
{
	return (&_rectClient);
}

BOOL CDxWindow::PreCreateWindow(LPCREATESTRUCT cs)
{
	HBRUSH hbr = (HBRUSH)GetClassLong(_hwnd, GCL_HBRBACKGROUND);
	DeleteObject(hbr);
	hbr = CreateSolidBrush(RGB(0, 0, 0));
	SetClassLong(_hwnd, GCL_HBRBACKGROUND, (LONG)hbr);
	return TRUE;
}

void CDxWindow::OnSize(UINT /*nType*/, int cx, int cy)
{
	if (_atelier && cx > 0 && cy > 0) {
		::GetClientRect(_hwnd, &_rectClient);
		_atelier->SetRect(_rectClient);
	}
}

void CDxWindow::OnDraw()
{
	if (_atelier)
		_atelier->Draw();
}

int  CDxWindow::OnEraseBkgnd(HDC)
{
	return (0);
}


void CDxWindow::OnDestroy()
{
	::PostQuitMessage(0);
}

int CDxWindow::InitAtelier()
{
	_atelier = new COneAtelier();
	_atelier->SetConfigFile(_config);
	if (FAILED(_atelier->CreateRenderTarget(_hwnd)))
		return (-2);

	_atelier->InitAtelier(_hwnd, _config);
	return (0);
}

} //namespace WARMGUI
