#ifndef __test_summer_canvas__
#define __test_summer_canvas__

class CTestSummerCanvas : public WARMGUI::ICanvas_summer
{
public:
    CTestSummerCanvas(void);
    CTestSummerCanvas(const char* name);
    ~CTestSummerCanvas(void);

    virtual HRESULT init();
    void            draw_time_series();
    void            set_rect(RECT& rect);

protected:
    WARMGUI::CCurveGraph_summer* _curve_1;
    WARMGUI::CCurveGraph_summer* _curve_2;
    WARMGUI::CCurveGraph_summer* _curve_3;
    WARMGUI::CCurveGraph_summer* _curve_4;

    WARMGUI::CBlind_summer*      _blind;
    WARMGUI::CCoordFrame_summer* _coord_1;
    WARMGUI::CCoordFrame_summer* _coord_2;
private:
    void SetClass() { SetMyClass("CTestSummerCanvas"); }
};

#endif //__test_summer_canvas__