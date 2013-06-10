#include "StdAfx.h"
#include "qks_incs.h"

/////////////////////////////////////////////////////////////////////
//layout
CTickDataLayout::CTickDataLayout(void)
{
    setClass();
}

CTickDataLayout::~CTickDataLayout(void)
{
}

void CTickDataLayout::SetConfig(CWarmguiConfig* config, const char* conf_str)
{
    ILayout::SetConfig(config, conf_str);
}

void CTickDataLayout::Disposal(WARMGUI::IAtelier* atelier, RECT& rectAtelier)
{
	RECT rect = rectAtelier, rectRealAtelier = rectAtelier;
	if (RectWidth(rect) <= 0 || RectHeight(rect) <= 0)
		return;

	CTickDataAtelier* zit = (CTickDataAtelier*)atelier;
   
	WARMGUI::ConstCanvasIter iter = zit->_canvasses.begin();

    zit->_canvasImgbkg->SetRect(rect);
    /*RECT tmp = {RectWidth(rect) * (1 - .618) / 2,
                RectHeight(rect) * (1 - .618) / 2,
                RectWidth(rect) * (1 + .618) / 2, 
                RectHeight(rect) * (1 + .618) / 2};*/


 
    float atelier_height = fRectHeight(rectRealAtelier);// * .55f;
	float col_1_1_width  = fRectWidth (rectRealAtelier) * .618f;
//	float col_1_2_width = static_cast<float>(RectWidth (rectRealAtelier)) * .5f * .618f;
//	float col_2_width   = static_cast<float>(RectWidth (rectRealAtelier)) * (.5f + .5f * .618f) / 3.0f;
	MARGIN margin = {5, 5, 5, 5};

	rect.left = rectRealAtelier.left + margin.left,
		rect.top = rectRealAtelier.top + margin.top,
		rect.right = static_cast<int>(col_1_1_width - static_cast<float>(margin.right)),
		//rect.bottom = rect.top + static_cast<int>(atelier_height * .618f);
	    //rect.bottom = rect.top + static_cast<int>(atelier_height * (1.0f - .618f) * .5f);
	    rect.bottom = static_cast<int>((float)rectRealAtelier.top + atelier_height - (float)margin.bottom);
    zit->_pvicanvas->SetRect(rect);

    rect.left = (int)col_1_1_width,
        rect.right = (int)rectRealAtelier.right,
        rect.top = (int)rectRealAtelier.top,
        rect.bottom = static_cast<int>(fRectHeight(rectRealAtelier) * (1.0f - .618f));
    zit->_tickdata_canvas->SetRect(rect);
    
    ///rect.top = rect.bottom  + margin.top,
    ///    rect.bottom = (int)((float)rect.top + fRectHeight(rectRealAtelier) * .618f - (float)margin.top - (float)margin.bottom);
    ///zit->_zit_canvas->SetRect(rect);
    //MYTRACE(L"%s %d %d %d %d\n", zit->_trademsg->_name, rect.left, rect.top, rect.right, rect.bottom);

    //if toolbar is visible
	//0. toolbar
	//set the toolbar pos, but now i let him on the top
    if (zit->_toolbar->IsVisible()) {
        rect.left = (int)((fRectWidth(rectRealAtelier) - fRectWidth(zit->_toolbar->get_toolbar_rect())) / 2.0f),
            rect.right = (int)((fRectWidth(rectRealAtelier) + fRectWidth(zit->_toolbar->get_toolbar_rect())) / 2.0f),
            rect.top = (int)((atelier_height - fRectHeight(zit->_toolbar->get_toolbar_rect())) / 2.0f),
            rect.bottom = (int)((atelier_height + fRectHeight(zit->_toolbar->get_toolbar_rect())) / 2.0f);
	    zit->_toolbar->SetRect(rect);
//        rectRealAtelier.top = rect.bottom;
    }
}
