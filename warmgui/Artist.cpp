#include "StdAfx.h"
#include "warmgui_incs.h"

namespace WARMGUI {

eArtist:: eArtist(void)
	:       _pHwndRT(0)
	, _pDefaultBmpRT(0)
	,      _pscBrush(0)
	,       _pStroke(0)
	,     _pbmpBrush(0)
	,      _plgBrush(0)
	,      _prgBrush(0)
	,   _pTextFormat(0)
	,   _pTextLayout(0)
	,      _pPaintRT(0)
	,         _layer(0)
	,   _bmprt_layer(0)
	,     _pBackupRT(0)
	,   _pDefaultBmp(0)
	, _hwnd(0)
{
}


eArtist::~eArtist(void)
{
	SafeRelease(&   _pDefaultBmp);
	SafeRelease(&      _pscBrush);
	SafeRelease(&       _pStroke);
	SafeRelease(&     _pbmpBrush);
	SafeRelease(&      _plgBrush);
	SafeRelease(&      _prgBrush);
	SafeRelease(& _pDefaultBmpRT);
	SafeRelease(&         _layer);
	SafeRelease(&   _bmprt_layer);
}

void eArtist::SetHwndRenderTarget(ID2D1HwndRenderTarget* pHwndRT, HWND hwnd)
{ 
	if (pHwndRT) {
		_pHwndRT = pHwndRT, _hwnd = hwnd;
	}
}

inline HRESULT eArtist::ResizeRenderTarget(int cx, int cy)
{
    CriticalLock::Scoped scope(_lock_artist);

    SafeRelease(&_pDefaultBmpRT);

	HRESULT hr = _pHwndRT->CreateCompatibleRenderTarget(
		D2D1::SizeF(static_cast<float>(cx), static_cast<float>(cy)),
		&_pDefaultBmpRT);

    if (SUCCEEDED(hr))
            _pPaintRT = _pDefaultBmpRT;
        else
            _pPaintRT = _pHwndRT;      		

	SafeRelease(&_pscBrush);
	if (SUCCEEDED(hr))
		hr = _pPaintRT->CreateSolidColorBrush(D2D1::ColorF(BGR(0, 0, 0)), &_pscBrush);

	SafeRelease(&_pStroke);
	if (SUCCEEDED(hr))
		hr = SetStrokeStyle();

	SafeRelease(&_pDefaultBmp);
	if (SUCCEEDED(hr))
		hr = _pPaintRT->CreateBitmap(
			D2D1::SizeU(cx, cy),
			D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
			&_pDefaultBmp);

	return hr;
}

///Create a stroke-style without D2D1_DASH_STYLE_CUSTOM
inline HRESULT eArtist::SetStrokeStyle(const D2D1_STROKE_STYLE_PROPERTIES& strokeStyleProperties)
{
	if (strokeStyleProperties.dashStyle == D2D1_DASH_STYLE_CUSTOM)
		return (-1);

	SafeRelease(&_pStroke);
	return CDxFactorys::GetInstance()->GetD2DFactory()->CreateStrokeStyle(
				strokeStyleProperties,
				0,
				0,
				&_pStroke);
}

///Create a stroke-style without D2D1_DASH_STYLE_CUSTOM
inline HRESULT eArtist::SetStrokeStyle(
					D2D1_CAP_STYLE startCap    /*= D2D1_CAP_STYLE_ROUND  */,
					D2D1_CAP_STYLE  endCap     /*= D2D1_CAP_STYLE_ROUND  */,
					D2D1_CAP_STYLE  dashCap    /*= D2D1_CAP_STYLE_ROUND  */,
					D2D1_LINE_JOIN  lineJoin   /*= D2D1_LINE_JOIN_ROUND  */,
					FLOAT           miterLimit /*= 10.0f                 */,
					D2D1_DASH_STYLE dashStyle  /*= D2D1_DASH_STYLE_SOLID */,
					FLOAT           dashOffset /*= 0.0f                  */)
{
	if (dashStyle == D2D1_DASH_STYLE_CUSTOM)
		return (-1);

	SafeRelease(&_pStroke);
	return CDxFactorys::GetInstance()->GetD2DFactory()->CreateStrokeStyle(
				D2D1::StrokeStyleProperties(startCap  ,
											endCap    ,
											dashCap   ,
											lineJoin  ,
											miterLimit,
											dashStyle ,
											dashOffset),
				0,
				0,
				&_pStroke);
}

inline HRESULT eArtist::SetStrokeStyle(int argc, TCHAR * argv[])
{
    int c = -1;
	D2D1_CAP_STYLE  startCap   = D2D1_CAP_STYLE_ROUND;
	D2D1_CAP_STYLE  endCap     = D2D1_CAP_STYLE_ROUND;
	D2D1_CAP_STYLE  dashCap    = D2D1_CAP_STYLE_ROUND;
	D2D1_LINE_JOIN  lineJoin   = D2D1_LINE_JOIN_ROUND;
	FLOAT           miterLimit = 1.0f;
	D2D1_DASH_STYLE dashStyle  = D2D1_DASH_STYLE_SOLID;
	FLOAT           dashOffset = 0.0f;

    while(-1 != (c = gettopt(argc, argv, _T("a:e:d:j:m:s:o:"))))
    {
        switch (c)
        {
        case _T('a') :
            startCap = (D2D1_CAP_STYLE)(_tstoi(toptarg));
            break;
        case _T('e') :
            endCap   = (D2D1_CAP_STYLE)(_tstoi(toptarg));
            break;
        case _T('d') :
            dashCap  = (D2D1_CAP_STYLE)(_tstoi(toptarg));
            break;
        case _T('j') :
            lineJoin = (D2D1_LINE_JOIN)(_tstoi(toptarg));
            break;
		case _T('m'):
			miterLimit = (float)_tstof(toptarg);
			break;
		case _T('s'):
			dashStyle = (D2D1_DASH_STYLE)(_tstoi(toptarg));
			break;
		case _T('o'):
			dashOffset = (float)_tstof(toptarg);
			break;
        default :
            return (-1);
        }
    }

	return SetStrokeStyle(startCap, endCap, dashCap, lineJoin, miterLimit, dashStyle, dashOffset);
}

inline HRESULT eArtist::SetSolidColorBrush(const COLORALPHA   &color, GLYPH_STATE_TYPE scbt/* = SOLID_COLOR_BRUSH_NORMAL*/)
{
	if (!_pHwndRT) return (-1);

	if (_pscBrush) {
		_pscBrush->SetColor(color);
        return S_OK;
	} else {
		return _pHwndRT->CreateSolidColorBrush(color, &_pscBrush);
	}
}

inline HRESULT eArtist::SetSolidColorBrush(int argc, TCHAR * argv[])
{
	int c = -1;
	int r = 0, g = 0, b = 0;
	float alpha = 1.0;

    while(-1 != (c = gettopt(argc, argv, _T("r:g:b:a:"))))
    {
        switch (c)
        {
        case _T('a') :
            alpha = (float)_tstof(toptarg);
            break;
        case _T('r') :
            r = (D2D1_CAP_STYLE)(_tstoi(toptarg));
            break;
        case _T('g') :
            g = (D2D1_CAP_STYLE)(_tstoi(toptarg));
            break;
        case _T('b') :
            b = (D2D1_LINE_JOIN)(_tstoi(toptarg));
            break;
        default :
            return (-1);
        }
    }

	return SetSolidColorBrush(D2D1::ColorF(RGB(r, g, b), alpha));
}

inline void eArtist::SetTextFormat(IDWriteTextFormat* pTextFormat)
{
    _pTextFormat = pTextFormat;
}


inline void eArtist::SetTextLayout(IDWriteTextLayout* pTextLayout)
{
    _pTextLayout = pTextLayout;
}



//HRESULT eArtist::SetTextFormat(const TCHAR * fontFamilyName,
							   //FLOAT  fontSize,
							   //DWRITE_FONT_WEIGHT  fontWeight /*= DWRITE_FONT_WEIGHT_NORMAL */,
							   //DWRITE_FONT_STYLE   fontStyle  /*= DWRITE_FONT_STYLE_NORMAL  */,
							   //DWRITE_FONT_STRETCH fontStretch/*= DWRITE_FONT_STRETCH_NORMAL*/,
							   //DWRITE_TEXT_ALIGNMENT textAlignment /*= DWRITE_TEXT_ALIGNMENT_CENTER*/,
							   //DWRITE_PARAGRAPH_ALIGNMENT  paragraphAlignment /*= DWRITE_PARAGRAPH_ALIGNMENT_CENTER*/,
							   //const WCHAR * localeName /*= L"en-us"*/)
/*
{
	HRESULT hr = S_OK;
	IDWriteFontCollection* pFontCollection = NULL;

    // Get the system font collection.
    if (SUCCEEDED(hr))
		hr = CDxFactorys::GetInstance()->GetDWriteFactory()->GetSystemFontCollection(&pFontCollection);

    if (SUCCEEDED(hr)) {
		SafeRelease(&_pTextFormat);
		hr = CDxFactorys::GetInstance()->GetDWriteFactory()->CreateTextFormat(
				fontFamilyName,
				pFontCollection,
				fontWeight,
				fontStyle,
				fontStretch,
				fontSize,
				localeName,
				&_pTextFormat);
	}
	SafeRelease(&pFontCollection);

	if (SUCCEEDED(hr))
		hr = _pTextFormat->SetTextAlignment(textAlignment);

	if (SUCCEEDED(hr))
		hr = _pTextFormat->SetParagraphAlignment(paragraphAlignment);

	return hr;
}

HRESULT eArtist::SetTextFormat(int argc, TCHAR * argv[])
{
	int c = -1;
	TCHAR * fontFamilyName = 0;
	DWRITE_FONT_WEIGHT  fontWeight  = DWRITE_FONT_WEIGHT_NORMAL;
	DWRITE_FONT_STYLE   fontStyle   = DWRITE_FONT_STYLE_NORMAL;
	DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL;
	FLOAT   fontSize = 12.0f;
	TCHAR * localeName = 0;

    while(-1 != (c = gettopt(argc, argv, _T("n:i:w:s:t:l:"))))
    {
        switch (c)
        {
        case _T('n') :
			fontFamilyName = _tcsdup(toptarg);
            break;
        case _T('i') :
			fontSize = (FLOAT)_tstof(toptarg);
            break;
        case _T('w') :
			fontWeight = (DWRITE_FONT_WEIGHT)_tstoi(toptarg);
            break;
        case _T('s') :
            fontStyle  = (DWRITE_FONT_STYLE)_tstoi(toptarg);
			break;
        case _T('t') :
			fontStretch = (DWRITE_FONT_STRETCH)_tstoi(toptarg);
			break;
        case _T('l') :
			localeName = _tcsdup(toptarg);
            break;
		default:
            return (-1);
        }
    }

	HRESULT hr = S_OK;
	if (!fontFamilyName) {
		if (localeName) free(localeName);
		return (-1);
	}

	if (!localeName)
		hr = SetTextFormat(fontFamilyName,
				fontSize,
				fontWeight,
				fontStyle,
				fontStretch);
	else
		hr = SetTextFormat(fontFamilyName,
				fontSize,
				fontWeight,
				fontStyle,
				fontStretch,
				DWRITE_TEXT_ALIGNMENT_CENTER,
				DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
				localeName);

	if (fontFamilyName) free(fontFamilyName);
	if (localeName) free(localeName);

	return hr;
}
*/
inline HRESULT eArtist::Flush()
{
	D2D1_TAG tag1, tag2;
	return _pPaintRT->Flush(&tag1, &tag2);
}


inline void eArtist::BeginBmpDraw(bool clear/* = false */)
{
	if (!_pHwndRT) return;

	UsingBmpRT();
	_pDefaultBmpRT->BeginDraw();
	if (clear) _pDefaultBmpRT->Clear();
}

inline void eArtist::BeginDraw(bool clear/* = false*/)
{
	if (!_pHwndRT) return;

	UsingHwndRT();
	_pHwndRT->BeginDraw();
	if (clear) _pHwndRT->Clear();
}

inline HRESULT eArtist::EndDraw()
{
	_pPaintRT = _pBackupRT;
	return _pHwndRT->EndDraw();
}


inline void eArtist::UsingHwndRT()
{
	_pBackupRT = _pPaintRT;
	_pPaintRT = _pHwndRT;
}

inline void eArtist::UsingBmpRT()
{
	_pBackupRT = _pPaintRT;
	_pPaintRT = _pDefaultBmpRT;
}

inline HRESULT eArtist::EndBmpDraw()
{
	_pPaintRT = _pBackupRT;
    HRESULT hr = S_OK;
	//get rt's size
	D2D1_SIZE_F size = _pDefaultBmpRT->GetSize();
	//D2D1_SIZE_F size2 = _pDefaultBmp->GetSize();

	//MYTRACE(L"Size 1 %.02f %.02f, Size 2 %.02f %.02f\n", size.width, size.height, size2.width, size2.height);
	//save and set transform
	MATRIX_2D trans;
	_pDefaultBmpRT->GetTransform(&trans);
	_pDefaultBmpRT->SetTransform(D2D1::Matrix3x2F::Identity());

	//copy bitmap
	D2D1_POINT_2U pnt0 = D2D1::Point2U(0,0);
	D2D1_RECT_U rect = D2D1::RectU(0, 0, static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height));
	if (_pDefaultBmp)
        hr = _pDefaultBmp->CopyFromRenderTarget(&pnt0, _pDefaultBmpRT, &rect);

