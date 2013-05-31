#include "StdAfx.h"
#include "warmgui.h"

namespace WARMGUI {


static COLORREF _default_color[] = {
	BGR(0,     0, 255),
	BGR(0,   255,   0),
	BGR(255,   0,   0),
};
static const int __default_color_number__ = 3;
	
/*
ISeriesDataGraph::ISeriesDataGraph(void)
    : _manipulator(DATA_MANIPULATOR_TYPE_GRAPH)
{
}


ISeriesDataGraph::ISeriesDataGraph(const char* name)
    : IGlyph(name)
    , _manipulator(DATA_MANIPULATOR_TYPE_GRAPH)
{
}


ISeriesDataGraph::~ISeriesDataGraph(void)
{
}
*/



CDataLineGraph::~CDataLineGraph(void)
{
	SafeRelease(&_pathg);
    SafeRelease(&_pSink);
}

CDataLineGraph::CDataLineGraph(const char* name, bool own_artist, CSeriesDataChart *parent)
	: IDataGraph(name, own_artist)
	, _color(0)
	, _draw_type(SDATA_GRAPH_LINE_TYPE_PATH_GEOMETRY)
	, _data_x_offset(0)
	, _data_y_offset(0)
	//, _datasize(datasize)
	, _alpha(1.0f)
	, _pathg(0)
    , _pSink(0)
	, _stroke_width(1.0f)
    , _parent(parent)
    , _rtcset(0)
{
    InitDataPtr();

    memset(&psForNew, 0, sizeof(psForNew));
    setClass();
}


void CDataLineGraph::InitDataPtr()
{
    memset(&psForNew, 0, sizeof(psForNew));
}


HRESULT CDataLineGraph::Renew()
{
    HRESULT hr = S_OK;
    if (_myown_artist) {
        CriticalLock::Scoped scope(_myown_artist->_lock_artist);
        eArtist* _back_artist = _artist;
        _artist = _myown_artist;

        _artist->BeginBmpDraw();
	    if (_referframe) {
    		_artist->GetTransform(&_backup_trans);
            MATRIX_2D* m = _referframe->GetTransform();
            _artist->SetTransform(m);
        }

        hr = RenewGraph();

        if (_referframe)
		    _artist->SetTransform(&_backup_trans);
        _artist->EndBmpDraw();

        _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CHANGED);
        _artist = _back_artist;
    }
    return hr;
}


