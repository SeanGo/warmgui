#include "StdAfx.h"
#include "summer.h"
#include "SummerApp.h"

extern CSummerApp the_app;


CZitPredictAtelier::CZitPredictAtelier(void)
    : _bkg_canvas(0)
    , _toolbar(0)
    , _ctp_canvas(0)
    , _tick_canvas(0)
{
    strcpy_s(_name, MAX_PATH, "CZitPredictAtelier");
}


CZitPredictAtelier::CZitPredictAtelier(const char* name)
    : WARMGUI::IAtelier_summer(name)
    , _bkg_canvas(0)
    , _toolbar(0)
    , _ctp_canvas(0)
{
}


CZitPredictAtelier::~CZitPredictAtelier(void)
{
}

HRESULT CZitPredictAtelier::init(HWND hwnd)
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

    _ctp_canvas = new CCtpmdCanvas_summer("canvas-summer");
    append_canvas(_ctp_canvas);
    _ctp_canvas->init();

    _tick_canvas = new CTickdataCanvas("canvas-tick");
    append_canvas(_tick_canvas);
    _tick_canvas->init();

    _toolbar = new WARMGUI::CToolbar_summer("canvas-toolbar");
    append_canvas(_toolbar);
    _toolbar->init();

    return hr;
}



void CZitPredictAtelier::disposal(RECT& rect)
{
    if (_bkg_canvas)
        _bkg_canvas->set_rect(rect);

    RECT crect = rect;
    crect.right = (int)((float)_rect.right * .618f - 2.0f);
    if (_ctp_canvas)
        _ctp_canvas->set_rect(crect);

    crect.left = crect.right + 4, crect.right = rect.right;
    if (_tick_canvas)
        _tick_canvas->set_rect(crect);

    if (_toolbar)
        _toolbar->set_rect(rect);
}

void CZitPredictAtelier::Changed(GLYPH_CHANGED_TYPE change_type)
{
    _changed |= change_type;
}
