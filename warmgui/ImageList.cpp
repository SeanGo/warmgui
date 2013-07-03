#include "StdAfx.h"
#include "warmgui_summer.h"

namespace WARMGUI {

//////////////////////////////////////////////////////////////////////////////////
// class CImageList 
CImageList::CImageList()
	: _srcRect(D2D1::RectF(0, 0, 0, 0))
	, _dstRect(D2D1::RectF(0, 0, 0, 0))
	, _pBmp(0)
	, _opacity(1.0f)
    , _sm(GLYPH_STRETCH_METHOD_RECT)
{
	memset(_imgpath, 0, MAX_WARMGUI_NAME_LEN);
    setClass();
}

CImageList::CImageList(IGlyph::GLYPH_TYPE glyph_type, WGBitmap* pBmp, GLYPH_STRETCH_METHOD sm/* = GLYPH_STRETCH_METHOD_RECT*/)
	: _srcRect(D2D1::RectF(0, 0, 0, 0))
	, _dstRect(D2D1::RectF(0, 0, 0, 0))
	, _pBmp(pBmp)
	, _opacity(1.0f)
    , _sm(sm)
{
	_glyph_type = glyph_type;
    setClass();
}

CImageList::~CImageList()
{
}

HRESULT CImageList::DrawImage(RECT& rectSrc, RECT& rectDest, float opacity/* = 1.0f*/)
{
	D2D1_RECT_F srcRect = D2D1::RectF(static_cast<float>(rectSrc.left),
									static_cast<float>(rectSrc.top),
									static_cast<float>(rectSrc.right),
									static_cast<float>(rectSrc.bottom));

    D2D1_RECT_F dstRect = D2D1::RectF(static_cast<float>(rectDest.left),
		    				  static_cast<float>(rectDest.top),
                              static_cast<float>(rectDest.right),
                              static_cast<float>(rectDest.bottom));
	if (!_pBmp || !_artist)
		return (-1);

	_artist->DrawBitmap(_pBmp, dstRect, srcRect, opacity);
	return (0);
}

void CImageList::SetSharedImage(RECT& rectSrc, float opacity/* = 1.0f*/)
{
	_srcRect = D2D1::RectF(static_cast<float>(rectSrc.left),
						   static_cast<float>(rectSrc.top),
						   static_cast<float>(rectSrc.right),
						   static_cast<float>(rectSrc.bottom));

	_opacity = opacity;
}

inline void CImageList::SetRect(RECT& rect)
{
	IGlyph::SetRect(rect);
  	if (_sm == GLYPH_STRETCH_METHOD_RECT) 
	    _dstRect = D2D1::RectF(static_cast<float>(rect.left),
						       static_cast<float>(rect.top),
						       static_cast<float>(rect.right),
						       static_cast<float>(rect.bottom));
    else
        _dstRect = _srcRect;
}

inline HRESULT CImageList::DrawGraph(bool /*redraw = false*/)
{
	if (!_pBmp || !_artist)
		return (-1);
	//D2D1_SIZE_F size = _pBmp->GetSize();
	_artist->DrawBitmap(_pBmp, _dstRect, _srcRect, _opacity);
	return (0);
}

}//namespace WARMGUI

