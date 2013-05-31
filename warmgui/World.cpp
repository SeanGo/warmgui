#include "StdAfx.h"
#include "warmgui.h"

namespace WARMGUI {

/////////////////////////////////////////////////////////////////////////////////
/// class CWorld
inline CWorld::CWorld()
	: _transform(D2D1::Matrix3x2F::Identity())
{
	memset(&_rect , 0, sizeof(RECT));
    *_name = 0;
}

inline CWorld::~CWorld()
{
}

bool CWorld::setConfig(CWarmguiConfig* config, const char* config_str)
{
    _config = config;
    _snprintf_s(_strconf, MAX_PATH, "%s.world", config_str);

    bool b = true;
    char key[MAX_PATH];

    if (b) {
        _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.name", _strconf);
        const std::string myname = _config->getString(key);
        setName(myname.c_str());
    }


    if (b) {
        _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.increase", _strconf);
        b = _config->getValueIncrease(_vi, key);
    }

    if (b) {
        _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.world"   , _strconf);
        b = _config->getWorldRect(_zero_world, key);
        if (b)
            this->SetWorldRect(_zero_world);
    }

    return b;
}

/// if the scale was changed return 1, else return 0
inline void CWorld::SetRectSize(RECT& rect)
{
	_rect.right = RectWidth(rect), _rect.bottom = RectHeight(rect), _rect.left = _rect.top = 0;
	SetScale();
}

/// if the scale was changed return 1, else return 0
inline void CWorld::SetWorldRect(WORLD_RECT& world)
{
    _bak_world = _real_world, _real_world = world;
    real_world_to_screen();
    SetScale();
}

/// if the scale was changed return 1, else return 0
inline void CWorld::SetWorldRect(double miny, double minpos, double maxy, double maxpos, double x0, double y0, double xn, double yn)
{
    _bak_world = _real_world;
	_real_world.set((float)miny, (float)minpos, (float)maxy, (float)maxpos, (float)x0, (float)y0, (float)xn, (float)yn);
    real_world_to_screen();
    SetScale();
}

/// if the scale was changed return 1, else return 0
inline void CWorld::SetWorldRect(float miny, float minpos, float maxy, float maxpos, float x0, float y0, float xn, float yn)
{
    _bak_world = _real_world;
	_real_world.set(miny, minpos, maxy, maxpos, x0, y0, xn, yn);
    real_world_to_screen();
    SetScale();
}

inline void CWorld::RenewYLimit(float x, float y)
{
    _bak_world  = _real_world;

    _real_world.minpos = x, _real_world.miny = y;
    _real_world.maxpos = x, _real_world.maxy = y;

    real_world_to_screen();
	SetScale();
}


inline void CWorld::ChangeYlimit(float miny, float maxy, float y0)
{
    _bak_world  = _real_world;
    _real_world.miny = miny, _real_world.maxy = maxy, _real_world.y0 = y0;
    real_world_to_screen();
	SetScale();
}

inline void CWorld::ChangeXlimit(float minx, float maxx, float x0)
{
    _bak_world  = _real_world;
    _real_world.x0 = minx, _real_world.xn = maxx, _real_world.x0 = x0;
    real_world_to_screen();
	SetScale();
}


/// if the scale was changed return 1, else return 0
inline void CWorld::SetScale()
{
	/*
    MYTRACE(L"SET SCALE %.02f %.02f vs %.02f %.02f\n",
        (float)(_real_world.width ()),
        (float)(_real_world.height()),
        fRectWidth(_rect),
        fRectHeight(_rect));
    */

    float xs =  fRectWidth(_rect)  / (float)(_real_world.width ());
    float ys =  fRectHeight(_rect) / (float)(_real_world.height());

	/*float x0 = _rect.left, y0 = _rect.top;
	D2D1::Matrix3x2F m4( xs,     0,
				         0,   - ys,
		   -minx * xs + x0,   miny * ys + rect.bottom - y0);*/

	//BECAUSE THERE HAVE PUSH-POP LAYOUT OF RENDER-TARGET, SO DO NOT ADD THE X0, Y0
	
    /*
    _transform = D2D1::Matrix3x2F(                 xs,   0,
                                                    0, -ys,
    static_cast<float>(_rect.left) - _real_world.minx * xs, _real_world.miny * ys + static_cast<float>(_rect.bottom));
    */

    _transform = D2D1::Matrix3x2F(
                             xs,   0,
                              0, -ys,
          - _real_world.x0 * xs, _real_world.miny * ys + (float)_rect.bottom);


	/*
	DEBUG_TRACE(L"\n\nCCartesian::SetScale\n");
	DEBUG_TRACE(L"RECT %d, %d, %d, %d\n", _rect.left, _rect.top, _rect.right,  _rect.bottom);
	DEBUG_TRACE(L"W %.05f, H %.05f %.05f\n", fRectWidth(_rect), static_cast<float>(_rect.bottom - _rect.top ), _real_world.maxy - _real_world.miny);
	DEBUG_TRACE(L"LIMT %.05f %.05f %.05f %.05f %.05f %.05f\n", _real_world.minx, _real_world.maxx, _real_world.miny, _real_world.maxy, _real_world.x0, _real_world.y0);
	DEBUG_TRACE(L"%.05f, %.05f\n%.05f, %.05f\n%.05f, %.05f\n", _transform._11, _transform._12, _transform._21, _transform._22, _transform._31, _transform._32);
	DEBUG_TRACE(L"CWorld::SetScale\n\n");
	*/
}

inline bool CWorld::Intersect(int x, int y)
{
	D2D1::Matrix3x2F invmatrix(_transform._11, _transform._12, _transform._21, _transform._22, _transform._31, _transform._32);
	if (invmatrix.Invert()) {
		POINT_f pt = invmatrix.TransformPoint(D2D1::Point2F((float)x, (float)y));
        return _real_world.in_world(pt.x, pt.y);
	}
	return false;
}

inline POINT CWorld::Transform(float x, float y)
{
	POINT_f pt(D2D1::Point2F());
	D2D1::Matrix3x2F matrix(_transform._11, _transform._12, _transform._21, _transform._22, _transform._31, _transform._32);
	pt = matrix.TransformPoint(D2D1::Point2F(x, y));
	POINT pnt = {(int)pt.x, (int)pt.y};
	return pnt;
}

inline POINT_f CWorld::InvertTransform(int x, int y)
{
	POINT_f pt(D2D1::Point2F());

	D2D1::Matrix3x2F invmatrix(_transform._11, _transform._12, _transform._21, _transform._22, _transform._31, _transform._32);
	if (invmatrix.Invert())
		pt = invmatrix.TransformPoint(D2D1::Point2F((float)x, (float)y));

	//DEBUG_TRACE(L"invmatrix.TransformPoint %.02f %.02f\n", pt.x, pt.y);
	return pt;
}


void CWorld::Zoom(float minx, float maxx, float miny, float maxy)
{
	_bak_world = _real_world;
	_real_world.x0   = minx, _real_world.xn = maxx, _real_world.miny = miny, _real_world.maxy = maxy;
    real_world_to_screen();
    SetScale();
}

void CWorld::UndoZoom()
{
	WORLD_RECT tmp = _bak_world;
	_bak_world = _real_world;
	_real_world = tmp;
}

void CWorld::ResetZoom()
{
	WORLD_RECT tmp = _bak_world;
	_bak_world = _real_world;
	_real_world = tmp;
}



inline void CWorld::real_world_to_screen()
{
    if (_vi._breadth_type == DATA_BREADTH_TYPE_PERCENT)
        _real_world.miny = _real_world.miny * (1 - _vi._min_decres_mag),
        _real_world.maxy = _real_world.maxy * (1 + _vi._max_incres_mag);
    else if (_vi._breadth_type == DATA_BREADTH_TYPE_VALUE)
        _real_world.miny = _real_world.miny - _vi._min_decres_mag,
        _real_world.maxy = _real_world.maxy + _vi._max_incres_mag;
}

inline void CWorld::RenewByRealWorld()
{
    _bak_world = _real_world;
    real_world_to_screen();
    SetScale();
}

} //namespace WARMGUI
