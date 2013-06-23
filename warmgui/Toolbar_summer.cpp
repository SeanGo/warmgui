#include "StdAfx.h"
#include "warmgui_summer.h"


namespace WARMGUI {
CToolbar_summer::CToolbar_summer(void)
	: _imgToolbar(0)
    , _first_cmd(0)
    , _margin(10)
    , _blind(0)
{
	_visible = false;
    setClass();
}

CToolbar_summer::CToolbar_summer(const char* name)
    : ICanvas_summer(name)
	, _imgToolbar(0)
    , _first_cmd(0)
    , _margin(10)
    , _blind(0)
{
	_visible = false;
    setClass();
}

CToolbar_summer::CToolbar_summer(const char* name, UINT nFirstCommand)
	: ICanvas_summer(name)
	, _imgToolbar(0)
    , _first_cmd(nFirstCommand)
    , _margin(10)
    , _blind(0)
{
	_visible = false;
    setClass();
}

CToolbar_summer::~CToolbar_summer(void)
{
}

void CToolbar_summer::set_rect(RECT& rect)
{
    //this rect is the full window's rect
    
    if (_pos == 4) //TOOLBAR_POSITION_MIDDLE
    {
        RECT toolbarrect;
        toolbarrect.left = (int)((fRectWidth(rect) - fRectWidth(_toolbar_rect)) / 2.0f),
            toolbarrect.right = (int)((fRectWidth(rect) + fRectWidth(_toolbar_rect)) / 2.0f),
            toolbarrect.top = (int)((fRectHeight(rect) - fRectHeight(_toolbar_rect)) / 2.0f),
            toolbarrect.bottom = (int)((fRectHeight(rect) + fRectHeight(_toolbar_rect)) / 2.0f);

        if (_imgToolbar) _imgToolbar->set_rect(toolbarrect);
        _abs_rect = toolbarrect;
        _abs_rect.left -= _margin, _abs_rect.top -= _margin, _abs_rect.right += _margin, _abs_rect.bottom += _margin;
        _rect.left = _rect.top = 0, _rect.right = RectWidth(_abs_rect), _rect.bottom = RectHeight(_abs_rect);

        _blind->set_rect(_abs_rect);
    }

}

HRESULT CToolbar_summer::init()
{
	if (!_config)
		return (-1);

	char temp[MAX_PATH];
	_snprintf_s(temp, MAX_PATH, _TRUNCATE, "blind-%s", _name);
    _blind = new CBlind_summer(temp, BGR(0, 0, 0), 0.5, IGlyph_summer::GLYPH_TYPE_GLYPH);
    append_glyph(_blind);

	bool r = true;
	try {
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.toolbar-materials-rect", _str_conf);
		r = _config->getRect(_toolbar_rect, temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.position", _str_conf);
        _config->getToolbarPos(_pos, temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.button-size", _str_conf);
        _config->getSize(_btn_size, temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.interval", _str_conf);
		_interval = _config->getInt(temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.first-command", _str_conf);
        _first_cmd = _config->getInt(temp);

		if (r) {
			_imgToolbar = new CSharedImage_summer(GLYPH_TYPE_GLYPH, _appbmp->_material);
            _imgToolbar->set_name("toolbar-image");
			//the opacity must be 1.0f for backgound canvas
			_imgToolbar->SetSharedImageRect(_toolbar_rect);
			append_glyph(_imgToolbar);
		}
	} catch(Poco::NotFoundException&) {
		return(-2);
	}

	return (r) ? S_OK : (-1);
}



int CToolbar_summer::OnLButtonUp(int x, int y)
{
	if (!_visible) return (0);
    
    RECT rect = _rect;
    rect.left += _margin, rect.top += _margin, rect.bottom -= _margin, rect.right -= _margin;
	if (IsInRect(rect, x, y)) {
        SendMessage(
            _atelier->get_hwnd(),
            WM_COMMAND,
            MAKEWPARAM(GetCommandID(x - rect.left), 0),
            (LPARAM)(_atelier->get_hwnd()));
        return (1);
	} else {
		return (0);
	}
}

UINT CToolbar_summer::GetCommandID(int x)
{
    return ((UINT)x / _btn_size.cx) + _first_cmd;
}


}
