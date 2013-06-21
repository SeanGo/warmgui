#include "StdAfx.h"
#include "warmgui_summer.h"

namespace WARMGUI {
IAtelier_summer::IAtelier_summer(void)
    : _hwnd(0)
    , _pHwndRT(0)
{
}

IAtelier_summer::IAtelier_summer(const char* name)
    : IGlyph_summer(name)
    , _hwnd(0)
    , _pHwndRT(0)
{
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
    canvas->inherit(this, _glyph_tree, canvas_iter, _artist, _config);
}

IGlyph_summer* IAtelier_summer::find_glyph(const char* name)
{
    for (GlyphTreeIter_summer iter = _glyph_tree->begin(); iter != _glyph_tree->end(); iter++)
        if ((*iter)->is_me(name))
            return (*iter);

    return (0);
}

} //namespace WARMGUI