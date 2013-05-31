#include "StdAfx.h"
#include "warmgui_incs.h"

namespace WARMGUI {
CGridCanvas::CGridCanvas(void)
    : _gc(0)
{
    setClass();
}


CGridCanvas::~CGridCanvas(void)
{
}


CGridCanvas::CGridCanvas(const char* name)
	: ICanvas(name)
    , _gc(0)
{
    setClass();
}

void CGridCanvas::SetGlyphRect()
{
    for (GlyphTreeIter iter = _gt.begin(); iter != _gt.end(); iter++) {
        (*iter)->SetRect(_rect);
    }
}

const HRESULT CGridCanvas::Init(const char* name/* = 0*/)
{
	if (!_config)
		return (-1);

    set_background();
    
    _gc = new CGridCtrl("grid_contrl");
    InsertAfter(_gc);

	return S_OK;
}


}
