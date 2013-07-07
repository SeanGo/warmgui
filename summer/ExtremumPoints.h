#ifndef __extremum_points__
#define __extremum_points__

class CExtremumPoints : public WARMGUI::IDataGraph_summer
{
public:
    CExtremumPoints(const char* name);
    ~CExtremumPoints(void);


    inline HRESULT draw(bool redraw = false);
    inline HRESULT init();

    virtual void   set_world(WARMGUI::CWorld* world)
                   {
                       if (!_my_own_world) _world = world;
                       _points.set_world(world);
                   }
    void set_extremum(EXTRINFO* ext) { _ext = ext; }

protected:
    EXTRINFO* _ext;

    WARMGUI::CPoints_summer _points;
  	COLORALPHA               _clr_1;
    COLORALPHA               _clr_2;
    FONT                      _font;
    IDWriteTextFormat* _pTextFormat;
};


#endif //__extremum_points__