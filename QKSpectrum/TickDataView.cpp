#include "StdAfx.h"
#include "qks_incs.h"
#include "network_inc.h"

#include "Dxut9Dialog.h"
#include "TickDataView.h"
#include "LoginDlg.h"
#include "resource.h"

#include "test-unit.h"

#define __TRACE_INFO__ 0
#if (__TRACE_INFO__)
#	define DEBUG_TRACE MYTRACE
#else
#	define DEBUG_TRACE(x, ...)
#endif //__TRACE_INFO__

CTickDataView::CTickDataView(void)
	: _atelier(0)
	, _config(0)
    , _dispatchers(0)
    , _netcli(AMERICA::ClientServiceHandler::RECEIVER_TYPE_THREAD)
    , _b_gui_ready(false)
    , _calculators(0)
{
    _rectClient.left =  _rectClient.top = _rectClient.right = _rectClient.bottom = 0;
}


CTickDataView::~CTickDataView(void)
{
}

int CTickDataView::ReceiveData()
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

int CTickDataView::OnCreate(LPCREATESTRUCT /*cs*/)
{
    bool result = _qks_factory.CreateGui(_config, "realtime-ctpdata", _hwnd);
    if (!result)
        return (-1);

    _dispatchers = the_app.getDispatchers();
    _calculators = the_app.GetCalculators();

    _atelier = (CTickDataAtelier*)_qks_factory.GetAtelier();
	SetWindowLong(_hwnd, GWL_STYLE, WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW);

	int cx = (int)(GetSystemMetrics(SM_CXSCREEN) * .6f);
	int cy = (int)(GetSystemMetrics(SM_CYSCREEN) * .6f);

	MoveWindow(_hwnd, 0, 0, cx, cy, true);
	SetFocus(_hwnd);

	return 0;
}

inline RECT * CTickDataView::GetClientRect()
{
	return (&_rectClient);
}

BOOL CTickDataView::PreCreateWindow(LPCREATESTRUCT cs)
{
	HBRUSH hbr = (HBRUSH)GetClassLong(_hwnd, GCL_HBRBACKGROUND);
	DeleteObject(hbr);
	hbr = CreateSolidBrush(RGB(0, 0, 0));
	SetClassLong(_hwnd, GCL_HBRBACKGROUND, (LONG)hbr);
	return TRUE;
}

void CTickDataView::OnSize(UINT /*nType*/, int cx, int cy)
{
    //MYTRACE(L"OnSize\n");
	if (_atelier && cx > 0 && cy > 0) {
		::GetClientRect(_hwnd, &_rectClient);
		_atelier->SetRect(_rectClient);
	}
}

void CTickDataView::OnDraw()
{
    //MYTRACE(L"OnDraw\n");
	if (_atelier) {
		_atelier->Draw();
        _b_gui_ready = true;
    }
}

int  CTickDataView::OnEraseBkgnd(HDC)
{
	return (0);
}

int  CTickDataView::OnClose(WPARAM wParam, LPARAM lParam)
{
    MYTRACE(L"OnClose\n");
    Logout();
    ShowWindow(_hwnd, SW_HIDE);

    for (CalConstIter iter = _calculators->begin(); iter != _calculators->end(); iter++)
        (*iter)->stop();

    for (DispatcherConstIter iter = _dispatchers->begin(); iter != _dispatchers->end(); iter++)
        (*iter)->stop();
    return (0);
}


void CTickDataView::OnDestroy()
{
    MYTRACE(L"OnDestroy\n");

    ::PostQuitMessage(0);
    SafeDelete(_atelier);
}

void CTickDataView::OnLButtonUp(UINT uFlag, int x, int y)
{
    _atelier->OnLButtonUp(x, y);
    MYTRACE(L"CTickDataView POINT is %d %d\n", x, y);
}

void CTickDataView::OnRButtonUp(UINT, int , int)
{
}

void CTickDataView::ReDraw()
{
    InvalidateRect(_hwnd, 0, FALSE);
}

void CTickDataView::OnMouseMove(UINT /*nFlags*/, int x, int y)
{
//	_atelier->OnMouseMove(x, y);
}

int CTickDataView::OnCommand(WORD nCmdId, WORD /*nSource*/, HWND /*hwnd*/)
{
	switch(nCmdId) {
	case ID_VIEW_TOOLBAR:
		_atelier->ToggleToolbar();
		break;
    case ID_NETWORK_LOGIN:
        {
	        CLoginDlg* dlg = new CLoginDlg();
	        dlg->ShowDialog(L"Test Dialog", 0, 0, 0, 320, 240);
	        if (dlg->GetResultValue() == 1) {
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
    case ID_SHOW_ANALYST_VIEW:
        ShowAnalystView(this);
        break;
    case ID_TEST_SUMMER:
        test_generate_glyph_tree(_hwnd);
        break;
	}

	return (0);
}

//static int lllk = 0;

int CTickDataView::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
	case WM_CALUC_FINISHED:
		{
		}
		break;
    case WM_RENEW_WINDOW:
        //MYTRACE(L"WM_RENEW_WINDOW 0x%x\n", wParam);
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



int CTickDataView::Login(const char* username, const char* password)
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

int CTickDataView::Logout()
{
    _netcli.Logoff();
    return (0);
}
