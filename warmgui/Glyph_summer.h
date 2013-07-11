#ifndef __interface_glyph_summer_h__
#define __interface_glyph_summer_h__

namespace WARMGUI {

class WARMGUI_API AppBitmap_summer {
public:
    AppBitmap_summer()
        :   _material(0)
        ,     _screen(0)
        , _background(0)
        //,    _content(0)
    {}

    ~AppBitmap_summer()
    {
        ReleaseScreenBitmap();
        //SafeRelease(&_material  );
    }

    void SetSize(ID2D1HwndRenderTarget* pHwndRT, RECT& rect) {
        ReleaseScreenBitmap();
        
        pHwndRT->CreateBitmap(
            D2D1::SizeU(RectWidth(rect), RectHeight(rect)),
            D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
            &_screen);
    }

private:
    void ReleaseScreenBitmap()
    {
        SafeRelease(&_screen);
        //SafeRelease(&_background);
        //SafeRelease(&_content   );
    }

public:
    WGBitmap*   _material;  ///material of all application
    WGBitmap*     _screen;  ///bitmap of the all screen
    WGBitmap* _background;  ///bitmap of background
};



class IGlyph_summer;
typedef tree<IGlyph_summer*> WARMGUI_API CGlyphTree_summer;
typedef CGlyphTree_summer::iterator GlyphTreeIter_summer;
typedef tree_node_<IGlyph_summer*> WARMGUI_API glyph_node;

class IAtelier_summer;
class ICanvas_summer;

class WARMGUI_API IGlyph_summer : public IObject, public IInputResponse
{
public:
                           IGlyph_summer(void);
                           IGlyph_summer(const char* name);
    virtual               ~IGlyph_summer(void);
    
    inline  void           set_config(CWarmguiConfig* config) { _config = config; }
    inline virtual HRESULT init() { return S_OK; }
    virtual HRESULT        pre_draw() { return S_OK; }

    void                   set_name(const char* name) { strcpy_s(_name, MAX_PATH, name); }
    const char*            get_name()  { return  _name; }

    inline virtual void    set_rect(RECT& rect);
    RECT&                  get_rect()  { return  _rect; }
    inline void            set_children_rect();

    GlyphTreeIter_summer&  get_tree_iter() { return _tree_iter; }
    void                   set_tree(CGlyphTree_summer* tree) { _glyph_tree = tree; }
    void                   set_tree_iter(GlyphTreeIter_summer iter) { _tree_iter = iter; }
    inline void            append_glyph(IGlyph_summer* glyph);

    inline bool            is_me(const char* name);

    void                   set_myown_artist() { _own_artist = true; }
    eArtist*               get_artist() { return _artist; }
    inline virtual void    inherit(IAtelier_summer* atelier, CGlyphTree_summer* tree, ICanvas_summer* canvas, GlyphTreeIter_summer& tree_iter, eArtist* artist, CWarmguiConfig* config);
    inline void            inherit_config_string();
    virtual int            is_changed() { return _changed; }

    enum GLYPH_TYPE {
        GLYPH_TYPE_GLYPH, ///as a graph
        GLYPH_TYPE_BKG,   ///as a backgound graph
    };
    void                   set_glyph_type(GLYPH_TYPE glyph_type) { _mytype = glyph_type; }
    GLYPH_TYPE             get_glyph_type() { return _mytype; }

    void                   set_change(GLYPH_CHANGED_TYPE changed) {_changed =  changed ;}
    inline void            change(GLYPH_CHANGED_TYPE     changed);
	void                   dechange(GLYPH_CHANGED_TYPE   changed) {_changed &= ~changed;}

    const char*            get_config_str() { return _str_conf; }
    bool                   is_visible() { return _visible; }
    void                   toggle_visible() { _visible = (_visible) ? false : true; }

    CriticalLock*          get_change_lock() { return &_lockChange; }

    //for mouse
    IGlyph_summer*         get_selected_child_graph() { return _selected_child_graph; }
    bool                   is_selected() { return _selected; }
    virtual bool           is_interior(int x, int y) { return false; }
    //for mouse

    IAtelier_summer*       get_atelier() { return _atelier; }
    ICanvas_summer*        get_canvas()  { return _canvas;  }


  	virtual int            OnLButtonUp(UINT nFlags, int x, int y);
    bool                   intesect(int x, int y) { return pt_in_rect(_abs_rect, x, y); }
    inline                 void clear_select();

protected:

    HRESULT                draw_graph(bool redraw_all = false, GLYPH_TYPE glyph_type =IGlyph_summer::GLYPH_TYPE_GLYPH);
    virtual HRESULT        draw(bool redraw_all = false);

    inline HRESULT         push_layer();
    inline HRESULT         pop_layer();

protected:
    bool                  _own_artist;
    eArtist*                  _artist;

    RECT                        _rect;
    RECT                    _abs_rect;
    char              _name[MAX_PATH];

