#include "StdAfx.h"
#include "warmgui_summer.h"
#include "TestSummerCanvas.h"


CTestSummerCanvas::CTestSummerCanvas(void)
    : _curve_1(0)
    , _curve_2(0)
    , _blind(0)
{
    SetClass();
}

CTestSummerCanvas::CTestSummerCanvas(const char* name)
    : ICanvas_summer(name)
    , _curve_1(0)
    , _curve_2(0)
    , _blind(0)
{
    SetClass();
}

CTestSummerCanvas::~CTestSummerCanvas(void)
{
}

HRESULT CTestSummerCanvas::init()
{
    _blind = new WARMGUI::CBlind_summer("tsc-blind");
    append_glyph(_blind);

    _curve_1 = new WARMGUI::CCurveGraph_summer("tsc-curve", true, false, false);
    append_glyph(_curve_1);

    _curve_2 = new WARMGUI::CCurveGraph_summer("tsc-curve", true, true, false);
    append_glyph(_curve_2);

    return S_OK;
}
