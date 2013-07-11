#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"

#define WINDOW_LEN     1024
#define PREDICT_LEN    180

CZitPredictGraph::CZitPredictGraph(const char* name)
    : WARMGUI::IDataGraph_summer(name, false, false, false)
    , _approx_low_price(0) 
    , _approx_he_price(0)  
    , _zit_inflex(0)     
    , _zit_prd_result(0)
    , _zit_ext(0)
    , _low_prdct(0)
    , _he_prdct(0)
    , _coord_price(0)      
    , _coord_volume(0)     
    , _coord_interest(0)   
    , _line_price(0)       
    , _line_volume(0)      
    , _line_rel_interest(0)
    //, //_line_dif_interest(0)
    , _pvi_width(0)
{
    SetClass();
}


CZitPredictGraph::~CZitPredictGraph(void)
{
}


HRESULT CZitPredictGraph::init()
{
    _coord_price       = new WARMGUI::CCoordFrame_summer("coord-price"   );   append_glyph(_coord_price      ); _coord_price      ->init();
    _coord_volume      = new WARMGUI::CCoordFrame_summer("coord-volume"  );   append_glyph(_coord_volume     ); _coord_volume     ->init();
    _coord_interest    = new WARMGUI::CCoordFrame_summer("coord-interest");   append_glyph(_coord_interest   ); _coord_interest   ->init();


    char key[MAX_PATH];
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.pvi-width", _str_conf);
    _pvi_width = (float)_config->getDouble(key);
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.pvi-height", _str_conf);
    _config->getTripleFloat(_pvi_height, key);

    _line_price        = new WARMGUI::CCurveGraph_summer("line-price"       , false, true, false); append_glyph(_line_price       ); _line_price       ->init();
    _line_volume       = new WARMGUI::CCurveGraph_summer("line-volume"      , false, true, false); append_glyph(_line_volume      ); _line_volume      ->init();
    _line_rel_interest = new WARMGUI::CCurveGraph_summer("line-rel-interest", false, true, false); append_glyph(_line_rel_interest); _line_rel_interest->init();
    //_line_dif_interest = new WARMGUI::CCurveGraph_summer("line-dif-interest", true , true, false); append_glyph(//_line_dif_interest); //_line_dif_interest->init();

    _line_price       ->set_size(WINDOW_LEN);
    _line_volume      ->set_size(WINDOW_LEN);
    _line_rel_interest->set_size(WINDOW_LEN);
    //_line_dif_interest->set_size(WINDOW_LEN);


    _approx_low_price  = new WARMGUI::CCurveGraph_summer("approx-low-price", false, false, false); append_glyph(_approx_low_price ); _approx_low_price ->init();
    _approx_he_price   = new WARMGUI::CCurveGraph_summer("approx-he-price" , false, false, false); append_glyph(_approx_he_price  ); _approx_he_price  ->init();
    _low_prdct  = new WARMGUI::CCurveGraph_summer("low-predict", false, false, false); append_glyph(_low_prdct); _low_prdct->init();
    _he_prdct   = new WARMGUI::CCurveGraph_summer("he-predict" , false, false, false); append_glyph(_he_prdct ); _he_prdct ->init();

    //_zit_inflex        = new CInflexionLines("zit-inflex"   ); append_glyph(_zit_inflex); _zit_inflex->init();
    //_zit_ext           = new CExtremumPoints("zit-ext");
    //append_glyph(_zit_ext);
    //_zit_ext->init();

	_approx_low_price ->set_size(WINDOW_LEN);
	_approx_he_price  ->set_size(WINDOW_LEN);
    _low_prdct->set_size(PREDICT_LEN);
    _he_prdct->set_size(PREDICT_LEN);

    _approx_low_price ->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);
    _approx_he_price  ->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);
    _low_prdct->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);
    _he_prdct->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);
    _line_price->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);
    _line_volume->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);
    _line_rel_interest->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);
    //_line_dif_interest->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);


    set_pvi_world(_coord_price->get_world(), _coord_volume->get_world(), _coord_interest->get_world());

    return (0);
}

