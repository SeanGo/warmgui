#include "StdAfx.h"
#include "warmgui.h"


namespace WARMGUI {
CToolbar::CToolbar(void)
	: _imgToolbar(0)
    , _first_cmd(0)
{
	_visible = false;
}

CToolbar::CToolbar(const TCHAR* name, UINT nFirstCommand)
	: ICanvas(name, false)
	, _imgToolbar(0)
    , _first_cmd(nFirstCommand)
{
	_visible = false;
}

CToolbar::~CToolbar(void)
{
}

HRESULT CToolbar::Init()
{
	ICanvas::Init();

	if (!_config)
		return (-1);

	TCHAR name[MAX_PATH];
	_sntprintf_s(name, MAX_PATH, _TRUNCATE, L"blind-%s", _name);
	CBlind* blind = new CBlind(name, BGR(0, 0, 0), 0.5, false);
	AppendChild(_gt.begin(), blind);
    
	bool r = true;
	try {
		RECT rect;
		r = _config->getRect(rect, "toolbar.toolbar-materials-rect");
		_config->getToolbarPos(this->_pos, "toolbar.position");
		_config->getSize(this->_btn_size, "toolbar.button-size");
		_interval = _config->getInt("toolbar.interval");
		if (r) {
			_imgToolbar = new CSharedImage(IGlyph::GLYPH_TYPE_GLYPH, _appbmp->_material);
            _imgToolbar->SetName(L"toolbar-image");
			//the opacity must be 1.0f for backgound canvas
			_imgToolbar->SetSharedImage(rect);
            rect.right = RectWidth(rect), rect.bottom = RectHeight(rect), rect.left = rect.top = 0;
            _imgToolbar->SetRect(rect);
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
    
    //and do not change the size of toolbar image
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
	if (this->Intersect(x, y)) {
        SendMessage(
            _atelier->GetHwnd(),
            WM_COMMAND,
            MAKEWPARAM(GetCommandID(x), 0),
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
