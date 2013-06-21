#include "StdAfx.h"
#include "warmgui_summer.h"

namespace WARMGUI {
IAtelier_summer::IAtelier_summer(void)
    : _hwnd(0)
    , _pHwndRT(0)
    , _layout(0)
    , _sizing(false)
{
}

IAtelier_summer::IAtelier_summer(const char* name)
    : IGlyph_summer(name)
    , _hwnd(0)
    , _pHwndRT(0)
    , _layout(0)
    , _sizing(false)
{
}


IAtelier_summer::~IAtelier_summer(void)
{
    GlyphTreeIter_summer iter = _glyph_tree->begin();
    iter++; //do not delete myself
    for (; iter != _glyph_tree->end(); iter++)
        SafeDelete(*iter);

    SafeDelete(_glyph_tree);
    SafeDelete(_layout);
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
    canvas->inherit(this, _glyph_tree, canvas_iter, _artist, _config);
}

IGlyph_summer* IAtelier_summer::find_glyph(const char* name)
{
    for (GlyphTreeIter_summer iter = _glyph_tree->begin(); iter != _glyph_tree->end(); iter++)
        if ((*iter)->is_me(name))
            return (*iter);

    return (0);
}

inline HRESULT IAtelier_summer::Draw()
{
    HRESULT hr = S_OK;
    {
        CriticalLock::Scoped scope(_lockChange);
        if (_changed & GLYPH_CHANGED_ATELIER_BKG
            || _changed & GLYPH_CHANGED_CANVAS_BKG
            || _changed & GLYPH_CHANGED_GLYPH_BKG
            || _changed & GLYPH_CHANGED_ATELIER_RESIZE
            || _changed & GLYPH_CHANGED_CANVAS_RESIZE)
        {
            hr = draw_graph(IGlyph_summer::GLYPH_TYPE_BKG);
            if (SUCCEEDED(hr))
                hr = copy_bkg_to_bitmap();
        } else
            hr = draw_bkg_bitmap();
    }

    if (SUCCEEDED(hr)) {
        CriticalLock::Scoped scope(_lockChange);
        if (_changed & GLYPH_CHANGED_CHANGED
            || _changed & GLYPH_CHANGED_ATELIER_RESIZE
            || _changed & GLYPH_CHANGED_CANVAS_RESIZE)
        {
            hr = draw_graph();
            if (SUCCEEDED(hr))
                hr = copy_screen_to_bitmap();
        } else
            hr = draw_screen_bitmap();
    }

    set_change(GLYPH_CHANGED_NONE);
    return hr;
}

inline HRESULT IAtelier_summer::copy_bkg_to_bitmap()
{
    HRESULT hr = S_OK;
#ifdef _DEBUG
    MYTRACE(L"IAtelier_summer::copy_bkg_to_bitmap\n");
#endif 
    return hr;
}

inline HRESULT IAtelier_summer::draw_bkg_bitmap()
{
    HRESULT hr = S_OK;
#ifdef _DEBUG
    MYTRACE(L"IAtelier_summer::draw_bkg_bitmap\n");
#endif 
    return hr;
}

inline HRESULT IAtelier_summer::copy_screen_to_bitmap()
{
    HRESULT hr = S_OK;
#ifdef _DEBUG
    MYTRACE(L"IAtelier_summer::copy_screen_to_bitmap\n");
#endif 
    return hr;
}

inline HRESULT IAtelier_summer::draw_screen_bitmap()
{
    HRESULT hr = S_OK;
#ifdef _DEBUG
    MYTRACE(L"IAtelier_summer::draw_screen_bitmap\n");
#endif 
    return hr;
}


} //namespace WARMGUI