#include "StdAfx.h"
#include "qks_incs.h"


CEuclidLayout::CEuclidLayout(void)
{
    setClass();
}


CEuclidLayout::~CEuclidLayout(void)
{
}

void CEuclidLayout::SetConfig(CWarmguiConfig* config, const char* conf_str)
{
    ILayout::SetConfig(config, conf_str);
}


void CEuclidLayout::Disposal(WARMGUI::IAtelier* atelier, RECT& rectAtelier)
{
	RECT rect = rectAtelier;
	if (RectWidth(rect) <= 0 || RectHeight(rect) <= 0)
		return;

	CEuclidAtelier* euclidAtelier = (CEuclidAtelier*)atelier;  
    euclidAtelier->_canvas_imgbkg->SetRect(rect);

    float row_height = fRectHeight(rectAtelier) * .618f;
    float col_width  = fRectWidth(rectAtelier) * .3333f;
    MARGIN margin = {5, 5, 5, 5};

    //first row
    rect.top += margin.top,
        rect.bottom = rect.top + row_height - margin.top - margin.bottom;
	
    //recent zit
    rect.left += margin.left,
        rect.right = rect.left + col_width - margin.left - margin.right;
    euclidAtelier->_canvas_recent_zit->SetRect(rect);

    rect.left = rect.right + margin.right + margin.left,
        rect.right = rect.left + col_width - margin.left - margin.right;
    euclidAtelier->_canvas_short_term->SetRect(rect);

    //long term
    rect.left = rect.right + margin.right + margin.left,
        rect.right = rectAtelier.right - margin.right;
    euclidAtelier->_canvas_long_term->SetRect(rect);

    //second row
    rect.top = rect.bottom + margin.bottom + margin.top,
        rect.bottom = rectAtelier.bottom - margin.bottom;

    //1m and zit
    rect.left = rectAtelier.left + margin.left;
        rect.right = rect.left + col_width - margin.left - margin.right;
    euclidAtelier->_canvas_1m_zit->SetRect(rect);

    //5m and zit
    rect.left = rect.right + margin.left + margin.right,
        rect.right = rect.left + col_width - margin.left - margin.right;
    euclidAtelier->_canvas_5m_zit->SetRect(rect);

    //15m and zit
    rect.left = rect.right + margin.left + margin.right,
        rect.right = rectAtelier.right - margin.right;
    euclidAtelier->_canvas_15m_zit->SetRect(rect);
}