/// if the referframe was changed then return 1
GLYPH_CHANGED_TYPE CDataLineGraph::AppendData(DataObjectPtr dopNewData)
{
    _changed_type = GLYPH_CHANGED_TYPE_CHANGED;
    WORLD_RECT limit = _referframe->GetWorldRect();

    if (!psForNew._count) {
        psForNew._O = psForNew._pntOld = psForNew._pntO0 = psForNew._pntNew = FPOINT(
            *(float*)((char*)(dopNewData->GetData()) + _data_x_offset),
            *(float*)((char*)(dopNewData->GetData()) + _data_y_offset));

		//first data
		//set the ref-fremework
		if (_draw_type == WARMGUI::CDataLineGraph::SDATA_GRAPH_LINE_TYPE_DIFF_LINEBAR) {
			psForNew._pntOld.y = psForNew._pntNew.y = 0;

            _referframe->ChangeYlimit(0, _rtcset->_vi._max_incres_mag, 0);
		} else {
			if (_rtcset->_vi._breadth_type == DATA_BREADTH_TYPE_PERCENT)
				_referframe->ChangeYlimit(
					psForNew._pntO0.y * (1.0f - _rtcset->_vi._min_decres_mag),
					psForNew._pntO0.y * (1.0f + _rtcset->_vi._max_incres_mag),
                    psForNew._pntO0.y);
			else 
				_referframe->ChangeYlimit(
                    psForNew._pntO0.y - _rtcset->_vi._min_decres_mag,
					psForNew._pntO0.y + _rtcset->_vi._max_incres_mag,
                    psForNew._pntO0.y);
		}
		_changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int) GLYPH_CHANGED_TYPE_COORDFRAME);
	} else {
		//keep old point
		psForNew._pntOld = psForNew._pntNew;
		//Add new point
		if (_draw_type == WARMGUI::CDataLineGraph::SDATA_GRAPH_LINE_TYPE_DIFF_LINEBAR) {
			//Add new point as diff data
			psForNew._pntNew = FPOINT(
				*(float*)((char*)(dopNewData->GetData()) + _data_x_offset),
				*(float*)((char*)(dopNewData->GetData()) + _data_y_offset) - psForNew._O.y);
		} else {
			psForNew._pntNew = FPOINT(
				*(float*)((char*)(dopNewData->GetData()) + _data_x_offset),
				*(float*)((char*)(dopNewData->GetData()) + _data_y_offset));
		}
		psForNew._O = FPOINT(
				*(float*)((char*)(dopNewData->GetData()) + _data_x_offset),
				*(float*)((char*)(dopNewData->GetData()) + _data_y_offset));
		
		//set the ref-fremework
		if (psForNew._pntNew.y < limit.miny) {
            _stroke_width *= ( (limit.maxy - psForNew._pntNew.y) / (limit.maxy - limit.miny));

            _referframe->ChangeYlimit(
				psForNew._pntNew.y - _rtcset->_vi._min_decres_mag,
				limit.maxy,
                limit.y0);

    		_changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int) GLYPH_CHANGED_TYPE_COORDFRAME);
		}

		if (psForNew._pntNew.y > limit.maxy) {
            _stroke_width *= ( (psForNew._pntNew.y - limit.miny) / (limit.maxy - limit.miny));

            _referframe->ChangeYlimit(
				limit.miny,
				psForNew._pntNew.y + _rtcset->_vi._max_incres_mag,
                limit.y0);
			
    		_changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int) GLYPH_CHANGED_TYPE_COORDFRAME);
		}
	}

    //change the x axis
    if (psForNew._pntNew.x >= limit.xn - _rtcset->_predict_len) {
        MoveBitmapToLeft();

        _referframe->ChangeXlimit(limit.x0 + _rtcset->_space_len,
            limit.xn + _rtcset->_space_len,
            limit.x0 + _rtcset->_space_len);
    }

	++psForNew._count;
	_canvas->Changed(_changed_type);
	_atelier->Changed(_changed_type);


	/** for test redraw-coordinate
	if (!_tcscmp(_name, L"rtdata-data_ctp_price")) {
		_changed_type |= GLYPH_CHANGED_TYPE_COORDFRAME;
		_canvas->Changed(_changed_type);
		_atelier->Changed(_changed_type);
	}
	*/

    if (_changed_type & GLYPH_CHANGED_TYPE_COORDFRAME)
        PreDraw();
    else
        Renew();

	return _changed_type;
}


inline void CDataLineGraph::SetDataOffset(int pos, int x_offset, int y_offset)
{
	_data_x_offset = x_offset,
		_data_y_offset = y_offset,
		_color = _default_color[0],
		_alpha = 1.0f;
}

/*
inline HRESULT CDataLineGraph::PreparePathGeometry()
{
	HRESULT hr = S_OK;
	SafeRelease(&_pathg);
	hr = CDxFactorys::GetInstance()->GetD2DFactory()->CreatePathGeometry(&_pathg);

	ID2D1GeometrySink* _pSink = 0;
	hr = _pathg->Open(&_pSink);
	if (SUCCEEDED(hr)) {
		_pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
		_pSink->BeginFigure(*_points, D2D1_FIGURE_BEGIN_HOLLOW);
			
		for (int i = 1; i < psForNew._count; i++) {
			_pSink->AddLine(_points[i]);
            //MYTRACE(L"%d %.02f %.02f\n", i, _points[i].x, _points[i].y);
        }

		_pSink->EndFigure(D2D1_FIGURE_END_OPEN);
	}
	hr = _pSink->Close();
	SafeRelease(&_pSink);

    return hr;
}
*/

