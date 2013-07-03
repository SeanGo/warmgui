#ifndef __ctpmdif_realtime_data_canvas__
#define __ctpmdif_realtime_data_canvas__

class CCtpmdCanvas_summer : public WARMGUI::ICanvas_summer
{
public:
    CCtpmdCanvas_summer(const char* name);
    ~CCtpmdCanvas_summer(void);

    virtual HRESULT init();
    virtual void    set_rect(RECT& rect);

protected:
    WARMGUI::CCoordFrame_summer* _coord_price;
    WARMGUI::CCoordFrame_summer* _coord_volume;
    WARMGUI::CCoordFrame_summer* _coord_interest;

    CCtpmdCurve_summer*          _line_price;
    CCtpmdCurve_summer*          _line_volume;
    CCtpmdCurve_summer*          _line_rel_interest;
    CCtpmdCurve_summer*          _line_dif_interest;

    WARMGUI::CCurveGraph_summer* _approx_low_price;
    WARMGUI::CCurveGraph_summer* _approx_he_price;
    WARMGUI::CCurveGraph_summer* _approx_volume;
    WARMGUI::CCurveGraph_summer* _approx_interest;

    CInflexionLines*             _inflex_price;
    CInflexionLines*             _inflex_interest;


protected:
    TripleTuple _pvi_height;
    float       _pvi_width;
    MARGIN      _margin;
};

#endif //__ctpmdif_realtime_data_canvas__
