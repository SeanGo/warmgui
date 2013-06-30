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

CCoordFrame::CCoordFrame(const char* name)
	: IDataGraph(name, true, true)
    , _pTextFormat(0)
    , _pTextLayout(0)
{
    _glyph_type = IGlyph::GLYPH_TYPE_BKG;
    setClass();
}

CCoordFrame::~CCoordFrame()
{
    SafeRelease(&_pTextFormat);
    SafeRelease(&_pTextLayout);
}

inline void CCoordFrame::SetLineColor(COLORREF color, float a/* = 1.0f */)
{
	_artist->SetSolidColorBrush(D2D1::ColorF(color, a));
}


#define RULER_LEFT   static_cast<float>(_ruler_width.left)
#define RULER_RIGHT  static_cast<float>(_ruler_width.right)
#define RULER_TOP    static_cast<float>(_ruler_width.top)
#define RULER_BOTTOM static_cast<float>(_ruler_width.bottom)


HRESULT CCoordFrame::DrawGraph(bool redraw)
{
	//MYTRACE(L"CCoordFrame::Draw()\n");

	if (_artist && _rect.right - _rect.left > 0 && _rect.bottom - _rect.top > 0) {
        //TCHAR myname[MAX_PATH];
        //CChineseCodeLib::Gb2312ToUnicode(myname, MAX_PATH, _name);
        //MYTRACE(L"A CCoordFrame %s (%d, %d), (%d, %d)\n", myname, _rect.left, _rect.top, _rect.right, _rect.bottom);

        _artist->SetSolidColorBrush(_clr_line);
		_rect_calc.right = RectWidth(_rect), _rect_calc.bottom = RectHeight(_rect), _rect_calc.left = _rect_calc.top = 0;

		_artist->SetStrokeStyle(D2D1_CAP_STYLE_ROUND,
			D2D1_CAP_STYLE_ROUND,
			D2D1_CAP_STYLE_ROUND,
			D2D1_LINE_JOIN_ROUND,
			10.0f,
			D2D1_DASH_STYLE_SOLID);

		D2D1_ANTIALIAS_MODE am = _artist->GetHwndRT()->GetAntialiasMode();
		_artist->GetUsingRT()->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        _artist->SetTextFormat(_pTextFormat);

        //MYTRACE(L"B CCoordFrame %s (%d, %d), (%d, %d)\n", myname, _rect_calc.left, _rect_calc.top, _rect_calc.right, _rect_calc.bottom);
        //MYTRACE(L"B CCoordFrame %s (%d, %d), (%d, %d)\n", myname, _rect.left, _rect.top, _rect.right, _rect.bottom);

        /*
        MATRIX_2D backmatrix, m;
        _artist->GetTransform(&backmatrix);
        m = backmatrix;
        //m._31 += _rect.left, m._32 += _rect.top;
        _artist->SetTransform(&m);

		MYTRACE(L"CCoordFrame::DrawGraph %s: %.02f %.02f, %.02f %.02f, %.02f %.02f\n", _name, m._11, m._12, m._21, m._22, m._31, m._32);
        MYTRACE(L"CCoordFrame %s (%d, %d), (%d, %d)\n", _name, _rect_calc.left, _rect_calc.top, _rect_calc.right, _rect_calc.bottom);
        MYTRACE(L"CCoordFrame %s (%d, %d), (%d, %d)\n", _name, _rect.left, _rect.top, _rect.right, _rect.bottom);

		D2D1::Matrix3x2F trans(m._11, m._12, m._21, m._22, m._31, m._32);
		POINT_f p1 = trans.TransformPoint(D2D1::Point2F(0, 0));
		POINT_f p2 = trans.TransformPoint(D2D1::Point2F(_rect_calc.right, _rect_calc.bottom));

		MYTRACE(L"CCoordFrame::DrawGraph %s: %.02f %.02f, %.02f %.02f ==> %.02f %.02f, %.02f %.02f\n",
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

void CCoordFrame::DrawSideBar()
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


void CCoordFrame::DrawGrid()
{
	float fRectInterval = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / SPLIT_PIECES;
	float scale = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / (_referframe->_real_world.maxy - _referframe->_real_world.miny);
	float y0 = (_referframe->_real_world.maxy - _referframe->_real_world.y0) * scale + RULER_TOP;

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

void CCoordFrame::DrawRuler()
{
	//_artist->SetTransform(D2D1::Matrix3x2F::Translation(static_cast<float>(_nMarginL), static_cast<float>(_nMarginT)));

	float fRectInterval = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / SPLIT_PIECES;
    float fValueInterval = _referframe->_real_world.height() / SPLIT_PIECES;
    float scale = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / _referframe->_real_world.height();
	float y0 = (_referframe->_real_world.maxy - _referframe->_real_world.y0) * scale + RULER_TOP;

	//Draw Y0
	float y = y0, Y = _referframe->_real_world.y0;
	TCHAR wszText[MAX_PATH];
	DrawRulerText(wszText, MAX_PATH, Y, y);

	y = y0 - fRectInterval;
	Y = _referframe->_real_world.y0 + fValueInterval;
	while(y > _ruler_width.top) {
		DrawRulerText(wszText, MAX_PATH, Y, y);
		y -= fRectInterval, Y += fValueInterval;
	}

	y =  y0 + fRectInterval;
	Y = _referframe->_real_world.y0 - fValueInterval;
	while(y < _rect_calc.bottom - _ruler_width.bottom) {
		DrawRulerText(wszText, MAX_PATH, Y, y);
		y += fRectInterval, Y -= fValueInterval;
	}


	DrawRulerText(wszText, MAX_PATH, _referframe->_real_world.miny, static_cast<float>(_rect_calc.bottom - _ruler_width.bottom));
	DrawRulerText(wszText, MAX_PATH, _referframe->_real_world.maxy, static_cast<float>(_rect_calc.top + _ruler_width.top));
}


void CCoordFrame::DrawRulerText(TCHAR * wszText, size_t len, float Y, float y)
{
	_sntprintf_s(wszText, len, _TRUNCATE, L"%.02f", Y);
		D2D1_RECT_F textRect = D2D1::RectF(
			static_cast<float>(0) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y - _ruler_width.top * .5) / CDxFactorys::GetInstance()->m_dpiScaleY,
			static_cast<float>(_ruler_width.left) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y + _ruler_width.bottom * .5) / CDxFactorys::GetInstance()->m_dpiScaleY);

		_artist->DrawText(wszText, textRect);

		//Right, percent text
		if (_referframe->_real_world.y0 == 0) {
			if (Y - _referframe->_real_world.y0 < 0)
				_sntprintf_s(wszText, len, _TRUNCATE, L"%.02f%%",  (Y - _referframe->_real_world.y0)/(_referframe->_real_world.maxy - _referframe->_real_world.y0) * 100);
			else
				_sntprintf_s(wszText, len, _TRUNCATE, L"+%.02f%%", (Y - _referframe->_real_world.y0)/(_referframe->_real_world.maxy - _referframe->_real_world.y0) * 100);
		} else {
			if (Y - _referframe->_real_world.y0 < 0)
				_sntprintf_s(wszText, len, _TRUNCATE, L"%.02f%%",  (Y - _referframe->_real_world.y0)/_referframe->_real_world.y0 * 100);
			else
				_sntprintf_s(wszText, len, _TRUNCATE, L"+%.02f%%", (Y - _referframe->_real_world.y0)/_referframe->_real_world.y0 * 100);
		}

		textRect = D2D1::RectF(
			static_cast<float>(_rect_calc.right - _ruler_width.right) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y - _ruler_width.top * .5) / CDxFactorys::GetInstance()->m_dpiScaleY,
			static_cast<float>(_rect_calc.right) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y + _ruler_width.bottom * .5) / CDxFactorys::GetInstance()->m_dpiScaleY);
		_artist->DrawText(wszText, textRect);
}

inline void CCoordFrame::SetRect(RECT& rect)
{
    IGlyph::SetRect(rect);
    RECT rect_graph = rect;
    rect_graph.left += getRulerWidth().left,
        rect_graph.top += getRulerWidth().top,
        rect_graph.right -= getRulerWidth().right,
        rect_graph.bottom -= getRulerWidth().bottom;

    _referframe->SetRectSize(rect_graph);
    PreDraw();
}


HRESULT CCoordFrame::PreDraw()
{
    if (_myown_artist) {
        CriticalLock::Scoped scope(_myown_artist->_lock_artist);

		eArtist* _back_artist = _artist;
        _artist = _myown_artist;
        MATRIX_2D a = D2D1::Matrix3x2F::Identity();
        //a._31 = _rect.left, a._32 = _rect.top;
        _artist->BeginBmpDraw(true);
        _artist->SetTransform(&a);
        //MYTRACE(L"CCoordFrame::PreDraw() %.02f %.02f\n", a._31, a._32);
        DrawGraph(true);
        _artist->EndBmpDraw();
        _artist = _back_artist;
    }

    return S_OK;
}


inline HRESULT CCoordFrame::Init()
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.ruler-size", _strconf);
    bool b = _config->getRulerWidth(_ruler_width, temp);
    if (b) {
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.ruler-color-alpha", _strconf);
         b = _config->getColorAlpha(_clr_line, temp);
    }
    if (b) {
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.font", _strconf);
        b = _config->getFontSetting(_font, temp);
    }

    if (b)
        b = CDxFactorys::GetInstance()->CreateTextFormat(_font, &_pTextFormat);
    //add the additional length to limit
    //_rtcset._limit.xn += (_rtcset._predict_len + _rtcset._space_len);
    //must set the world after got down-sampler-interval

    if (b)
        b = IDataGraph::Init();

    return (b) ? S_OK : (-1);
}



inline GLYPH_CHANGED_TYPE CCoordFrame::NewData(IDataContainer* data_cont, MARKET_DATA_TYPE datatype)
{
    /*
    WORLD_RECT* wr = _dc->getWorldByName(_data_name);
    if (!wr) return false;

    if (renew_world(wr)) {
        real_world_to_screen();
        redraw();
    }
    */
    return GLYPH_CHANGED_TYPE_NONE;
}

/*
GLYPH_CHANGED_TYPE CCoordFrame::NewData(int index, float x, float y)
{
    _changed_type = GLYPH_CHANGED_TYPE_NONE;
    if (!index) {
        _real_world.set_minmax(x, y);
        real_world_to_screen();
        _changed_type = (GLYPH_CHANGED_TYPE)((int) _changed_type | ((int)GLYPH_CHANGED_TYPE_COORDFRAME | (int)GLYPH_CHANGED_TYPE_CANVAS_BKG));
        return _changed_type;
    }

    if (_real_world.renew_min_max(x, y)) {
        real_world_to_screen();
        _changed_type = (GLYPH_CHANGED_TYPE)((int) _changed_type | ((int)GLYPH_CHANGED_TYPE_COORDFRAME | (int)GLYPH_CHANGED_TYPE_CANVAS_BKG));
    }
    if (_changed_type)
        PreDraw();
    return _changed_type;
}
*/

GLYPH_CHANGED_TYPE CCoordFrame::renew_world(float x, float y, bool redraw_cood/* = false*/, bool bfirstdata/* = false*/)
{
    _changed_type = GLYPH_CHANGED_TYPE_NONE;
    if (bfirstdata) {
        _referframe->RenewYLimit(x, y);
        _changed_type = GLYPH_CHANGED_TYPE_COORDFRAME;
    } else {
        _changed_type = (_referframe->_real_world.renew_min_max(x, y)) ? GLYPH_CHANGED_TYPE_COORDFRAME : GLYPH_CHANGED_TYPE_NONE;
    }
    if (_changed_type){
        _referframe->RenewByRealWorld();
    }
    
    if (_changed_type || redraw_cood)
    {
        PreDraw();
        _changed_type = (GLYPH_CHANGED_TYPE)(((int)GLYPH_CHANGED_TYPE_COORDFRAME | (int)GLYPH_CHANGED_TYPE_CANVAS_BKG));
    }
    return _changed_type;
}

void CCoordFrame::redraw()
{
    TCHAR name[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    MYTRACE(L"COORD-%s %.02f, %.02f, %.02f, %.02f, %.02f, %.02f, %.02f, %.02f\n",
        name,
        _referframe->_real_world.x0, _referframe->_real_world.y0, _referframe->_real_world.xn, _referframe->_real_world.yn,
        _referframe->_real_world.minpos, _referframe->_real_world.miny, _referframe->_real_world.maxpos, _referframe->_real_world.maxy);
}


}






//openmd e:\datas-old\ctpdata\ctpmmd\if1207-20120629.dat
