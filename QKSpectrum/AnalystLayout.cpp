#include "StdAfx.h"
#include "qks_incs.h"


CAnalystLayout::CAnalystLayout(void)
{
    setClass();
}


CAnalystLayout::~CAnalystLayout(void)
{
}


void CAnalystLayout::SetConfig(CWarmguiConfig* config, const char* conf_str)
{
    ILayout::SetConfig(config, conf_str);
}


void CAnalystLayout::Disposal(WARMGUI::IAtelier* atelier, RECT& rectAtelier)
{
	RECT rect = rectAtelier;
	if (RectWidth(rect) <= 0 || RectHeight(rect) <= 0)
		return;

	CAnalystAtelier* _atelier = (CAnalystAtelier*)atelier;  
    _atelier->_canvas_imgbkg->SetRect(rect);

    float row_height = fRectHeight(rectAtelier) * .5f;
    float col_width  = fRectWidth(rectAtelier)  * .5f;
    MARGIN margin = {4, 4, 4, 4};

    //first row
    rect.top += margin.top,
        rect.bottom = rect.top + row_height - margin.top - margin.bottom / 2;
	
    //rt data
    rect.left = margin.left,
        rect.right = rect.left + col_width - margin.left - margin.right / 2;
    _atelier->_rt_analyst->SetRect(rect);

    //1m data
    rect.left = rect.right + margin.left,
        rect.right = rectAtelier.right - margin.right;
    _atelier->_1m_analyst->SetRect(rect);

    rect.top = rect.bottom + margin.top, rect.bottom = rectAtelier.bottom - margin.bottom;

    //5m data
    rect.left = margin.left,
        rect.right = rect.left + col_width - margin.left - margin.right / 2;
    _atelier->_5m_analyst->SetRect(rect);

    //15m data
    rect.left = rect.right + margin.left,
        rect.right = rectAtelier.right - margin.right;
    _atelier->_15m_analyst->SetRect(rect);

    if (_atelier->_toolbar->IsVisible()) {
        rect.left = (int)((fRectWidth(rectAtelier) - fRectWidth(_atelier->_toolbar->get_toolbar_rect())) / 2.0f),
            rect.right = (int)((fRectWidth(rectAtelier) + fRectWidth(_atelier->_toolbar->get_toolbar_rect())) / 2.0f),
            rect.top = (int)((fRectHeight(rectAtelier) - fRectHeight(_atelier->_toolbar->get_toolbar_rect())) / 2.0f),
            rect.bottom = (int)((fRectHeight(rectAtelier) + fRectHeight(_atelier->_toolbar->get_toolbar_rect())) / 2.0f);
	    _atelier->_toolbar->SetRect(rect);
    }
}
