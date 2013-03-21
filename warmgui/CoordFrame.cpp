#include "stdafx.h"
#include "warmgui_incs.h"

#define __TRACE_INFO__ 1
#if (__TRACE_INFO__)
#	define DEBUG_TRACE MYTRACE
#else
#	define DEBUG_TRACE(x, ...)
#endif //__TRACE_INFO__

namespace WARMGUI {

/////////////////////////////////////////////////////////////////////////////////
/// class CCoordFrame
CCoordFrame::CCoordFrame()
	: _limit(0)
{
//    _glyph_type = IGlyph::GLYPH_TYPE_BKG;
}

CCoordFrame::CCoordFrame(const TCHAR * name, bool bHasBuffer/* = false*/)
	: IGlyph(name, bHasBuffer)
	, _limit(0)
{
//    _glyph_type = IGlyph::GLYPH_TYPE_BKG;
}

CCoordFrame::~CCoordFrame()
{
}

inline void CCoordFrame::SetWorldRect(LIMIT_2D* limit)
{
	_limit = limit;
}










/////////////////////////////////////////////////////////////////////////////////
/// class CCoordGrid

CCoordGrid::CCoordGrid(const TCHAR * name)
	: CCoordFrame(name, true)
{
}

inline void CCoordGrid::SetTextFormat(	const TCHAR * fontFamilyName,
										float fontSize,
										DWRITE_FONT_WEIGHT   fontWeight  /*= DWRITE_FONT_WEIGHT_NORMAL*/,
										DWRITE_FONT_STYLE    fontStyle   /*= DWRITE_FONT_STYLE_NORMAL*/,
										DWRITE_FONT_STRETCH  fontStretch /*= DWRITE_FONT_STRETCH_NORMAL*/,
										const WCHAR * localeName /*= L"en-us"*/)
{
	if (_artist)
		_artist->SetTextFormat(fontFamilyName, fontSize, fontWeight, fontStyle, fontStretch, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, localeName);
}

inline void CCoordGrid::SetLineColor(COLORREF color, float a/* = 1.0f */)
{
	_artist->SetSolidColorBrush(D2D1::ColorF(color, a));
}



#define RULER_LEFT   static_cast<float>(_ruler_width.left)
#define RULER_RIGHT  static_cast<float>(_ruler_width.right)
#define RULER_TOP    static_cast<float>(_ruler_width.top)
#define RULER_BOTTOM static_cast<float>(_ruler_width.bottom)


HRESULT CCoordGrid::DrawGraph(bool redraw)
{
	//MYTRACE(L"CCoordGrid::Draw()");
	if (!_limit) return (-1);

	if (_artist && _rect.right - _rect.left > 0 && _rect.bottom - _rect.top > 0) {
		_artist->SetSolidColorBrush(D2D1::ColorF(BGR(255, 0, 0), 1.0f));
		_rect_calc.right = RectWidth(_rect), _rect_calc.bottom = RectHeight(_rect), _rect_calc.left = _rect_calc.top = 0;

		_artist->SetStrokeStyle(D2D1_CAP_STYLE_ROUND,
			D2D1_CAP_STYLE_ROUND,
			D2D1_CAP_STYLE_ROUND,
			D2D1_LINE_JOIN_ROUND,
			10.0f,
			D2D1_DASH_STYLE_SOLID);

		D2D1_ANTIALIAS_MODE am = _artist->GetHwndRT()->GetAntialiasMode();
		_artist->GetUsingRT()->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

        /*
        MATRIX_2D backmatrix, m;
        _artist->GetTransform(&backmatrix);
        m = backmatrix;
        //m._31 += _rect.left, m._32 += _rect.top;
        _artist->SetTransform(&m);

		MYTRACE(L"CCoordGrid::DrawGraph %s: %.02f %.02f, %.02f %.02f, %.02f %.02f\n", _name, m._11, m._12, m._21, m._22, m._31, m._32);
        MYTRACE(L"CCoordGrid %s (%d, %d), (%d, %d)\n", _name, _rect_calc.left, _rect_calc.top, _rect_calc.right, _rect_calc.bottom);
        MYTRACE(L"CCoordGrid %s (%d, %d), (%d, %d)\n", _name, _rect.left, _rect.top, _rect.right, _rect.bottom);

		D2D1::Matrix3x2F trans(m._11, m._12, m._21, m._22, m._31, m._32);
		POINT_f p1 = trans.TransformPoint(D2D1::Point2F(0, 0));
		POINT_f p2 = trans.TransformPoint(D2D1::Point2F(_rect_calc.right, _rect_calc.bottom));

		MYTRACE(L"CCoordGrid::DrawGraph %s: %.02f %.02f, %.02f %.02f ==> %.02f %.02f, %.02f %.02f\n",
			_name,
			0, 0,
			_rect_calc.right, _rect_calc.bottom,
			p1.x, p1.y,
			p2.x, p2.y);*/

        
		DrawSideBar();
		DrawGrid();
		DrawRuler();

        //_artist->SetTransform(&backmatrix);
		_artist->GetUsingRT()->SetAntialiasMode(am);

		_artist->SetStrokeStyle(D2D1_CAP_STYLE_ROUND,
			D2D1_CAP_STYLE_ROUND,
			D2D1_CAP_STYLE_ROUND,
			D2D1_LINE_JOIN_ROUND,
			10.0f,
			D2D1_DASH_STYLE_SOLID);
	}

	return S_OK;
}

void CCoordGrid::DrawSideBar()
{
	//draw the Outer Frame
    D2D1_RECT_F rectf = D2D1::RectF(1, 1, (float)_rect_calc.right, (float)_rect_calc.bottom);
	_artist->DrawRectangle(rectf);

	//Left side Bar
	_artist->DrawLine(D2D1::Point2F(RULER_LEFT, .0f), D2D1::Point2F(RULER_LEFT, (float)_rect_calc.bottom));

	//Righ side Bar
	_artist->DrawLine(D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, .0f),
					  D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, fRectHeight(_rect)));

	//Top side Bar
	_artist->DrawLine(D2D1::Point2F(RULER_LEFT, RULER_TOP),
					  D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, RULER_TOP));

	//Bottom side Bar
	_artist->DrawLine(D2D1::Point2F(RULER_LEFT, (float)_rect_calc.bottom - RULER_BOTTOM),
					  D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, (float)_rect_calc.bottom - RULER_BOTTOM));
}