	hr = _pDefaultBmpRT->EndDraw();
	//backup transfprm
	_pDefaultBmpRT->SetTransform(&trans);
	return hr;
}

inline void eArtist::DrawBmpRTBitmap(WGBitmap* pImage,
						RECT& rectDest,
						RECT& rectSrc,
						float opacity/* = 1.0f*/,
						D2D1_BITMAP_INTERPOLATION_MODE interpolationMode/* = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR*/)
{
	if (!pImage) return;
	D2D1_POINT_2U upntDest = D2D1::Point2U(rectDest.left, rectDest.top);
	D2D1_RECT_U urectSrc = D2D1::RectU(rectSrc.left, rectSrc.top, rectSrc.right, rectSrc.bottom);
	if (SUCCEEDED(pImage->CopyFromRenderTarget(&upntDest, _pDefaultBmpRT, &urectSrc))) {
		D2D1_RECT_F dest = D2D1::RectF(	static_cast<float>(rectDest.left),
										static_cast<float>(rectDest.top),
										static_cast<float>(rectDest.right),
										static_cast<float>(rectDest.bottom));

		D2D1_RECT_F src  = D2D1::RectF(	static_cast<float>(rectSrc.left),
										static_cast<float>(rectSrc.top),
										static_cast<float>(rectSrc.right),
										static_cast<float>(rectSrc.bottom));

		DrawBitmap(pImage, dest, src, opacity, interpolationMode);
	}
}


