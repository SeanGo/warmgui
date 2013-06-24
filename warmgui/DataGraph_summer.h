#ifndef __interface_chat_summer__
#define __interface_chat_summer__

namespace WARMGUI {


///////////////////////////////////////////////////////////////////////////////
//class IDataGraph_summer
class WARMGUI_API IDataGraph_summer : public IGlyph_summer
{
public:
    inline                     IDataGraph_summer(void);
    inline                     IDataGraph_summer(const char* name, bool own_world, bool own_artist, bool own_data);
    inline                    ~IDataGraph_summer(void);

    void                       set_world(CWorld* world)
                               {if (_my_own_world) SafeDelete(_world); _my_own_world = false, _world = world;}
    bool                       my_own_world() { return _my_own_world; }

    virtual GLYPH_CHANGED_TYPE update(dataptr data) = 0;
    virtual HRESULT            predraw() { return S_OK; }
                               //reset glyph rect, and world's rect
    virtual void               set_rect(RECT& rect);


protected:
    CWorld*            _world;             ///the
    bool               _my_own_world;

    eArtist*           _my_artist;
    bool               _my_own_artist;
    CriticalLock*      _my_lockArtist;

    dataptr            _pdata;
    bool               _my_own_data;
    WORLD_CHANGED_TYPE _world_change;
};

///////////////////////////////////////////////////////////////////////////////
//class CCurveGraph_summer
class WARMGUI_API CCurveGraph_summer : public IDataGraph_summer
{
public:
    inline  CCurveGraph_summer(void);
    inline  CCurveGraph_summer(const char* name, bool own_world, bool own_artist, bool own_data);
    inline ~CCurveGraph_summer(void);

    void                       set_size(size_t bufsize) { _points.set_size(bufsize); }
    void                       reset() { _points.reset(); }
    virtual GLYPH_CHANGED_TYPE update(dataptr data);

	void                       SetLineColor(COLORREF clrBGR, float alpha = 1.0f)
		                       {_color = clrBGR, _alpha = alpha; }
	inline void                SetStrokeWidth(float stroke_width) {_stroke_width = stroke_width;}

                               //if add_to_point_set = true, reset the data-point set
    inline void                begin_set_data(float x, float y, bool add_to_point_set = true);
    inline void                add_data_to_path_geometry(float x, float y, bool add_to_point_set = true);
    inline void                end_set_data();

    virtual HRESULT            draw();

    enum UPDATE_METHOD {
        UPDATE_METHOD_INCREST,
        UPDATE_METHOD_RENEW,
    };

    void                       set_update_method(UPDATE_METHOD update_method) { _update_method = update_method; }
protected:
    DATA_POINTS        _points;

    COLORREF           _color;
	float              _alpha;
	float              _stroke_width;
	ID2D1PathGeometry* _pathg;
	ID2D1GeometrySink* _pSink;

    UPDATE_METHOD      _update_method;


    virtual HRESULT    move_bitmap_left();
    virtual HRESULT    draw_whole_line();
    virtual HRESULT    draw_new_point();
    inline virtual void        update_data();
};




///////////////////////////////////////////////////////////////////////////////
//class IChat_summer
class WARMGUI_API IChat_summer : public IGlyph_summer
{
public:
    inline IChat_summer(void);
    inline ~IChat_summer(void);


};

} //namespace WARMGUI
#endif //__interface_chat_summer__