#include "StdAfx.h"
#include "qks_incs.h"
#include "Test1Atelier.h"



//////////////////////////////////////////////////////////////////////////////////////////
// class CTest1Atelier

CTest1Atelier::CTest1Atelier()
    : _canvasImgbkg(0)
    , _pvicanvas(0)
    , _layoutOne(0)
{
    _tcscpy_s(_name, MAX_WARMGUI_NAME_LEN, L"test-1");
}

void CTest1Atelier::SetLayout()
{
	_layoutOne = new CTest1Layout();
	_layout = _layoutOne;
}

HRESULT CTest1Atelier::InitAtelier(HWND hwnd, WARMGUI::CWarmguiConfig* config)
{
	HRESULT hr = CAtelier::InitAtelier(hwnd, config);

	if (SUCCEEDED(hr) && GetBitmapMaterial()) {
		_canvasImgbkg = new WARMGUI::CBkgCanvas(L"bkgcanvas");
		_canvasImgbkg->SetBitmap(&_appbmp);
		Insert(_canvasImgbkg);
	}

    _pvicanvas = new CPVICanvas(L"canvas-main-pvi");
    Insert(_pvicanvas);

    SetLayout();
	return hr;
}







/////////////////////////////////////////////////////////////////////
//layout
CTest1Layout::CTest1Layout()
{
}
CTest1Layout::~CTest1Layout()
{
}

void CTest1Layout::Disposal(WARMGUI::CAtelier* atelier, RECT& rect)
{
	CTest1Atelier* oneAtelier = (CTest1Atelier*)atelier;
	WARMGUI::ConstCanvasIter iter = oneAtelier->_canvasses.begin();

    oneAtelier->_canvasImgbkg->SetRect(rect);
    RECT tmp = {(int)(RectWidth(rect)  * (1 - .618) / 2),
                (int)(RectHeight(rect) * (1 - .618) / 2),
                (int)(RectWidth(rect)  * (1 + .618) / 2),
                (int)(RectHeight(rect) * (1 + .618) / 2)};
    oneAtelier->_pvicanvas->SetRect(tmp);

}
