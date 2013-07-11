#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"


CZitPredictCanvas::CZitPredictCanvas(const char* name)
    : ICanvas_summer(name)
    , _blind(0)
    , _zit_prdt_graph(0)
{
}


CZitPredictCanvas::~CZitPredictCanvas(void)
{
}


HRESULT CZitPredictCanvas::init()
{
    _blind = new WARMGUI::CBlind_summer("blind", BGR(0, 0, 0), 0.8f);
    append_glyph(_blind);

    char key[MAX_PATH];
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.margin", _str_conf);
    _config->getRect(_margin, key);

    _zit_prdt_graph = new CZitPredictGraph("zit-predict");
    append_glyph(_zit_prdt_graph);
    _zit_prdt_graph->init();

    the_app.get_calculators()->reg_graph(_zit_prdt_graph, "zit-prdct");

    return (0);
}

void CZitPredictCanvas::set_rect(RECT& rect)
{
    RECT crect = rect;
    crect.left += _margin.left, crect.top += _margin.top, crect.right -= _margin.right, crect.bottom -= _margin.bottom;
    _abs_rect = crect;
    _rect.left = _rect.top = 0, _rect.right = RectWidth(_abs_rect), _rect.bottom = RectHeight(_abs_rect);
    _blind->set_rect(crect);
    _zit_prdt_graph->set_rect(crect);
}
