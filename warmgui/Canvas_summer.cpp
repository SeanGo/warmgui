#include "stdafx.h"
#include "warmgui_summer.h"

namespace WARMGUI {
ICanvas_summer::ICanvas_summer()
    : _atelier(0)
    , _appbmp(0)
    , _canvas_bmp(0)
    , _mouse_graph(0)
{
}

ICanvas_summer::ICanvas_summer(const char* name)
    : IGlyph_summer(name)
    , _atelier(0)
    , _appbmp(0)
    , _canvas_bmp(0)
    , _mouse_graph(0)
{
}

ICanvas_summer::~ICanvas_summer()
{
    SafeRelease(&_canvas_bmp);
    SafeDelete(_mouse_graph);
}

inline void ICanvas_summer::append_glyph(IGlyph_summer* glyph)
{
    GlyphTreeIter_summer glyph_iter = _glyph_tree->append_child(this->_tree_iter, glyph);
    glyph->inherit(_atelier, _glyph_tree, this, glyph_iter, _artist, _config);
}

inline void  ICanvas_summer::inherit(
    IAtelier_summer*      atelier,
    CGlyphTree_summer*    tree,
    GlyphTreeIter_summer& tree_iter,
    eArtist*              artist,
    CWarmguiConfig*       config,
    AppBitmap_summer*     appbmp)
{
    _atelier = atelier,
    _glyph_tree = tree,
    _tree_iter = tree_iter,
    _artist = artist,
    _config = config,
    _appbmp = appbmp;

    inherit_config_string();
}

HRESULT ICanvas_summer::init()
{
    return S_OK;
}


CBkgCanvas_summer::CBkgCanvas_summer(void)
	: _imgBkg(0)
{
    _mytype = IGlyph_summer::GLYPH_TYPE_BKG;
    setClass();
}

CBkgCanvas_summer::CBkgCanvas_summer(const char* name)
	: ICanvas_summer(name)
{
    _mytype = IGlyph_summer::GLYPH_TYPE_BKG;
    setClass();
}

CBkgCanvas_summer::~CBkgCanvas_summer(void)
{
}


HRESULT CBkgCanvas_summer::init()
{
	if (!_config)
		return (-1);

	bool r = true;
	try {
		RECT rect;
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.rect", _str_conf);
		r = _config->getRect(rect, temp);
		if (r) {
			_imgBkg = new CSharedImage_summer(GLYPH_TYPE_BKG, _appbmp->_material);
            _imgBkg->set_name("backgound-image");
			//the opacity must be 1.0f for backgound canvas
			_imgBkg->SetSharedImageRect(rect);
            this->append_glyph(_imgBkg);
		}
	} catch(Poco::NotFoundException&) {
		return(-2);
	}

	return (r) ? S_OK : (-1);
}

} //namespace WARMGUI