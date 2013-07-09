#ifndef __zit_predict_canvas__
#define __zit_predict_canvas__

class CZitPredictCanvas : public WARMGUI::ICanvas_summer
{
public:
    CZitPredictCanvas(const char* name);
    ~CZitPredictCanvas(void);

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
protected:
    TripleTuple _pvi_height;
    float       _pvi_width;
    MARGIN      _margin;
};

#endif //__zit_predict_canvas__