    GlyphTreeIter_summer   _tree_iter;
    CGlyphTree_summer*    _glyph_tree;  //reference to the global tree

    MATRIX_2D             _back_trans;
    CWarmguiConfig*           _config;
    char          _str_conf[MAX_PATH];

    IAtelier_summer*         _atelier;
    ICanvas_summer*           _canvas;

    GLYPH_CHANGED_TYPE       _changed;
    CriticalLock          _lockChange;

    GLYPH_TYPE                _mytype;
    bool                     _visible;
    //bool                     _drawing;


    IGlyph_summer*     _selected_child_graph;
    bool               _selected;
private:
    virtual void setClass() { SetMyClass("IGlyph_summer"); }
};




///////////////////////////////////////////////////////////////////////////////////////////////////
// class CImage
class WARMGUI_API CImage_summer : public IGlyph_summer {
public:
    CImage_summer();
    CImage_summer(const char* name, const char* imgpath);
    CImage_summer(const char* name, const TCHAR * imgpath);
    CImage_summer(const char* name);
    virtual ~CImage_summer();

    HRESULT         DrawGraph_wired();
    virtual HRESULT draw(bool redraw_all = false);
    inline  HRESULT DrawImage(RECT& rectSrc, RECT& rectDest, float opacity = 1.0f);
    inline  HRESULT DrawImage(RECT_f& rectSrc, RECT_f& rectDest, float opacity = 1.0f);
    inline  virtual bool Intersect(int x, int y) { return IsInRect(_rect, x, y); }
    inline  HRESULT SetImage(const TCHAR* imgpath = 0);
    inline  void    SetOpacity(float opacity);
    inline  void    SetSrcRect(RECT* rect);
    inline  void    SetSrcRectFromScaledRect(RECT* scaledRect, RECT* targetRect);

    inline HRESULT  GetSharedImage(WGBitmap** pDestBmp) const;
    inline HRESULT  SetSharedImage(const WGBitmap* pSrcBmp) const;

protected:
    WGBitmap*        _pImage;          ///Dont call SafeRelease to _pImage!!!
    TCHAR _imgpath[MAX_PATH];
    float           _opacity;

    RECT            _srcRect;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CImage"); }
};




///////////////////////////////////////////////////////////////////////////////////////////////////
// class CImage
class WARMGUI_API CSharedImage_summer : public IGlyph_summer {
public:
    CSharedImage_summer();
    CSharedImage_summer(GLYPH_TYPE glyph_type, WGBitmap* pBmp);
    ~CSharedImage_summer();

    inline virtual HRESULT draw(bool redraw_all = false);
    inline void            SetSharedImage(WGBitmap* pBmp) {_pBmp = pBmp;}
    inline HRESULT         DrawImage(RECT& rectSrc, RECT& rectDest, float opacity = 1.0f);
    inline void            SetSharedImageRect(RECT& rectSrc, float opacity = 1.0f);

protected:
    WGBitmap*          _pBmp;            ///Dont call SafeRelease to _pImage!!!
    TCHAR _imgpath[MAX_PATH];
    float           _opacity;
    D2D1_RECT_F     _srcRect;           ///the rect in the _pBmp
    D2D1_RECT_F     _dstRect;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CSharedImage"); }
};




/////////////////////////////////////////////////////////////////////////////////////////////////
// class CBlind
class WARMGUI_API CBlind_summer : public IGlyph_summer
{
public:
    CBlind_summer(const char* name)
        : _bkgclr(0)
        , _alpha(0.3f)
    {
        _snprintf_s(_name, MAX_PATH, _TRUNCATE, "Blind-%s", name);
        _mytype = GLYPH_TYPE_BKG;
        setClass();
    }

    CBlind_summer(const char* name, COLORREF bkgclr, float alpha = 0.3f, IGlyph_summer::GLYPH_TYPE type = GLYPH_TYPE_BKG)
        : _bkgclr(bkgclr)
        , _alpha(alpha)
    {
        _snprintf_s(_name, MAX_PATH, _TRUNCATE, "Blind-%s", name);
        _mytype = type;
        setClass();
    }

    inline virtual HRESULT draw(bool redraw_all = false);
    void            set_brush_color(COLORREF bkgclr, float alpha = 0.5f) {_bkgclr = bkgclr, _alpha = alpha;}
    virtual int     is_selected(int x, int y) { return (0); }

private:
    float     _alpha;
    COLORREF _bkgclr;


private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CBlind"); }
};

class WARMGUI_API CMouseGraph_summer : public IGlyph_summer
{
public:
    CMouseGraph_summer() {}
    CMouseGraph_summer(const char* name) : IGlyph_summer(name) {}
    ~CMouseGraph_summer(){}

    virtual HRESULT draw(bool redraw_all = false) {return S_OK;}
};




} //namespace WARMGUI

#endif //__interface_glyph_summer_h__