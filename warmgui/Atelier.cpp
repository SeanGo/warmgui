#include "StdAfx.h"
#include "warmgui_incs.h"

POINT     pntZero  = {0, 0};
MATRIX_2D idmatrix = D2D1::Matrix3x2F::Identity();

namespace WARMGUI {

IAtelier::IAtelier(void)
	: _hwnd(0)
	, _clear_render_target(true)
	, _layout(0)
	, _pHwndRT(0)
	, _sizing(false)
    , _drawing(false)
    , _selected_canvas(0)
{
//    _iterCanvasSlct = _canvasses.end();
    _common_artist  = new eArtist();
    setClass();
}


IAtelier::~IAtelier(void)
{
	Clear();
    if (_common_artist) delete _common_artist;

	SafeRelease(&_pHwndRT);
    SafeDelete(_layout);
}

HRESULT IAtelier::InitAtelier(HWND hwnd, CWarmguiConfig* config)
{
	_hwnd = hwnd, _config = config;
    char key[MAX_PATH];
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.dispatchers", this->_str_conf);
    strcpy_s(_dispatchers, MAX_PATH, _config->getString(key).c_str());

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.calculators", this->_str_conf);
    strcpy_s(_calculators, MAX_PATH, _config->getString(key).c_str());

	return CreateRenderTarget(_hwnd);
}

HRESULT IAtelier::Draw(bool /*redraw = false*/)
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
HRESULT IAtelier::DrawBkg(bool redraw/* = false*/)
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
HRESULT IAtelier::RealDraw(bool redraw/* = false */)
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


HRESULT IAtelier::Renew()
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


HRESULT IAtelier::DrawScreenImage()
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



inline HRESULT IAtelier::CreateRenderTarget(HWND hwnd)
{
	_hwnd = hwnd;
	HRESULT hr = WARMGUI::CDxFactorys::GetInstance()->CreateRenderTarget(hwnd, &_pHwndRT);
    _common_artist->SetHwndRenderTarget(_pHwndRT, hwnd);
	if (SUCCEEDED(hr)) {
		SetArtistToCanvas();
	}

	return hr;
}

inline void IAtelier::SetArtistToCanvas()
{
	for (ConstCanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
        (*iter)->SetCommonArtist(_common_artist);
}

inline void IAtelier::SetRect(RECT& rect)
{
	if (RectEqulas(rect, _rect))
		return;

	ClearChanged();
	Changed(GLYPH_CHANGED_TYPE_ATELIER_RESIZE);

    _rect = rect;
	_pHwndRT->Resize(D2D1::SizeU(RectWidth(_rect), RectHeight(_rect)));
	_appbmp.SetSize(_pHwndRT, _rect);
    _common_artist->ResizeRenderTarget(RectWidth(_rect), RectHeight(_rect));
	_layout->Disposal(this, _rect);

	//MYTRACE(L"atelier changed is %d\n", _changed_type);
}

/*
inline void IAtelier::give_some(ICanvas* const canvas)
{
	canvas->SetAtelier(this);
	canvas->SetCommonArtist(_common_artist);
	canvas->SetBitmap(&_appbmp);
	canvas->Init();
}
*/

inline void IAtelier::Insert(ICanvas* const new_canvas)
{
    _canvasses.push_back(new_canvas);
}

inline bool IAtelier::Remove(const ICanvas* canvas)
{
	for (CanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
		if ((ICanvas*)canvas == (*iter)) {
			delete (*iter);
			_canvasses.erase(iter);
			return true;
		}

	return false;
}

inline bool IAtelier::Remove(const char* name)
{
	for (CanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
		if ((*iter)->IsMe(name)) {
			delete (*iter);
			_canvasses.erase(iter);
			return true;
		}

	return false;
}

inline ICanvas* IAtelier::FindCanvas(const char* name) const
{
	for (ConstCanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
		if ((*iter)->IsMe(name))
			return (*iter);

	return (0);
}



inline IGlyph* IAtelier::FindGlyph (const char* name) const
{
	IGlyph * g = 0;
	for (ConstCanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++) {
		g = (*iter)->Find(name);
		if (g) return (g);
	}

	return (0);
}
inline void IAtelier::Clear()
{
	for (ConstCanvasIter iter = _canvasses.begin(); iter != _canvasses.end(); iter++)
		delete (*iter);

	_canvasses.clear();
}

inline ICanvas* IAtelier::GetSelectedCanvas()
{
    return _selected_canvas;
}

bool IAtelier::GetBitmapMaterial()
{
	if (_config) {
		try {
            char temp[MAX_PATH];
            _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.bitmap_materials", _str_conf);
			std::string strImgpath = _config->getString(temp);
			return (SUCCEEDED(GetBitmapFromFile(strImgpath.c_str(), _pHwndRT, &_appbmp._material)));
		} catch(Poco::NotFoundException&) {
            MYTRACE(L"IAtelier::GetBitmapMaterial()\n");
			return false;
		}
	}
	return true;
}


bool IAtelier::GetMyBackgroundRect()
{
	try {
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.mainwindow-bkgimg-rect", _str_conf);
		return _config->getRect(_winrect_in_material, temp);
	} catch(Poco::NotFoundException&) {
        MYTRACE(L"IAtelier::GetMyBackgroundRect\n");
		return false;
	}
}

inline void IAtelier::GetWinrectInMaterial(RECT& canvas_rect, RECT_f* bmprect)
{
    float x_scale = fRectWidth(_winrect_in_material) / fRectWidth(_rect), y_scale = fRectHeight(_winrect_in_material) / fRectHeight(_rect);
	bmprect->left = canvas_rect.left * x_scale,
		bmprect->top = canvas_rect.top * y_scale,
		bmprect->right = canvas_rect.right * x_scale,
		bmprect->bottom = canvas_rect.bottom * y_scale;
}


//Message Functions
inline int IAtelier::OnMouseMove(int x, int y)
{
	for (CCanvasArray::reverse_iterator riter = _canvasses.rbegin(); riter != _canvasses.rend(); riter++)
		if ((*riter)->OnMouseMove(x, y))
			return (1);

	return (0);
}

inline int IAtelier::OnLButtonUp(int x, int y)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    if (_selected_canvas)
        changed = _selected_canvas->unselected();

	for (CCanvasArray::reverse_iterator riter = _canvasses.rbegin(); riter != _canvasses.rend(); riter++)
        if ((*riter)->OnLButtonUp(x, y)) {
			_selected_canvas = *riter;
            break;
        }


    if (_selected_canvas) {
        changed = (GLYPH_CHANGED_TYPE)((int)changed | (int)_selected_canvas->selected());
        return ::SendMessage(_hwnd, WM_RENEW_WINDOW, (WPARAM)changed, 0);
    }
    return (0);
}

inline int IAtelier::OnRButtonUp(int x, int y)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    if (_selected_canvas)
        changed = _selected_canvas->unselected();

	for (CCanvasArray::reverse_iterator riter = _canvasses.rbegin(); riter != _canvasses.rend(); riter++)
        if ((*riter)->OnLButtonUp(x, y)) {
			_selected_canvas = *riter;
            break;
        }
    if (_selected_canvas) {
        changed = (GLYPH_CHANGED_TYPE)((int)changed | (int)_selected_canvas->selected());
        return ::SendMessage(_hwnd, WM_RENEW_WINDOW, (WPARAM)changed, 0);
    }
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
	HRESULT hr = IAtelier::InitAtelier(hwnd, config);

	if (SUCCEEDED(hr) && GetBitmapMaterial()) {
		CBkgCanvas* bkgcanvas = new CBkgCanvas("bkgcanvas");
		bkgcanvas->SetBitmap(&_appbmp);
		Insert(bkgcanvas);


		SetLayout();
	}

	return hr;
}


}//namespace WARMGUI
