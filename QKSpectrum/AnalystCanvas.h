#ifndef __analyst_canvas_h__
#define __analyst_canvas_h__

class CAnalystCanvas : public WARMGUI::IDataCanvas
{
public:
    CAnalystCanvas(const char* name);
    ~CAnalystCanvas(void);

    virtual const HRESULT      Init(const char* name = 0);
	virtual void               SetGlyphRect();

    void                       setDataOffset(int pxo, int pyo);
    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    virtual GLYPH_CHANGED_TYPE NewData(dataptr data, size_t datalen, DataObject::MARKET_DATA_TYPE datatype);
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype);
protected:
    CPriceTickChart*       _prc_chart;
    /*
    CZITGraph*             _zit_graph;
    CDataLineGraph*  _approx_short_lo;
    CDataLineGraph*  _approx_short_he;
    CDataLineGraph*   _approx_long_lo;
    CDataLineGraph*   _approx_long_he;
    CDataLineGraph* _predict_short_lo;
    CDataLineGraph* _predict_short_he;
    CDataLineGraph*  _predict_long_lo;
    CDataLineGraph*  _predict_long_he;
    */
    CCtpmmdContainer*     _data_cont;

    GLYPH_CHANGED_TYPE NewDataForCtpmmd(CCtpmmdContainer* czc, DataObject::MARKET_DATA_TYPE datatype);

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CAnalystCanvas"); }
};

#endif //__analyst_canvas_h__