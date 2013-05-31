#include "StdAfx.h"
#include "qks_incs.h"
#include "Test1Atelier.h"
#include "Test1.h"


#define __TRACE_INFO__ 0
#if (__TRACE_INFO__)
#	define DEBUG_TRACE MYTRACE
#else
#	define DEBUG_TRACE(x, ...)
#endif //__TRACE_INFO__

CTest1::CTest1(void)
	: _atelier(0)
	, _config(0)
    , stop(true)
    , _ctpmmd(0)
{
    _rectClient.left =  _rectClient.top = _rectClient.right = _rectClient.bottom = 0;
    const TCHAR* datafile = L"E:\\datas-old\\ctpdata\\trade\\IF1008-20100810.dat";
    _pdata = ReadMarketDataFromFile(datafile, &_nDataCount);
    //_ctpmmd = new CTPMMD[_nDataCount];
    //memset(_ctpmmd, 0, sizeof(CTPMMD) * _nDataCount);
}


CTest1::~CTest1(void)
{
    if (_ctpmmd) delete _ctpmmd;
    if (_pdata) freedata(_pdata);
    if (_atelier) delete _atelier;
}


int CTest1::OnCreate(LPCREATESTRUCT /*cs*/)
{
	if (InitAtelier())
		return (-1);

    //_data_container->RegisterCanvass(_atelier);
    //    _atelier->SetDataContanier();


    return 0;
}

inline RECT * CTest1::GetClientRect()
{
	return (&_rectClient);
}

BOOL CTest1::PreCreateWindow(LPCREATESTRUCT cs)
{
	HBRUSH hbr = (HBRUSH)GetClassLong(_hwnd, GCL_HBRBACKGROUND);
	DeleteObject(hbr);
	hbr = CreateSolidBrush(RGB(0, 0, 0));
	SetClassLong(_hwnd, GCL_HBRBACKGROUND, (LONG)hbr);
	return TRUE;
}

void CTest1::OnSize(UINT /*nType*/, int cx, int cy)
{
	if (_atelier && cx > 0 && cy > 0) {
		::GetClientRect(_hwnd, &_rectClient);
		_atelier->SetRect(_rectClient);
	}
}

void CTest1::OnDraw()
{
	if (_atelier)
		_atelier->Draw();
}

int  CTest1::OnEraseBkgnd(HDC)
{
	return (0);
}


void CTest1::OnDestroy()
{
    if (!stop)
        stop = true;

    ::PostQuitMessage(0);

    Sleep(1000);
}

int CTest1::InitAtelier()
{
	_atelier = new CTest1Atelier();
	_atelier->SetConfigFile(_config);
	if (FAILED(_atelier->CreateRenderTarget(_hwnd)))
		return (-2);

	_atelier->InitAtelier(_hwnd, _config);
	return (0);
}

static int k = 0;

DWORD WINAPI CTest1::simulator_sending(LPVOID param)
{
    CTest1* me = (CTest1*)param;
    //WARMGUI::CPVICanvas* pvi = me->_atelier->GetPVICanvas();
    CTPTIMESEC ctpsec((time_t)(me->_pdata->m_time));
    
    ++k;
    if (k > 1) {
        me->stop = true;
        return (0);
    }

    for(int n = 0; n < me->_nDataCount; n++) {
        if (me->stop)
            return (1);

        //ConvertMmd2Ctpmmd(me->_pdata + n, ctpsec, me->_ctpmmd + n);

        DataObjectPtr dop;
        dop->InitData(sizeof(CTPMMD));
        CTPMMD ctpmmd;
        ConvertMmd2Ctpmmd(me->_pdata + n, ctpsec, &ctpmmd);

        dop->CopyData(&ctpmmd, sizeof(CTPMMD));

        me->ReDraw(GLYPH_CHANGED_TYPE_NONE);
        
        if (n > 510) {
            //caluc
            Sleep(10);
        } else
            Sleep(10);
    }
    return (0);
}

void CTest1::OnLButtonUp(UINT uFlag, int x, int y)
{
    if (!stop) return;

    stop = false;
    hSimulator = CreateThread(0, 0, simulator_sending, this, 0, &dwSimuId);
}

void CTest1::OnRButtonUp(UINT, int , int)
{
    if (!stop)
        stop = true;
}

void CTest1::ReDraw(GLYPH_CHANGED_TYPE changed_type)
{
    InvalidateRect(_hwnd, 0, FALSE);
}

