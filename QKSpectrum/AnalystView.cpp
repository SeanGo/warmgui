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
{
    _rectClient.left =  _rectClient.top = _rectClient.right = _rectClient.bottom = 0;
}


CAnalystView::~CAnalystView(void)
{
}

int CAnalystView::ReceiveData()
{
    return (0);
}

int CAnalystView::get_param_config()
{
    WARMGUI::CWarmguiConfig* config = 0;
    try {
        config = new WARMGUI::CWarmguiConfig(_config->getString("euclid-analyst-params-file"));
        if (!config) return (-1);

        char temp[MAX_PATH];
        strcpy_s(temp, MAX_PATH, "histroy-data.rt-path");
        strcpy_s(_analyst.get_ctpdata()->rt_path, MAX_PATH, config->getString(temp).c_str());
        strcpy_s(temp, MAX_PATH, "histroy-data.m1-path");
        strcpy_s(_analyst.get_ctpdata()->m1_path, MAX_PATH, config->getString(temp).c_str());
        strcpy_s(temp, MAX_PATH, "histroy-data.m5-path");
        strcpy_s(_analyst.get_ctpdata()->m5_path, MAX_PATH, config->getString(temp).c_str());
        strcpy_s(temp, MAX_PATH, "histroy-data.m15-path");
        strcpy_s(_analyst.get_ctpdata()->m15_path, MAX_PATH, config->getString(temp).c_str());

        EUCLID::ANALYST_PARAM* ap = _analyst.get_param();

        ap->_zp.len                 = config->getInt("zit.len"               );
        ap->_zp.down_sample         = config->getInt("zit.down-sample"       );
   		ap->_zp.wavelet_level       = config->getInt("zit.wavelet-level"     );
		ap->_zp.hilvl_filter_type   = config->getInt("zit.hilvl-filter-type" );
		ap->_zp.low_filter_level    = config->getInt("zit.low-filter-level"  );
		ap->_zp.hi_filter_level     = config->getInt("zit.hi-filter-level"   );
        ap->_zp.extr_len            = config->getInt("zit.extr-len"          );
        ap->_zp.price_slope_scaling = config->getDouble("zit.price-slope-scaling");

        ap->_short_pp.appx_len            = config->getInt("short-term.appx.len");
        ap->_short_pp.appx_down_sample    = config->getInt("short-term.appx.down-sample");
        ap->_short_pp.appx_low_appx       = config->getInt("short-term.appx.low-appx");
        ap->_short_pp.appx_high_appx      = config->getInt("short-term.appx.high-appx");
        ap->_short_pp.prdt_feture_len     = config->getInt("short-term.predict.feture-len");
        ap->_short_pp.prdt_coff_len       = config->getInt("short-term.predict.coff-len");
        ap->_short_pp.extr_len            = config->getInt("short-term.appx.extr-len"   );
        ap->_short_pp.wavelet             = (WAVELET_TYPE)config->getWaveletType("short-term.appx.wavelet");
        ap->_short_pp.price_slope_scaling = config->getDouble("short-term.appx.price-slope-scaling");

        ap->_long_pp.appx_len            = config->getInt("long-term.appx.len");
        ap->_long_pp.appx_down_sample    = config->getInt("long-term.appx.down-sample");
        ap->_long_pp.appx_low_appx       = config->getInt("long-term.appx.low-appx");
        ap->_long_pp.appx_high_appx      = config->getInt("long-term.appx.high-appx");
        ap->_long_pp.prdt_feture_len     = config->getInt("long-term.predict.feture-len");
        ap->_long_pp.prdt_coff_len       = config->getInt("long-term.predict.coff-len");
        ap->_long_pp.wavelet             = (WAVELET_TYPE)config->getWaveletType("long-term.appx.wavelet");
        ap->_long_pp.extr_len            = config->getInt("long-term.appx.extr-len"   );
        ap->_long_pp.price_slope_scaling = config->getDouble("long-term.appx.price-slope-scaling");

        ap->_volume_pp.min_len             = config->getInt("volume.min-len");
        ap->_volume_pp.max_len             = config->getInt("volume.max-len");
        ap->_volume_pp.level               = config->getInt("volume.level");
        ap->_volume_pp.wavelet             = (WAVELET_TYPE)config->getWaveletType("volume.wavelet");
        ap->_volume_pp.extr_len            = config->getInt("volume.extr-len"          );
        ap->_volume_pp.price_slope_scaling = config->getDouble("volume.price-slope-scaling");

        ap->_interest_pp.min_len             = config->getInt("interest.min-len");
        ap->_interest_pp.max_len             = config->getInt("interest.max-len");
        ap->_interest_pp.level               = config->getInt("interest.level");
        ap->_interest_pp.wavelet             = (WAVELET_TYPE)config->getWaveletType("interest.wavelet");
        ap->_interest_pp.extr_len            = config->getInt("interest.extr-len"          );
        ap->_interest_pp.price_slope_scaling = config->getDouble("interest.price-slope-scaling");

        _analyst.init_time_series();
        _analyst.set_params();
        SafeDelete(config);
        return (0);
    }catch(...) {
        SafeDelete(config);
        return (-2);
    }
}

int CAnalystView::OnCreate(LPCREATESTRUCT /*cs*/)
{
    //atelier-euclid in xml
    if (get_param_config())
        return (-1);
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

void print_result(int npos, HRESULT hr, int analysis)
{
    MYTRACE(L"%d ", npos);
    if (hr)
        MYTRACE(L"error code %d\n", hr);
    else {
        if (analysis) {
            if (analysis & HAS_ZIT_ANALYSIS)
                MYTRACE(L"zit ");
            if (analysis & HAS_SHORT_ANALYSIS)
                MYTRACE(L"short ");
            if (analysis & HAS_LONG_ANALYSIS)
                MYTRACE(L"long ");
            if (analysis & HAS_VOLUME_ANALYSIS)
                MYTRACE(L"volume ");
            if (analysis & HAS_INTEREST_ANALYSIS)
                MYTRACE(L"interest ");
        } else {
            MYTRACE(L"no error but no analysis");
        }
        MYTRACE(L"\n");
    }
}

int CAnalystView::open_history_data()
{
    COpenHistoryDataDlg* dlgtest = new COpenHistoryDataDlg();
    dlgtest->set_data_path(_analyst.get_ctpdata()->rt_path);
    int open_result = 0;
    dlgtest->ShowDialog(L"Open History Data", 0, 0, 0, 320, 240);
    if (dlgtest->GetResultValue()) {
        char filepath[MAX_PATH];
        CChineseCodeLib::UnicodeToGB2312(filepath, MAX_PATH, dlgtest->filepath);
        open_result = _analyst.open_data(filepath);
    }
    delete dlgtest;

    if (!open_result) {
        //calculate something
        {//test cal all data
#ifdef _DEBUG
         //   EUCLID::CEuclidAnalyst::AnalystData* ad = _analyst.get_ctpdata();
         //   for (int i = 0; i < ad->rtdata._length; i++) {
         //       HRESULT hr = _analyst.analysis(i);
         //       print_result(i, hr, _analyst.get_analysis_result());
         //   }
#endif //_DEBUG
        }

        {//draw tickdata
            _atelier->set_analyst(&_analyst);
            _atelier->draw_data();
        }
    }

    return (0);
}
