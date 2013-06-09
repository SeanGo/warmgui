#ifndef __zit_canvas_h__
#define __zit_canvas_h__

class CZitCanvas :  public IDataCanvas
{
public:
    CZitCanvas(const char* name);
    ~CZitCanvas(void);


    virtual const HRESULT      Init(const char* name = 0);
	virtual void               SetGlyphRect();

    void                       setDataOffset(int pxo, int pyo);
    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    virtual GLYPH_CHANGED_TYPE NewData(dataptr data, size_t datalen, DataObject::MARKET_DATA_TYPE datatype);
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype);
protected:
    CPriceTickChart*           _prc_chart;
    CZITGraph*                 _zit_graph;
    //CDataLineGraph*         _first_appx;
    //CDataLineGraph*         _short_prdt;
    //CDataLineGraph*          _long_prdt;

    CCtpmmdContainer*          _data_cont;

    GLYPH_CHANGED_TYPE NewDataForCtpmmd(CCtpmmdContainer* czc, DataObject::MARKET_DATA_TYPE datatype);

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CZitCanvas"); }
};

#endif //__zit_canvas_h__