#ifndef __zit_predict_graph__
#define __zit_predict_graph__

class CZitPredictGraph : public WARMGUI::IDataGraph_summer
{
public:
    CZitPredictGraph(const char* name);
    ~CZitPredictGraph(void);

    virtual HRESULT init();
    virtual void    set_world(WARMGUI::CWorld*) {}
    virtual void    set_rect(RECT& rect);
    void            set_pvi_world(WARMGUI::CWorld* price_world, WARMGUI::CWorld* volume_world, WARMGUI::CWorld* interest_world);
    void            set_pvi_rect(RECT& price_rect, RECT& volume_rect, RECT& interest_rect);
    virtual void               set_user_data(dataptr data) { _pdata = data; _zit_prd_result = (ZIT_PREDICT_RESULT*)data;}
    inline  virtual void       update_data();

private:
    WARMGUI::CCoordFrame_summer* _coord_price;
    WARMGUI::CCoordFrame_summer* _coord_volume;
    WARMGUI::CCoordFrame_summer* _coord_interest;

    WARMGUI::CCurveGraph_summer* _line_price;
    WARMGUI::CCurveGraph_summer* _line_volume;
    WARMGUI::CCurveGraph_summer* _line_rel_interest;
    //WARMGUI::CCurveGraph_summer* _line_dif_interest;

    WARMGUI::CCurveGraph_summer* _zit_approx;
    WARMGUI::CCurveGraph_summer* _zit_low_ma;
    WARMGUI::CCurveGraph_summer* _zit_he_ma;
    CInflexionLines*             _zit_inflex;
    CExtremumPoints*             _zit_ext;

    WARMGUI::CCurveGraph_summer* _approx_low_price;
    WARMGUI::CCurveGraph_summer* _approx_he_price;
    WARMGUI::CCurveGraph_summer* _low_prdct;
    WARMGUI::CCurveGraph_summer* _he_prdct;

    ZIT_PREDICT_RESULT*          _zit_prd_result;

    TripleTuple _pvi_height;
    float       _pvi_width;


    void SetClass() { SetMyClass("CZitPredictGraph"); }
};

#endif //__zit_predict_graph__