void CCoordGrid::DrawGrid()
{
	float fRectInterval = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / SPLIT_PIECES;
	float scale = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / (_limit->maxy - _limit->miny);
	float y0 = (_limit->maxy - _limit->y0) * scale + RULER_TOP;

	_artist->SetStrokeStyle(D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_LINE_JOIN_ROUND,
		10.0f,
		D2D1_DASH_STYLE_SOLID);

	//Draw Y0 horizon line
	_artist->DrawLine(D2D1::Point2F(RULER_LEFT, y0), D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, y0));

	_artist->SetStrokeStyle(D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_LINE_JOIN_ROUND,
		10.0f,
		D2D1_DASH_STYLE_DASH_DOT_DOT);


	//Draw horizon line that large than Y0
	float y = y0;
	while(y > _ruler_width.top) {
		_artist->DrawLine(D2D1::Point2F(RULER_LEFT, y), D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, y));
		y -= fRectInterval;
	}

	//Draw horizon line that less than Y0
	y = y0;
	while(y < _rect_calc.bottom - _ruler_width.bottom) {
		_artist->DrawLine(D2D1::Point2F(RULER_LEFT, y), D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, y));
		y += fRectInterval;
	}

	//Draw vertical line
	fRectInterval = ((float)_rect_calc.right - RULER_LEFT - RULER_RIGHT) / SPLIT_PIECES;
	for (int i = 1; i < SPLIT_PIECES; i++) {
		_artist->DrawLine(D2D1::Point2F(fRectInterval * i + RULER_LEFT, RULER_TOP),
						  D2D1::Point2F(fRectInterval * i + RULER_LEFT, (float)_rect_calc.bottom - RULER_BOTTOM));
	}
}

