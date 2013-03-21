#include "StdAfx.h"
#include "warmgui_incs.h"

POINT     pntZero  = {0, 0};
MATRIX_2D idmatrix = D2D1::Matrix3x2F::Identity();

namespace WARMGUI {

CAtelier::CAtelier(void)
	: _hwnd(0)
	, _clear_render_target(true)
	, _layout(0)
	, _pHwndRT(0)
	, _sizing(false)
{
    _iterCanvasSlct = _canvasses.end();
    _common_artist  = new eArtist();
}


CAtelier::~CAtelier(void)
{
	Clear();
	if (_layout) delete _layout;
    if (_common_artist) delete _common_artist;

	SafeRelease(&_pHwndRT);
}

HRESULT CAtelier::InitAtelier(HWND hwnd, CWarmguiConfig* config)
{
	_hwnd = hwnd, _config = config;
	return CreateRenderTarget(_hwnd);
}

HRESULT CAtelier::Draw(bool /*redraw = false*/)
{
	if (_sizing) return S_OK;
	HRESULT r = S_OK;

    CriticalLock::Scoped scope(_lockChange);

    if (!_changed_type)
        return DrawScreenImage();
    else if (_changed_type == GLYPH_CHANGED_TYPE_INCREASED)
    {
        Renew();
    } else {
        bool redraw = (_changed_type 
                        & ( GLYPH_CHANGED_TYPE_ATELIER_RESIZE
                            | GLYPH_CHANGED_TYPE_CANVAS_RESIZE
                            | GLYPH_CHANGED_TYPE_ATELIER_BKG)) 
                       ?
                       true
                       :
                        false;
                        // (GLYPH_CHANGED_TYPE_GLYPH_RESIZE | GLYPH_CHANGED_TYPE_CANVAS_BKG | GLYPH_CHANGED_TYPE_GLYPH_RESIZE | GLYPH_CHANGED_TYPE_COORDFRAME | GLYPH_CHANGED_TYPE_CHANGED)
  
        r = DrawBkg(redraw);
        if (SUCCEEDED(r))
            r = RealDraw(redraw);
    }

    if (SUCCEEDED(r))
        r = CopyFromRenderTarget(_pHwndRT, _appbmp._screen, pntZero, _rect);

    _changed_type = GLYPH_CHANGED_TYPE_NONE;
	return r;
}

///draw background
HRESULT CAtelier::DrawBkg(bool redraw/* = false*/)
{
    HRESULT r = S_OK;

    //clear all rendertarget if redraw
    _common_artist->BeginBmpDraw(redraw);
    _common_artist->SetTransform(&idmatrix);
	for (ConstCanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
		(*iter)->DrawBkg(redraw);

    r = _common_artist->EndBmpDraw();

    return r;
}

//output to hwnd-render-target
HRESULT CAtelier::RealDraw(bool redraw/* = false */)
{
    HRESULT r = S_OK;
    ///////////////////////////////////////////////////////////////////////////////////////////
    // real draw
    _common_artist->BeginDraw(redraw);
    _common_artist->SetTransform(&idmatrix);

    //draw background
    _common_artist->DrawBitmap(_common_artist->GetDefaultBmp(), _rect, _rect);

    for (ConstCanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
		(*iter)->Draw(redraw);

    r = _common_artist->EndDraw();

    return r;
}


HRESULT CAtelier::Renew()
{
    HRESULT r = S_OK;
    ///////////////////////////////////////////////////////////////////////////////////////////
    // real draw
    _common_artist->BeginDraw();
    _common_artist->SetTransform(&idmatrix);

    for (ConstCanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
        (*iter)->Renew();

    r = _common_artist->EndBmpDraw();

    return r;
}


HRESULT CAtelier::DrawScreenImage()
{
	if (_sizing) return S_OK;
	if (_appbmp._screen) {
		_pHwndRT->SetTransform(D2D1::Matrix3x2F::Identity());
		_pHwndRT->BeginDraw();
		_pHwndRT->DrawBitmap(_appbmp._screen);
		return _pHwndRT->EndDraw();
	} else {
		return (-1);
	}
}



inline HRESULT CAtelier::CreateRenderTarget(HWND hwnd)
{
	_hwnd = hwnd;
	HRESULT hr = WARMGUI::CDxFactorys::GetInstance()->CreateRenderTarget(hwnd, &_pHwndRT);
    _common_artist->SetHwndRenderTarget(_pHwndRT, hwnd);
	if (SUCCEEDED(hr)) {
		SetArtistToCanvas();
	}

	return hr;
}

inline void CAtelier::SetArtistToCanvas()
{
	for (ConstCanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
        (*iter)->SetCommonArtist(_common_artist);
}

inline void CAtelier::SetRect(RECT& rect)
{
	if (RectEqulas(rect, _rect)) {
		MYTRACE(L"RectEqulas\n");
		return;
	}
	_rect = rect;
	_pHwndRT->Resize(D2D1::SizeU(RectWidth(_rect), RectHeight(_rect)));
	_appbmp.SetSize(_pHwndRT, _rect);
    _common_artist->ResizeRenderTarget(RectWidth(_rect), RectHeight(_rect));
	_layout->Disposal(this, _rect);

	ClearChanged();
	Changed(GLYPH_CHANGED_TYPE_ATELIER_RESIZE);
	//MYTRACE(L"atelier changed is %d\n", _changed_type);
}

inline void CAtelier::give_some(ICanvas* const canvas)
{
	canvas->SetAtelier(this);
	canvas->SetCommonArtist(_common_artist);
	canvas->SetConfigFile(_config);
	canvas->SetBitmap(&_appbmp);
    canvas->SetDataContainer(_data_container);
	canvas->Init();
}


inline void CAtelier::Insert(ICanvas* const new_canvas)
{
	give_some(new_canvas);
    _canvasses.push_back(new_canvas);
}

inline bool CAtelier::Remove(const ICanvas* canvas)
{
	for (CanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
		if ((ICanvas*)canvas == (*iter)) {
			delete (*iter);
			_canvasses.erase(iter);
			return true;
		}

	return false;
}

inline bool CAtelier::Remove(const TCHAR * name)
{
	for (CanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
		if ((*iter)->IsMe(name)) {
			delete (*iter);
			_canvasses.erase(iter);
			return true;
		}

	return false;
}

inline ICanvas* CAtelier::FindCanvas(const char* name) const
{
    TCHAR wname[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(wname, MAX_PATH, name);
    return FindCanvas(wname);
}


inline ICanvas* CAtelier::FindCanvas(const TCHAR * name) const
{
	for (ConstCanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
		if ((*iter)->IsMe(name))
			return (*iter);

	return (0);
}

inline IGlyph* CAtelier::FindGlyph(const char* name) const
{
    TCHAR wname[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(wname, MAX_PATH, name);
    return FindGlyph(wname);
}


inline IGlyph* CAtelier::FindGlyph (const TCHAR * name) const
{
	IGlyph * g = 0;
	for (ConstCanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++) {
		g = (*iter)->Find(name);
		if (g) return (g);
	}

	return (0);
}
inline void CAtelier::Clear()
{
	for (ConstCanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
		delete (*iter);

	_canvasses.clear();
}

inline ICanvas* CAtelier::GetSelectedCanvas()
{
	if (_iterCanvasSlct == _canvasses.end())
		return (0);
	else
		return *_iterCanvasSlct;
}

bool CAtelier::GetBitmapMaterial()
{
	if (_config) {
		try {
			std::string strImgpath = _config->getString("application_bitmap_materials");
			return (SUCCEEDED(GetBitmapFromFile(strImgpath.c_str(), _pHwndRT, &_appbmp._material)));
		} catch(Poco::NotFoundException&) {
			return false;
		}
	}
	return true;
}


void CAtelier::GetMyBackgroundRect(RECT& myrect, RECT_f* bmprect)
{
	//D2D1_SIZE_F bmpsize = _appbmp._material->GetSize();

    D2D1_SIZE_F bmpsize;
	try {
		RECT rect;
		bool r = _config->getRect(rect, "mainwindow-bkgimg-rect");
		if (r)
            bmpsize.width = fRectWidth(rect), bmpsize.height = fRectHeight(rect);
	} catch(Poco::NotFoundException&) {
		return;
	}

	float x_scale = bmpsize.width / fRectWidth(_rect), y_scale = bmpsize.height / fRectHeight(_rect);
	bmprect->left = myrect.left * x_scale,
		bmprect->top = myrect.top * y_scale,
		bmprect->right = myrect.right * x_scale,
		bmprect->bottom = myrect.bottom * y_scale;
}


//Message Functions
inline int CAtelier::OnMouseMove(int x, int y)
{
	for (CCanvasArray::reverse_iterator riter = _canvasses.rbegin(); riter != _canvasses.rend(); riter++)
		if ((*riter)->OnMouseMove(x, y))
			return (1);

	return (0);
}

inline int CAtelier::OnLButtonUp(int x, int y)
{
	for (CCanvasArray::reverse_iterator riter = _canvasses.rbegin(); riter != _canvasses.rend(); riter++)
        if ((*riter)->OnLButtonUp(x, y))
			return (1);

	return (0);
}

inline int CAtelier::OnRButtonUp(int x, int y)
{
	for (CCanvasArray::reverse_iterator riter = _canvasses.rbegin(); riter != _canvasses.rend(); riter++)
		if ((*riter)->OnRButtonUp(x, y))
			return (1);

	return (0);
}





//////////////////////////////////////////////////////////////////////////////////////////
// class COneAtelier
void COneAtelier::SetLayout()
{
	_layoutOne = new COneLayout();
	_layout = _layoutOne;
}

HRESULT COneAtelier::InitAtelier(HWND hwnd, CWarmguiConfig* config)
{
	HRESULT hr = CAtelier::InitAtelier(hwnd, config);

	if (SUCCEEDED(hr) && GetBitmapMaterial()) {
		CBkgCanvas* bkgcanvas = new CBkgCanvas(L"bkgcanvas");
		bkgcanvas->SetBitmap(&_appbmp);
		Insert(bkgcanvas);


		SetLayout();
	}

	return hr;
}


}//namespace WARMGUI
