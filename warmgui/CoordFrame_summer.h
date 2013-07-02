#ifndef __coord_frame_warmgui_summer__
#define __coord_frame_warmgui_summer__

namespace WARMGUI {

class WARMGUI_API CCoordFrame_summer : public IGlyph_summer
{
public:
    CCoordFrame_summer(const char* name);
    ~CCoordFrame_summer(void);

    virtual HRESULT            init();
    inline virtual void        set_rect(RECT& rect);

    RULER_WIDTH&               get_ruler_width() { return _ruler_width; }
    CWorld*                    get_world() { return _world; }
    RECT&                      get_graph_rect() { return _rect_graph; }
private:
    virtual void               setClass() { SetMyClass("CCoordFrame_summer"); }

	void                       DrawSideBar(eArtist* artist);
	void                       DrawGrid(eArtist* artist);
	void                       DrawRuler(eArtist* artist);
    void                       DrawRulerText(eArtist* artist, TCHAR * wszText, size_t len, float Y, float y);

private:
  	COLORALPHA            _clr_line;
	float                 _barWidth;
	RULER_WIDTH        _ruler_width;
    RECT                 _rect_calc;
    RECT                _rect_graph;
    FONT                      _font;
    IDWriteTextFormat* _pTextFormat;
    IDWriteTextLayout* _pTextLayout;
    float             _split_pieces;

    CWorld*                  _world;
    eArtist*             _my_artist;
    bool             _my_own_artist;

    virtual HRESULT            draw(bool redraw_all = false);
    HRESULT                    _draw_coord(eArtist* artist);
};

}

#endif //__coord_frame_warmgui_summer__