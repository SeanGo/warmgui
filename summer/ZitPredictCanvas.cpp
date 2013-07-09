#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"


CZitPredictCanvas::CZitPredictCanvas(const char* name)
    : ICanvas_summer(name)
    , _coord_price(0)      
    , _coord_volume(0)     
    , _coord_interest(0)   
    , _line_price(0)       
    , _line_volume(0)      
    , _line_rel_interest(0)
    , _line_dif_interest(0)
    , _pvi_width(0)
    , _days_graph(0)
{
}


CZitPredictCanvas::~CZitPredictCanvas(void)
{
}


HRESULT CZitPredictCanvas::init()
{
    _coord_price       = new WARMGUI::CCoordFrame_summer("coord-price"   );   append_glyph(_coord_price      ); _coord_price      ->init();
    _coord_volume      = new WARMGUI::CCoordFrame_summer("coord-volume"  );   append_glyph(_coord_volume     ); _coord_volume     ->init();
    _coord_interest    = new WARMGUI::CCoordFrame_summer("coord-interest");   append_glyph(_coord_interest   ); _coord_interest   ->init();

    _line_price        = new CCtpmdCurve_summer("line-price"       , false, true, false); append_glyph(_line_price       ); _line_price       ->init();
    _line_volume       = new CCtpmdCurve_summer("line-volume"      , false, true, false); append_glyph(_line_volume      ); _line_volume      ->init();
    _line_rel_interest = new CCtpmdCurve_summer("line-rel-interest", false, true, false); append_glyph(_line_rel_interest); _line_rel_interest->init();
    _line_dif_interest = new CCtpmdCurve_summer("line-dif-interest", true , true, false); append_glyph(_line_dif_interest); _line_dif_interest->init();


    _line_price       ->set_size(_config->getInt("memfile-setting.maxdata-size"));
    _line_volume      ->set_size(_config->getInt("memfile-setting.maxdata-size"));
    _line_rel_interest->set_size(_config->getInt("memfile-setting.maxdata-size"));
    _line_dif_interest->set_size(_config->getInt("memfile-setting.maxdata-size"));

    _line_price->set_world(_coord_price->get_world());
    _line_volume->set_world(_coord_volume->get_world());
    _line_rel_interest->set_world(_coord_interest->get_world());
    //!!! do not set this !!! _line_dif_interest->set_world(_coord_interest->get_world());

    char key[MAX_PATH];
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.pvi-width", _str_conf);
    _pvi_width = (float)_config->getDouble(key);
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.pvi-height", _str_conf);
    _config->getTripleFloat(_pvi_height, key);
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.margin", _str_conf);
    _config->getRect(_margin, key);


    CTPMMD mmd;
    _line_price       ->set_offset((char*)&(mmd.fIndex) - (char*)&mmd, (char*)&(mmd.LastPrice)   - (char*)&mmd);
    _line_volume      ->set_offset((char*)&(mmd.fIndex) - (char*)&mmd, (char*)&(mmd.relVolume)   - (char*)&mmd);
    _line_rel_interest->set_offset((char*)&(mmd.fIndex) - (char*)&mmd, (char*)&(mmd.relInterest) - (char*)&mmd);
    _line_dif_interest->set_offset((char*)&(mmd.fIndex) - (char*)&mmd, (char*)&(mmd.difInterest) - (char*)&mmd);

    the_app.get_dispatchers()->reg_graph(_line_price       , "ctp-disp");
    the_app.get_dispatchers()->reg_graph(_line_volume      , "ctp-disp");
    the_app.get_dispatchers()->reg_graph(_line_rel_interest, "ctp-disp");
    the_app.get_dispatchers()->reg_graph(_line_dif_interest, "ctp-disp");

    _days_graph = new CDaysCtpmmdGraph("days-graph");
    append_glyph(_days_graph);
    _days_graph->init();
    _days_graph->set_pvi_world(_coord_price->get_world(), _coord_volume->get_world(), _coord_interest->get_world());

    the_app.get_calculators()->reg_graph(_days_graph, "days-approx");
    return (0);
}

void CZitPredictCanvas::set_rect(RECT& rect)
{
    RECT crect = rect;
    crect.left += _margin.left, crect.top += _margin.top, crect.right -= _margin.right, crect.bottom -= _margin.bottom;
    _abs_rect = crect;
    _rect.left = _rect.top = 0, _rect.right = RectWidth(_abs_rect), _rect.bottom = RectHeight(_abs_rect);

    //_coord_price
    crect.bottom = crect.top + (int)(_rect.bottom * _pvi_height.f1);
    _coord_price->set_rect(crect);
    _line_price->set_rect(_coord_price->get_graph_rect());

    //_coord_volume
    crect.top = crect.bottom, crect.bottom = crect.top + (int)(_rect.bottom * _pvi_height.f2);
    _coord_volume->set_rect(crect);
    _line_volume->set_rect(_coord_volume->get_graph_rect());

    //_coord_interest
    crect.top = crect.bottom, crect.bottom = _abs_rect.bottom;
    _coord_interest->set_rect(crect);
    _line_rel_interest->set_rect(_coord_interest->get_graph_rect());
    _line_dif_interest->set_rect(_coord_interest->get_graph_rect());

    _days_graph->set_rect(rect);
    _days_graph->set_pvi_rect(_coord_price->get_graph_rect(), _coord_volume->get_graph_rect(), _coord_interest->get_graph_rect());
}
