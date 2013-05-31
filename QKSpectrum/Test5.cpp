#include "StdAfx.h"
#include "qks_incs.h"
#include "network_inc.h"

#include "Dxut9Dialog.h"
#include "Test5.h"
#include "LoginDlg.h"
#include "resource.h"

#define __TRACE_INFO__ 0
#if (__TRACE_INFO__)
#	define DEBUG_TRACE MYTRACE
#else
#	define DEBUG_TRACE(x, ...)
#endif //__TRACE_INFO__

CTest5::CTest5(void)
	: _atelier(0)
	, _config(0)
    , _netcli(AMERICA::ClientServiceHandler::RECEIVER_TYPE_THREAD)
    , _dispatchers(0)
    , _b_gui_ready(false)
    , _calculators(0)
{
    _rectClient.left =  _rectClient.top = _rectClient.right = _rectClient.bottom = 0;
}


CTest5::~CTest5(void)
{
}

int CTest5::ReceiveData()
{
    if (_b_gui_ready) {
        for (DispatcherConstIter iter = _dispatchers->begin(); iter != _dispatchers->end(); iter++)
            (*iter)->start();

        for (CalConstIter iter = _calculators->begin(); iter != _calculators->end(); iter++)
            (*iter)->start();

        return (0);
    } else
        return (-1);
}

int CTest5::OnCreate(LPCREATESTRUCT /*cs*/)
{
    bool result = _zit_factory.CreateGui(_config, "realtime-ctpdata", _hwnd);
    if (!result)
        return (-1);

    _atelier = (CZITAtelier*)_zit_factory.GetAtelier();
    //_dispatchers = _zit_factory.getDispatchers();
    //_calculators = _zit_factory.GetCalculators();
	SetWindowLong(_hwnd, GWL_STYLE, WS_VISIBLE | WS_CLIPCHILDREN);

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(_hwnd, 0, 0, cx, cy, true);
	SetFocus(_hwnd);

	return 0;
}

inline RECT * CTest5::GetClientRect()
{
	return (&_rectClient);
}

BOOL CTest5::PreCreateWindow(LPCREATESTRUCT cs)
{
	HBRUSH hbr = (HBRUSH)GetClassLong(_hwnd, GCL_HBRBACKGROUND);
	DeleteObject(hbr);
	hbr = CreateSolidBrush(RGB(0, 0, 0));
	SetClassLong(_hwnd, GCL_HBRBACKGROUND, (LONG)hbr);
	return TRUE;
}

void CTest5::OnSize(UINT /*nType*/, int cx, int cy)
{
    //MYTRACE(L"OnSize\n");
	if (_atelier && cx > 0 && cy > 0) {
		::GetClientRect(_hwnd, &_rectClient);
		_atelier->SetRect(_rectClient);
	}
}

void CTest5::OnDraw()
{
    //MYTRACE(L"OnDraw\n");
	if (_atelier) {
		_atelier->Draw();
        _b_gui_ready = true;
    }
}

int  CTest5::OnEraseBkgnd(HDC)
{
	return (0);
}


void CTest5::OnDestroy()
{
    Logout();
    Sleep(1000);

    for (CalConstIter iter = _calculators->begin(); iter != _calculators->end(); iter++)
        (*iter)->stop();

    for (DispatcherConstIter iter = _dispatchers->begin(); iter != _dispatchers->end(); iter++)
        (*iter)->stop();

    Sleep(1000);

    ::PostQuitMessage(0);
}

void CTest5::OnLButtonUp(UINT uFlag, int x, int y)
{
    _atelier->OnLButtonUp(x, y);
    MYTRACE(L"POINT is %d %d\n", x, y);
}

void CTest5::OnRButtonUp(UINT, int , int)
{
}

void CTest5::ReDraw()
{
    InvalidateRect(_hwnd, 0, FALSE);
}

void CTest5::OnMouseMove(UINT /*nFlags*/, int x, int y)
{
//	_atelier->OnMouseMove(x, y);
}

int CTest5::OnCommand(WORD nCmdId, WORD /*nSource*/, HWND /*hwnd*/)
{
	switch(nCmdId) {
	case ID_VIEW_TOOLBAR:
		_atelier->ToggleToolbar();
		break;
    case ID_NETWORK_LOGIN:
        {
	        CLoginDlg* dlg = new CLoginDlg();
	        dlg->ShowDialog(L"Test Dialog", 0, 0, 0, 320, 240);
	        if (dlg->GetDialogValue() == 1) {
                _atelier->Draw();
                Login(dlg->_username, dlg->_password);
	        }
	        delete dlg;
        }
        break;
    case ID_NETWORK_LOGOFF:
        Logout();
        break;
    case ID_CTPMMD_RECEIVER:
        ReceiveData();
        break;
    case ID_SHOW_EUCLID_VIEW:
        ShowEuclidView(this);
        break;
	}

	return (0);
}

//static int lllk = 0;

int CTest5::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	int argc = (int)wParam;
	TCHAR ** argv = (TCHAR**)lParam;

	switch(message) {
	case WM_CALUC_FINISHED:
		{
		}
		break;
    case WM_RENEW_WINDOW:
        MYTRACE(L"WM_RENEW_WINDOW 0x%x\n", wParam);
        if (_atelier->HadChanged(GLYPH_CHANGED_TYPE_ATELIER_RESIZE))
            break;
        else {
            _atelier->Changed((GLYPH_CHANGED_TYPE)wParam);
            ReDraw();
        }
        break;
    }

    return (0);
}



int CTest5::Login(const char* username, const char* password)
{
    if (!(_netcli.Connect(
        _config->getString("network.Client-Key").c_str(),
		_config->getString("network.Client-CRT").c_str(),
		_config->getString("network.CA-CRT").c_str(),
        _config->getString("network.rtcalc-server-address").c_str(),
        _dispatchers->getDispatcher("america-qks")->GetThreadId())))
    {
        if (_netcli.Login(username, password))
            return (-1);
        else
            return (0);
    } else {
        return (-1);
    }
}

int CTest5::Logout()
{
    _netcli.Logoff();
    return (0);
}
