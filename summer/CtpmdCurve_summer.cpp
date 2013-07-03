#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"


CCtpmdCurve_summer::CCtpmdCurve_summer(const char* name, bool own_world, bool own_artist, bool own_data)
    : CCurveGraph_summer(name, own_world, own_artist, own_data)
    , _x_offset(0)
    , _y_offset(0)
{
}


CCtpmdCurve_summer::~CCtpmdCurve_summer(void)
{
}


GLYPH_CHANGED_TYPE CCtpmdCurve_summer::update(dataptr data)
{
    POINTF pnt;
    pnt.x = *(float*)((char*)data + _x_offset);
    pnt.y = *(float*)((char*)data + _y_offset);

    WARMGUI::CCurveGraph_summer::update(&pnt);
    return (0);
}


HRESULT CCtpmdCurve_summer::init()
{
    return WARMGUI::CCurveGraph_summer::init();
}

void CCtpmdCurve_summer::set_rect(RECT& rect)
{
    WARMGUI::CCurveGraph_summer::set_rect(rect);
}
