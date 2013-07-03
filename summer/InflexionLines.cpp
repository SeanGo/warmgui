#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"


CInflexionLines::CInflexionLines(const char* name)
    : WARMGUI::IDataGraph_summer(name)
    //, _infl(0)
{
}


CInflexionLines::~CInflexionLines(void)
{
}

GLYPH_CHANGED_TYPE CInflexionLines::update(dataptr data)
{
    return (0);
}

GLYPH_CHANGED_TYPE CInflexionLines::new_data(DataObjectPtr dop)
{
    return (0);
}

HRESULT CInflexionLines::_predraw()
{
    return (0);
}

HRESULT CInflexionLines::_draw(bool redraw_all/* = false*/)
{
    return (0);
}

void CInflexionLines::_update_data()
{
}

HRESULT CInflexionLines::init()
{
    return WARMGUI::IDataGraph_summer::init();
    //return (0);
}

void CInflexionLines::set_rect(RECT& rect)
{
    WARMGUI::IDataGraph_summer::set_rect(rect);
}
