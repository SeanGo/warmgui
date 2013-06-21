#ifndef __interface_glyph_summer_h__
#define __interface_glyph_summer_h__

namespace WARMGUI {

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

    HRESULT                draw_graph();
    virtual HRESULT        draw();
    inline  void           set_config(CWarmguiConfig* config) { _config = config; }
    inline  void           set_config(CWarmguiConfig* config, const char *conf_str);
    inline virtual HRESULT init() { return S_OK; }
    virtual HRESULT        pre_draw() { return S_OK; }

    char*                  get_name()  { return  _name; }
    CWorld*                get_world() { return _world; }
    RECT&                  get_rect()  { return  _rect; }
    GlyphTreeIter_summer&  get_tree_iter() { return _tree_iter; }
    eArtist*               get_artist() { return _artist; }

    void                   set_tree(CGlyphTree_summer* tree) { _glyph_tree = tree; }
    void                   set_tree_iter(GlyphTreeIter_summer iter) { _tree_iter = iter; }

    inline bool            is_me(const char* name);
    void                   inherit(IAtelier_summer* atelier, CGlyphTree_summer* tree, ICanvas_summer* canvas, GlyphTreeIter_summer& tree_iter, eArtist* artist, CWarmguiConfig* config)
                           { _atelier = atelier, _glyph_tree = tree, _canvas = canvas, _tree_iter = tree_iter, _artist = artist, _config = config; }

    inline void            append_glyph(IGlyph_summer* glyph);
    void                   set_myown_artist() { _own_artist = GLYPH_OWN_ARTIST_TYPE_MYSELF; }
protected:
    inline HRESULT         push_layer();
    inline HRESULT         pop_layer();

protected:
    GLYPH_OWN_ARTIST_TYPE _own_artist;
    eArtist*                  _artist;
    //eArtist*               _my_artist;

    GLYPH_OWN_WORLD_TYPE   _own_world;
    CWorld*                    _world;
    CWorld*                 _my_world;

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

private:
    virtual void setClass() { SetMyClass("IGlyph_summer"); }
};

} //namespace WARMGUI

#endif //__interface_glyph_summer_h__