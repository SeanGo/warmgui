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
    WARMGUI::CBlind_summer*      _blind;

    CZitPredictGraph*            _zit_prdt_graph;
protected:
    MARGIN      _margin;
};

#endif //__zit_predict_canvas__
