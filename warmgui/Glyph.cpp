#include "StdAfx.h"
#include "warmgui_incs.h"

WARMGUI_API RECT NULL_RECT = {0, 0, 0, 0};
WARMGUI_API POINT_f NULL_POINT_f = {0, 0};

namespace WARMGUI {

IGlyphNode::IGlyphNode(void)
	: _myid(0)
	, _rect(NULL_RECT)
	, _common_artist(0)
	, _glyph_state(GLYPH_STATE_NORMAL)
	, _changed_type(GLYPH_CHANGED_TYPE_NONE)
	, _visible(true)
{
	*_name = 0;
    setClass();
}

IGlyphNode::IGlyphNode(const char* name)
	: _myid(0)
	, _rect(NULL_RECT)
	, _common_artist(0)
	, _glyph_state(GLYPH_STATE_NORMAL)
	, _changed_type(GLYPH_CHANGED_TYPE_NONE)
	, _visible(true)
{
	strcpy_s(_name, MAX_WARMGUI_NAME_LEN, name);
    setClass();
}


IGlyphNode::~IGlyphNode(void)
{
}


inline bool IGlyphNode::Intersect(int x, int y)
{
	return (x <= _rect.right
            && x >= _rect.left
			&& y <= _rect.bottom
            && y >= _rect.top);
}

inline IGlyphNode::EDGE_POSITION_TYPE IGlyphNode::IsEdge(int /*x*/, int /*y*/)
{
	return IGlyphNode::EDGE_POSITION_TYPE_NONE;
}

int  IGlyphNode::OnMouseMove    (int x, int y) {return (0);}
int  IGlyphNode::OnLButtonUp    (int x, int y) {return (0);}
int  IGlyphNode::OnRButtonUp    (int x, int y) {return (0);}

void IGlyphNode::OnSize         (UINT, int, int) {}
int  IGlyphNode::OnSetfocus     (WPARAM, LPARAM) {return (0);}
int  IGlyphNode::OnKillfocus    (WPARAM, LPARAM) {return (0);}
int  IGlyphNode::OnEnable       (WPARAM, LPARAM) {return (0);}
void IGlyphNode::OnPaint        () {}
int  IGlyphNode::OnClose        (WPARAM, LPARAM) {return (0);}
int  IGlyphNode::OnErasebkgnd   (WPARAM, LPARAM) {return (0);}




////////////////////////////////////////////////////////////////////////////////
/// class IGlyph
IGlyph::IGlyph(void)
	: _canvas(0)
    , _artist(0)
	, _atelier(0)
	, _bFill(false)
    , _lockArtist(0)
    , _myown_artist(0)
	, _bResponseInput(false)
	, _glyph_type(GLYPH_TYPE_GLYPH)
	, _draw_method(DRAW_METHOD_TYPE_DRAW)
    , _ops(0)
{
	memset(&_rect, 0, sizeof(RECT));
	*_name = L'\0';
    setClass();
}


IGlyph::IGlyph(const char* name, bool own_artist/* = false*/)
	: _canvas(0)
    , _artist(0)
	, _atelier(0)
	, _bFill(false)
    , _myown_artist(0)
	, _bResponseInput(false)
	, _glyph_type(GLYPH_TYPE_GLYPH)
	, _draw_method(DRAW_METHOD_TYPE_DRAW)
    , _ops(0)
{
	memset(&_rect, 0, sizeof(RECT));
	strcpy_s(_name, MAX_WARMGUI_NAME_LEN, name);

    if (own_artist) {
        _myown_artist = new eArtist();
          _lockArtist = new CriticalLock();
    } else {
        _myown_artist = 0;
        _lockArtist   = 0;
    }
    setClass();
}

IGlyph::~IGlyph(void)
{
    if (_ops)          delete _ops;
    if (_lockArtist)   delete _lockArtist;
    if (_myown_artist) delete _myown_artist;
}


HRESULT IGlyph::PreDraw()
{
    if (_myown_artist) {
        CriticalLock::Scoped scope(_myown_artist->_lock_artist);
		eArtist* back_artist = _artist;
		_artist = _myown_artist;
        HRESULT r = Draw();
		_artist = back_artist;
		return r;
    }
    
    return  S_OK;
}

HRESULT IGlyph::Draw(bool redraw/* = false*/)
{
	if (!_visible) return S_OK;

	HRESULT hr = S_OK;
    if (_myown_artist) {
        CriticalLock::Scoped scope(_myown_artist->_lock_artist);

		eArtist* back_artist = _artist;
        D2D1_RECT_F dest = {0, 0, fRectWidth(_rect), fRectHeight(_rect)};
        D2D1_RECT_F src  = {0, 0, fRectWidth(_rect), fRectHeight(_rect)};

        MATRIX_2D backmatrix, m;
        _artist->GetTransform(&backmatrix);
        m = backmatrix;
        m._31 = _rect.left, m._32 = _rect.top;
        _artist->SetTransform(&m);

        _artist->DrawBitmap(_myown_artist->GetDefaultBmp(), dest, src);
        _artist->SetTransform(&backmatrix);

		_artist = back_artist;
    } else {
        hr = DrawGraph(redraw);
    }
	//GetbackArtistTransform();
	_changed_type = GLYPH_CHANGED_TYPE_NONE;
	return hr;
}

HRESULT IGlyph::Renew()
{
	HRESULT hr = S_OK;
	//using common_artist
	//SetReferenceFrameTransform();
	hr = RenewGraph();
	//GetbackArtistTransform();
	_changed_type = GLYPH_CHANGED_TYPE_NONE;
	return hr;
}

HRESULT IGlyph::DrawBkg(bool drawbuf/* = true*/)
{
    //using common_artist
	if (this->_glyph_type == IGlyph::GLYPH_TYPE_BKG) {
        _artist = _common_artist;
		return Draw(drawbuf);
    } else
		return S_OK;
}

HRESULT IGlyph::RenewGraph()
{
	return S_OK;
}

inline void IGlyph::SetRenderTarget()
{
    if (_myown_artist && _common_artist) {
		CriticalLock::Scoped scope(_myown_artist->_lock_artist);
		_myown_artist->SetHwndRenderTarget(_common_artist->GetHwndRT(), _common_artist->GetHwnd());
    }
}


inline void IGlyph::SetTransform(const MATRIX_2D* mytrans)
{
    MATRIX_2D _backup_trans;
	_artist->GetTransform(&_backup_trans);
    MATRIX_2D trans = D2D1::Matrix3x2F::Identity();
    trans._11 = mytrans->_11, trans._12 = mytrans->_12, trans._21 = mytrans->_21, trans._22 = mytrans->_22,
    trans._31 += _backup_trans._31 + _rect.left, trans._32 += _backup_trans._32 + _rect.top;
	_artist->SetTransform(&trans);

#		ifdef _DEBUG
	/*
	MATRIX_2D m;
	_artist->GetTransform(&m);
	MYTRACE(L"ReferFrame %s %.02f %.02f %.02f %.02f %.02f %.02f\n", 
		_name,
		m._11, m._12, m._21, m._22, m._31, m._32);
	*/
#		endif //_DEBUG
}


inline void IGlyph::InvertTransform(int x, int y, POINT_f* ptOut)
{
	D2D1::Matrix3x2F transform;
	_artist->GetTransform(&transform);
	POINT_f pt;
	if (transform.Invert()) {
		pt= transform.TransformPoint(D2D1::Point2F((float)x, (float)y));
		ptOut->x = pt.x, ptOut->y = pt.y;
	} else {
		ptOut->x = (float)x, ptOut->y = (float)y;
	}
}

inline void IGlyph::Transform(float x, float y, POINT* ptOut)
{
	D2D1::Matrix3x2F transform;
	_artist->GetTransform(&transform);
	POINT_f pt;
	pt= transform.TransformPoint(D2D1::Point2F(x, y));
	ptOut->x = (int)pt.x, ptOut->y = (int)pt.y;
}


int  IGlyph::OnCut          (WPARAM, LPARAM) {return (0);}
int  IGlyph::OnCopy         (WPARAM, LPARAM) {return (0);}
int  IGlyph::OnPaste        (WPARAM, LPARAM) {return (0);}
int  IGlyph::OnClear        (WPARAM, LPARAM) {return (0);}
int  IGlyph::OnUndo         (WPARAM, LPARAM) {return (0);}

void  IGlyph::Translation() {}
void  IGlyph::Roate() {}
void  IGlyph::Skew() {}
void  IGlyph::Mirror() {}


inline void IGlyph::SetRect(RECT& rect)
{
    _changed_type = GLYPH_CHANGED_TYPE_GLYPH_RESIZE, _rect = rect;

    if (_myown_artist) {
		CriticalLock::Scoped scope(_myown_artist->_lock_artist);
        _myown_artist->ResizeRenderTarget(RectWidth(rect), RectHeight(rect));
	}
}




////////////////////////////////////////////////////////////////////////////////
/// class CImage
CImage::CImage()
	: _opacity(1.0f)
//	, _srcRect(D2D1::RectF(0, 0, 0, 0))
	, _pImage(0)
{
	memset(_imgpath, 0, MAX_PATH);
	memset(&_srcRect,0, sizeof(RECT));
    setClass();
}

CImage::~CImage()
{
	SafeRelease(&_pImage);
}

CImage::CImage(const char* name, const char* imgpath)
	: _pImage(0)
	, _opacity(1.0f)
//	, _srcRect(D2D1::RectF(0, 0, 0, 0))
{
	strcpy_s(_name, MAX_WARMGUI_NAME_LEN, name);
	TCHAR Out[MAX_PATH];
	CChineseCodeLib::Gb2312ToUnicode(Out, MAX_PATH, (char*)imgpath);
	_tcscpy_s(_imgpath, MAX_PATH, Out);
	memset(&_srcRect,0, sizeof(RECT));
    setClass();
}

CImage::CImage(const char* name, const TCHAR* imgpath)
	: _pImage(0)
	, _opacity(1.0f)
//	, _srcRect(D2D1::RectF(0, 0, 0, 0))
{
	strcpy_s(_name, MAX_WARMGUI_NAME_LEN, name);
	_tcscpy_s(_imgpath, MAX_PATH, imgpath);
	memset(&_srcRect,0, sizeof(RECT));
    setClass();
}


CImage::CImage(const char* name)
	: _pImage(0)
	, _opacity(1.0f)
{
	strcpy_s(_name, MAX_WARMGUI_NAME_LEN, name);
	memset(_imgpath, 0, MAX_PATH);
	memset(&_srcRect,0, sizeof(RECT));
    setClass();
}


inline void CImage::SetSrcRect(RECT* rect)
{
	_srcRect = *rect;
}

inline void CImage::SetSrcRectFromScaledRect(RECT* scaledRect, RECT* targetRect)
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


HRESULT CImage::DrawGraph_wired()
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

HRESULT CImage::DrawGraph(bool /*redraw = false*/)
{
	return DrawImage(_srcRect, _rect, _opacity);
}

inline HRESULT CImage::DrawImage(RECT& rectSrc, RECT& rectDest, float opacity/* = 1.0f*/)
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


inline HRESULT CImage::DrawImage(RECT_f& rectSrc, RECT_f& rectDest, float opacity/* = 1.0f*/)
{
	if (!_pImage || !_artist)
		return (-1);

	_artist->DrawBitmap(_pImage, rectDest, rectSrc, opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	return (S_OK);
}

inline HRESULT CImage::SetImage(const TCHAR* imgpath /*= 0 */)
{
	if (!_artist || (!imgpath && !_imgpath))
		return (-1);

	if (imgpath)
		_tcscpy_s(_imgpath, MAX_PATH, imgpath);

	SafeRelease(&_pImage);
	return WARMGUI::GetBitmapFromFile((const TCHAR*)_imgpath, _artist->GetBmpRT(), &_pImage);
}


inline HRESULT CImage::GetSharedImage(WGBitmap** pDestBmp) const
{
	if (!_artist->GetBmpRT() || !_pImage || !pDestBmp)
		return (-1);
	SafeRelease(pDestBmp);
	return WARMGUI::GetSharedBitmap(_artist->GetBmpRT(), _pImage, pDestBmp);
}

inline HRESULT CImage::SetSharedImage(const WGBitmap* pSrcBmp) const
{
	if (!_artist->GetBmpRT() || !pSrcBmp)
		return (-1);
	SafeRelease((WGBitmap**)&pSrcBmp);
	return WARMGUI::GetSharedBitmap(_artist->GetBmpRT(), pSrcBmp, (WGBitmap**)(&_pImage));
}


inline void CImage::SetOpacity(float opacity)
{
	_opacity = opacity;
}



//////////////////////////////////////////////////////////////////////////////////
// class CSharedImage 
CSharedImage::CSharedImage()
	: _srcRect(D2D1::RectF(0, 0, 0, 0))
	, _dstRect(D2D1::RectF(0, 0, 0, 0))
	, _pBmp(0)
	, _opacity(1.0f)
    , _sm(GLYPH_STRETCH_METHOD_RECT)
{
	memset(_imgpath, 0, MAX_WARMGUI_NAME_LEN);
    setClass();
}

CSharedImage::CSharedImage(IGlyph::GLYPH_TYPE glyph_type, WGBitmap* pBmp, GLYPH_STRETCH_METHOD sm/* = GLYPH_STRETCH_METHOD_RECT*/)
	: _srcRect(D2D1::RectF(0, 0, 0, 0))
	, _dstRect(D2D1::RectF(0, 0, 0, 0))
	, _pBmp(pBmp)
	, _opacity(1.0f)
    , _sm(sm)
{
	_glyph_type = glyph_type;
    setClass();
}

CSharedImage::~CSharedImage()
{
}

HRESULT CSharedImage::DrawImage(RECT& rectSrc, RECT& rectDest, float opacity/* = 1.0f*/)
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

void CSharedImage::SetSharedImage(RECT& rectSrc, float opacity/* = 1.0f*/)
{
	_srcRect = D2D1::RectF(static_cast<float>(rectSrc.left),
						   static_cast<float>(rectSrc.top),
						   static_cast<float>(rectSrc.right),
						   static_cast<float>(rectSrc.bottom));

	_opacity = opacity;
}

inline void CSharedImage::SetRect(RECT& rect)
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

inline HRESULT CSharedImage::DrawGraph(bool /*redraw = false*/)
{
	if (!_pBmp || !_artist)
		return (-1);
	//D2D1_SIZE_F size = _pBmp->GetSize();
	_artist->DrawBitmap(_pBmp, _dstRect, _srcRect, _opacity);
	return (0);
}

static uint32_t k = 0;
static COLORREF ccc[3] = {BGR(255, 0, 0), BGR(0, 255, 0), BGR(0, 0, 255)};

/////////////////////////////////////////////////////////////////////////////////////////////////
// class CBlind
HRESULT CBlind::DrawGraph(bool /*redraw = false*/)
{
	//MYTRACE(L"BLIND %s: DrawGraph\n", _name);
	RECT_f frect;
	frect.left = 0,
		frect.right = static_cast<float>(RectWidth(_rect)),
		frect.top = 0,
		frect.bottom = static_cast<float>(RectHeight(_rect));
	//MATRIX_2D matrix;
	//_artist->GetTransform(&matrix);
	//MYTRACE(L"BLIND %s: %.02f %.02f, frect: %.02f %.02f %.02f %.02f\n", _name, matrix._31, matrix._32, frect.left, frect.top, frect.right, frect.bottom);
	ID2D1SolidColorBrush* scBrush = _artist->GetSCBrush();
	COLORALPHA   clr = scBrush->GetColor();
	scBrush->SetColor(D2D1::ColorF(_bkgclr, _alpha));
	//scBrush->SetColor(D2D1::ColorF(ccc[(k++)%3], _alpha));
	_artist->FillRectangle(frect);
	scBrush->SetColor(clr);

	return S_OK;
}






} //namespace WARMGUI
