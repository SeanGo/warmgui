#include "StdAfx.h"
#include "warmgui_incs.h"

namespace WARMGUI {
CGridCanvas::CGridCanvas(void)
    : _gc(0)
{
}


CGridCanvas::~CGridCanvas(void)
{
}


CGridCanvas::CGridCanvas(const TCHAR * name)
	: ICanvas(name)
    , _gc(0)
{
}

void CGridCanvas::SetGlyphRect()
{
    for (GlyphTreeIter iter = _gt.begin(); iter != _gt.end(); iter++)
        (*iter)->SetRect(_rect);
}

HRESULT CGridCanvas::Init()
{
	ICanvas::Init();

	if (!_config)
		return (-1);

    
    _gc = new CWarmGridCtrl(L"grid_contrl");
    InsertAfter(_gc);

	return S_OK;
}


}
