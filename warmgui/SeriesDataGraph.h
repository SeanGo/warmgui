#ifndef __warmgui_series_data_graph_h_include__
#define __warmgui_series_data_graph_h_include__


namespace WARMGUI {

class ISeriesDataGraph : public IGlyph
{
public:
    ISeriesDataGraph(void);
    ISeriesDataGraph(const TCHAR* name);
    ~ISeriesDataGraph(void);

    enum DATA_MANIPULATOR_TYPE {
        DATA_MANIPULATOR_TYPE_GRAPH,
        DATA_MANIPULATOR_TYPE_CANVAS,
    };

    void                  SetDataManipulator(DATA_MANIPULATOR_TYPE manipulator) { _manipulator = manipulator; }
    DATA_MANIPULATOR_TYPE GetDataManipulator() { return _manipulator; }

    inline virtual void   AddData(dataptr data)      {}
    inline virtual void   BeginSetData(dataptr data) {}
    inline virtual void   EndSetData()               {}

protected:
    DATA_MANIPULATOR_TYPE _manipulator;
};


class IFrameDataGraph : public IGlyph
{
public:
    IFrameDataGraph(void);
    IFrameDataGraph(const TCHAR* name);
    ~IFrameDataGraph(void);



};


class CSeriesDataChart;

class WARMGUI_API CDataLineGraph : public IGlyph
{
public:
	CDataLineGraph(const TCHAR* name, bool own_artist, CSeriesDataChart *parent);
	virtual ~CDataLineGraph(void);


	enum SDATA_GRAPH_LINE_TYPE {
         SDATA_GRAPH_LINE_TYPE_PATH_GEOMETRY,
         SDATA_GRAPH_LINE_TYPE_LINE,
         SDATA_GRAPH_LINE_TYPE_LINEBAR,
         SDATA_GRAPH_LINE_TYPE_DIFF_LINEBAR,
         SDATA_GRAPH_LINE_TYPE_POINT,
	};

    typedef struct POINTSET {
        FPOINT _O, _pntOld, _pntO0, _pntNew;
        int _count;
    } POINTSET;

	inline void           InitDataPtr();
	/// if the referframe was changed then return 1
	/// if an error occured return less than zero
	/// otherwith return zero
    inline  virtual       GLYPH_CHANGED_TYPE AppendData(DataObjectPtr /*dopNewData*/);
	virtual HRESULT       DrawGraph(bool redraw = false);
	virtual HRESULT       RenewGraph();
    virtual HRESULT       Renew();
    virtual HRESULT       PreDraw();

	virtual void          SetDrawType(SDATA_GRAPH_LINE_TYPE draw_type) { _draw_type = draw_type; }
    SDATA_GRAPH_LINE_TYPE getDrawType() { return _draw_type; }

	void                  SetLineColor(COLORREF clrBGR, float alpha = 1.0f)
		                     {_color = clrBGR, _alpha = alpha; }

	///pos starts from 1
	void                  SetDataOffset(int x_offset, int y_offset, COLORREF clrBGR = BGR(255, 255, 255), float alpha = 1.0f)
	                         { _color = clrBGR, _data_x_offset = x_offset, _data_y_offset = y_offset, _alpha = alpha; }
	///pos starts from 1 with default color
	inline  void          SetDataOffset(int pos, int x_offset, int y_offset);
	inline  void          SetStrokeWidth(float stroke_width) {_stroke_width = stroke_width;}
    virtual void          SetRect(RECT& rect);


    inline void           SetRtChartSettings(RtChartSettings* rtcset)
                              { _rtcset = rtcset; SetStrokeWidth(_rtcset->_stroke_width); }

    void                  MoveBitmapToLeft();

    void                  SetGeometryData(dataptr pdata, int count, int datasize);

    inline virtual void   AddData(dataptr data);
    inline virtual void   BeginSetData(dataptr data);
    inline virtual void   EndSetData();
    void                  AddDataToPathGeometry(dataptr data);

protected:
	COLORREF                  _color;
	float                     _alpha;
	float              _stroke_width;
	ID2D1PathGeometry*        _pathg;
	ID2D1GeometrySink*        _pSink;
	SDATA_GRAPH_LINE_TYPE _draw_type;
    CSeriesDataChart*        _parent;
	int               _data_x_offset;
	int               _data_y_offset;
    POINTSET                psForNew;
    POINTSET            psForSetData;
    RtChartSettings*         _rtcset;

protected:
    void             RedrawGraph();

};


}//namespace WARMGUI

#endif //__warmgui_series_data_graph_h_include__