inline void eArtist::DrawBitmap(WGBitmap* pImage,
								RECT& rectDest,
								RECT& rectSrc,
								float opacity /* = 1.0f */,
								D2D1_BITMAP_INTERPOLATION_MODE interpolationMode/* = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR*/)
{
	D2D1_RECT_F dest = D2D1::RectF(	static_cast<float>(rectDest.left),
									static_cast<float>(rectDest.top),
									static_cast<float>(rectDest.right),
									static_cast<float>(rectDest.bottom));

	D2D1_RECT_F src  = D2D1::RectF(	static_cast<float>(rectSrc.left),
									static_cast<float>(rectSrc.top),
									static_cast<float>(rectSrc.right),
									static_cast<float>(rectSrc.bottom));

	DrawBitmap(pImage, dest, src, opacity, interpolationMode);
}

inline void eArtist::DrawBitmap(WGBitmap* pImage,
								RECT& rectDest,
								RECT_f& rectSrc,
								float opacity /* = 1.0f */,
								D2D1_BITMAP_INTERPOLATION_MODE interpolationMode/* = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR*/)
{
	D2D1_RECT_F dest = D2D1::RectF(	static_cast<float>(rectDest.left),
									static_cast<float>(rectDest.top),
									static_cast<float>(rectDest.right),
									static_cast<float>(rectDest.bottom));
	DrawBitmap(pImage, dest, rectSrc, opacity, interpolationMode);
}


