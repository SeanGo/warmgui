#include "StdAfx.h"
#include "qks_incs.h"

/////////////////////////////////////////////////////////////////////
//layout
CZenInTwiningLayout::CZenInTwiningLayout(void)
{
    setClass();
}

CZenInTwiningLayout::~CZenInTwiningLayout(void)
{
}

void CZenInTwiningLayout::Disposal(WARMGUI::IAtelier* atelier, RECT& rectAtelier)
{
	RECT rect = rectAtelier, rectRealAtelier = rectAtelier;
	if (RectWidth(rect) <= 0 || RectHeight(rect) <= 0)
		return;

	CZenInTwiningAtelier* zit = (CZenInTwiningAtelier*)atelier;
   
	WARMGUI::ConstCanvasIter iter = zit->_canvasses.begin();

    zit->_canvasImgbkg->SetRect(rect);
    /*RECT tmp = {RectWidth(rect) * (1 - .618) / 2,
                RectHeight(rect) * (1 - .618) / 2,
                RectWidth(rect) * (1 + .618) / 2, 
                RectHeight(rect) * (1 + .618) / 2};*/


 
    float row_1_height  = static_cast<float>(RectHeight(rectRealAtelier)) * .55f;
	float col_1_1_width = static_cast<float>(RectWidth (rectRealAtelier)) * .5f;
	float col_1_2_width = static_cast<float>(RectWidth (rectRealAtelier)) * .5f * .618f;
	float col_2_width   = static_cast<float>(RectWidth (rectRealAtelier)) * (.5f + .5f * .618f) / 3.0f;
	MARGIN margin = {5, 5, 5, 5};

	rect.left = rectRealAtelier.left + margin.left,
		rect.top = rectRealAtelier.top + margin.top,
		rect.right = static_cast<int>(col_1_1_width - static_cast<float>(margin.right)),
		rect.bottom = rect.top + static_cast<int>(row_1_height * .618f);
	rect.bottom = rect.top + static_cast<int>(row_1_height * (1.0f - .618f) * .5f);
	rect.bottom = static_cast<int>((float)rectRealAtelier.top + row_1_height - (float)margin.bottom);
    zit->_pvicanvas->SetRect(rect);


    //4. Realtime Data Analyse canvas
	rect.left = rect.right + (margin.left + margin.right),
		rect.top = rectRealAtelier.top + margin.top,
		rect.right = int((float)rect.left + col_1_2_width);
		rect.bottom = static_cast<int>((float)rectRealAtelier.top + row_1_height - (float)margin.bottom);
    zit->_rtcanvas->SetRect(rect);

	//5. Message Canvas
	rect.left = rect.right + (margin.left + margin.right),
		rect.right = rectRealAtelier.right - margin.right;
    zit->_message->SetRect(rect);
    MYTRACE(L"%s %d %d %d %d\n", zit->_message->_name, rect.left, rect.top, rect.right, rect.bottom);

	//6. twine in 1 minute
	rect.top = (int)((float)rectRealAtelier.top + row_1_height + (float)margin.top),
		rect.bottom = rectRealAtelier.bottom - margin.bottom,
		rect.left = rectRealAtelier.left + margin.left,
		rect.right = (int)((float)rect.left - (float)margin.left + col_2_width - (float)margin.right);
    zit->_1mcanvas->SetRect(rect);

	//7. twine in 5 minute
	rect.left = rect.right + margin.right + margin.left,
		rect.right = (int)((float)rect.left - (float)margin.left + col_2_width - (float)margin.right);
    zit->_5mcanvas->SetRect(rect);

	//8. twine in 15 minute
	rect.left = rect.right + margin.right + margin.left,
		rect.right = (int)((float)rect.left - (float)margin.left + col_2_width - (float)margin.right);
    zit->_15mcanvas->SetRect(rect);
	
	//9. trade message
	rect.left = rect.right + margin.right + margin.left,
		rect.right = rectRealAtelier.right - margin.right;
    zit->_trademsg->SetRect(rect);
    //MYTRACE(L"%s %d %d %d %d\n", zit->_trademsg->_name, rect.left, rect.top, rect.right, rect.bottom);

    //if toolbar is visible
	//0. toolbar
	//set the toolbar pos, but now i let him on the top
    if (zit->_toolbar->IsVisible()) {
        rect.left = rect.top = 0,
            rect.right = rectRealAtelier.right, rect.bottom = zit->_toolbar->_btn_size.cy;
	    zit->_toolbar->SetRect(rect);
//        rectRealAtelier.top = rect.bottom;
    }
}
