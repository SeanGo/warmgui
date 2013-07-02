#include "StdAfx.h"
#include "warmgui_summer.h"

#define RULER_LEFT   static_cast<float>(_ruler_width.left)
#define RULER_RIGHT  static_cast<float>(_ruler_width.right)
#define RULER_TOP    static_cast<float>(_ruler_width.top)
#define RULER_BOTTOM static_cast<float>(_ruler_width.bottom)

namespace WARMGUI {

CCoordFrame_summer::CCoordFrame_summer(const char* name)
    : IGlyph_summer(name)
    , _pTextFormat(0)
    , _pTextLayout(0)
{
    _my_own_artist = true;
    _my_artist = new eArtist();
    _world = new CWorld();
    _world->set_world_change(WORLD_CHANGED_TYPE_MAX_X | WORLD_CHANGED_TYPE_MAX_Y | WORLD_CHANGED_TYPE_MIN_X | WORLD_CHANGED_TYPE_MIN_Y);
}


CCoordFrame_summer::~CCoordFrame_summer(void)
{
    SafeRelease(&_pTextFormat);
    SafeRelease(&_pTextLayout);
    SafeDelete(_world);
    SafeDelete(_my_artist);
}

inline HRESULT CCoordFrame_summer::init()
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.ruler-size", _str_conf);
    bool b = _config->getRulerWidth(_ruler_width, temp);
    if (b) {
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.ruler-color-alpha", _str_conf);
         b = _config->getColorAlpha(_clr_line, temp);
    }
    if (b) {
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.font", _str_conf);
        b = _config->getFontSetting(_font, temp);
    }
    if (b) {
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.color", _str_conf);
        b = _config->getColorAlpha(_clr_line, temp);
    }

    if (b) {
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.split-pieces", _str_conf);
        _split_pieces = (float)_config->getDouble(temp);
    }

    if (b) {
        if (!_world->setConfig(_config, _str_conf))
            b = false;
    }

    if (b)
        b = CDxFactorys::GetInstance()->CreateTextFormat(_font, &_pTextFormat);
    //add the additional length to limit
    //_rtcset._limit.xn += (_rtcset._predict_len + _rtcset._space_len);
    //must set the world after got down-sampler-interval
    _my_artist->SetHwndRenderTarget(_artist->GetHwndRT(), _artist->GetHwnd());

    if (b)
        return IGlyph_summer::init();
    else
        return (-1);
}

inline void CCoordFrame_summer::set_rect(RECT& rect)
{
    IGlyph_summer::set_rect(rect);

    _rect_graph = rect;
    _rect_graph.left += get_ruler_width().left,
        _rect_graph.top += get_ruler_width().top,
        _rect_graph.right -= get_ruler_width().right,
        _rect_graph.bottom -= get_ruler_width().bottom;

    _world->SetRectSize(_rect_graph);  //absolutely position
    _my_artist->ResizeRenderTarget(RectWidth(rect), RectHeight(rect));
}


HRESULT CCoordFrame_summer::draw(bool redraw_all/* = false*/)
{
    HRESULT hr = S_OK;
    if (redraw_all || _world->get_world_change()) {
        if (_my_own_artist) {
            //draw all using my artist
            CriticalLock::Scoped scope(_my_artist->_lock_artist);
            _my_artist->BeginBmpDraw(true);
            _draw_coord(_my_artist);
            hr = _my_artist->EndBmpDraw();

            if (SUCCEEDED(hr))
                _artist->DrawBitmap(_my_artist->GetDefaultBmp(), _rect, _rect, 1.0f);
        } else {
            //draw all using common artist
            _draw_coord(_artist);
        }
        //clear world's change
        _world->set_world_change();
    } else if (_my_own_artist) {
        //copy bitmap
        CriticalLock::Scoped scope(_my_artist->_lock_artist);
        _artist->DrawBitmap(_my_artist->GetDefaultBmp(), _rect, _rect, 1.0f);
    }

    return hr;
}

HRESULT CCoordFrame_summer::_draw_coord(eArtist* artist)
{
    artist->SetSolidColorBrush(_clr_line);
	_rect_calc.right = RectWidth(_rect), _rect_calc.bottom = RectHeight(_rect), _rect_calc.left = _rect_calc.top = 0;

	artist->SetStrokeStyle(D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_LINE_JOIN_ROUND,
		10.0f,
		D2D1_DASH_STYLE_SOLID);

	D2D1_ANTIALIAS_MODE am = artist->GetHwndRT()->GetAntialiasMode();
	artist->GetUsingRT()->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
    artist->SetTextFormat(_pTextFormat);
        
	DrawSideBar(artist);
	DrawGrid(artist);
	DrawRuler(artist);

	artist->GetUsingRT()->SetAntialiasMode(am);

	artist->SetStrokeStyle(D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_LINE_JOIN_ROUND,
		10.0f,
		D2D1_DASH_STYLE_SOLID);

    return S_OK;
}


void CCoordFrame_summer::DrawSideBar(eArtist* artist)
{
	//draw the Outer Frame
    D2D1_RECT_F rectf = D2D1::RectF(1, 1, (float)_rect_calc.right, (float)_rect_calc.bottom);
	artist->DrawRectangle(rectf);

	//Left side Bar
	artist->DrawLine(D2D1::Point2F(RULER_LEFT, .0f), D2D1::Point2F(RULER_LEFT, (float)_rect_calc.bottom));

	//Righ side Bar
	artist->DrawLine(D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, .0f),
					  D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, fRectHeight(_rect)));

	//Top side Bar
	artist->DrawLine(D2D1::Point2F(RULER_LEFT, RULER_TOP),
					  D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, RULER_TOP));

	//Bottom side Bar
	artist->DrawLine(D2D1::Point2F(RULER_LEFT, (float)_rect_calc.bottom - RULER_BOTTOM),
					  D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, (float)_rect_calc.bottom - RULER_BOTTOM));
}


