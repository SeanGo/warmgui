#ifndef __warmgui_chart_h_include__
#define __warmgui_chart_h_include__

namespace WARMGUI {
class WARMGUI_API CSeriesDataChart : public IGlyph
{
public:
    CSeriesDataChart(const TCHAR* name);
    ~CSeriesDataChart(void);

    ///resize
    virtual void SetRect(RECT& rect);
    ///resize sub-glyph
    void SetGlyphRect(RECT& rect);

    ///set background of chart
    void SetBlind() {}
    ///add a new graph
    HRESULT AddGraph(IGlyph* g, bool bDataLine = true);
    virtual HRESULT DrawGraph(bool /*redraw = false*/) {return S_OK;}

	inline void SetWorldRect(LIMIT_2D& limit);
	/// if the scale was changed return 1, else return 0
	inline void SetWorldRect(float minx, float maxx, float miny, float maxy, float x0, float y0);
	inline CCartesian* GetCartesian() { return &_cart; }

    GLYPH_CHANGED_TYPE NewData(DataObjectPtr dop);

    inline CDataLineGraph* GetGraph();

    HRESULT Init();
    void GetChartSettings();

    void SetGeometryData(dataptr pdata, int count, int datasize);

    RtChartSettings* GetRtcSet() { return &_rtcset; }

public:
    RtChartSettings _rtcset;

protected:
    CBlind*          _blind;
	CCartesian        _cart;
    CCoordGrid*      _coord;
};

}//namespace WARMGUI

#endif //__warmgui_chart_h_include__