#ifndef __price_volume_openintrest_canvas_h__
#define __price_volume_openintrest_canvas_h__


class WARMGUI_API CPVICanvas : public IDataCanvas
{
public:
    CPVICanvas(const char* name);
    ~CPVICanvas(void);

    virtual const HRESULT      Init(const char* name = 0);
	virtual void               SetGlyphRect();

    void                       setDataOffset(int pxo, int pyo, int vxo, int vyo, int ixo, int iyo);
    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype) {return GLYPH_CHANGED_TYPE_NONE;}
    virtual void               SetGeometryData(dataptr pdata, int count, int datasize);

    void                       AddDataChart(const char* name, CSeriesDataChart** chart);
protected:
    CSeriesDataChart*    _chart_price;
    CSeriesDataChart*   _chart_volume;
    CSeriesDataChart* _chart_interest;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CPVICanvas"); }
};



#endif //__price_volume_openintrest_canvas_h__