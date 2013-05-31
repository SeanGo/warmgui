#ifndef __zit_graph_h__
#define __zit_graph_h__

class CZITGraph : public IDataGraph
{
public:
    CZITGraph(const char* name);
    ~CZITGraph(void);

    virtual HRESULT DrawGraph(bool /*redraw = false*/);
    virtual GLYPH_CHANGED_TYPE AppendData(DataObjectPtr dopNewData);

    void                       SetDownSample(int ds) { _down_sample = ds; }
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype);
    virtual GLYPH_CHANGED_TYPE NewData(dataptr pdata, bool need_renew = true);
    virtual HRESULT            Init();

private:
    HRESULT DrawPoint(double x, float y);
    HRESULT DrawInflLine(INFLEXION& infl);
    HRESULT DrawArea();

private:
    ZitData*     _zit_data;
    int       _down_sample;
    COLORALPHA _crAreaUpText, _crAreaDownText;
    IDWriteTextFormat* _pAreaFont;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CZITGraph"); }
};

#endif //__zit_graph_h__