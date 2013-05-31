#include "StdAfx.h"
#include "qks_incs.h"
#include "Dxut9Dialog.h"
#include "Test2.h"
#include "LoginDlg.h"

#define __TRACE_INFO__ 0
#if (__TRACE_INFO__)
#	define DEBUG_TRACE MYTRACE
#else
#	define DEBUG_TRACE(x, ...)
#endif //__TRACE_INFO__

CTest2::CTest2(void)
	: _atelier(0)
	, _config(0)
    , stop(true)
{
    _rectClient.left =  _rectClient.top = _rectClient.right = _rectClient.bottom = 0;
    const TCHAR* datafile = L"d:\\projects\\bin\\IF1008-20100810.dat";
    _pdata = ReadMarketDataFromFile(datafile, &_nDataCount);
}


CTest2::~CTest2(void)
{
    if (_pdata) freedata(_pdata);
    if (_atelier) delete _atelier;
}


int CTest2::OnCreate(LPCREATESTRUCT /*cs*/)
{
	if (InitAtelier())
		return (-1);


	return 0;
}

inline RECT * CTest2::GetClientRect()
{
	return (&_rectClient);
}

BOOL CTest2::PreCreateWindow(LPCREATESTRUCT cs)
{
	HBRUSH hbr = (HBRUSH)GetClassLong(_hwnd, GCL_HBRBACKGROUND);
	DeleteObject(hbr);
	hbr = CreateSolidBrush(RGB(0, 0, 0));
	SetClassLong(_hwnd, GCL_HBRBACKGROUND, (LONG)hbr);
	return TRUE;
}

void CTest2::OnSize(UINT /*nType*/, int cx, int cy)
{
	if (_atelier && cx > 0 && cy > 0) {
		::GetClientRect(_hwnd, &_rectClient);
		_atelier->SetRect(_rectClient);
	}
}

void CTest2::OnDraw()
{
	if (_atelier)
		_atelier->Draw();
}

int  CTest2::OnEraseBkgnd(HDC)
{
	return (0);
}


void CTest2::OnDestroy()
{
    if (!stop)
        stop = true;

    ::PostQuitMessage(0);

    Sleep(1000);
}

int CTest2::InitAtelier()
{
	_atelier = new CZenInTwiningAtelier("zen-in-twining");
	_atelier->SetConfigFile(_config);

	_atelier->InitAtelier(_hwnd, _config);
	return (0);
}

static int k = 0;

DWORD WINAPI CTest2::simulator_sending(LPVOID param)
{
    CTest2* me = (CTest2*)param;
    CTPTIMESEC ctpsec((time_t)(me->_pdata->m_time));
    
    ++k;
    if (k > 1) {
        me->stop = true;
        return (0);
    }

    for(int n = 0; n < me->_nDataCount; n++) {
        if (me->stop)
            return (1);

        DataObjectPtr dop;
        dop->InitData(sizeof(CTPMMD));
        CTPMMD ctpmmd;
        ConvertMmd2Ctpmmd(me->_pdata + n, ctpsec, &ctpmmd);

        dop->CopyData(&ctpmmd, sizeof(CTPMMD));
        me->ReDraw();
        
        if (n > 510) {
            //caluc
            Sleep(10);
        } else
            Sleep(10);
    }
    return (0);
}

void CTest2::OnLButtonUp(UINT uFlag, int x, int y)
{
	CLoginDlg* dlg = new CLoginDlg();
	dlg->ShowDialog(L"Test Dialog", 0, 0, 0, 320, 240);
	if (dlg->GetDialogValue() == 1) {
		//MYTRACE(L"username %s, password %s\n", dlg._username, dlg._password);
	}
	delete dlg;
	MYTRACE(L"Point %d %d\n", x, y);
    if (!stop) return;

    stop = false;
    hSimulator = CreateThread(0, 0, simulator_sending, this, 0, &dwSimuId);
}

void CTest2::OnRButtonUp(UINT, int , int)
{
    if (!stop)
        stop = true;
}

void CTest2::ReDraw()
{
    InvalidateRect(_hwnd, 0, FALSE);
}