void CZitPredictGraph::set_pvi_world(WARMGUI::CWorld* price_world, WARMGUI::CWorld* volume_world, WARMGUI::CWorld* interest_world)
{
    _line_price->set_world(price_world);
    _line_volume->set_world(volume_world);
    _line_rel_interest->set_world(interest_world);
    //!!! do not set this !!! //_line_dif_interest->set_world(_coord_interest->get_world());

    _approx_low_price->set_world(price_world);
    _approx_he_price->set_world(price_world);
    _low_prdct->set_world(price_world);
    _he_prdct->set_world(price_world);
    //_zit_inflex->set_world(price_world);
    //_zit_ext->set_world(price_world);
}

void CZitPredictGraph::set_rect(RECT& rect)
{
    _abs_rect = rect;
    _rect.left = _rect.top = 0, _rect.right = RectWidth(rect), _rect.bottom = RectHeight(rect);

    RECT crect = rect;
    //_coord_price
    crect.bottom = crect.top + (int)(_rect.bottom * _pvi_height.f1);
    _coord_price->set_rect(crect);

    //_coord_volume
    crect.top = crect.bottom, crect.bottom = crect.top + (int)(_rect.bottom * _pvi_height.f2);
    _coord_volume->set_rect(crect);

    //_coord_interest
    crect.top = crect.bottom, crect.bottom = _abs_rect.bottom;
    _coord_interest->set_rect(crect);
    
    set_pvi_rect(_coord_price->get_graph_rect(), _coord_volume->get_graph_rect(), _coord_interest->get_graph_rect());
}

void CZitPredictGraph::set_pvi_rect(RECT& price_rect, RECT& volume_rect, RECT& interest_rect)
{
    _approx_low_price->set_rect(price_rect);
    _approx_he_price->set_rect(price_rect);
    _low_prdct->set_rect(price_rect);
    _he_prdct->set_rect(price_rect);
    //_zit_inflex->set_rect(price_rect);
    //_zit_ext->set_rect(price_rect);

    _line_price->set_rect(price_rect);
    _line_volume->set_rect(volume_rect);
    _line_rel_interest->set_rect(interest_rect);
    //_line_dif_interest->set_rect(interest_rect);
}

#define DAYSTS _zit_prd_result->_ts
#define DAYSAMPLER _zit_prd_result->_sampler
#define LOW_APPX_PRICE _zit_prd_result->_low_approx_price
#define INDEX_X        DAYSTS->_findex
#define PRICE          DAYSTS->_fprice
#define VOLUME         DAYSTS->_f_rel_volume
#define REL_INTEREST   DAYSTS->_f_rel_interest
#define HE_APPX_PRICE  _zit_prd_result->_he_approx_price
#define VOLUME_APPX    _zit_prd_result->_approx_volume
#define INTEREST_APPX  _zit_prd_result->_approx_interest
#define LOW_PREDICT    _zit_prd_result->_prdct_low
#define HE_PREDICT     _zit_prd_result->_prdct_he


