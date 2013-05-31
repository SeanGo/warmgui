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
        _count = 0;
    }

    size_t AddData(double x, double y) {
        if (_count < _buf_size)
            _points[_count].x = (float)x, _points[_count++].y = (float)y;
        return _count;
    }

    size_t AddData(float x, float y) {
        if (_count < _buf_size)
            _points[_count].x = x, _points[_count++].y = y;
        return _count;
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

	CDataLineGraph(const char* name, GEOMETRY_PATH_TYPE path_type = GEOMETRY_PATH_TYPE_LINE, bool world_own_type = false, bool data_own_type  = false, bool own_artist = false);
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
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype) {return GLYPH_CHANGED_TYPE_NONE;}
    virtual GLYPH_CHANGED_TYPE NewData(dataptr pdata, bool need_renew = true);

	void                  SetLineColor(COLORREF clrBGR, float alpha = 1.0f)
		                     {_color = clrBGR, _alpha = alpha; }

	///pos starts from 1
	///pos starts from 1 with default color
	inline  void          SetDataOffset(int x_offset, int y_offset);
	inline  void          SetStrokeWidth(float stroke_width) {_stroke_width = stroke_width;}
    virtual void          SetRect(RECT& rect);


    void                  MoveBitmapToLeft();

    void                  SetGeometryData(dataptr pdata, int count, int datasize);

    inline virtual void   AddData(dataptr data);
    inline virtual void   BeginSetData(dataptr data);
    inline virtual void   EndSetData();
    void                  AddDataToPathGeometry(dataptr data);

    GRAPH_DATA*           GetGraphData() {if (_data_own_type) return &_myown_data; else return (0);}


protected:
	COLORREF                  _color;
	float                     _alpha;
	float              _stroke_width;
	ID2D1PathGeometry*        _pathg;
	ID2D1GeometrySink*        _pSink;
	int               _data_x_offset;
	int               _data_y_offset;
    POINTSET                psForNew;
    POINTSET            psForSetData;
    GEOMETRY_PATH_TYPE    _path_type;

    GRAPH_DATA           _myown_data;
protected:
    void               RedrawGraph();

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CDataLineGraph"); }
    HRESULT _draw_lines(bool redraw);

};


}//namespace WARMGUI

#endif //__warmgui_series_data_graph_h_include__