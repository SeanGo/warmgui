#ifndef __zen_in_twining_canvas_h__
#define __zen_in_twining_canvas_h__

class CZenInTwiningCanvas : public WARMGUI::IDataCanvas
{
public:
    CZenInTwiningCanvas(const char* name);
    ~CZenInTwiningCanvas(void);

    virtual const HRESULT      Init(const char* name = 0);
    virtual void               SetGlyphRect();

    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    void                       setDataOffset(int pxo, int pyo);
    void                       SetChatToDataContanier();
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype) { return GLYPH_CHANGED_TYPE_NONE; }

protected:
    CBlind*            _blind;
    CSeriesDataChart*    _chart;
    CZenInTwiningGraph* _zitg;
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CZenInTwiningCanvas"); }
};


#endif //__zen_in_twining_canvas_h__
