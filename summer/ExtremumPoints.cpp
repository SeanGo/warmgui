#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"

CExtremumPoints::CExtremumPoints(const char* name)
    : WARMGUI::IDataGraph_summer(name)
    , _ext(0)
    , _points("ext-points")
    , _pTextFormat(0)
{
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
        for (int i = 0; i < _ext->nAllNum; i++) {
            if (_ext->extremum[i].nType > 0) {
                _points.draw(
                    _ext->extremum[i].fIndex,
                    _ext->extremum[i].fOrngValue,
                    'x',
                    WARMGUI::CPoints_summer::UP,
                    _clr_1);
            } else {
                _points.draw(
                    _ext->extremum[i].fIndex,
                    _ext->extremum[i].fOrngValue,
                    'o',
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

    _points.set_txt_format(_pTextFormat);
    return (b? 0 : -1);
}
