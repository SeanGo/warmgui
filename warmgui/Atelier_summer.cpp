#include "StdAfx.h"
#include "warmgui_summer.h"


namespace WARMGUI {
IAtelier_summer::IAtelier_summer(void)
    : _hwnd(0)
    , _pHwndRT(0)
    , _sizing(false)
{
    _winrect_in_material = zero_rect;
}

IAtelier_summer::IAtelier_summer(const char* name)
    : IGlyph_summer(name)
    , _hwnd(0)
    , _pHwndRT(0)
    , _sizing(false)
{
    _winrect_in_material = zero_rect;
}


IAtelier_summer::~IAtelier_summer(void)
{
    GlyphTreeIter_summer iter = _glyph_tree->begin();
    iter++; //do not delete myself
    for (; iter != _glyph_tree->end(); iter++)
        SafeDelete(*iter);

    SafeDelete(_glyph_tree);
}

inline HRESULT IAtelier_summer::init(HWND hwnd)
{
    HRESULT hr = S_OK;
    if (!_glyph_tree) {
        _glyph_tree = new CGlyphTree_summer(this);
        _tree_iter = _glyph_tree->begin();
    }

    if (!_artist) {
        _artist = new eArtist();
        set_myown_artist();
	    _hwnd = hwnd;
	    hr = WARMGUI::CDxFactorys::GetInstance()->CreateRenderTarget(hwnd, &_pHwndRT);
        _artist->SetHwndRenderTarget(_pHwndRT, hwnd);
    }

    return hr;
}

inline void IAtelier_summer::append_canvas(ICanvas_summer* canvas)
{
    GlyphTreeIter_summer canvas_iter = _glyph_tree->append_child(_tree_iter, canvas);
    canvas->inherit(this, _glyph_tree, canvas_iter, _artist, _config, &_appbmp);
}

IGlyph_summer* IAtelier_summer::find_glyph(const char* name)
{
    for (GlyphTreeIter_summer iter = _glyph_tree->begin(); iter != _glyph_tree->end(); iter++)
        if ((*iter)->is_me(name))
            return (*iter);

    return (0);
}


#define bkg_is_changed(change) \
    ((change) & GLYPH_CHANGED_ATELIER_BKG \
        || (change) & GLYPH_CHANGED_CANVAS_BKG \
        || (change) & GLYPH_CHANGED_GLYPH_BKG \
        || (change) & GLYPH_CHANGED_ATELIER_RESIZE \
        || (change) & GLYPH_CHANGED_CANVAS_RESIZE)

#define graph_is_changed(change) \
    ((change) & GLYPH_CHANGED_GLYPH \
        || (change) & GLYPH_CHANGED_CANVAS \
        || (change) & GLYPH_CHANGED_ATELIER_RESIZE \
        || (change) & GLYPH_CHANGED_CANVAS_RESIZE)


static MATRIX_2D idmatrix = D2D1::Matrix3x2F::Identity();
static POINT     pntZero  = {0, 0};

inline HRESULT IAtelier_summer::Draw()
{
    HRESULT hr = S_OK;
    CriticalLock::Scoped scope(_lockChange);

    bool bkgchanged = bkg_is_changed(_changed);
    //redraw all back-ground bitmap
    if (bkgchanged)
    {
        //draw backup graph using default bitmap render target
#ifdef  _DEBUG
        MYTRACE(L"draw backup graph\n");
#endif  //_DEBUG
        _artist->BeginBmpDraw(true);
        _artist->SetTransform(&idmatrix);

        hr = draw_graph(true, IGlyph_summer::GLYPH_TYPE_BKG);

        if (SUCCEEDED(hr))
            hr = _artist->EndBmpDraw();
    }

    //if (SUCCEEDED(hr)) {
    //    if (bkgchanged || graph_is_changed(_changed))
    //    {
            _artist->BeginDraw(true);
            _artist->SetTransform(&idmatrix);

            //draw background first
            _artist->DrawBitmap(_artist->GetDefaultBmp(), _rect, _rect);

            //draw all graph
            hr = draw_graph(bkgchanged);

            //copy to screen-bitmap
            if (SUCCEEDED(hr))
                hr = CopyFromRenderTarget(_pHwndRT, _appbmp._screen, pntZero, _rect);

            if (SUCCEEDED(hr))
                hr = _artist->EndDraw();
    //    } else
    //        hr = draw_screen_bitmap();
    //}

    if (SUCCEEDED(hr))
        set_change(GLYPH_CHANGED_NONE);

    return hr;
}

inline HRESULT IAtelier_summer::draw_screen_bitmap()
{
#ifdef _DEBUG
    MYTRACE(L"IAtelier_summer::draw_screen_bitmap\n");
    //HRESULT hr = S_OK;
    //return hr;
#endif 

    //if test glyph-tree only, desable this block
    {
	    if (_appbmp._screen) {
		    _pHwndRT->SetTransform(D2D1::Matrix3x2F::Identity());
		    _pHwndRT->BeginDraw();
		    _pHwndRT->DrawBitmap(_appbmp._screen);
		    return _pHwndRT->EndDraw();
	    } else {
		    return (-1);
	    }

    }

}

inline void IAtelier_summer::set_rect(RECT& rect)
{
    if (RectEqulas(rect, _abs_rect))
		return;

    CriticalLock::Scoped scope(_lockChange);
    set_change(GLYPH_CHANGED_ATELIER_RESIZE);

    _abs_rect = rect, _rect.left = _rect.top = 0, _rect.right = RectWidth(rect), _rect.bottom = RectHeight(rect);

    _pHwndRT->Resize(D2D1::SizeU(_rect.right, _rect.bottom));
	_appbmp.SetSize(_pHwndRT, _rect);
    _artist->ResizeRenderTarget(_rect.right, _rect.bottom);

    disposal(_abs_rect);
}


inline bool IAtelier_summer::GetBitmapMaterial()
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


inline bool IAtelier_summer::GetMyBackgroundRect()
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

inline void IAtelier_summer::GetWinrectInMaterial(RECT& canvas_rect, RECT_f* bmprect)
{
    float x_scale = fRectWidth(_winrect_in_material) / fRectWidth(_rect), y_scale = fRectHeight(_winrect_in_material) / fRectHeight(_rect);
	bmprect->left = canvas_rect.left * x_scale,
		bmprect->top = canvas_rect.top * y_scale,
		bmprect->right = canvas_rect.right * x_scale,
		bmprect->bottom = canvas_rect.bottom * y_scale;
}


inline void IAtelier_summer::ToggleToolbar(const char* toolbar_name/* = "toolbar"*/)
{
    CToolbar_summer* tb = (CToolbar_summer*)find_glyph(toolbar_name);
    if (tb) tb->toggle_visible();

    //change(GLYPH_CHANGED_CANVAS_BKG | GLYPH_CHANGED_CANVAS);
    redraw_window();
}

inline void IAtelier_summer::redraw_window(bool all_redraw/* = false*/)
{
    //if (all_redraw)
    //change(GLYPH_CHANGED_ATELIER_RESIZE);
    //MYTRACE(L"redraw window\n");
    PostMessage(_hwnd, WM_WINDOWS_REDRAW, (WPARAM)&_abs_rect, all_redraw);
}

inline int IAtelier_summer::OnLButtonUp(UINT flag, int x, int y)
{
    clear_select();
    
    for (int i = _glyph_tree->number_of_children(_tree_iter) - 1; i >= 0; i--) {
        //draw all 
        GlyphTreeIter_summer it = _glyph_tree->child(_tree_iter, (unsigned int)i);
        if ((*it)->OnLButtonUp(flag, x, y))
            break;
    }
    return (0);
}


} //namespace WARMGUI
