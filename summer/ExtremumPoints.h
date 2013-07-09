#ifndef __extremum_points__
#define __extremum_points__

class CExtremumPoints : public WARMGUI::IDataGraph_summer
{
public:
    CExtremumPoints(const char* name);
    ~CExtremumPoints(void);


    inline HRESULT draw(bool redraw = false);
    inline void    draw_point(float x, float y, char c, WARMGUI::CPoints_summer::POINT_TEXT_POSISION pos, COLORALPHA clr);
    inline HRESULT init();

    void           set_extremum(EXTRINFO* ext) { _ext = ext; }

protected:
    EXTRINFO* _ext;

    //WARMGUI::CPoints_summer _points;
  	COLORALPHA               _clr_1;
    COLORALPHA               _clr_2;
    FONT                      _font;
    IDWriteTextFormat* _pTextFormat;

private:
    void SetClass() { SetMyClass("CExtremumPoints"); }
};



#endif //__extremum_points__