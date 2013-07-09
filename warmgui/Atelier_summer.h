#ifndef __warmgui_summer_interface_atelier__
#define __warmgui_summer_interface_atelier__

namespace WARMGUI {

class WARMGUI_API IAtelier_summer : public IGlyph_summer
{
public:
                           IAtelier_summer(void);
                           IAtelier_summer(const char* name);
    virtual               ~IAtelier_summer(void);

    inline virtual HRESULT init(HWND hwnd);
    inline void            append_canvas(ICanvas_summer* canvas);
    IGlyph_summer*         find_glyph(const char* name);

    virtual void           WindowSizing(bool sizing = true) {_sizing = sizing;}
	bool                   IsWindowSizing() { return _sizing; }
    inline HRESULT         Draw();

    inline virtual void    set_rect(RECT& rect);

    inline bool            GetBitmapMaterial();
    inline bool            GetMyBackgroundRect();
    inline void            GetWinrectInMaterial(RECT& canvas_rect, RECT_f* bmprect);

    virtual void           disposal(RECT& rect) {}
    HWND                   get_hwnd() { return _hwnd; }
    inline void            redraw_window(bool all_redraw = false);

    void                   ToggleToolbar(const char* toolbar_name = "toolbar");

    virtual int            is_changed()
                           {
                               CriticalLock::Scoped scope(_lockChange);
                               return ((_changed & GLYPH_CHANGED_ATELIER_RESIZE)
                                   || (_changed & GLYPH_CHANGED_ATELIER_BKG));
                           }
    virtual int            OnLButtonUp(UINT nFlags, int x, int y);
protected:
    HWND                   _hwnd;
	ID2D1HwndRenderTarget* _pHwndRT;    ///HwndRT of the window

	AppBitmap_summer       _appbmp;    ///bitmap of materies and background of all
	bool                   _sizing;    ///the windows is re-sizing

    inline HRESULT         DrawBkg();
    inline HRESULT         DrawGraph();

    inline HRESULT         copy_screen_to_bitmap();
    inline HRESULT         draw_screen_bitmap();

    RECT                   _winrect_in_material;    ///the window-rect in material-bitmap
private:
    void setClass() { SetMyClass("IAtelier_summer"); }
};


EXPORT_STL_VECTOR(WARMGUI_API, IAtelier_summer*)
typedef std::vector<IAtelier_summer*> AtelierArray_summer;
typedef AtelierArray_summer::iterator AtelierIter_summer;
typedef AtelierArray_summer::const_iterator AtelierConstIter_summer;

} //namespace WARMGUI

#endif //__warmgui_summer_interface_atelier__