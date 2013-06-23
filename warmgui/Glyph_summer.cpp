#include "StdAfx.h"
#include "warmgui_summer.h"

namespace WARMGUI {
IGlyph_summer::IGlyph_summer(void)
    : _own_artist(GLYPH_OWN_ARTIST_TYPE_OUTSIDE)
    , _artist(0)
    //, _my_artist(0)
    , _glyph_tree(0)
    , _tree_iter(0)
    , _config(0)
    , _changed(0)
    , _atelier(0)
    , _canvas(0)
    , _mytype(GLYPH_TYPE_GLYPH)
    , _visible(true)
{
    memset(&_rect, 0 , sizeof(RECT));
    _abs_rect = _rect;
    *_name = *_str_conf = '\0';
}

IGlyph_summer::IGlyph_summer(const char* name)
    : _own_artist(GLYPH_OWN_ARTIST_TYPE_OUTSIDE)
    , _artist(0)
    //, _my_artist(0)
    , _glyph_tree(0)
    , _tree_iter(0)
    , _changed(0)
    , _atelier(0)
    , _canvas(0)
    , _mytype(GLYPH_TYPE_GLYPH)
    , _visible(true)
{
    memset(&_rect, 0 , sizeof(RECT));
    _abs_rect = _rect;
    strcpy_s(_name, MAX_PATH, name);
    *_str_conf = '\0';
}

IGlyph_summer::~IGlyph_summer(void)
{
    if (_own_artist == GLYPH_OWN_ARTIST_TYPE_MYSELF)
        SafeDelete(_artist);
}


bool IGlyph_summer::is_me(const char* name)
{
    return (!strcmp(name, _name));
}

HRESULT IGlyph_summer::draw()
{
#ifdef _DEBUG
    TCHAR name[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    MYTRACE(L"Graph::draw %s\n", name);
#endif //_DEBUG

    return S_OK;
}

inline void IGlyph_summer::set_config(CWarmguiConfig* config, const char *conf_str)
{
    _config = config; strcpy_s(_str_conf, MAX_PATH, conf_str);
}

inline HRESULT IGlyph_summer::push_layer()
{
#ifdef _DEBUG
    TCHAR name[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    MYTRACE(L"Graph::PUSH layer %s %d, %d, %d, %d\n", name, _abs_rect.left, _abs_rect.top, _abs_rect.right, _abs_rect.bottom);
#endif //_DEBUG
    _artist->GetTransform(&_back_trans);
    MATRIX_2D I = D2D1::IdentityMatrix();
    I._31 = (float)_abs_rect.left, I._32 = (float)_abs_rect.top;
    _artist->SetTransform(&I);
    _artist->PushLayer(_rect);
    return S_OK;
}

inline HRESULT IGlyph_summer::pop_layer()
{
#ifdef _DEBUG
    TCHAR name[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    MYTRACE(L"Graph::POP layer %s\n", name);
#endif //_DEBUG
    _artist->PopLayer();
    _artist->SetTransform(&_back_trans);
    return S_OK;
}


HRESULT IGlyph_summer::draw_graph(GLYPH_TYPE glyph_type /*=IGlyph_summer::GLYPH_TYPE_GLYPH*/)
{
    HRESULT hr = S_OK;

    if (_artist && _glyph_tree && (*_tree_iter)) {
        int num_of_children = _glyph_tree->number_of_children(_tree_iter);
        if (num_of_children) {
            if (SUCCEEDED(hr) && _tree_iter != _glyph_tree->begin())
                hr = push_layer();

            if (SUCCEEDED(hr) &&  _visible) {
                for (int i = 0; i < num_of_children; i++) {
                    //draw all 
                    GlyphTreeIter_summer it = _glyph_tree->child(_tree_iter, (unsigned int)i);
                    if ((*it)->get_glyph_type() == glyph_type)
                        hr = (*it)->draw_graph(glyph_type);
                    if (FAILED(hr))
                        break;
                }
            }

            if (SUCCEEDED(hr) && _tree_iter != _glyph_tree->begin())
                hr = pop_layer();
        } else {
            //set transform
            if (this->_mytype == glyph_type) {
                _artist->GetTransform(&_back_trans);
                MATRIX_2D I = D2D1::IdentityMatrix();
                I._31 = (float)_abs_rect.left, I._32 = (float)_abs_rect.top;
                _artist->SetTransform(&I);

#ifdef _DEBUG
                TCHAR name[MAX_PATH];
                CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
                MYTRACE(L"Graph::draw %s\n", name);
#endif //_DEBUG
                hr = draw();

                //get back for transform
                _artist->SetTransform(&_back_trans);
                set_change(GLYPH_CHANGED_NONE);
            }
        }

        return hr;
    }

    return ERROR_GLYPH_TREE;
}


inline void IGlyph_summer::append_glyph(IGlyph_summer* glyph)
{
    GlyphTreeIter_summer glyph_iter = _glyph_tree->append_child(this->_tree_iter, glyph);
    glyph->inherit(_atelier, _glyph_tree, _canvas, glyph_iter, _artist, _config);
}

inline void IGlyph_summer::set_rect(RECT& rect)
{
    _abs_rect = rect;
    _rect.left = _rect.top = 0, _rect.right = RectWidth(rect), _rect.bottom = RectHeight(rect);
    set_children_rect();
}

inline void IGlyph_summer::set_children_rect()
{
    for (int i = 0; i < _glyph_tree->number_of_children(_tree_iter); i++) {
        //draw all 
        GlyphTreeIter_summer it = _glyph_tree->child(_tree_iter, (unsigned int)i);
        (*it)->set_rect(_abs_rect);
    }
}

inline void IGlyph_summer::change(GLYPH_CHANGED_TYPE     changed)
{
    CriticalLock::Scoped scope(_lockChange);
    _changed |= changed ;
    
    if (_atelier) _atelier->change(changed);
    if (_canvas)   _canvas->change(changed);
}




////////////////////////////////////////////////////////////////////////////////
/// class CImage_summer
CImage_summer::CImage_summer()
	: _opacity(1.0f)
//	, _srcRect(D2D1::RectF(0, 0, 0, 0))
	, _pImage(0)
{
	memset(_imgpath, 0, MAX_PATH);
	memset(&_srcRect,0, sizeof(RECT));
    setClass();
}

CImage_summer::~CImage_summer()
{
	SafeRelease(&_pImage);
}

CImage_summer::CImage_summer(const char* name, const char* imgpath)
	: _pImage(0)
	, _opacity(1.0f)
//	, _srcRect(D2D1::RectF(0, 0, 0, 0))
{
	strcpy_s(_name, MAX_PATH, name);
	TCHAR Out[MAX_PATH];
	CChineseCodeLib::Gb2312ToUnicode(Out, MAX_PATH, (char*)imgpath);
	_tcscpy_s(_imgpath, MAX_PATH, Out);
	memset(&_srcRect,0, sizeof(RECT));
    setClass();
}

CImage_summer::CImage_summer(const char* name, const TCHAR* imgpath)
	: _pImage(0)
	, _opacity(1.0f)
//	, _srcRect(D2D1::RectF(0, 0, 0, 0))
{
	strcpy_s(_name, MAX_PATH, name);
	_tcscpy_s(_imgpath, MAX_PATH, imgpath);
	memset(&_srcRect,0, sizeof(RECT));
    setClass();
}


CImage_summer::CImage_summer(const char* name)
	: _pImage(0)
	, _opacity(1.0f)
{
	strcpy_s(_name, MAX_PATH, name);
	memset(_imgpath, 0, MAX_PATH);
	memset(&_srcRect,0, sizeof(RECT));
    setClass();
}


inline void CImage_summer::SetSrcRect(RECT* rect)
{
	_srcRect = *rect;
}

inline void CImage_summer::SetSrcRectFromScaledRect(RECT* scaledRect, RECT* targetRect)
{
	if (!_pImage || scaledRect->right  - scaledRect->left <= 0 || scaledRect->bottom - scaledRect->top <= 0)
		memset(&_srcRect, 0, sizeof(RECT));
	else {
		D2D1_SIZE_F size = _pImage->GetSize();
		float x_scale = size.width  / static_cast<float>(scaledRect->right  - scaledRect->left);
		float y_scale = size.height / static_cast<float>(scaledRect->bottom - scaledRect->top );

		//_srcRect = D2D1::RectF(
		_srcRect.left  = static_cast<int>(targetRect->left   * x_scale),
		_srcRect.top   = static_cast<int>(targetRect->top    * y_scale),
		_srcRect.right = static_cast<int>(targetRect->right  * x_scale),
		_srcRect.bottom= static_cast<int>(targetRect->bottom * y_scale);
	}
}


HRESULT CImage_summer::DrawGraph_wired()
{
	if (!_pImage || !_artist)
		return (-1);

	D2D1_RECT_F dest = D2D1::RectF(
							static_cast<float>(_rect.left),
							static_cast<float>(_rect.top),
							static_cast<float>(_rect.right),
							static_cast<float>(_rect.bottom));
	D2D1_RECT_F src;
	
	if (_srcRect.right - _srcRect.left <= 0 || _srcRect.right - _srcRect.left <= 0) {
		D2D1_SIZE_F size = _pImage->GetSize();
		src = D2D1::RectF(
					static_cast<float>(0),
					static_cast<float>(0),
					size.width,
					size.height);
	} else {
		src = D2D1::RectF(
					static_cast<float>(_srcRect.left),
					static_cast<float>(_srcRect.top),
					static_cast<float>(_srcRect.right),
					static_cast<float>(_srcRect.bottom));
	}

	_artist->DrawBitmap(_pImage,
		dest,
		src,
		_opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	return S_OK;
}

HRESULT CImage_summer::draw()
{
	return DrawImage(_srcRect, _rect, _opacity);
}

inline HRESULT CImage_summer::DrawImage(RECT& rectSrc, RECT& rectDest, float opacity/* = 1.0f*/)
{
	if (!_pImage || !_artist)
		return (-1);
	RECT_f destrect = D2D1::RectF(static_cast<float>(rectDest.left),
					static_cast<float>(rectDest.top),
					static_cast<float>(rectDest.right),
					static_cast<float>(rectDest.bottom));
	RECT_f srcrect = D2D1::RectF(static_cast<float>(rectSrc.left),
					static_cast<float>(rectSrc.top),
					static_cast<float>(rectSrc.right),
					static_cast<float>(rectSrc.bottom));

	_artist->DrawBitmap(_pImage, destrect, srcrect, opacity);

	return S_OK;
}


inline HRESULT CImage_summer::DrawImage(RECT_f& rectSrc, RECT_f& rectDest, float opacity/* = 1.0f*/)
{
	if (!_pImage || !_artist)
		return (-1);

	_artist->DrawBitmap(_pImage, rectDest, rectSrc, opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	return (S_OK);
}

inline HRESULT CImage_summer::SetImage(const TCHAR* imgpath /*= 0 */)
{
	if (!_artist || (!imgpath && !_imgpath))
		return (-1);

	if (imgpath)
		_tcscpy_s(_imgpath, MAX_PATH, imgpath);

	SafeRelease(&_pImage);
	return WARMGUI::GetBitmapFromFile((const TCHAR*)_imgpath, _artist->GetBmpRT(), &_pImage);
}


inline HRESULT CImage_summer::GetSharedImage(WGBitmap** pDestBmp) const
{
	if (!_artist->GetBmpRT() || !_pImage || !pDestBmp)
		return (-1);
	SafeRelease(pDestBmp);
	return WARMGUI::GetSharedBitmap(_artist->GetBmpRT(), _pImage, pDestBmp);
}

inline HRESULT CImage_summer::SetSharedImage(const WGBitmap* pSrcBmp) const
{
	if (!_artist->GetBmpRT() || !pSrcBmp)
		return (-1);
	SafeRelease((WGBitmap**)&pSrcBmp);
	return WARMGUI::GetSharedBitmap(_artist->GetBmpRT(), pSrcBmp, (WGBitmap**)(&_pImage));
}


inline void CImage_summer::SetOpacity(float opacity)
{
	_opacity = opacity;
}



//////////////////////////////////////////////////////////////////////////////////
// class CSharedImage_summer 
CSharedImage_summer::CSharedImage_summer()
	: _srcRect(D2D1::RectF(0, 0, 0, 0))
	, _dstRect(D2D1::RectF(0, 0, 0, 0))
	, _pBmp(0)
	, _opacity(1.0f)
{
	memset(_imgpath, 0, MAX_PATH);
    setClass();
}

CSharedImage_summer::CSharedImage_summer(GLYPH_TYPE glyph_type, WGBitmap* pBmp)
	: _srcRect(D2D1::RectF(0, 0, 0, 0))
	, _dstRect(D2D1::RectF(0, 0, 0, 0))
	, _pBmp(pBmp)
	, _opacity(1.0f)
{
	_mytype = glyph_type;
    setClass();
}

CSharedImage_summer::~CSharedImage_summer()
{
}

HRESULT CSharedImage_summer::DrawImage(RECT& rectSrc, RECT& rectDest, float opacity/* = 1.0f*/)
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

void CSharedImage_summer::SetSharedImageRect(RECT& rectSrc, float opacity/* = 1.0f*/)
{
	_srcRect = D2D1::RectF(static_cast<float>(rectSrc.left),
						   static_cast<float>(rectSrc.top),
						   static_cast<float>(rectSrc.right),
						   static_cast<float>(rectSrc.bottom));

	_opacity = opacity;
}


inline HRESULT CSharedImage_summer::draw()
{
	if (!_pBmp || !_artist)
		return (-1);

    _artist->DrawBitmap(_pBmp, _rect, _srcRect, _opacity);
	return (0);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
// class CBlind
HRESULT CBlind_summer::draw()
{
    _artist->SetSolidColorBrush(D2D1::ColorF(_bkgclr, _alpha));
    _artist->FillRectangle(_rect);

	return S_OK;
}




} //namespace WARMGUI
