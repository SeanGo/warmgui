#include "StdAfx.h"
#include "warmgui_summer.h"
#include "TestDispatcher_summer.h"
#include "SummerApp.h"
#include "TestSummerCanvas.h"
#include "SummerAtelier.h"


CSummerAtelier::CSummerAtelier(void)
    : _bkg_canvas(0)
    , _toolbar(0)
    , _ts_canvas(0)
{
    strcpy_s(_name, MAX_PATH, "CSummerAtelier");
}


CSummerAtelier::CSummerAtelier(const char* name)
    : WARMGUI::IAtelier_summer(name)
    , _bkg_canvas(0)
    , _toolbar(0)
    , _ts_canvas(0)
{
}


CSummerAtelier::~CSummerAtelier(void)
{
}

HRESULT CSummerAtelier::init(HWND hwnd)
{
    HRESULT hr = S_OK;
    _config = the_app.GetConfig();
    if (!_config)
        hr = (-1);

    _snprintf_s(_str_conf, MAX_PATH, _TRUNCATE, "atelier-%s", _name);
    if (SUCCEEDED(hr))
        hr = WARMGUI::IAtelier_summer::init(hwnd);

    if (SUCCEEDED(hr))
        if (!(GetBitmapMaterial() && GetMyBackgroundRect()))
            hr = -4;


    _bkg_canvas = new WARMGUI::CBkgCanvas_summer("canvas-bkg");
    append_canvas(_bkg_canvas);
    _bkg_canvas->init();

    _ts_canvas = new CTestSummerCanvas("canvas-summer");
    append_canvas(_ts_canvas);
    _ts_canvas->init();

    _toolbar = new WARMGUI::CToolbar_summer("canvas-toolbar");
    append_canvas(_toolbar);
    _toolbar->init();

    the_app.get_test_dispatcher()->register_atelier(this);

    return hr;
}



DWORD WINAPI draw_time_series(LPVOID param)
{
    if (param)
        ((CTestSummerCanvas*)param)->draw_time_series();
    return (0);
}

void CSummerAtelier::draw_time_series()
{
    DWORD id_thread;

    CreateThread(0, 0, ::draw_time_series, _ts_canvas, 0, &id_thread);
}

void CSummerAtelier::disposal(RECT& rect)
{
    if (_bkg_canvas)
        _bkg_canvas->set_rect(rect);

    if (_ts_canvas)
        _ts_canvas->set_rect(rect);

    if (_toolbar)
        _toolbar->set_rect(rect);
}

void CSummerAtelier::Changed(GLYPH_CHANGED_TYPE change_type)
{
    _changed |= change_type;
}
