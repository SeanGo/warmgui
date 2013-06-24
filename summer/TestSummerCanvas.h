#ifndef __test_summer_canvas__
#define __test_summer_canvas__

class CTestSummerCanvas : public WARMGUI::ICanvas_summer
{
public:
    CTestSummerCanvas(void);
    CTestSummerCanvas(const char* name);
    ~CTestSummerCanvas(void);

    virtual HRESULT init();

protected:
    WARMGUI::CCurveGraph_summer* _curve_1;
    WARMGUI::CCurveGraph_summer* _curve_2;
    WARMGUI::CBlind_summer*      _blind;

private:
    void SetClass() { SetMyClass("CTestSummerCanvas"); }
};

#endif //__test_summer_canvas__