#include "StdAfx.h"
#include "qks_incs.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////////////////////////
// class CZITAtelier

CZITAtelier::CZITAtelier(const char* name)
    :   _canvasImgbkg(0)
    ,      _pvicanvas(0)
    ,      _layoutOne(0)
	,        _toolbar(0)
    ,_tickdata_canvas(0)
    ,     _zit_canvas(0)
{
    strcpy_s(_name, MAX_WARMGUI_NAME_LEN, name);
    setClass();
}

HRESULT CZITAtelier::InitAtelier(HWND hwnd, WARMGUI::CWarmguiConfig* config)
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


void CZITAtelier::ToggleToolbar()
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

bool CZITAtelier::GetAllCanvas()
{
    CanvasIter iter = _canvasses.begin();
    _canvasImgbkg   = (WARMGUI::CBkgCanvas* ) (*iter); ++iter;
    _pvicanvas      = (CPVI_Canvas*)          (*iter); ++iter;
    _tickdata_canvas= (CTickdataCanvas*)      (*iter); ++iter;
	_toolbar        = (WARMGUI::CToolbar*   ) (*iter);



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

    return (true);
}
