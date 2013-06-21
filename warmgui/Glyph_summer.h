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

class WARMGUI_API IGlyph_summer : public IObject
{
public:
                           IGlyph_summer(void);
                           IGlyph_summer(const char* name);
                          ~IGlyph_summer(void);
    
    inline  void           set_config(CWarmguiConfig* config) { _config = config; }
    inline  void           set_config(CWarmguiConfig* config, const char *conf_str);
    inline virtual HRESULT init() { return S_OK; }
    virtual HRESULT        pre_draw() { return S_OK; }

    void                   set_name(const char* name) { strcpy_s(_name, MAX_PATH, name); }
    char*                  get_name()  { return  _name; }

    inline virtual void    set_rect(RECT& rect);
    RECT&                  get_rect()  { return  _rect; }
    inline void            set_children_rect();

    GlyphTreeIter_summer&  get_tree_iter() { return _tree_iter; }
    void                   set_tree(CGlyphTree_summer* tree) { _glyph_tree = tree; }
    void                   set_tree_iter(GlyphTreeIter_summer iter) { _tree_iter = iter; }
    inline void            append_glyph(IGlyph_summer* glyph);

    inline bool            is_me(const char* name);

    void                   set_myown_artist() { _own_artist = GLYPH_OWN_ARTIST_TYPE_MYSELF; }
    eArtist*               get_artist() { return _artist; }
    void                   inherit(IAtelier_summer* atelier, CGlyphTree_summer* tree, ICanvas_summer* canvas, GlyphTreeIter_summer& tree_iter, eArtist* artist, CWarmguiConfig* config)
                           { _atelier = atelier, _glyph_tree = tree, _canvas = canvas, _tree_iter = tree_iter, _artist = artist, _config = config; }

    int                    is_changed() { return _changed; }

    enum GLYPH_TYPE {
        GLYPH_TYPE_GLYPH, ///as a graph
        GLYPH_TYPE_BKG,   ///as a backgound graph
    };
    void                   set_glyph_type(GLYPH_TYPE glyph_type) { _mytype = glyph_type; }

    void                   set_change(GLYPH_CHANGED_TYPE changed) {CriticalLock::Scoped scope(_lockChange); _changed =  changed ;}
    inline void            change(GLYPH_CHANGED_TYPE     changed);
	void                   dechange(GLYPH_CHANGED_TYPE   changed) {CriticalLock::Scoped scope(_lockChange); _changed &= ~changed;}
protected:

    HRESULT                draw_graph(GLYPH_TYPE glyph_type =IGlyph_summer::GLYPH_TYPE_GLYPH);
    virtual HRESULT        draw();

    inline HRESULT         push_layer();
    inline HRESULT         pop_layer();

protected:
    GLYPH_OWN_ARTIST_TYPE _own_artist;
    eArtist*                  _artist;

    RECT                        _rect;
    RECT                    _abs_rect;
    char              _name[MAX_PATH];

    GlyphTreeIter_summer   _tree_iter;
    CGlyphTree_summer*    _glyph_tree;  //reference to the global tree

    MATRIX_2D             _back_trans;
    CWarmguiConfig*           _config;
    char          _conf_str[MAX_PATH];

    IAtelier_summer*         _atelier;
    ICanvas_summer*           _canvas;

    GLYPH_CHANGED_TYPE       _changed;
    CriticalLock          _lockChange;

    GLYPH_TYPE                _mytype;
private:
    virtual void setClass() { SetMyClass("IGlyph_summer"); }
};

} //namespace WARMGUI

#endif //__interface_glyph_summer_h__