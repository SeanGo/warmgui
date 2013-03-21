#include "StdAfx.h"
#include "warmgui.h"

namespace WARMGUI {

/////////////////////////////////////////////////////////////////////////////////
/// class CCartesian
inline CCartesian::CCartesian()
	: _transform(D2D1::Matrix3x2F::Identity())
{
	memset(&_rect , 0, sizeof(RECT));
	memset(&_limit, 0, sizeof(LIMIT_2D));
}

inline CCartesian::~CCartesian()
{
}

/// if the scale was changed return 1, else return 0
inline void CCartesian::SetRectSize(RECT& rect)
{
	_rect.right = RectWidth(rect), _rect.bottom = RectHeight(rect), _rect.left = _rect.top = 0;
	SetScale();
}

/// if the scale was changed return 1, else return 0
inline void CCartesian::SetWorldRect(LIMIT_2D& limit)
{
    _limit_zoom_back = _limit;
	_limit = limit, _limit_zoom_front = _limit, _limit_zoom_back = _limit;
	_limit_zoom_front = _limit; 
	SetScale();
}

/// if the scale was changed return 1, else return 0
inline void CCartesian::SetWorldRect(float minx, float maxx, float miny, float maxy, float x0, float y0)
{
    _limit_zoom_back = _limit;
	_limit.minx = minx, _limit.maxx = maxx, _limit.miny = miny, _limit.maxy = maxy, _limit.x0 = x0, _limit.y0 = y0;
	_limit_zoom_front = _limit; 
	SetScale();
}


inline void CCartesian::ChangeYlimit(float miny, float maxy, float y0)
{
    _limit_zoom_back  = _limit;
    _limit.miny = miny, _limit.maxy = maxy, _limit.y0 = y0;
	_limit_zoom_front = _limit; 
	SetScale();
}

inline void CCartesian::ChangeXlimit(float minx, float maxx, float x0)
{
    _limit_zoom_back  = _limit;
    _limit.minx = minx, _limit.maxx = maxx, _limit.x0 = x0;
	_limit_zoom_front = _limit; 
	SetScale();
}


/// if the scale was changed return 1, else return 0
inline void CCartesian::SetScale()
{
	if (!(_limit.maxx - _limit.minx) || !(_limit.maxy - _limit.miny) || !fRectWidth(_rect) || !fRectHeight(_rect))
		return;
	
	/*MYTRACE(L"SET SCALE %d %d %d %d with %.02f %.02f %.02f %.02f\n",
		_rect.left, _rect.top, _rect.right, _rect.bottom,
		_limit.minx, _limit.maxx, _limit.miny, _limit.maxy);*/

	float xs =  fRectWidth(_rect)  / (_limit.maxx - _limit.minx);
	float ys =  fRectHeight(_rect) / (_limit.maxy - _limit.miny);

	/*float x0 = _rect.left, y0 = _rect.top;
	D2D1::Matrix3x2F m4( xs,     0,
				         0,   - ys,
		   -minx * xs + x0,   miny * ys + rect.bottom - y0);*/

	//BECAUSE THERE HAVE PUSH-POP LAYOUT OF RENDER-TARGET, SO DO NOT ADD THE X0, Y0
	
    /*
    _transform = D2D1::Matrix3x2F(                 xs,   0,
                                                    0, -ys,
    static_cast<float>(_rect.left) - _limit.minx * xs, _limit.miny * ys + static_cast<float>(_rect.bottom));
    */

    _transform = D2D1::Matrix3x2F(
                        xs,   0,
                         0, -ys,
        - _limit.minx * xs, _limit.miny * ys + static_cast<float>(_rect.bottom));


	/*
	DEBUG_TRACE(L"\n\nCCartesian::SetScale\n");
	DEBUG_TRACE(L"RECT %d, %d, %d, %d\n", _rect.left, _rect.top, _rect.right,  _rect.bottom);
	DEBUG_TRACE(L"W %.05f, H %.05f %.05f\n", fRectWidth(_rect), static_cast<float>(_rect.bottom - _rect.top ), _limit.maxy - _limit.miny);
	DEBUG_TRACE(L"LIMT %.05f %.05f %.05f %.05f %.05f %.05f\n", _limit.minx, _limit.maxx, _limit.miny, _limit.maxy, _limit.x0, _limit.y0);
	DEBUG_TRACE(L"%.05f, %.05f\n%.05f, %.05f\n%.05f, %.05f\n", _transform._11, _transform._12, _transform._21, _transform._22, _transform._31, _transform._32);
	DEBUG_TRACE(L"CCartesian::SetScale\n\n");
	*/
}

inline bool CCartesian::Intersect(int x, int y)
{
	D2D1::Matrix3x2F invmatrix(_transform._11, _transform._12, _transform._21, _transform._22, _transform._31, _transform._32);
	if (invmatrix.Invert()) {
		POINT_f pt = invmatrix.TransformPoint(D2D1::Point2F((float)x, (float)y));
		return (pt.x >= _limit.minx && pt.x <= _limit.maxx && pt.y >= _limit.miny && pt.y <= _limit.maxy);
	}
	return false;
}

inline POINT CCartesian::Transform(float x, float y)
{
	POINT_f pt(D2D1::Point2F());
	D2D1::Matrix3x2F matrix(_transform._11, _transform._12, _transform._21, _transform._22, _transform._31, _transform._32);
	pt = matrix.TransformPoint(D2D1::Point2F(x, y));
	POINT pnt = {(int)pt.x, (int)pt.y};
	return pnt;
}

inline POINT_f CCartesian::InvertTransform(int x, int y)
{
	POINT_f pt(D2D1::Point2F());

	D2D1::Matrix3x2F invmatrix(_transform._11, _transform._12, _transform._21, _transform._22, _transform._31, _transform._32);
	if (invmatrix.Invert())
		pt = invmatrix.TransformPoint(D2D1::Point2F((float)x, (float)y));

	//DEBUG_TRACE(L"invmatrix.TransformPoint %.02f %.02f\n", pt.x, pt.y);
	return pt;
}


void CCartesian::Zoom(float minx, float maxx, float miny, float maxy)
{
	_limit_zoom_back = _limit_zoom_front;
	_limit_zoom_front.minx = minx, _limit_zoom_front.maxx = maxx, _limit_zoom_front.miny = miny, _limit_zoom_front.maxy = maxy;
}

void CCartesian::UndoZoom()
{
	LIMIT_2D tmp = _limit_zoom_back;
	_limit_zoom_back = _limit_zoom_front;
	_limit_zoom_front = tmp;
}

void CCartesian::ResetZoom()
{
	_limit_zoom_back = _limit_zoom_front;
	_limit_zoom_front = _limit;
}


} //namespace WARMGUI