HRESULT CDataLineGraph::DrawGraph(bool redraw/* = false*/)
{
    //MYTRACE(L"CDataLineGraph::DrawGraph %s\n", _name);
	if (psForNew._count < 2) return S_OK;

	HRESULT hr = S_OK;


	D2D1_COLOR_F clr = _artist->GetSCBrush()->GetColor();
	_artist->GetSCBrush()->SetColor(D2D1::ColorF(_color));
	//_artist->DrawLine(_rect.left, _rect.top, _rect.right, _rect.bottom);
    SetReferenceFrameTransform();
        
    if (_pathg)
	    _artist->DrawGeometry(_pathg, _artist->GetSCBrush(), _stroke_width, _artist->GetStrokeStyle());

	_artist->GetSCBrush()->SetColor(clr);

    {/*
	    MATRIX_2D m;
	    _artist->GetTransform(&m);
	    //MYTRACE(L"DRAW %s: %.02f %.02f, %.02f %.02f, %.02f %.02f\n", _name, m._11, m._12, m._21, m._22, m._31, m._32);
	    POINT_f P1 = _points[0], P2 = _points[psForNew._count - 1];
	    D2D1::Matrix3x2F trans(m._11, m._12, m._21, m._22, m._31, m._32);
	    POINT_f p1 = trans.TransformPoint(P1);
	    POINT_f p2 = trans.TransformPoint(P2);
	    MYTRACE(L"%s %.02f %.02f, %.02f %.02f ==> %.02f %.02f, %.02f %.02f\n",
		    _name,
		    P1.x, P1.y,
		    P2.x, P2.y,
		    p1.x, p1.y,
		    p2.x, p2.y);
    */}

    GetbackArtistTransform();

	return hr;
}


HRESULT CDataLineGraph::RenewGraph()
{
	if (psForNew._count < 2) return S_OK;

	D2D1_COLOR_F clr = _artist->GetSCBrush()->GetColor();
	_artist->GetSCBrush()->SetColor(D2D1::ColorF(_color));
	
	if (_changed_type & GLYPH_CHANGED_TYPE_COORDFRAME) {
        if (_pathg) {
            _artist->Clear();
	    	_artist->DrawGeometry(_pathg, _artist->GetSCBrush(), _stroke_width, _artist->GetStrokeStyle());
        }
    } else {
        POINT_f pntOld, pntNew;
        pntOld.x = psForNew._pntOld.x,
            pntOld.y = psForNew._pntOld.y,
            pntNew.x = psForNew._pntNew.x,
            pntNew.y = psForNew._pntNew.y;
        _artist->DrawLine(pntOld, pntNew, _stroke_width);
    }

	_artist->GetSCBrush()->SetColor(clr);

	{/*
#   ifdef _DEBUG
		MATRIX_2D m;
		_artist->GetTransform(&m);
		D2D1::Matrix3x2F trans(m._11, m._12, m._21, m._22, m._31, m._32);
		MYTRACE(L"Re_new %s: %.02f %.02f, %.02f %.02f, %.02f %.02f\n", _name, m._11, m._12, m._21, m._22, m._31, m._32);
		//MATRIX_2D_t* m = _referframe->GetTransform();
		//D2D1::Matrix3x2F trans(m->_11, m->_12, m->_21, m->_22, m->_31, m->_32);
		POINT_f p1 = trans.TransformPoint(_points[psForNew._count - 2]);
		POINT_f p2 = trans.TransformPoint(_points[psForNew._count - 1]);
		MYTRACE(L"%s %d: %.02f %.02f, %.02f %.02f ==> %.02f %.02f, %.02f %.02f\n",
			_name,
			psForNew._count,
			_points[psForNew._count - 2].x, _points[psForNew._count - 2].y,
			_points[psForNew._count - 1].x, _points[psForNew._count - 1].y,
			p1.x, p1.y,
			p2.x, p2.y);
#	endif //_DEBUG
	*/}
	return S_OK;
}


HRESULT CDataLineGraph::PreDraw()
{
    int ndown = (_rtcset->_down_intval < 2) ? 1 : _rtcset->_down_intval;
    char* p = (char*)(_graph_data._pdata);


    int mycount = (int)((float)_graph_data._count / (float)ndown) + 1;
    if (mycount < 2) return S_OK;

    BeginSetData(p);

    p += _graph_data._data_size * ndown;
    for (int i = 1; i < _graph_data._count; i += ndown, p += _graph_data._data_size * ndown) {
        AddDataToPathGeometry(p);
    }

    EndSetData();

    RedrawGraph();

    return S_OK;
}

