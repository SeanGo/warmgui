#ifndef __warmgui_chart_h_include__
#define __warmgui_chart_h_include__

namespace WARMGUI {
class WARMGUI_API CSeriesDataChart : public IDataGraph
{
public:
    CSeriesDataChart(const char* name);
    ~CSeriesDataChart(void);

    ///resize
    virtual void SetRect(RECT& rect);
    ///resize sub-glyph
    void SetGlyphRect(RECT& rect);

    ///add a new graph
    HRESULT AddGraph(IGlyph* g, bool bDataLine = true);
    virtual HRESULT DrawGraph(bool /*redraw = false*/) {return S_OK;}

	inline void SetWorldRect(WORLD_RECT& limit);
	inline CWorld* GetCartesian() { return &_cart; }

    GLYPH_CHANGED_TYPE NewData(DataObjectPtr dop);
    GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype) {return GLYPH_CHANGED_TYPE_NONE;}

    inline CDataLineGraph* GetGraph();

    virtual HRESULT Init();
    void GetChartSettings();

    void SetGeometryData(dataptr pdata, int count, int datasize);

    RtChartSettings* GetRtcSet() { return &_rtcset; }

    virtual bool               AddNewData(DataObjectPtr) {return true;}

public:
    RtChartSettings _rtcset;

protected:
	CWorld            _cart;
    CCoordGrid*      _coord;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CSeriesDataChart"); }
};

}//namespace WARMGUI

#endif //__warmgui_chart_h_include__