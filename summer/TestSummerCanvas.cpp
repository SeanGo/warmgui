#include "StdAfx.h"
#include "warmgui_summer.h"
#include "TestSummerCanvas.h"
#include "TestDispatcher_summer.h"
#include "SummerApp.h"


CTestSummerCanvas::CTestSummerCanvas(void)
    : _curve_1(0)
    , _curve_2(0)
    , _curve_3(0)
    , _curve_4(0)
    , _blind(0)
{
    SetClass();
}

CTestSummerCanvas::CTestSummerCanvas(const char* name)
    : ICanvas_summer(name)
    , _curve_1(0)
    , _curve_2(0)
    , _curve_3(0)
    , _curve_4(0)
    , _blind(0)
{
    SetClass();
}

CTestSummerCanvas::~CTestSummerCanvas(void)
{
}


void  CTestSummerCanvas::set_rect(RECT& rect)
{
    _abs_rect = rect;
    _rect.top = _rect.left = 0, _rect.right = RectWidth(rect), _rect.bottom = RectHeight(rect);

    RECT r;
    r.left = rect.left + 5, r.top = rect.top + 5, r.bottom = r.top + 300, r.right = r.left + 400;
    if (_curve_1) _curve_1->set_rect(r);

    r.left = r.right + 20, r.right = r.left + 400;
    if (_curve_2) _curve_2->set_rect(r);

    r.left = rect.left + 5, r.right = r.left + 400, r.top = r.bottom + 5, r.bottom = r.top + 300;
    if (_curve_3) _curve_3->set_rect(r);

    r.left = r.right + 20, r.right = r.left + 400;
    if (_curve_4) _curve_4->set_rect(r);
}

HRESULT CTestSummerCanvas::init()
{
    _blind = new WARMGUI::CBlind_summer("tsc-blind");
    append_glyph(_blind);

    //own world, no own artist, no own data
    _curve_1 = new WARMGUI::CCurveGraph_summer("dataline-curve1", true, false, false);   
    append_glyph(_curve_1);
    _curve_1->init();
    _curve_1->set_size(1024);
    _curve_1->SetLineColor(BGR(255, 0, 255), 1.0f);
    _curve_1->SetStrokeWidth(0.618f);
    _curve_1->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);

    //own world, own artist, no own data
    _curve_2 = new WARMGUI::CCurveGraph_summer("dataline-curve2", true, true, false);
    append_glyph(_curve_2);
    _curve_2->init();
    _curve_2->set_size(1024);
    _curve_2->SetLineColor(BGR(255, 0, 0), 1.0f);
    _curve_2->SetStrokeWidth(0.618f);
    _curve_2->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);

    _curve_3 = new WARMGUI::CCurveGraph_summer("dataline-curve3", true, true, false);   
    append_glyph(_curve_3);
    _curve_3->init();
    _curve_3->set_size(1024);
    _curve_3->SetLineColor(BGR(0, 255, 0), 1.0f);
    _curve_3->SetStrokeWidth(0.618f);
    _curve_3->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_INCREST);

    //own world, own artist, no own data
    _curve_4 = new WARMGUI::CCurveGraph_summer("dataline-curve4", true, true, false);
    append_glyph(_curve_4);
    _curve_4->init();
    _curve_4->set_size(1024);
    _curve_4->SetLineColor(BGR(0, 0, 255), 1.0f);
    _curve_4->SetStrokeWidth(0.309f);
    _curve_4->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_INCREST);

    /*
    generate_random();
    if (_curve_1) _curve_1->update(&pset1);
    if (_curve_2) _curve_2->update(&pset2);
    */

    the_app.get_test_dispatcher()->reg_data_graph(_curve_3);
    //the_app.get_test_dispatcher()->reg_data_graph(_curve_4);

    return S_OK;
}

void CTestSummerCanvas::draw_time_series()
{
    /*
    for (int i = 0; i < 100; i++) { //pset1.count; i++) {
        POINTF point;
        point.x = pset1.x[i], point.y = pset1.y[i];

        _curve_3->update(&point);
        point.x = pset2.x[i], point.y = pset2.y[i];
        _curve_4->update(&point);
        
        _atelier->redraw_window();

        Sleep(100);
    }
    */
    _atelier->redraw_window(true);
}

