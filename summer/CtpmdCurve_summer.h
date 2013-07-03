#ifndef __ctpmd_data_curve_summer__
#define __ctpmd_data_curve_summer__

class CCtpmdCurve_summer : public WARMGUI::CCurveGraph_summer
{
public:
    CCtpmdCurve_summer(const char* name, bool own_world, bool own_artist, bool own_data);
    ~CCtpmdCurve_summer(void);

    void set_offset(int x_offset, int y_offset) { _x_offset = x_offset, _y_offset = y_offset; }

    virtual GLYPH_CHANGED_TYPE update(dataptr data);

    virtual HRESULT            init();
    virtual void               set_rect(RECT& rect);
private:
    int _x_offset;
    int _y_offset;
};


#endif//__ctpmd_data_curve_summer__