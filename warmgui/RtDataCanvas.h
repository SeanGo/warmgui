#ifndef __warmgui_realtime_data_h_include__
#define __warmgui_realtime_data_h_include__

namespace WARMGUI {

class IDataContainer;

class WARMGUI_API CRtDataCanvas :  public ICanvas
{
public:
    CRtDataCanvas(const char* name);
    ~CRtDataCanvas(void);

    virtual HRESULT            Init();
    virtual void               SetGlyphRect();

    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    void                       setDataOffset(int pxo, int pyo);
    inline  void               SetChatToDataContanier();

protected:
    CBlind*         _blind;
    CSeriesDataChart* _chart;
};


}

#endif //__warmgui_realtime_data_h_include__

