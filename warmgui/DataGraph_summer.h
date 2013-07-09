#ifndef __interface_chat_summer__
#define __interface_chat_summer__

namespace WARMGUI {


///////////////////////////////////////////////////////////////////////////////
//class IDataGraph_summer
class WARMGUI_API IDataGraph_summer : public IGlyph_summer
{
public:
    inline                     IDataGraph_summer(void);
    inline                     IDataGraph_summer(const char* name, bool own_world = false, bool own_artist = false, bool own_data = false);
    inline virtual            ~IDataGraph_summer(void);

    bool                       my_own_world() { return _my_own_world; }

    virtual void               set_user_data(dataptr data) { _pdata = data; }
    virtual GLYPH_CHANGED_TYPE update(DataObjectPtr dop) { return (0); }
    virtual GLYPH_CHANGED_TYPE update(dataptr data) { return (0); }
    inline  virtual void       update_data() {}

    HRESULT                    predraw() { return (0); }
                               //reset glyph rect, and world's rect
    virtual void               set_rect(RECT& rect);

    CWorld*                    get_world() { return _world; }
    inline virtual void        inherit(IAtelier_summer* atelier, CGlyphTree_summer* tree, ICanvas_summer* canvas, GlyphTreeIter_summer& tree_iter, eArtist* artist, CWarmguiConfig* config);

    void                       set_world(CWorld* world)
                               {
                                   if (!_my_own_world) _world = world;
                                   child_inherit_world();
                               }

    void child_inherit_world()
    {
        for (unsigned int i = 0; i < _glyph_tree->number_of_children(_tree_iter); i++) {
            GlyphTreeIter_summer it = _glyph_tree->child(_tree_iter, i);
            if ((*it)->isClass("IDataGraph_summer"))
                ((IDataGraph_summer*)(*it))->set_world(_world);
        }
    }
protected:
    CWorld*            _world;             ///the
    bool               _my_own_world;

    eArtist*           _my_artist;
    bool               _my_own_artist;
    CriticalLock       _my_lockArtist;

    dataptr            _pdata;
    bool               _my_own_data;
    WORLD_CHANGED_TYPE _world_change;

    bool               _selected;
private:
    void SetClass() { SetMyClass("IDataGraph_summer"); }
};


typedef std::vector<IDataGraph_summer*>       DataGraphArray_summer;
typedef DataGraphArray_summer::iterator       DataGraphIter_summer ;
typedef DataGraphArray_summer::const_iterator DataGraphConstIter_summer;


///////////////////////////////////////////////////////////////////////////////
//class CCurveGraph_summer
class WARMGUI_API CCurveGraph_summer : public IDataGraph_summer
{
public:
    inline  CCurveGraph_summer(void);
    inline  CCurveGraph_summer(const char* name, bool own_world = false, bool own_artist = false, bool own_data = false);
    inline ~CCurveGraph_summer(void);

    void                       set_size(size_t bufsize) { _points.set_size(bufsize); }
    void                       reset() { _points.reset(); }

    virtual GLYPH_CHANGED_TYPE update(dataptr data);

    void                       SetLineColor(COLORREF clrBGR, float alpha = 1.0f)
                               {_color_alpha = D2D1::ColorF(clrBGR, alpha);}
	inline void                SetStrokeWidth(float stroke_width) {_stroke_width = stroke_width;}

                               //if add_to_point_set = true, reset the data-point set
    inline void                begin_set_data(float x, float y, bool add_to_point_set = true);
    inline void                add_data_to_path_geometry(float x, float y, bool add_to_point_set = true);
    inline void                begin_set_data(double x, double y, bool add_to_point_set = true)
                               { begin_set_data((float)x, (float)y, add_to_point_set);}
    inline void                add_data_to_path_geometry(double x, double y, bool add_to_point_set = true)
                               {add_data_to_path_geometry((float)x, (float)y, add_to_point_set);}
    inline void                end_set_data();

    virtual HRESULT            init();

    enum UPDATE_METHOD {
        UPDATE_METHOD_INCREST,
        UPDATE_METHOD_RENEW,
    };
    void                       set_update_method(UPDATE_METHOD update_method) { _update_method = update_method; }

    inline  virtual HRESULT predraw();
    virtual HRESULT         draw(bool redraw_all = false);
    inline  virtual void    update_data();

protected:
    DATA_POINTS             _points;

    COLORALPHA              _color_alpha;
	float                   _stroke_width;
	ID2D1PathGeometry*      _pathg;
	ID2D1GeometrySink*      _pSink;

    UPDATE_METHOD           _update_method;


    virtual HRESULT         move_bitmap_left();
    virtual HRESULT         draw_whole_line();
    virtual void            _draw_whole_line(eArtist* artist);
    virtual HRESULT         draw_new_point();
    virtual void            _draw_new_point(eArtist* artist);
    inline  virtual void    prepare_path();

    inline  void            clear_bmp_target();

private:
    void SetClass() { SetMyClass("CCurveGraph_summer"); }
};


} //namespace WARMGUI
#endif //__interface_chat_summer__