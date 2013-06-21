#include "StdAfx.h"
#include "warmgui_incs.h"

namespace WARMGUI {

static CriticalLock gLockCanvas;

#define ReturnGlyphIterFromIter(iter, tree) \
	if ((iter) == (tree).end())\
		return (0);\
	else {\
        g->SetGlyphTreeIter(iter); \
		return (IGlyph*)(*(p));\
    }

#define IsNullIter(iter, tree) (((iter) == (tree).end()) ? true : false)

#define GlyphIter ((IGlyph*)(*iter))

ICanvas::ICanvas(void)
	: _margin(NULL_RECT)
	, _offset(NULL_POINT_f)
	, _conter(0)
	, _appbmp(0)
	, _bHasBackground(true)
	, _bUsingBkg(false)
{
	_iterSelected = _gt.end();
    setClass();
}


ICanvas::ICanvas(const char* name, bool bHasBkgBlind/* = true*/)
	: IGlyphNode(name)
	, _margin(NULL_RECT)
	, _offset(NULL_POINT_f)
	, _conter(0)
	, _appbmp(0)
	, _bHasBackground(bHasBkgBlind)
	, _bUsingBkg(false)
{
	_iterSelected = _gt.end();
    setClass();
}

ICanvas::~ICanvas(void)
{
	Clear();
}

void ICanvas::set_background()
{
	if (_bHasBackground) {
		char name[MAX_PATH];
		_snprintf_s(name, MAX_PATH, _TRUNCATE, "blind-%s", _name);
		CBlind* blind = new CBlind(name, BGR(0, 0, 0), 0.5);
		AppendChild(_gt.begin(), blind);
	}
}

int ICanvas::OnLButtonUp(int x, int y)
{
    if (!_visible) return (0);

    int s = IsInRect(_rect, x, y);
    _iterSelected = _gt.end();
    if (s) {
#ifdef _DEBUG
        TCHAR name[MAX_PATH];
        CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
        MYTRACE(L"Canvas %s selected\n", name);
#endif //_DEBUG
        GlyphTreeIter it = _gt.begin();
        if (it != _gt.end()) {
            size_t n = _gt.number_of_siblings(it);
            for (int i = n; i > 0; i--) {
                GlyphTreeIter iter = _gt.sibling(it, i);
                if ((*iter)->OnLButtonUp(x, y)) {
                    _iterSelected = iter;
                }
            }
        }
    }
    return (s);
}

int ICanvas::OnRButtonUp(int x, int y)
{
    if (!_visible) return (0);

    int s = IsInRect(_rect, x, y);
    _iterSelected = _gt.end();
    if (s) {
#ifdef _DEBUG
        TCHAR name[MAX_PATH];
        CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
        MYTRACE(L"Canvas %s selected\n", name);
#endif //_DEBUG
        GlyphTreeIter it = _gt.begin();
        if (it != _gt.end()) {
            size_t n = _gt.number_of_siblings(it);
            for (int i = n; i > 0; i--) {
                GlyphTreeIter iter = _gt.sibling(it, i);
                if ((*iter)->OnLButtonUp(x, y)) {
                    _iterSelected = iter;
                }
            }
        }
    }
    return (s);
}


HRESULT ICanvas::Draw(bool redraw/* = false*/)
{
	if (!_visible) return S_OK;

	HRESULT r = S_OK;

    MATRIX_2D m = D2D1::Matrix3x2F::Identity(), backup;
    //MYTRACE(L"Canvas %s rect %d %d %d %d\n", _name, _rect.left, _rect.top, _rect.right, _rect.bottom);
#ifdef _DEBUG
    TCHAR name[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    //MYTRACE(L"canvas %s draw ", name);
#endif //_DEBUG

    m._31 = (float)(_rect.left + _margin.left), m._32 = (float)(_rect.top + _margin.top);
    _common_artist->GetTransform(&backup);
    _common_artist->SetTransform(&m);

	for (GlyphTreeIter iter = _gt.begin(); iter != _gt.end(); iter++) {
        if (GlyphIter->GetGlyphType() != IGlyph::GLYPH_TYPE_BKG) {
#              ifdef _DEBUG
                //TCHAR myname[MAX_PATH];
                //CChineseCodeLib::Gb2312ToUnicode(myname, MAX_PATH, (*iter)->_name);
                //MYTRACE(L"%s draw graph, ", myname);
                //if ((*iter)->isClass("CCoordFrame"))
                //{
                //    int kkk = 0;
                //    kkk = 1;
                //}
#              endif //_DEBUG
		    (*iter)->Draw(redraw);
        }
	}
    //MYTRACE(L"\n");
    _common_artist->SetTransform(&backup);
    
    ClearChanged();
	return r;
}


HRESULT ICanvas::_DrawBkg(bool drawbuf/* = false*/)
{

    //!!!
    //the backgound canvas has one image only, so do not need draw _material bitmap
    if (!drawbuf) {
        //draw materias first
		RECT_f bmprect;
		_atelier->GetWinrectInMaterial(_rect, &bmprect);

        RECT_f myrect;
		myrect.left = myrect.top = 0.0f, myrect.right = fRectWidth(_rect), myrect.bottom = fRectHeight(_rect);
		_common_artist->DrawBitmap(_appbmp->_material, myrect, bmprect);
    }

    for (GlyphTreeIter iter = _gt.begin(); iter != _gt.end(); iter++) {
        if (GlyphIter->GetGlyphType() == IGlyph::GLYPH_TYPE_BKG) {
#           ifdef _DEBUG
            //TCHAR myname[MAX_PATH];
            //CChineseCodeLib::Gb2312ToUnicode(myname, MAX_PATH, (*iter)->_name);
            //MYTRACE(L"%s draw graph\n", myname);
            //if ((*iter)->isClass("CCoordFrame"))
            //{
            //    int kkk = 0;
            //    kkk = 1;
            //}
#           endif //_DEBUG
            (*iter)->Draw(drawbuf);
        }
    }

    if (_glyph_state & (int)GLYPH_STATE_SELECTED) {
        D2D1_COLOR_F clr = _common_artist->GetSCBrush()->GetColor();
        MATRIX_2D m;
        _common_artist->GetTransform(&m);
        MATRIX_2D idt = D2D1::IdentityMatrix();
        _common_artist->SetTransform(&idt);
        _common_artist->SetSolidColorBrush(D2D1::ColorF(BGR(128, 128, 128), 1.0f));
        _common_artist->DrawRectangle(_rect.left + 1, _rect.top + 1, _rect.right - 1, _rect.bottom - 1, 0.8f);
        _common_artist->SetSolidColorBrush(clr);
        _common_artist->SetTransform(&m);
    }

    return S_OK;
}

///draw background
HRESULT ICanvas::DrawBkg(bool drawbuf/* = false*/)
{
 	if (!_visible) return S_OK;

#ifdef _DEBUG
    //TCHAR name[MAX_PATH];
    //CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    //MYTRACE(L"canvas %s draw bkg\n", name);
#endif //_DEBUG

    HRESULT r = S_OK;
    MATRIX_2D m = D2D1::Matrix3x2F::Identity(), backup;
    m._31 = (float)(_rect.left + _margin.left), m._32 = (float)(_rect.top + _margin.top);
    _common_artist->GetTransform(&backup);
    _common_artist->SetTransform(&m);

    if (drawbuf)
        _DrawBkg(drawbuf);
    else if (IsChanged()
        &
        (GLYPH_CHANGED_TYPE_GLYPH_RESIZE | GLYPH_CHANGED_TYPE_CANVAS_BKG | GLYPH_CHANGED_TYPE_GLYPH_RESIZE | GLYPH_CHANGED_TYPE_COORDFRAME | GLYPH_CHANGED_TYPE_CHANGED))
        _DrawBkg(drawbuf);

    _common_artist->SetTransform(&backup);
    return r;
}

///draw the new part of graph
HRESULT ICanvas::Renew()
{
	HRESULT r = S_OK;
	return r;
}


///the point(x,y) of screen is inside of graph
bool ICanvas::Intersect(int x, int y)
{
	return pt_in_rect(_rect, x, y);
}

///the point(x,y) of screen is edge of graph
IGlyphNode::EDGE_POSITION_TYPE ICanvas::IsEdge(int x, int y)
{
	return IGlyphNode::EDGE_POSITION_TYPE_NONE;
}

inline GlyphTreeIter ICanvas::Find(IGlyph* const g) const
{
	return std::find(_gt.begin(), _gt.end(), g);
}


inline GlyphTreeIter ICanvas::Find(UINT gid) const
{
	GlyphTreeIter it = _gt.begin();
	for(;it != _gt.end(); it++) {
		if ((*it)->IsMe(gid))
			return it;
	}
	return _gt.end();
}

inline IGlyph* ICanvas::Find(const char* name) const
{
	GlyphTreeIter it = _gt.begin();
	for(;it != _gt.end(); it++) {
		if ((*it)->IsMe(name))
			return (IGlyph*)(*it);
	}
	return (0);
}

inline bool ICanvas::Remove(const IGlyph* g)
{
	GlyphTreeIter it = Find((IGlyph*)g);
	if (IsNullIter(it, _gt)) return false;

	delete (*it);
	_gt.erase(it);
	return true;
}

inline bool ICanvas::Remove(const UINT id)
{
	GlyphTreeIter it = Find(id);
	if (IsNullIter(it, _gt)) return false;

	delete (*it);
	_gt.erase(it);
	return true;
}

int ICanvas::getChildNumber(const GlyphTreeIter iter)
{
    return _gt.number_of_children(iter);
}

//index start from 0
inline IGlyph* ICanvas::getChild(const GlyphTreeIter iter, int index)
{
    GlyphTreeIter it = _gt.child(iter, index);
    if (it != _gt.end())
        return (IGlyph*)(*it);
    else
        return (0);
}

inline IGlyph* ICanvas::InsertAfter(IGlyph* const g)
{
	CriticalLock::Scoped lock(gLockCanvas);
	GiveGlyphSomething(g);

	GlyphTreeIter iter = _gt.begin();
	GlyphTreeIter p    = _gt.end();

	if (iter == _gt.end())
		p = _gt.set_head((IGlyph*)g);
	else {
		p = _gt.append_child(iter, (IGlyph*)g);
	}

	if (p == _gt.end())
		return (0);
	else {
        g->SetGlyphTreeIter(p);
		return (IGlyph*)(*(p));
    }

}


inline void ICanvas::Clear()
{
	CriticalLock::Scoped lock(gLockCanvas);

	for(GlyphTreeIter it = _gt.begin();it != _gt.end(); it++)
		delete (*it);
	_gt.clear();
	
	_conter = 0;
}


inline IGlyph * ICanvas::AppendChild(const GlyphTreeIter iter, IGlyph* const g)
{
	CriticalLock::Scoped lock(gLockCanvas);
	GiveGlyphSomething(g);

	GlyphTreeIter p;
	if (iter == _gt.end() && iter == _gt.begin())
		p = _gt.set_head((IGlyph*)g);
	else 
		p = _gt.append_child(iter, (IGlyph*)g);

	if (p == _gt.end())
		return (0);
	else {
        g->SetGlyphTreeIter(p);
		return (IGlyph*)(*(p));
    }
}

inline IGlyph * ICanvas::PrependChild(const GlyphTreeIter iter, IGlyph* const g)
{
	CriticalLock::Scoped lock(gLockCanvas);
	GiveGlyphSomething(g);

	GlyphTreeIter p;
	if (iter == _gt.end())
		p = _gt.set_head((IGlyph*)g);
	else 
		p = _gt.prepend_child(iter, (IGlyph*)g);

	if (p == _gt.end())
		return (0);
	else {
        g->SetGlyphTreeIter(p);
		return (IGlyph*)(*(p));
    }
}


inline IGlyph* ICanvas::InsertNext(const GlyphTreeIter iter, IGlyph* const g)
{
	CriticalLock::Scoped lock(gLockCanvas);
	GiveGlyphSomething(g);

	if (iter == _gt.end()) return (0);
	GlyphTreeIter p = _gt.insert_after(iter,(IGlyph*)g);

	if (p == _gt.end())
		return (0);
	else {
        g->SetGlyphTreeIter(p);
		return (IGlyph*)(*(p));
    }
}

inline IGlyph* ICanvas::InsertPrev(const GlyphTreeIter iter, IGlyph* const g)
{
	CriticalLock::Scoped lock(gLockCanvas);
	GiveGlyphSomething(g);

	if (iter == _gt.end()) return (0);
	GlyphTreeIter p = _gt.insert(iter,(IGlyph*)g);

	if (p == _gt.end())
		return (0);
	else {
        g->SetGlyphTreeIter(p);
		return (IGlyph*)(*(p));
    }
}

void ICanvas::SetRect(RECT& rect)
{
	IGlyphNode::SetRect(rect);

	ClearChanged();
	Changed(GLYPH_CHANGED_TYPE_CANVAS_RESIZE);

    SetGlyphRect();
}

IGlyph* ICanvas::GetSelectedGlyph()
{
	if (_iterSelected == _gt.end())
		return (0);
	else
		return (IGlyph*)(*_iterSelected);
}

inline void ICanvas::GiveGlyphSomething(IGlyph* const g)
{
    g->SetParam(_conter++, _common_artist, this, _atelier);
    g->Init();
}


inline void ICanvas::SetAtelier(IAtelier *atelier)
{
	_atelier = atelier;
}



/*
HRESULT ICanvas::Init()
{
	set_background();

    return S_OK;
}
*/








CBkgCanvas::CBkgCanvas(void)
	: _imgBkg(0)
{
	_bHasBackground = false;
    setClass();
}

CBkgCanvas::CBkgCanvas(const char* name)
	: ICanvas(name, false)
	, _imgBkg(0)
{
	_bHasBackground = false;
    setClass();
}

CBkgCanvas::~CBkgCanvas(void)
{
}


void CBkgCanvas::SetGlyphRect()
{
	if (_imgBkg)
		_imgBkg->SetRect(_rect);
}

const HRESULT CBkgCanvas::Init(const char* name/* = 0*/)
{
	if (!_config)
		return (-1);

	bool r = true;
	try {
		RECT rect;
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.rect", _strconf);
		r = _config->getRect(rect, temp);
		if (r) {
			_imgBkg = new CSharedImage(IGlyph::GLYPH_TYPE_BKG, _appbmp->_material);
            _imgBkg->SetName("backgound-image");
			//the opacity must be 1.0f for backgound canvas
			_imgBkg->SetSharedImage(rect);
			InsertAfter(_imgBkg);
		}
	} catch(Poco::NotFoundException&) {
		return(-2);
	}

	return (r) ? S_OK : (-1);
}

int CBkgCanvas::OnMouseMove(int x, int y)
{
    return (0);
}

int CBkgCanvas::OnLButtonUp(int x, int y)
{
    return (0);
}

int CBkgCanvas::OnRButtonUp(int x, int y)
{
    return (0);
}



}//namespace WARMGUI
