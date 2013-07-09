#ifndef __zit_predict_graph__
#define __zit_predict_graph__

class CZitPredictGraph : public WARMGUI::IDataGraph_summer
{
public:
    CZitPredictGraph(const char* name);
    ~CZitPredictGraph(void);

    virtual HRESULT init();
    virtual void    set_world(WARMGUI::CWorld*) {}
    virtual void    set_rect(RECT& rect)
                    {
                        _abs_rect = rect;
                        _rect.left = _rect.top = 0, _rect.right = RectWidth(rect), _rect.bottom = RectHeight(rect);
                    }
    void            set_pvi_world(WARMGUI::CWorld* price_world, WARMGUI::CWorld* volume_world, WARMGUI::CWorld* interest_world);
    void            set_pvi_rect(RECT& price_rect, RECT& volume_rect, RECT& interest_rect);
    virtual void               set_user_data(dataptr data) { _pdata = data; _days_result = (DAYS_CTPMMD_RESULT*)data;}
    inline  virtual void       update_data();

private:
    WARMGUI::CCurveGraph_summer* _approx_low_price;
    WARMGUI::CCurveGraph_summer* _approx_he_price;
    WARMGUI::CCurveGraph_summer* _approx_volume;
    WARMGUI::CCurveGraph_summer* _approx_interest;

    CInflexionLines*             _inflex_price;
    CInflexionLines*             _inflex_interest;

    CExtremumPoints*             _ext_points;
    DAYS_CTPMMD_RESULT*          _days_result;

    void SetClass() { SetMyClass("CZitPredictGraph"); }
};

#endif //__zit_predict_graph__