#ifndef __warmgui_series_data_graph_h_include__
#define __warmgui_series_data_graph_h_include__


namespace WARMGUI {


typedef struct GRAPH_DATA {
    FPOINT*  _points;
    size_t _buf_size;
    size_t    _count;

    GRAPH_DATA() : _points(0), _buf_size(0), _count(0) {}
    ~GRAPH_DATA() {
        SafeDelete(_points);
    }

    void SetSize(size_t bufsize) {
        SafeDelete(_points);
        _count = 0, _buf_size = bufsize;

        _points = new FPOINT[_buf_size];
    }

    void Reset() {
        _count = 0 ;

        if (_points)
            memset(_points, 0, sizeof(FPOINT) * _buf_size);
        else
            _buf_size = 0;
    }

    bool AddData(float x, float y) {
        if (_count < _buf_size) {
            _points[_count].x = x, _points[_count++].y = y;
        } else {
            memmove(_points, _points + 1, (_count - 1) * sizeof(FPOINT));
            _points[_count - 1].x = x, _points[_count - 1].y = y;
        }
        return true;
    }
} GRAPH_DATA;


class WARMGUI_API CDataLineGraph : public IDataGraph
{
public:
    enum GEOMETRY_PATH_TYPE {
        GEOMETRY_PATH_TYPE_LINE,
        GEOMETRY_PATH_TYPE_BEZIER,
        GEOMETRY_PATH_TYPE_ARC,
    };

	CDataLineGraph(const char* name, GEOMETRY_PATH_TYPE path_type = GEOMETRY_PATH_TYPE_LINE, bool world_own_type = false, bool own_artist = false);
	virtual ~CDataLineGraph(void);

    typedef struct POINTSET {
        FPOINT _pntOld;     //last point
        FPOINT _pntNew;     //new point
        int _count;
    } POINTSET;

	inline void           InitDataPtr();
	/// if the referframe was changed then return 1
	/// if an error occured return less than zero
	/// otherwith return zero
	virtual HRESULT       DrawGraph(bool redraw = false);
	virtual HRESULT       RenewGraph();
    virtual HRESULT       Renew();
    virtual HRESULT       PreDraw();

	void                  SetLineColor(COLORREF clrBGR, float alpha = 1.0f)
		                     {_color = clrBGR, _alpha = alpha; }

	///pos starts from 1
	///pos starts from 1 with default color
	inline  void          SetStrokeWidth(float stroke_width) {_stroke_width = stroke_width;}
    virtual void          SetRect(RECT& rect);

    void                  MoveBitmapToLeft();

    GLYPH_CHANGED_TYPE    NewData(float x, float y);

    ///re-lu all data
    inline virtual void   BeginSetData(float x, float y);
    void                  AddDataToPathGeometry(float x, float y);
    inline virtual void   EndSetData();

    virtual int           is_selected(int x, int y);
    GRAPH_DATA*           GetGraphData() { return &_myown_data; }

    void                  set_data_buffer(size_t size) { _myown_data.SetSize(size); }
    void                  reset_data() { _myown_data.Reset(); }

    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype) { return GLYPH_CHANGED_TYPE_NONE; }

protected:
	COLORREF                  _color;
	float                     _alpha;
	float              _stroke_width;
	ID2D1PathGeometry*        _pathg;
	ID2D1GeometrySink*        _pSink;
    GEOMETRY_PATH_TYPE    _path_type;

    GRAPH_DATA           _myown_data;

    POINTSET                psForNew;
    POINTSET            psForSetData;

protected:
    void               RedrawGraph();

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CDataLineGraph"); }
    HRESULT _draw_lines(bool redraw);

};


}//namespace WARMGUI

#endif //__warmgui_series_data_graph_h_include__