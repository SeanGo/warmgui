#ifndef __pvi_canvas_h__
#define __pvi_canvas_h__


class WARMGUI_API CPVI_Canvas : public IDataCanvas
{
public:
    CPVI_Canvas(const char* name);
    ~CPVI_Canvas(void);

    virtual const HRESULT      Init(const char* name = 0);
	virtual void               SetGlyphRect();

    void                       setDataOffset(int pxo, int pyo, int vxo, int vyo, int ixo, int iyo);
    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    virtual GLYPH_CHANGED_TYPE NewData(dataptr data, size_t datalen, DataObject::MARKET_DATA_TYPE datatype);
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype);

    virtual void               SetGeometryData(dataptr pdata, int count, int datasize);

    //void                       AddDataChart(const char* name, CSeriesDataChart** chart);

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CPVI_Canvas"); }

    TripleTuple                 _tt_layout;
    MARGIN                         _margin;
    int                    _chart_interval;

    CPriceTickChart*            _prc_chart;
    CVolumeTickChart*           _vol_chart;
    CInterestTickChart*         _itr_chart;

    GLYPH_CHANGED_TYPE  _prc_graph_changed;
    GLYPH_CHANGED_TYPE  _vol_graph_changed;
    GLYPH_CHANGED_TYPE  _itr_graph_changed;

    CCtpmmdContainer*           _data_cont;
    GLYPH_CHANGED_TYPE NewDataForCtpmmd(CCtpmmdContainer* czc, DataObject::MARKET_DATA_TYPE datatype);

    /*
    WARMGUI::CCoordFrame*              _cf_price;
    WARMGUI::CCoordFrame*             _cf_volume;
    WARMGUI::CCoordFrame*           _cf_interest;


    WARMGUI::CDataLineGraph*         _dlg_price;
    WARMGUI::CDataLineGraph*        _dlg_relvol;
    WARMGUI::CDataLineGraph*        _dlg_volume;
    WARMGUI::CDataLineGraph*      _dlg_interest;
    WARMGUI::CDataLineGraph*  _dlg_rel_interest;

    WARMGUI::CDataLineGraph*    _dlg_lt_prcappr;   ///long  term approx
    WARMGUI::CDataLineGraph*    _dlg_st_prcappr;   ///short term approx
    WARMGUI::CDataLineGraph*    _dlg_vol_approx;   ///approx of volume
    WARMGUI::CDataLineGraph*    _dlg_ite_approx;   ///approx of iterest

    WARMGUI::CDataLineGraph*    _dlg_lt_predict;   ///long term predcit
    WARMGUI::CDataLineGraph*    _dlg_st_predict;   ///short term predict

    WARMGUI::GlyphTreeIter    _iter_price_lines;
    WARMGUI::GlyphTreeIter   _iter_volume_lines;
    WARMGUI::GlyphTreeIter _iter_interest_lines;
    */
};



#endif //__pvi_canvas_h__