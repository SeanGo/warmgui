#ifndef __price_volume_openintrest_canvas_h__
#define __price_volume_openintrest_canvas_h__


class WARMGUI_API CPVICanvas : public CRtDataCanvas
{
public:
    CPVICanvas(const TCHAR* name);
    ~CPVICanvas(void);

    virtual HRESULT Init();
	virtual void SetGlyphRect();

    void                       setDataOffset(int pxo, int pyo, int vxo, int vyo, int ixo, int iyo);
    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    virtual void               SetGeometryData(dataptr pdata, int count, int datasize);
    void                       SetChatToDataContanier();

protected:
    CSeriesDataChart*    _chart_price;
    CSeriesDataChart*   _chart_volume;
    CSeriesDataChart* _chart_interest;
};



#endif //__price_volume_openintrest_canvas_h__