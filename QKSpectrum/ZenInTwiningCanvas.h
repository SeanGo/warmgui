#ifndef __zen_in_twining_canvas_h__
#define __zen_in_twining_canvas_h__

class CZenInTwiningCanvas : public WARMGUI::ICanvas
{
public:
    CZenInTwiningCanvas(const TCHAR* name);
    ~CZenInTwiningCanvas(void);

    virtual HRESULT            Init();
    virtual void               SetGlyphRect();

    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    void                       setDataOffset(int pxo, int pyo);
    void                       SetChatToDataContanier();

protected:
    CBlind*            _blind;
    CSeriesDataChart*    _chart;
    CZenInTwiningGraph* _zitg;
};


#endif //__zen_in_twining_canvas_h__
