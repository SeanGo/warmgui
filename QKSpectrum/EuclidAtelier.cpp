#include "StdAfx.h"
#include "qks_incs.h"


CEuclidAtelier::CEuclidAtelier(const char* name)
    :     _canvas_imgbkg(0)
    , _canvas_recent_zit(0)
    , _canvas_short_term(0)
    ,  _canvas_long_term(0)
    ,     _canvas_1m_zit(0)
    ,     _canvas_5m_zit(0)
    ,    _canvas_15m_zit(0)
    ,           _toolbar(0)
{
    setClass();
}


CEuclidAtelier::~CEuclidAtelier(void)
{
}

HRESULT CEuclidAtelier::InitAtelier(HWND hwnd, WARMGUI::CWarmguiConfig* config)
{
	HRESULT hr = IAtelier::InitAtelier(hwnd, config);
    if (SUCCEEDED(hr))
        if (!(GetBitmapMaterial() && GetMyBackgroundRect()))
            hr = -1;

	/*
    if (SUCCEEDED(hr) && GetBitmapMaterial()) {
		_canvasImgbkg = new WARMGUI::CBkgCanvas("bkgcanvas");
		_canvasImgbkg->SetBitmap(&_appbmp);
		Insert(_canvasImgbkg);
	}
    */
	return hr;}

void CEuclidAtelier::ToggleToolbar()
{
}

inline bool CEuclidAtelier::GetAllCanvas()
{
    CanvasIter iter    = _canvasses.begin();
    _canvas_imgbkg     = (WARMGUI::CBkgCanvas *) (*iter); ++iter;
    _canvas_recent_zit = (CZitCanvas*)   (*iter); ++iter;
    _canvas_short_term = (CEuclidCanvas*)(*iter); ++iter;
    _canvas_long_term  = (CEuclidCanvas*)(*iter); ++iter;
    _canvas_1m_zit     = (CZitCanvas*)(*iter); ++iter;
    _canvas_5m_zit     = (CZitCanvas*)(*iter); ++iter;
    _canvas_15m_zit    = (CZitCanvas*)(*iter); ++iter;
    _toolbar           = (WARMGUI::CToolbar*)(*iter);;

    //it is a data-canvas, register to container
    for(CanvasIter canvas_iter = _canvasses.begin(); canvas_iter != _canvasses.end(); canvas_iter++) {
        if ((*canvas_iter)->isClass("IDataCanvas")) {

            for (DCConstIter iter = the_app.getDataContainers()->begin(); iter != the_app.getDataContainers()->end(); iter++) {
                if ((*iter)->isme(((IDataCanvas*) (*canvas_iter))->getContainerName()))
                    (*iter)->RegisterCanvas((IDataCanvas*)(*canvas_iter));
            }

            for (CalConstIter iter = the_app.GetCalculators()->begin(); iter != the_app.GetCalculators()->end(); iter++) {
                if ((*iter)->isme(((IDataCanvas*) (*canvas_iter))->getContainerName()))
                    (*iter)->RegisterCanvas((IDataCanvas*)(*canvas_iter));
            }
        }
    }

    //register this window to calculators and dispathchers
    for (CalConstIter iter = the_app.GetCalculators()->begin(); iter != the_app.GetCalculators()->end(); iter++) {
        if ((*iter)->isme(_calculators))
            (*iter)->register_hwnd(_hwnd);
    }

    for (DispatcherConstIter iter = the_app.getDispatchers()->begin(); iter != the_app.getDispatchers()->end(); iter++) {
        if ((*iter)->isme(_dispatchers))
            (*iter)->register_hwnd(_hwnd);
    }

    return true;
}


