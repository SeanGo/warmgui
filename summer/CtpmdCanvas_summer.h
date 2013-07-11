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


    CDaysCtpmmdGraph*            _days_graph;

    WARMGUI::CBlind_summer*      _blind;
protected:
    TripleTuple _pvi_height;
    float       _pvi_width;
    MARGIN      _margin;
};

#endif //__ctpmdif_realtime_data_canvas__
