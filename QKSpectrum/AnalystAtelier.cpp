#include "StdAfx.h"
#include "qks_incs.h"

CAnalystAtelier::CAnalystAtelier(const char* name)
    : _canvas_imgbkg(0)
    ,    _rt_analyst(0)
    ,    _1m_analyst(0)
    ,    _5m_analyst(0)
    ,   _15m_analyst(0)
    ,       _toolbar(0)
    ,       _analyst(0)
{
    setClass();
}


CAnalystAtelier::~CAnalystAtelier(void)
{
}

HRESULT CAnalystAtelier::InitAtelier(HWND hwnd, WARMGUI::CWarmguiConfig* config)
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
	return hr;
}

void CAnalystAtelier::ToggleToolbar()
{
 	if (_toolbar->IsVisible())
		_toolbar->SetVisible(false);
	else
		_toolbar->SetVisible(true);

	_layout->Disposal(this, _rect);

	ClearChanged();
	Changed(GLYPH_CHANGED_TYPE_ATELIER_RESIZE);

	InvalidateRect(_hwnd, 0, FALSE);
}

inline bool CAnalystAtelier::GetAllCanvas()
{
    CanvasIter iter = _canvasses.begin();
    _canvas_imgbkg  = (WARMGUI::CBkgCanvas *) (*iter); ++iter;
    _rt_analyst     = (CPVIAnalysisCanvas*)(*iter); ++iter;
    _1m_analyst     = (CPVIAnalysisCanvas*)(*iter); ++iter;
    _5m_analyst     = (CPVIAnalysisCanvas*)(*iter); ++iter;
    _15m_analyst    = (CPVIAnalysisCanvas*)(*iter); ++iter;
    _toolbar        = (WARMGUI::CToolbar*)(*iter);;

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

void CAnalystAtelier::set_analyst(EUCLID::CEuclidAnalyst* analyst)
{
    _analyst = analyst;
    _rt_analyst->set_analyst(_analyst);
    _1m_analyst->set_analyst(_analyst);
    _5m_analyst->set_analyst(_analyst);
    _15m_analyst->set_analyst(_analyst);
}

HRESULT CAnalystAtelier::draw_data()
{
    _rt_analyst->draw_data();
    _1m_analyst->draw_data();
    _5m_analyst->draw_data();
    _15m_analyst->draw_data();

    redraw_window();
    return S_OK;
}
