#include "StdAfx.h"
#include "warmgui_summer.h"

namespace WARMGUI {

///////////////////////////////////////////////////////////////////////////////
//class IDataLine_summer 
inline IDataGraph_summer::IDataGraph_summer(void)
    : _world(0)
    , _my_own_world(false)
    , _my_artist(0)
    , _pdata(0)
    , _my_own_data(false)
    , _world_change(WORLD_CHANGED_TYPE_NONE)
    , _my_own_artist(false)
{
}

inline IDataGraph_summer::IDataGraph_summer(const char* name, bool own_world, bool own_artist, bool own_data)
    : IGlyph_summer(name)
    , _pdata(0)
    , _my_own_world(own_world)
    , _my_own_data(own_data)
    , _my_own_artist(own_artist)
    , _world_change(WORLD_CHANGED_TYPE_NONE)
{
    if (own_artist) {
        _my_artist  = new eArtist();
    } else {
        _my_artist  = 0;
    }
    
    if (own_world)
        _world = new CWorld();
    else
        _world = 0;
}

inline IDataGraph_summer::~IDataGraph_summer(void)
{
    if (_my_own_world)
        SafeDelete(_world);

    if (_my_own_artist) {
        SafeDelete(_my_artist);
    }

    if (_my_own_data)
        SafeDelete(_pdata);
}

//reset glyph rect, and world's rect
void IDataGraph_summer::set_rect(RECT& rect)
{
    if (_my_own_world)
        _world->SetRectSize(rect);  //absolutely position
    if (_my_artist)
        _my_artist->ResizeRenderTarget(RectWidth(rect), RectHeight(rect));

    IGlyph_summer::set_rect(rect);

    predraw();
}

inline void IDataGraph_summer::inherit(IAtelier_summer* atelier, CGlyphTree_summer* tree, ICanvas_summer* canvas, GlyphTreeIter_summer& tree_iter, eArtist* artist, CWarmguiConfig* config)
{
    _atelier = atelier,
        _glyph_tree = tree,
        _canvas = canvas,
        _tree_iter = tree_iter,
        _artist = artist,
        _config = config;
    inherit_config_string();

    if (_my_artist)
        _my_artist->SetHwndRenderTarget(_artist->GetHwndRT(), _artist->GetHwnd());
}










///////////////////////////////////////////////////////////////////////////////
//class CCurveGraph_summer
inline CCurveGraph_summer::CCurveGraph_summer(void)
	: _color(0)
	//, _datasize(datasize)
	, _alpha(1.0f)
	, _pathg(0)
    , _pSink(0)
	, _stroke_width(1.0f)
    , _update_method(UPDATE_METHOD_INCREST)
{
}

inline CCurveGraph_summer::CCurveGraph_summer(const char* name, bool own_world, bool own_artist, bool own_data)
    : IDataGraph_summer(name, own_world, own_artist, own_data)
	, _color(0)
	//, _datasize(datasize)
	, _alpha(1.0f)
	, _pathg(0)
    , _pSink(0)
	, _stroke_width(1.0f)
    , _update_method(UPDATE_METHOD_INCREST)
{
}

inline CCurveGraph_summer::~CCurveGraph_summer(void)
{
    if (_pSink)
        SafeRelease(&_pSink);

    if (_pathg)
        SafeRelease(&_pathg);
}

GLYPH_CHANGED_TYPE CCurveGraph_summer::new_data(DataObjectPtr dop)
{
    return update(dop.get()->GetData());
}

GLYPH_CHANGED_TYPE CCurveGraph_summer::update(dataptr data)
{
    CriticalLock::Scoped scope(_lockChange);
    change(GLYPH_CHANGED_GLYPH);

    _pdata = data;
    if (_update_method == UPDATE_METHOD_INCREST) {
        //fresh world limit
        _world_change = WORLD_CHANGED_TYPE_NONE;
        _world_change = _world->fresh_limit(((POINTF*)data)->x, ((POINTF*)data)->y);           

        //add data to data-point-set
        _points.add_data(((POINTF*)data)->x, ((POINTF*)data)->y);

        if (_my_own_artist) {
            draw_new_point();
            set_change(GLYPH_CHANGED_NONE);
        }
    } else {
        //call virtual function, set the curve graph
        //the data pointer is DOUBLE_DATA_POINTER
        update_data();
    }

    return _changed;
}

inline HRESULT CCurveGraph_summer::_predraw()
{
    HRESULT hr = S_OK;
    if (_my_artist && _rect.right > 0 && _rect.bottom > 0) {
        if (_pathg)
            draw_whole_line();
        else if (_points._count < 2)
            clear_bmp_target();
        else {
            prepare_path();
            if (_pathg) draw_whole_line();
        }
    }
    return hr;
}


inline void CCurveGraph_summer::clear_bmp_target()
{
    if(_my_artist)
    {
        _my_artist->BeginBmpDraw(true);
        _my_artist->EndBmpDraw();
    }
}

//reset path gemoetry, if add_to_point_set is true, reset the data-point-set
//must  lock _lockChange before call this
inline void CCurveGraph_summer::begin_set_data(float x, float y, bool add_to_point_set/* = true*/)
{
	HRESULT hr = S_OK;
	SafeRelease(&_pathg);
    SafeRelease(&_pSink);
	hr = CDxFactorys::GetInstance()->GetD2DFactory()->CreatePathGeometry(&_pathg);

    //set_change(GLYPH_CHANGED_NONE);

	hr = _pathg->Open(&_pSink);
	if (SUCCEEDED(hr)) {
		_pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
        _pSink->BeginFigure(D2D1::Point2F(x, y),D2D1_FIGURE_BEGIN_HOLLOW);

        if (add_to_point_set) {
            _points.reset();
            _points.add_data(x, y);
        }
    }
}

//add data to path geometry, if add_to_point_set is true, reset the data-point-set
//must call begin_set_data before
inline void CCurveGraph_summer::add_data_to_path_geometry(float x, float y, bool add_to_point_set/* = true*/)
{
    _pSink->AddLine(D2D1::Point2F(x, y));
    if (add_to_point_set) _points.add_data(x, y);
}

//finish path geometry
//must call begin_set_data before
inline void CCurveGraph_summer::end_set_data()
{
	_pSink->EndFigure(D2D1_FIGURE_END_OPEN);

    _pSink->Close();
	SafeRelease(&_pSink);
    
    change(GLYPH_CHANGED_GLYPH);
}

inline void CCurveGraph_summer::update_data()
{
    CriticalLock::Scoped scope(_lockChange);
    change(GLYPH_CHANGED_GLYPH);

    DOUBLE_DATA_POINTER* ddp = (DOUBLE_DATA_POINTER*)_pdata;
    if (ddp && ddp->count > 2) {
        begin_set_data((float)*(ddp->x), (float)*(ddp->y), true);
        _world_change = WORLD_CHANGED_TYPE_NONE;

        for (int i = 1; i < ddp->count; i++) {
            add_data_to_path_geometry((float)ddp->x[i], (float)ddp->y[i], true);
            _world_change = _world->fresh_limit((float)ddp->x[i], (float)ddp->y[i]);
        }

        end_set_data();

        if (_my_own_artist) {
            predraw();
            set_change(GLYPH_CHANGED_NONE);
        }
    }
}

HRESULT CCurveGraph_summer::_draw(bool redraw_all/* = false*/)
{
    HRESULT hr = S_OK;
    CriticalLock::Scoped scope(_lockChange);

    if (SUCCEEDED(hr))
        hr = push_layer();

    if (_my_own_artist) {
        //draw bitmap
        if (_points._count > 1) {
            if (redraw_all && _changed) {
                MYTRACE(L"redraw--------------\n");
                prepare_path();
                hr = draw_whole_line();
            }
            
            _artist->DrawBitmap(_my_artist->GetDefaultBmp(), _rect, _rect, 1.0f);
        }
    } else {
        //draw whold line using common artist
        if (_update_method == UPDATE_METHOD_INCREST)
            prepare_path();

        //    hr = draw_new_point();
        //else
        hr = draw_whole_line();
    }

    if (SUCCEEDED(hr))
        hr = pop_layer();

    set_change(GLYPH_CHANGED_NONE);
    return hr;
}

void CCurveGraph_summer::prepare_path()
{
    SafeRelease(&_pathg);
    if (_points._count > 1) {
        begin_set_data(_points._points->x, _points._points->y, false);
        _world_change = WORLD_CHANGED_TYPE_NONE;

        for (int i = 1; i < _points._count; i++) {
            add_data_to_path_geometry(_points._points[i].x, _points._points[i].y, false);
            _world_change = _world->fresh_limit(_points._points[i].x, _points._points[i].y);
        }

        end_set_data();
    }
}

void CCurveGraph_summer::_draw_whole_line(eArtist* artist)
{
    artist->SetSolidColorBrush(D2D1::ColorF(_color, _alpha));
    ////////////////////////////////////////////////////////////////
    //artist->DrawRectangle(_rect);
    ////////////////////////////////////////////////////////////////

    artist->GetTransform(&_back_trans);

    ////////////////////////////////////////////////////////////////
    MATRIX_2D trans = *(_world->GetTransform());
    trans._31 += _back_trans._31, trans._32 += _back_trans._32;
    artist->SetTransform(&trans);
    ////////////////////////////////////////////////////////////////

    D2D1_ANTIALIAS_MODE am = artist->GetHwndRT()->GetAntialiasMode();
	artist->GetUsingRT()->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	artist->SetSolidColorBrush(D2D1::ColorF(_color, _alpha));
    artist->DrawGeometry(_pathg, artist->GetSCBrush(), _stroke_width / _world->GetTransform()->_11, artist->GetStrokeStyle());

    artist->SetTransform(&_back_trans);
    artist->GetUsingRT()->SetAntialiasMode(am);
}

HRESULT CCurveGraph_summer::draw_whole_line()
{
    HRESULT hr = S_OK;

    if (_points._count > 1) {
        if (_my_own_artist) {
            _my_artist->BeginBmpDraw(true);
            _draw_whole_line(_my_artist);
            hr = _my_artist->EndBmpDraw();
        } else {
            _draw_whole_line(_artist);
        }
    }
    return hr;
}

HRESULT CCurveGraph_summer::move_bitmap_left()
{
    HRESULT hr = S_OK;
    if (_my_own_artist) {
        float delta = _world->get_x_left();
        D2D1_RECT_F rectDest = D2D1::RectF(0.0f , 0.0f, fRectWidth(_rect) - delta, fRectHeight(_rect));
        D2D1_RECT_F rectSrc  = D2D1::RectF(delta, 0.0f, fRectWidth(_rect), fRectHeight(_rect));

        /*
        MATRIX_2D _backup_trans;
        _my_artist->GetTransform(&_backup_trans);
        MATRIX_2D id = D2D1::Matrix3x2F::Identity();
        _my_artist->SetTransform(&id);
        */
        _my_artist->DrawBitmap(_my_artist->GetDefaultBmp(), rectDest, rectSrc);

        //_my_artist->SetTransform(&_backup_trans);
    }
    return hr;
}

void CCurveGraph_summer::_draw_new_point(eArtist* artist)
{
    artist->GetTransform(&_back_trans);

    ////////////////////////////////////////////////////////////////
    MATRIX_2D trans = *(_world->GetTransform());
    trans._31 += _back_trans._31, trans._32 += _back_trans._32;
    artist->SetTransform(&trans);
    ////////////////////////////////////////////////////////////////

    artist->SetSolidColorBrush(D2D1::ColorF(_color, _alpha));
    //draw last two points
    artist->DrawLine(
        _points._points[_points._count - 2].x,
        _points._points[_points._count - 2].y,
        _points._points[_points._count - 1].x,
        _points._points[_points._count - 1].y,
        _stroke_width / _world->GetTransform()->_11);

    artist->SetTransform(&_back_trans);
}

HRESULT CCurveGraph_summer::draw_new_point()
{
    HRESULT hr = S_OK;

    if (_points._count > 1) {
        if (_my_own_artist) {
            _my_artist->BeginBmpDraw(true);
            ////////////////////////////////////////////////////////////////
            _my_artist->DrawRectangle(_rect);
            ////////////////////////////////////////////////////////////////
            if (_world_change & WORLD_CHANGED_TYPE_MIN_X || _world_change & WORLD_CHANGED_TYPE_MAX_X)
                move_bitmap_left();
            else
                _my_artist->DrawBitmap(_my_artist->GetDefaultBmp(), _rect, _rect, 1.0f);
            _draw_new_point(_my_artist);
            hr = _my_artist->EndBmpDraw();
        } else {
            _draw_new_point(_artist);
        }
    }
    return S_OK;
}


HRESULT CCurveGraph_summer::init()
{

    if (_my_own_world) {
        if (!_world->setConfig(_config, _str_conf))
            return (-1);
    }
    return (S_OK);
}

///////////////////////////////////////////////////////////////////////////////
//class IChat_summer
inline IChat_summer::IChat_summer(void)
{
}


inline IChat_summer::~IChat_summer(void)
{
}


} //namespace WARMGUI