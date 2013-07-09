#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"

CExtremumPoints::CExtremumPoints(const char* name)
    : WARMGUI::IDataGraph_summer(name)
    , _ext(0)
    , _pTextFormat(0)
{
    SetClass();
}


CExtremumPoints::~CExtremumPoints(void)
{
    SafeRelease(&_pTextFormat);
}

HRESULT CExtremumPoints::draw(bool redraw/* = false*/)
{
    CriticalLock::Scoped scope(_lockChange);
    HRESULT hr = S_OK;
    if (_ext) {
        //MYTRACE(L"draw %d extremum points\n", _ext->nAllNum);

        for (int i = 0; i < _ext->nAllNum; i++) {
            if (_ext->extremum[i].nType > 0) {
                draw_point(
                    _ext->extremum[i].fIndex,
                    _ext->extremum[i].fOrngValue,
                    'x',
                    WARMGUI::CPoints_summer::UP,
                    _clr_1);
            } else {
                draw_point(
                    _ext->extremum[i].fIndex,
                    _ext->extremum[i].fOrngValue,
                    '+',
                    WARMGUI::CPoints_summer::DOWN,
                    _clr_2);
            }
        }
    }
    return hr;
}

inline HRESULT CExtremumPoints::init()
{

    char temp[MAX_PATH];
    bool b = true;
    if (b) {
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.font", _str_conf);
        b = _config->getFontSetting(_font, temp);
    }
    if (b) {
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.color-1", _str_conf);
        b = _config->getColorAlpha(_clr_1, temp);
    }
    if (b) {
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.color-2", _str_conf);
        b = _config->getColorAlpha(_clr_2, temp);
    }

    if (b)
        b = WARMGUI::CDxFactorys::GetInstance()->CreateTextFormat(_font, &_pTextFormat);

    return (b? 0 : -1);
}

inline void CExtremumPoints::draw_point(float x, float y, char c, WARMGUI::CPoints_summer::POINT_TEXT_POSISION pos, COLORALPHA clr)
{
    static D2D1_POINT_2F pnt;
    static float x1, y1, x2, y2;

    pnt = _world->get_trans()->TransformPoint(D2D1::Point2F(x, y));
    if (c == 'x')
        x1 = pnt.x - 2, y1 = pnt.y - 2, x2 = pnt.x + 2, y2 = pnt.y + 2;
    else //'+'
        x1 = pnt.x - 2, x2 = pnt.x + 2, y1 = y2 = pnt.y;

    //MYTRACE(L"%.02f, %.02f ---> %.02f, %.02f : translation : %d %d\n", x, y, pnt.x, pnt.y, _abs_rect.left, _abs_rect.top);
    _artist->SetSolidColorBrush(clr);
    _artist->DrawLine(x1, y1, x2, y2);
}
