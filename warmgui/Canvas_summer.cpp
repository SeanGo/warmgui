#include "stdafx.h"
#include "warmgui_summer.h"

namespace WARMGUI {
ICanvas_summer::ICanvas_summer()
    : _atelier(0)
{
}

ICanvas_summer::ICanvas_summer(const char* name)
    : IGlyph_summer(name)
    , _atelier(0)
{
}

ICanvas_summer::~ICanvas_summer()
{
}

inline void ICanvas_summer::append_glyph(IGlyph_summer* glyph)
{
    GlyphTreeIter_summer glyph_iter = _glyph_tree->append_child(this->_tree_iter, glyph);
    glyph->inherit(_atelier, _glyph_tree, this, glyph_iter, _artist, _config);
}

} //namespace WARMGUI