#include "StdAfx.h"
#include "warmgui_summer.h"
#include "SummerApp.h"
#include "SummerAtelier.h"

extern CSummerApp the_app;

CSummerAtelier::CSummerAtelier(void)
    : _bkg_canvas(0)
    , _toolbar(0)
{
    strcpy_s(_name, MAX_PATH, "CSummerAtelier");
}


CSummerAtelier::CSummerAtelier(const char* name)
    : WARMGUI::IAtelier_summer(name)
    , _bkg_canvas(0)
    , _toolbar(0)
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


    _bkg_canvas = new WARMGUI::CBkgCanvas_summer("bkg-canvas");
    append_canvas(_bkg_canvas);
    _bkg_canvas->init();

    _toolbar = new WARMGUI::CToolbar_summer("toolbar");
    append_canvas(_toolbar);
    _toolbar->init();
    return hr;
}


void CSummerAtelier::disposal(RECT& rect)
{
    if (_bkg_canvas)
        _bkg_canvas->set_rect(rect);

    if (_toolbar)
        _toolbar->set_rect(rect);
}