inline void eArtist::DrawBitmap(WGBitmap* pImage,
								D2D1_RECT_F& rectDest,
								D2D1_RECT_F& rectSrc,
								float opacity /* = 1.0f */,
								D2D1_BITMAP_INTERPOLATION_MODE interpolationMode/* = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR*/)
{
	if (_pPaintRT && pImage)
		_pPaintRT->DrawBitmap(pImage, rectDest, opacity, interpolationMode, rectSrc);
}

inline void eArtist::SetTransform(MATRIX_2D* trans)
{
	if (_pPaintRT)
		_pPaintRT->SetTransform(trans);
}

inline void eArtist::GetTransform(MATRIX_2D* trans)
{
	if (_pPaintRT)
		_pPaintRT->GetTransform(trans);
}

inline void eArtist::SetTranslationTrasform(RECT& rect)
{
	if (_pPaintRT) {
		MATRIX_2D m = D2D1::Matrix3x2F::Identity();
		m._31 = static_cast<float>(rect.left), m._32 = static_cast<float>(rect.top);
		_pPaintRT->SetTransform(m);
	}
}

inline void eArtist::GetHwndRtTransform(MATRIX_2D* trans)
{
	if (_pHwndRT)
		_pHwndRT->GetTransform(trans);
}


inline void eArtist::DrawText(const TCHAR * text, D2D1_RECT_F& rect)
{
    if (_pTextFormat)
	    _pPaintRT->DrawText(text, _tcslen(text), _pTextFormat, rect, _pscBrush);
}