void CCoordFrame_summer::DrawGrid(eArtist* artist)
{
	float fRectInterval = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / _split_pieces;
	float scale = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / (_world->_real_world.maxy - _world->_real_world.miny);
	float y0 = (_world->_real_world.maxy - _world->_real_world.y0) * scale + RULER_TOP;

	artist->SetStrokeStyle(D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_LINE_JOIN_ROUND,
		10.0f,
		D2D1_DASH_STYLE_SOLID);

	//Draw Y0 horizon line
	artist->DrawLine(D2D1::Point2F(RULER_LEFT, y0), D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, y0));

	artist->SetStrokeStyle(D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_LINE_JOIN_ROUND,
		10.0f,
		D2D1_DASH_STYLE_DASH_DOT_DOT);


	//Draw horizon line that large than Y0
	float y = y0;
	while(y > _ruler_width.top) {
		artist->DrawLine(D2D1::Point2F(RULER_LEFT, y), D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, y));
		y -= fRectInterval;
	}

	//Draw horizon line that less than Y0
	y = y0;
	while(y < _rect_calc.bottom - _ruler_width.bottom) {
		artist->DrawLine(D2D1::Point2F(RULER_LEFT, y), D2D1::Point2F((float)_rect_calc.right - RULER_RIGHT, y));
		y += fRectInterval;
	}

	//Draw vertical line
	fRectInterval = ((float)_rect_calc.right - RULER_LEFT - RULER_RIGHT) / _split_pieces;
	for (int i = 1; i < _split_pieces; i++) {
		artist->DrawLine(D2D1::Point2F(fRectInterval * i + RULER_LEFT, RULER_TOP),
						  D2D1::Point2F(fRectInterval * i + RULER_LEFT, (float)_rect_calc.bottom - RULER_BOTTOM));
	}
}

void CCoordFrame_summer::DrawRuler(eArtist* artist)
{
	//artist->SetTransform(D2D1::Matrix3x2F::Translation(static_cast<float>(_nMarginL), static_cast<float>(_nMarginT)));

	float fRectInterval = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / _split_pieces;
    float fValueInterval = _world->_real_world.height() / _split_pieces;
    float scale = ((float)_rect_calc.bottom - RULER_TOP - RULER_BOTTOM) / _world->_real_world.height();
	float y0 = (_world->_real_world.maxy - _world->_real_world.y0) * scale + RULER_TOP;

	//Draw Y0
	float y = y0, Y = _world->_real_world.y0;
	TCHAR wszText[MAX_PATH];
	DrawRulerText(artist, wszText, MAX_PATH, Y, y);

	y = y0 - fRectInterval;
	Y = _world->_real_world.y0 + fValueInterval;
	while(y > _ruler_width.top) {
		DrawRulerText(artist, wszText, MAX_PATH, Y, y);
		y -= fRectInterval, Y += fValueInterval;
	}

	y =  y0 + fRectInterval;
	Y = _world->_real_world.y0 - fValueInterval;
	while(y < _rect_calc.bottom - _ruler_width.bottom) {
		DrawRulerText(artist, wszText, MAX_PATH, Y, y);
		y += fRectInterval, Y -= fValueInterval;
	}


	DrawRulerText(artist, wszText, MAX_PATH, _world->_real_world.miny, static_cast<float>(_rect_calc.bottom - _ruler_width.bottom));
	DrawRulerText(artist, wszText, MAX_PATH, _world->_real_world.maxy, static_cast<float>(_rect_calc.top + _ruler_width.top));
}


void CCoordFrame_summer::DrawRulerText(eArtist* artist, TCHAR * wszText, size_t len, float Y, float y)
{
	_sntprintf_s(wszText, len, _TRUNCATE, L"%.02f", Y);
		D2D1_RECT_F textRect = D2D1::RectF(
			static_cast<float>(0) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y - _ruler_width.top * .5) / CDxFactorys::GetInstance()->m_dpiScaleY,
			static_cast<float>(_ruler_width.left) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y + _ruler_width.bottom * .5) / CDxFactorys::GetInstance()->m_dpiScaleY);

		artist->DrawText(wszText, textRect);

		//Right, percent text
		if (_world->_real_world.y0 == 0) {
			if (Y - _world->_real_world.y0 < 0)
				_sntprintf_s(wszText, len, _TRUNCATE, L"%.02f%%",  (Y - _world->_real_world.y0)/(_world->_real_world.maxy - _world->_real_world.y0) * 100);
			else
				_sntprintf_s(wszText, len, _TRUNCATE, L"+%.02f%%", (Y - _world->_real_world.y0)/(_world->_real_world.maxy - _world->_real_world.y0) * 100);
		} else {
			if (Y - _world->_real_world.y0 < 0)
				_sntprintf_s(wszText, len, _TRUNCATE, L"%.02f%%",  (Y - _world->_real_world.y0)/_world->_real_world.y0 * 100);
			else
				_sntprintf_s(wszText, len, _TRUNCATE, L"+%.02f%%", (Y - _world->_real_world.y0)/_world->_real_world.y0 * 100);
		}

		textRect = D2D1::RectF(
			static_cast<float>(_rect_calc.right - _ruler_width.right) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y - _ruler_width.top * .5) / CDxFactorys::GetInstance()->m_dpiScaleY,
			static_cast<float>(_rect_calc.right) / CDxFactorys::GetInstance()->m_dpiScaleX,
			static_cast<float>(y + _ruler_width.bottom * .5) / CDxFactorys::GetInstance()->m_dpiScaleY);
		artist->DrawText(wszText, textRect);
}

} //namespace WARMGUI