void CCoordGrid::DrawRuler()
{
	//_artist->SetTransform(D2D1::Matrix3x2F::Translation(static_cast<float>(_nMarginL), static_cast<float>(_nMarginT)));
	_artist->SetTextFormat(L"Verdana", 10);

	float fRectInterval = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / SPLIT_PIECES;
	float fValueInterval = (_limit->maxy - _limit->miny) / SPLIT_PIECES;
	float scale = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / (_limit->maxy - _limit->miny);
	float y0 = (_limit->maxy - _limit->y0) * scale + RULER_TOP;

	//Draw Y0
	float y = y0, Y = _limit->y0;
	TCHAR wszText[MAX_PATH];
	DrawRulerText(wszText, MAX_PATH, Y, y);

	y = y0 - fRectInterval;
	Y = _limit->y0 + fValueInterval;
	while(y > _ruler_width.top) {
		DrawRulerText(wszText, MAX_PATH, Y, y);
		y -= fRectInterval, Y += fValueInterval;
	}

	y =  y0 + fRectInterval;
	Y = _limit->y0 - fValueInterval;
	while(y < _rect_calc.bottom - _ruler_width.bottom) {
		DrawRulerText(wszText, MAX_PATH, Y, y);
		y += fRectInterval, Y -= fValueInterval;
	}


	DrawRulerText(wszText, MAX_PATH, _limit->miny, static_cast<float>(_rect_calc.bottom - _ruler_width.bottom));
	DrawRulerText(wszText, MAX_PATH, _limit->maxy, static_cast<float>(_rect_calc.top + _ruler_width.top));
}


void CCoordGrid::DrawRulerText(TCHAR * wszText, size_t len, float Y, float y)
{
	_sntprintf_s(wszText, len, _TRUNCATE, L"%.02f", Y);
		D2D1_RECT_F textRect = D2D1::RectF(
			static_cast<float>(0) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y - _ruler_width.top * .5) / CDxFactorys::GetInstance()->m_dpiScaleY,
			static_cast<float>(_ruler_width.left) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y + _ruler_width.bottom * .5) / CDxFactorys::GetInstance()->m_dpiScaleY);

		_artist->DrawText(wszText, textRect);

		//Right, percent text
		if (_limit->y0 == 0) {
			if (Y - _limit->y0 < 0)
				_sntprintf_s(wszText, len, _TRUNCATE, L"%.02f%%",  (Y - _limit->y0)/(_limit->maxy - _limit->y0) * 100);
			else
				_sntprintf_s(wszText, len, _TRUNCATE, L"+%.02f%%", (Y - _limit->y0)/(_limit->maxy - _limit->y0) * 100);
		} else {
			if (Y - _limit->y0 < 0)
				_sntprintf_s(wszText, len, _TRUNCATE, L"%.02f%%",  (Y - _limit->y0)/_limit->y0 * 100);
			else
				_sntprintf_s(wszText, len, _TRUNCATE, L"+%.02f%%", (Y - _limit->y0)/_limit->y0 * 100);
		}

		textRect = D2D1::RectF(
			static_cast<float>(_rect_calc.right - _ruler_width.right) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y - _ruler_width.top * .5) / CDxFactorys::GetInstance()->m_dpiScaleY,
			static_cast<float>(_rect_calc.right) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y + _ruler_width.bottom * .5) / CDxFactorys::GetInstance()->m_dpiScaleY);
		_artist->DrawText(wszText, textRect);
}

inline void CCoordGrid::SetRect(RECT& rect)
{
    IGlyph::SetRect(rect);
    PreDraw();
}


HRESULT CCoordGrid::PreDraw()
{
    if (_myown_artist) {
        eArtist* _back_artist = _artist;
        _artist = _myown_artist;
        MATRIX_2D a = D2D1::Matrix3x2F::Identity();
        _artist->BeginBmpDraw(true);
        _artist->SetTransform(&a);
        DrawGraph(true);
        _artist->EndBmpDraw();
        _artist = _back_artist;
    }

    return S_OK;
}

}






//openmd e:\datas-old\ctpdata\ctpmmd\if1207-20120629.dat
