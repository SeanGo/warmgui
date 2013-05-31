#include "StdAfx.h"
#include "warmgui.h"


namespace WARMGUI {
CToolbar::CToolbar(void)
	: _imgToolbar(0)
    , _first_cmd(0)
    , _margin(10)
{
	_visible = false;
    setClass();
}

CToolbar::CToolbar(const char* name)
    : ICanvas(name, false)
	, _imgToolbar(0)
    , _first_cmd(0)
    , _margin(10)
{
	_visible = false;
    setClass();
}

CToolbar::CToolbar(const char* name, UINT nFirstCommand)
	: ICanvas(name, false)
	, _imgToolbar(0)
    , _first_cmd(nFirstCommand)
    , _margin(10)
{
	_visible = false;
    setClass();
}

CToolbar::~CToolbar(void)
{
}

void CToolbar::SetRect(RECT& rect)
{
    RECT myrect = rect;
    myrect.left -= _margin, myrect.top -= _margin, myrect.right += _margin, myrect.bottom += _margin;

    ICanvas::SetRect(myrect);
}

const HRESULT CToolbar::Init(const char* name/* = 0*/)
{
	if (!_config)
		return (-1);

	char temp[MAX_PATH];
	_snprintf_s(temp, MAX_PATH, _TRUNCATE, "blind-%s", _name);
	CBlind* blind = new CBlind(temp, BGR(0, 0, 0), 0.5, false);
	AppendChild(_gt.begin(), blind);
    
	bool r = true;
	try {
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.toolbar-materials-rect", _strconf);
		r = _config->getRect(_toolbar_rect, temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.position", _strconf);
        _config->getToolbarPos(_pos, temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.button-size", _strconf);
        _config->getSize(_btn_size, temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.interval", _strconf);
		_interval = _config->getInt(temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.first-command", _strconf);
        _first_cmd = _config->getInt(temp);

		if (r) {
			_imgToolbar = new CSharedImage(IGlyph::GLYPH_TYPE_GLYPH, _appbmp->_material);
            _imgToolbar->SetName("toolbar-image");
			//the opacity must be 1.0f for backgound canvas
			_imgToolbar->SetSharedImage(_toolbar_rect);
            _toolbar_rect.right = RectWidth(_toolbar_rect) + _margin, _toolbar_rect.bottom = RectHeight(_toolbar_rect) + _margin, _toolbar_rect.left = _toolbar_rect.top = _margin;
            _imgToolbar->SetRect(_toolbar_rect);
			InsertAfter(_imgToolbar);
		}
	} catch(Poco::NotFoundException&) {
		return(-2);
	}

	return (r) ? S_OK : (-1);
}

void CToolbar::SetGlyphRect(void)
{
    (*(_gt.begin()))->SetRect(_rect);  //set blind's rect
}


int CToolbar::OnMouseMove(int x, int y)
{
	if (!_visible) return (0);
	if (this->Intersect(x, y)) {
        //SendMessage(_atelier->GetHwnd(), WM_COMMAND, , 0);
        //show animation
        return (1);
	} else {
		return (0);
	}
}


int CToolbar::OnLButtonUp(int x, int y)
{
	if (!_visible) return (0);
    
    RECT rect = _rect;
    rect.left -= _margin, rect.top -= _margin, rect.bottom -= _margin, rect.right -= _margin;
	if (pt_in_rect(rect, x, y)) {
        SendMessage(
            _atelier->GetHwnd(),
            WM_COMMAND,
            MAKEWPARAM(GetCommandID(x - rect.left), 0),
            (LPARAM)(_atelier->GetHwnd()));
        return (1);
	} else {
		return (0);
	}
}

UINT CToolbar::GetCommandID(int x)
{
    return ((UINT)x / _btn_size.cx) + _first_cmd;
}


}
