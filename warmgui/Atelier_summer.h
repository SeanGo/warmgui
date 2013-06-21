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

    virtual void           WindowSizing(bool sizing = true) {_sizing = sizing;}
	bool                   IsWindowSizing() { return _sizing; }
    inline HRESULT         Draw();

protected:
    HWND                      _hwnd;
	ID2D1HwndRenderTarget* _pHwndRT;    ///HwndRT of the window
    ILayout_summer*         _layout;

	AppBitmap_summer        _appbmp;    ///bitmap of materies and background of all
	bool                    _sizing;    ///the windows is re-sizing

    inline HRESULT         DrawBkg();
    inline HRESULT         DrawGraph();

    inline HRESULT         copy_bkg_to_bitmap();
    inline HRESULT         draw_bkg_bitmap();
    inline HRESULT         copy_screen_to_bitmap();
    inline HRESULT         draw_screen_bitmap();

private:
    void setClass() { SetMyClass("IAtelier_summer"); }
};

} //namespace WARMGUI

#endif //__warmgui_summer_interface_atelier__