inline void eArtist::DrawText(const TCHAR * text, RECT& rect)
{
    if (_pTextFormat) {
        D2D1_RECT_F rect_f = D2D1::RectF(
            (float)rect.left,
            (float)rect.top,
            (float)rect.right,
            (float)rect.bottom);
	    _pPaintRT->DrawText(text, _tcslen(text), _pTextFormat, rect_f, _pscBrush);
    }
}


inline void eArtist::DrawRectangle(D2D1_RECT_F& rect_f, float fStrokeWidth/* = 1.0f*/)
{
	_pPaintRT->DrawRectangle(rect_f, _pscBrush, fStrokeWidth, _pStroke);
}

inline void eArtist::DrawRectangle(float x1, float y1, float x2, float y2, float fStrokeWidth/* = 1.0f*/)
{
	D2D1_RECT_F rectf = D2D1::RectF(x1, y1, x2, y2);
	DrawRectangle(rectf, fStrokeWidth);
}

inline void eArtist::DrawRectangle(RECT& rect, float fStrokeWidth/* = 1.0f*/)
{
    D2D1_RECT_F rect_f = D2D1::RectF(
        (float)rect.left,
        (float)rect.top,
        (float)rect.right,
        (float)rect.bottom);
	DrawRectangle(rect_f, fStrokeWidth);
}

