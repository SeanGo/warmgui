#ifndef __zen_in_twining_graph_h__
#define __zen_in_twining_graph_h__

class CZenInTwiningGraph : public IDataGraph
{
public:
    CZenInTwiningGraph(const char* name);
    ~CZenInTwiningGraph(void);

    virtual HRESULT DrawGraph(bool /*redraw = false*/);
    virtual GLYPH_CHANGED_TYPE AppendData(DataObjectPtr dopNewData);

    void SetDownSample(int ds) { _down_sample = ds; }

    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype) {return GLYPH_CHANGED_TYPE_NONE;}
private:
    HRESULT DrawPoint(double x, float y);
    HRESULT DrawInflLine(INFLEXION& infl);
    virtual bool               AddNewData(DataObjectPtr dop) {return true;}

private:
    ZitData      _zit_data;
    int       _down_sample;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CZenInTwiningGraph"); }
};

#endif //__zen_in_twining_graph_h__