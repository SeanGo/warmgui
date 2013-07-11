#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"


CTickdataCanvas::CTickdataCanvas(const char * name)
    : ICanvas_summer(name)
    , _data_grid(0)
    , _blind(0)
{
}


CTickdataCanvas::~CTickdataCanvas(void)
{
}

HRESULT CTickdataCanvas::init()
{
    HRESULT hr;
    _blind = new WARMGUI::CBlind_summer("tickdata", BGR(0, 0 ,0), 0.5);
    append_glyph(_blind);

    _data_grid = new CTickdataGrid("tick-grid");
    append_glyph(_data_grid);
    hr = _data_grid->init();


    the_app.get_dispatchers()->reg_graph(_data_grid, "ctp-disp");

    return hr;
}


void CTickdataCanvas::set_rect(RECT& rect)
{
    _abs_rect = rect;
    _rect.left = _rect.top = 0, _rect.right = RectWidth(rect), _rect.bottom = RectHeight(rect);

    _blind->set_rect(rect);

    RECT crect = _abs_rect;
    crect.top += 1; crect.bottom = _abs_rect.top + (int)(fRectHeight(rect) * .618f - 2.0f);
    _data_grid->set_rect(crect);
}
