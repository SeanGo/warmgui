#ifndef __euclid_canvas_h__
#define __euclid_canvas_h__


class CEuclidCanvas : public WARMGUI::IDataCanvas
{
public:
    CEuclidCanvas(const char* name);
    ~CEuclidCanvas(void);

    virtual const HRESULT      Init(const char* name = 0);
	virtual void               SetGlyphRect();

    void                       setDataOffset(int pxo, int pyo);
    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    virtual GLYPH_CHANGED_TYPE NewData(dataptr data, size_t datalen, DataObject::MARKET_DATA_TYPE datatype);
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype);

protected:
    bool        _bHasBackground;
    CBlind*              _blind;

    CPriceTickChart* _prc_chart;
    CDataLineGraph*  _approx_short_lo;
    CDataLineGraph*  _approx_short_he;
    CDataLineGraph*  _approx_long_lo;
    CDataLineGraph*  _approx_long_he;
    CDataLineGraph*  _predict_short_lo;
    CDataLineGraph*  _predict_short_he;
    CDataLineGraph*  _predict_long_lo;
    CDataLineGraph*  _predict_long_he;

    CCtpmmdContainer* _data_cont;
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CEuclidCanvas"); }

    GLYPH_CHANGED_TYPE NewDataForCtpmmd(CCtpmmdContainer* czc, DataObject::MARKET_DATA_TYPE datatype);
};



#endif //__euclid_canvas_h__