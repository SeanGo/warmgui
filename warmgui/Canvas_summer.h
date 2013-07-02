#ifndef __warmgui_summer_interface_canvas__
#define __warmgui_summer_interface_canvas__

namespace WARMGUI {

class IAtelier_summer;
class WARMGUI_API ICanvas_summer : public IGlyph_summer {
public:
                    ICanvas_summer();
                    ICanvas_summer(const char *name);
    virtual        ~ICanvas_summer();

    inline void     inherit(IAtelier_summer* atelier, CGlyphTree_summer* tree, GlyphTreeIter_summer& tree_iter, eArtist* artist, CWarmguiConfig* config, AppBitmap_summer* appbmp);
    virtual HRESULT init();
    inline  void    append_glyph(IGlyph_summer* glyph);

    void            set_appbitmap(AppBitmap_summer* appbmp) { _appbmp = appbmp; }
    virtual HRESULT draw_mouse_graph() { if (_mouse_graph) return _mouse_graph->draw(); else return S_OK; }

    virtual int            is_changed()
                           {
                               CriticalLock::Scoped scope(_lockChange);
                               return ((_changed & GLYPH_CHANGED_CANVAS_RESIZE)
                                   || (_changed & GLYPH_CHANGED_CANVAS_BKG)
                                   || (_changed & GLYPH_CHANGED_CANVAS));
                           }

protected:
	CSharedImage_summer* _imgBkg;
	AppBitmap_summer*    _appbmp;      ///bitmap of materies and background of all
    IAtelier_summer*     _atelier;

    WGBitmap*            _canvas_bmp;  ///graph of the canvas for redraw
    CMouseGraph_summer*  _mouse_graph; ///grraph of the mouse trike
private:
    void setClass() { SetMyClass("ICanvas_summer"); }
};



///////////////////////////////////////////////////////////////////////////////////////
// class CBkgCanvas_summer
class WARMGUI_API CBkgCanvas_summer : public ICanvas_summer
{
public:
	CBkgCanvas_summer(void);
	CBkgCanvas_summer(const char* name);
	~CBkgCanvas_summer(void);

	virtual HRESULT init();

protected:
	CSharedImage_summer* _imgBkg;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CBkgCanvas_summer"); }
};




} //namespace WARMGUI


#endif //__warmgui_summer_interface_canvas__