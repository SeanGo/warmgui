#include "StdAfx.h"
#include "qks_incs.h"
#include "network_inc.h"

#include "Dxut9Dialog.h"
#include "Test3.h"
#include "LoginDlg.h"
#include "resource.h"

#define __TRACE_INFO__ 0
#if (__TRACE_INFO__)
#	define DEBUG_TRACE MYTRACE
#else
#	define DEBUG_TRACE(x, ...)
#endif //__TRACE_INFO__

CTest3::CTest3(void)
	: _atelier(0)
	, _config(0)
    , _netcli(AMERICA::ClientServiceHandler::RECEIVER_TYPE_THREAD)
{
    _rectClient.left =  _rectClient.top = _rectClient.right = _rectClient.bottom = 0;
}


CTest3::~CTest3(void)
{
}


int CTest3::OnCreate(LPCREATESTRUCT /*cs*/)
{
    bool result = _zit_factory.CreateGui(_hwnd, _config, "realtime-ctpdata");
    if (!result)
        return (-1);

    _atelier = (CZenInTwiningAtelier*)_zit_factory.GetAtelier();
	//SetWindowLong(_hwnd, GWL_STYLE, WS_VISIBLE | WS_CLIPCHILDREN);

	//int cx = GetSystemMetrics(SM_CXSCREEN);
	//int cy = GetSystemMetrics(SM_CYSCREEN);

	//MoveWindow(_hwnd, 0, 0, cx, cy, true);
	//SetFocus(_hwnd);

    //_dispatcher = _zit_factory.GetDataDispathcer();
    //_th_data_receiver.start(*_dispatcher);

	return 0;
}

inline RECT * CTest3::GetClientRect()
{
	return (&_rectClient);
}

BOOL CTest3::PreCreateWindow(LPCREATESTRUCT cs)
{
	HBRUSH hbr = (HBRUSH)GetClassLong(_hwnd, GCL_HBRBACKGROUND);
	DeleteObject(hbr);
	hbr = CreateSolidBrush(RGB(0, 0, 0));
	SetClassLong(_hwnd, GCL_HBRBACKGROUND, (LONG)hbr);
	return TRUE;
}

void CTest3::OnSize(UINT /*nType*/, int cx, int cy)
{
    /*
	if (_atelier && cx > 0 && cy > 0) {
		::GetClientRect(_hwnd, &_rectClient);
		_atelier->SetRect(_rectClient);
	}
    */
}

void CTest3::OnDraw()
{
	//if (_atelier)
	//	_atelier->Draw();
}

int  CTest3::OnEraseBkgnd(HDC)
{
	return (0);
}


void CTest3::OnDestroy()
{
    Logout();
    Sleep(1000);
    ::PostQuitMessage(0);
}

void CTest3::OnLButtonUp(UINT uFlag, int x, int y)
{
    _atelier->OnLButtonUp(x, y);
}

void CTest3::OnRButtonUp(UINT, int , int)
{
}

void CTest3::ReDraw()
{
    InvalidateRect(_hwnd, 0, FALSE);
}

void CTest3::OnMouseMove(UINT /*nFlags*/, int x, int y)
{
	_atelier->OnMouseMove(x, y);
}

int CTest3::OnCommand(WORD nCmdId, WORD /*nSource*/, HWND /*hwnd*/)
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
                //_atelier->Draw();
                Login(dlg->_username, dlg->_password);
	        }
	        delete dlg;
        }
        break;
    case ID_NETWORK_LOGOFF:
        Logout();
        break;
	}
	return (0);
}

//static int lllk = 0;

int CTest3::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	int argc = (int)wParam;
	TCHAR ** argv = (TCHAR**)lParam;

	switch(message) {
	case WM_CALUC_FINISHED:
		{
			DataObjectPtr dataptr = DataObjectPtr((DataObject*)wParam);
            ((CZenInTwiningAtelier*)_atelier)->ReceiveData(dataptr, (const char*)lParam);
            ReDraw();
            //MYTRACE(_T("%d\n"), lllk++);
			break;
		}
		break;
    }

    return (0);
}



int CTest3::Login(const char* username, const char* password)
{
    if (!(_netcli.Connect(
        _config->getString("network.Client-Key").c_str(),
		_config->getString("network.Client-CRT").c_str(),
		_config->getString("network.CA-CRT").c_str(),
        _config->getString("network.rtcalc-server-address").c_str(),
        _dispatcher->GetThreadId())))
    {
        if (_netcli.Login(username, password))
            return (-1);
        else
            return (0);
    } else {
        return (-1);
    }
}

int CTest3::Logout()
{
    _netcli.Logoff();
    return (0);
}
