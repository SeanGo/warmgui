#include "StdAfx.h"
#include "qks_incs.h"
#include "Dxut9Dialog.h"
#include "resource.h"
#include "OpenHistoryDataDlg.h"
#include "LoginDlg.h"

#define __TRACE_INFO__ 0
#if (__TRACE_INFO__)
#	define DEBUG_TRACE MYTRACE
#else
#	define DEBUG_TRACE(x, ...)
#endif //__TRACE_INFO__

CAnalystView::CAnalystView(void)
	: _atelier(0)
	, _config(0)
    , _dispatchers(0)
    , _calculators(0)
    , _b_gui_ready(false)
    , _command_state(COMMAND_STATE_MOUSE_OVER)
    , _history_dc(0)
    , _analyst(0)
{
    _rectClient.left =  _rectClient.top = _rectClient.right = _rectClient.bottom = 0;
}


CAnalystView::~CAnalystView(void)
{
    SafeDelete(_analyst);
}

int CAnalystView::ReceiveData()
{
    return (0);
}

int CAnalystView::OnCreate(LPCREATESTRUCT /*cs*/)
{
    //atelier-euclid in xml
    bool result = _qks_factory.CreateGui(_config, "analyst", _hwnd);
    if (!result)
        return (-1);

    _dispatchers = the_app.getDispatchers();
    _calculators = the_app.GetCalculators();

    _atelier = (CAnalystAtelier*)_qks_factory.GetAtelier();

    SetWindowLong(_hwnd, GWL_STYLE, WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW);

	int cx = GetSystemMetrics(SM_CXSCREEN) / 2;
	int cy = GetSystemMetrics(SM_CYSCREEN) / 2;

	MoveWindow(_hwnd, cx, cy, cx, cy, true);
	SetFocus(_hwnd);

    _history_dc = (CHistoryDataContainer*)the_app.getDataContainer("history-data");
	return 0;
}

inline RECT * CAnalystView::GetClientRect()
{
	return (&_rectClient);
}

BOOL CAnalystView::PreCreateWindow(LPCREATESTRUCT cs)
{
	HBRUSH hbr = (HBRUSH)GetClassLong(_hwnd, GCL_HBRBACKGROUND);
	DeleteObject(hbr);
	hbr = CreateSolidBrush(RGB(0, 0, 0));
	SetClassLong(_hwnd, GCL_HBRBACKGROUND, (LONG)hbr);
	return TRUE;
}

void CAnalystView::OnSize(UINT /*nType*/, int cx, int cy)
{
    //MYTRACE(L"OnSize\n");
	if (_atelier && cx > 0 && cy > 0) {
		::GetClientRect(_hwnd, &_rectClient);
		_atelier->SetRect(_rectClient);
	}
}

void CAnalystView::OnDraw()
{
    //MYTRACE(L"OnDraw\n");
	if (_atelier) {
		_atelier->Draw();
        _b_gui_ready = true;
    }
}

int  CAnalystView::OnEraseBkgnd(HDC)
{
	return (0);
}


void CAnalystView::OnDestroy()
{
    MYTRACE(L"CAnalystView OnDestroy\n");
    ShowWindow(_hwnd, SW_HIDE);
    SafeDelete(_atelier);
    _hwnd = 0;
}

void CAnalystView::OnLButtonUp(UINT uFlag, int x, int y)
{
    _atelier->OnLButtonUp(x, y);
    MYTRACE(L"CAnalystView OnLButtonUp POINT is %d %d\n", x, y);
}

void CAnalystView::OnRButtonUp(UINT, int x, int y)
{
}

void CAnalystView::OnLButtonDown(UINT uFlag, int x, int y)
{
    MYTRACE(L"CAnalystView OnLButtonDown POINT is %d %d\n", x, y);
}

void CAnalystView::OnRButtonDown(UINT, int x, int y)
{
}

void CAnalystView::ReDraw()
{
    InvalidateRect(_hwnd, 0, FALSE);
}

void CAnalystView::OnMouseMove(UINT /*nFlags*/, int x, int y)
{
//	_atelier->OnMouseMove(x, y);
}

int CAnalystView::OnCommand(WORD nCmdId, WORD /*nSource*/, HWND /*hwnd*/)
{
	switch(nCmdId) {
	case ID_VIEW_TOOLBAR:
		_atelier->ToggleToolbar();
		break;
    case ID_OPEN_HISTORY_DATA:
        return open_history_data();
    case ID_ZOOM_IN:
        _atelier->GetSelectedCanvas();
        break;
    case ID_ZOOM_OUT:
        break;
    case ID_ZOOM_FREE:
        if (_command_state == COMMAND_STATE_ZOOM)
            _command_state = COMMAND_STATE_MOUSE_OVER;
        else if (_command_state == COMMAND_STATE_MOUSE_OVER)
            _command_state = COMMAND_STATE_ZOOM;
        break;
    case ID_PREDICTOR:
        if (_command_state == COMMAND_STATE_PREDICTOR)
            _command_state = COMMAND_STATE_MOUSE_OVER;
        else if (_command_state == COMMAND_STATE_MOUSE_OVER)
            _command_state = COMMAND_STATE_PREDICTOR;
        break;
	}
    return (0);
}

//static int lllk = 0;

int CAnalystView::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
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

int CAnalystView::open_history_data()
{
    COpenHistoryDataDlg* dlgtest = new COpenHistoryDataDlg();
    dlgtest->set_data_path(_history_dc->get_history_data_path());
    int open_result = 0;
    dlgtest->ShowDialog(L"Open History Data", 0, 0, 0, 320, 240);
    if (dlgtest->GetResultValue()) {
        open_result = _history_dc->OpenTickData(dlgtest->filepath);
    }
    delete dlgtest;

    if (open_result > 0) {
        //calculate something
        if (!_analyst) {
            _analyst = new CEuclid();
            if (_analyst->set_config()) {
            }
        }
    }

    return (0);
}