void CZitPredictGraph::update_data()
{
    {
    //    CriticalLock::Scoped scope_ext_price(*(_zit_ext->get_change_lock()));
    //    _zit_ext->set_extremum(_zit_prd_result->_ext_zit);
    }

    {
        _coord_price->get_world()->zero_world();
        _coord_volume->get_world()->zero_world();
        _coord_interest->get_world()->zero_world();
    }

    {
        CriticalLock::Scoped scope_1(*(_approx_low_price->get_change_lock()));
        CriticalLock::Scoped scope_2(*(_approx_he_price->get_change_lock()));
        CriticalLock::Scoped scope_3(*(_line_price->get_change_lock()));
        CriticalLock::Scoped scope_4(*(_line_volume->get_change_lock()));
        CriticalLock::Scoped scope_5(*(_line_rel_interest->get_change_lock()));
        //CriticalLock::Scoped scope_6(*(//_line_dif_interest->get_change_lock()));
        CriticalLock::Scoped scope_7(*(_low_prdct->get_change_lock()));
        CriticalLock::Scoped scope_8(*(_he_prdct->get_change_lock()));

        _line_price->begin_set_data(*INDEX_X, *PRICE);
        _line_volume->begin_set_data(*INDEX_X, *VOLUME);
        _line_rel_interest->begin_set_data(*INDEX_X, *REL_INTEREST);
        _approx_low_price->begin_set_data(*INDEX_X, *LOW_APPX_PRICE);
        _approx_he_price->begin_set_data(*INDEX_X, *HE_APPX_PRICE);
        //_line_dif_interest->begin_set_data(*INDEX_X, DAYSTS->_f_);
        double x = INDEX_X[DAYSAMPLER->get_curpos()] + 1.0f;
        _low_prdct->begin_set_data(x, *LOW_PREDICT, true, false);
        _he_prdct->begin_set_data(x, *HE_PREDICT, true, false);


        for (int i = 1; i < PREDICT_LEN; i++, x += 1.0) {
            _line_price->add_data_to_path_geometry(INDEX_X[i], PRICE[i]);
            _line_volume->add_data_to_path_geometry(INDEX_X[i], VOLUME[i]);
            _line_rel_interest->add_data_to_path_geometry(INDEX_X[i], REL_INTEREST[i]);
            _approx_low_price->add_data_to_path_geometry(INDEX_X[i], LOW_APPX_PRICE[i]);
            _approx_he_price->add_data_to_path_geometry(INDEX_X[i], HE_APPX_PRICE[i]);

            _low_prdct->add_data_to_path_geometry(x, LOW_PREDICT[i], true, false);
            _he_prdct->add_data_to_path_geometry(x, HE_PREDICT[i], true, false);
        }

        _low_prdct->end_set_data();
        _he_prdct->end_set_data();
        _low_prdct->predraw();
        _he_prdct->predraw();

        for (int i = PREDICT_LEN; i <= DAYSAMPLER->get_curpos(); i++) {
            _line_price->add_data_to_path_geometry(INDEX_X[i], PRICE[i]);
            _line_volume->add_data_to_path_geometry(INDEX_X[i], VOLUME[i]);
            _line_rel_interest->add_data_to_path_geometry(INDEX_X[i], REL_INTEREST[i]);
            _approx_low_price->add_data_to_path_geometry(INDEX_X[i], LOW_APPX_PRICE[i]);
            _approx_he_price->add_data_to_path_geometry(INDEX_X[i], HE_APPX_PRICE[i]);
        }

        _line_price->end_set_data();
        _line_volume->end_set_data();
        _line_rel_interest->end_set_data();
        _approx_he_price->end_set_data();
        _approx_low_price->end_set_data();

        _line_price->predraw();
        _line_volume->predraw();
        _line_rel_interest->predraw();
        _approx_he_price->predraw();
        _approx_low_price->predraw();
    }

    /*
    MYTRACE(L"CZitPredictGraph::update_data() %d\n", DAYSAMPLER->get_curpos());
    if (HE_APPX_PRICE && LOW_APPX_PRICE && INDEX_X)
    {
        for (int i = 0; i <= DAYSAMPLER->get_curpos(); i += 100) {
            MYTRACE(L"%.02f, %.02f -> %.02f %.02f\n", INDEX_X[i], DAYSTS->_fprice[i], LOW_APPX_PRICE[i], HE_APPX_PRICE[i]);
        }
        MYTRACE(
            L"%.02f, %.02f -> %.02f %.02f\n",
            INDEX_X[DAYSAMPLER->get_curpos()],
            DAYSTS->_fprice[DAYSAMPLER->get_curpos()],
            LOW_APPX_PRICE[DAYSAMPLER->get_curpos()],
            HE_APPX_PRICE[DAYSAMPLER->get_curpos()]);
    }
    */

    /*
    MYTRACE(L"x0-xn %.02f - %.02f, y %.02f - %.02f, index %.02f - %.02f - %.02f\n",
        _coord_price->get_world()->_real_world.x0,
        _coord_price->get_world()->_real_world.xn,
        _coord_price->get_world()->_real_world.miny,
        _coord_price->get_world()->_real_world.maxy,
        *INDEX_X,
        INDEX_X[DAYSAMPLER->get_curpos()],
        INDEX_X[DAYSAMPLER->get_curpos()] + PREDICT_LEN);
    */
}