inline void eArtist::FillRectangle(RECT_f& rect_f, float fStrokeWidth/* = 1.0f*/)
{
	_pPaintRT->FillRectangle(rect_f, _pscBrush);
}

inline void eArtist::FillRectangle(RECT& rect, float fStrokeWidth/* = 1.0f*/)
{
    D2D1_RECT_F rect_f = D2D1::RectF(
        (float)rect.left,
        (float)rect.top,
        (float)rect.right,
        (float)rect.bottom);
	_pPaintRT->FillRectangle(rect_f, _pscBrush);
}


inline void eArtist::FillRectangle(float x1, float y1, float x2, float y2, float fStrokeWidth/* = 1.0f*/)
{
	D2D1_RECT_F rectf = D2D1::RectF(x1, y1, x2, y2);
	FillRectangle(rectf, fStrokeWidth);
}

inline void eArtist::DrawEllipse(ELLIPSE_f& ellipse, float fStrokeWidth/* = 1.0f*/)
{
	_pPaintRT->DrawEllipse(ellipse, _pscBrush, fStrokeWidth, _pStroke);
}

inline void eArtist::FillEllipse(ELLIPSE_f& ellipse, float fStrokeWidth/* = 1.0f*/)
{
	_pPaintRT->FillEllipse(ellipse, _pscBrush);
}

inline void eArtist::Clear()
{
    _pPaintRT->Clear();
}

inline void eArtist::DrawGeometry(ID2D1Geometry *geometry, ID2D1Brush *brush, FLOAT strokeWidth/* = 1.0f*/,ID2D1StrokeStyle *strokeStyle/* = 0*/)
{
    _pPaintRT->DrawGeometry(geometry, brush, strokeWidth, strokeStyle);
}


inline HRESULT eArtist::CreateBitmap(WGBitmap** pBitmap, RECT& rect)
{
	SafeRelease(pBitmap);
	return _pHwndRT->CreateBitmap(
				D2D1::SizeU(RectWidth(rect), RectHeight(rect)),
				D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
				pBitmap);
}


inline HRESULT eArtist::CopyFromRenderTarget(WGBitmap* pBitmap, POINT pointDest, RECT rectSrc)
{
	HRESULT hr = (-1);
	if (pBitmap) {
		D2D1_POINT_2U pntDest = D2D1::Point2U(pointDest.x, pointDest.y);
		D2D1_RECT_U   src = D2D1::RectU(rectSrc.left, rectSrc.top, rectSrc.right, rectSrc.bottom);
		hr = pBitmap->CopyFromRenderTarget(&pntDest, _pDefaultBmpRT, &src);
	}
	return hr;
}

inline void eArtist::DrawLine(float x1, float y1, float x2, float y2, float fStrokeWidth/* = 1.0f*/)
{
	POINT_f p1 = D2D1::Point2F(x1, y1), p2 = D2D1::Point2F(x2, y2);
	_pPaintRT->DrawLine(p1, p2, _pscBrush, fStrokeWidth, _pStroke);
}

inline void eArtist::DrawLine(POINT_f p1, POINT_f p2, float fStrokeWidth/* = 1.0f*/)
{
	_pPaintRT->DrawLine(p1, p2, _pscBrush, fStrokeWidth, _pStroke);
}


inline HRESULT eArtist::PushLayer(float left, float top, float right, float bottom)
{
	SafeRelease(&_layer);
	HRESULT hr = S_OK;
	hr = _pPaintRT->CreateLayer(&_layer);
	if (SUCCEEDED(hr))
		_pPaintRT->PushLayer(
			D2D1::LayerParameters(
				D2D1::RectF(
					(float)left,
					(float)top,
					(float)right,
					(float)bottom)),
				_layer);
	return hr;
}

inline HRESULT eArtist::PushLayer(RECT& rect)
{
	return PushLayer((float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom);
}

inline void eArtist::PopLayer()
{
	_pPaintRT->PopLayer();
	SafeRelease(&_layer);
}


}
