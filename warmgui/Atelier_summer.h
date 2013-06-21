#ifndef __warmgui_summer_interface_atelier__
#define __warmgui_summer_interface_atelier__

namespace WARMGUI {

class WARMGUI_API IAtelier_summer : public IGlyph_summer
{
public:
    IAtelier_summer(void);
    IAtelier_summer(const char* name);
    ~IAtelier_summer(void);

    inline virtual HRESULT init(HWND hwnd);
    inline void            append_canvas(ICanvas_summer* canvas);
    IGlyph_summer*         find_glyph(const char* name);

protected:
    HWND                      _hwnd;
	ID2D1HwndRenderTarget* _pHwndRT;    ///HwndRT of the window

private:
    void setClass() { SetMyClass("IAtelier_summer"); }
};

} //namespace WARMGUI

#endif //__warmgui_summer_interface_atelier__