void CDataLineGraph::RedrawGraph()
{
    if (_myown_artist && psForNew._count > 2) {
        CriticalLock::Scoped scope(_myown_artist->_lock_artist);

        eArtist* _back_artist = _artist;
        _artist = _myown_artist;

        _artist->BeginBmpDraw();
    	_artist->GetSCBrush()->SetColor(D2D1::ColorF(_color));
	    if (_referframe) {
    		_artist->GetTransform(&_backup_trans);
            MATRIX_2D* m = _referframe->GetTransform();
            _artist->SetTransform(m);
        }

        if (_pathg) {
            _artist->Clear();
	        _artist->DrawGeometry(_pathg, _artist->GetSCBrush(), _stroke_width, _artist->GetStrokeStyle());
        }

        if (_referframe)
		    _artist->SetTransform(&_backup_trans);
        _artist->EndBmpDraw();

        _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CHANGED);
        _artist = _back_artist;
    }
}

void CDataLineGraph::SetRect(RECT& rect)
{
    IGlyph::SetRect(rect);
    RedrawGraph();
}

inline void CDataLineGraph::BeginSetData(dataptr new_data)
{

    memset(&psForSetData, 0, sizeof(psForSetData));
	psForSetData._O = psForSetData._pntOld = psForSetData._pntO0 = psForSetData._pntNew = FPOINT(
		*(float*)((char*)(new_data) + _data_x_offset),
        *(float*)((char*)(new_data) + _data_y_offset));

	//first data
	//set the ref-fremework
	if (_draw_type == WARMGUI::CDataLineGraph::SDATA_GRAPH_LINE_TYPE_DIFF_LINEBAR) {
		psForSetData._pntOld.y = psForSetData._pntNew.y = 0;
	} else {
	}

	HRESULT hr = S_OK;
	SafeRelease(&_pathg);
    SafeRelease(&_pSink);
	hr = CDxFactorys::GetInstance()->GetD2DFactory()->CreatePathGeometry(&_pathg);

	hr = _pathg->Open(&_pSink);
	if (SUCCEEDED(hr)) {
		_pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
        _pSink->BeginFigure(D2D1::Point2F(psForSetData._pntNew.x, psForSetData._pntNew.y), D2D1_FIGURE_BEGIN_HOLLOW);
    }
}

inline void CDataLineGraph::AddData(dataptr data)
{
    return AddDataToPathGeometry(data);
}

inline void CDataLineGraph::AddDataToPathGeometry(dataptr new_data)
{
	//keep old point
	psForSetData._pntOld = psForSetData._pntNew;
	//Add new point
	if (_draw_type == WARMGUI::CDataLineGraph::SDATA_GRAPH_LINE_TYPE_DIFF_LINEBAR) {
		//Add new point as diff data
		psForSetData._pntNew = FPOINT(
			*(float*)((char*)(new_data) + _data_x_offset),
			*(float*)((char*)(new_data) + _data_y_offset) - psForSetData._O.y);
	} else {
		psForSetData._pntNew = FPOINT(
			*(float*)((char*)(new_data) + _data_x_offset),
			*(float*)((char*)(new_data) + _data_y_offset));
	}
	psForSetData._O = FPOINT(
			*(float*)((char*)(new_data) + _data_x_offset),
			*(float*)((char*)(new_data) + _data_y_offset));
		

	++psForSetData._count;
    _pSink->AddLine(D2D1::Point2F(psForSetData._pntNew.x, psForSetData._pntNew.y));
}

inline void CDataLineGraph::EndSetData()
{
	_pSink->EndFigure(D2D1_FIGURE_END_OPEN);

    _pSink->Close();
	SafeRelease(&_pSink);
	_changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CHANGED);
}


void CDataLineGraph::MoveBitmapToLeft()
{
    float delta = _referframe->GetXScale() * (float)_rtcset->_space_len;
    D2D1_RECT_F rectDest = D2D1::RectF(0.0f , 0.0f, fRectWidth(_rect) - delta, fRectHeight(_rect));
    D2D1_RECT_F rectSrc  = D2D1::RectF(delta, 0.0f, fRectWidth(_rect), fRectHeight(_rect));

    CriticalLock::Scoped scope(_myown_artist->_lock_artist);

    eArtist* _back_artist = _artist;
    _artist = _myown_artist;

    _artist->BeginBmpDraw(true);

    _artist->GetTransform(&_backup_trans);
    MATRIX_2D id = D2D1::Matrix3x2F::Identity();
    _artist->SetTransform(&id);

    _artist->DrawBitmap(_artist->GetDefaultBmp(), rectDest, rectSrc);

    _artist->SetTransform(&_backup_trans);

    _artist->EndBmpDraw();

    _artist = _back_artist;
}

} //namespace WARMGUI