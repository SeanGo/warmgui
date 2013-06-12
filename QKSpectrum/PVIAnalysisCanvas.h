#ifndef __pvi_analysis_canvas_h__
#define __pvi_analysis_canvas_h__

class CPVIAnalysisCanvas : public WARMGUI::IDataCanvas
{
public:
    CPVIAnalysisCanvas(const char* name);
    ~CPVIAnalysisCanvas(void);

    virtual const HRESULT      Init(const char* name = 0);
	virtual void               SetGlyphRect();

    void                       setDataOffset(int pxo, int pyo, int vxo, int vyo, int ixo, int iyo);
    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    virtual GLYPH_CHANGED_TYPE NewData(dataptr data, size_t datalen, DataObject::MARKET_DATA_TYPE datatype);
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype);

    virtual void               SetGeometryData(dataptr pdata, int count, int datasize);

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("pvi_analysis_caanvas"); }

    TripleTuple                 _tt_layout;
    MARGIN                         _margin;
    int                    _chart_interval;

    CPriceAnalysChart*          _prc_chart;
    CVolumeTickChart*           _vol_chart;
    CInterestTickChart*         _itr_chart;

    GLYPH_CHANGED_TYPE  _prc_graph_changed;
    GLYPH_CHANGED_TYPE  _vol_graph_changed;
    GLYPH_CHANGED_TYPE  _itr_graph_changed;

    CZITGraph*                           _zit_graph;

    CCtpmmdContainer*           _data_cont;
    GLYPH_CHANGED_TYPE NewDataForCtpmmd(CCtpmmdContainer* czc, DataObject::MARKET_DATA_TYPE datatype);
};


#endif //__pvi_analysis_canvas_h__