#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"


CDaysCtpmmdGraph::CDaysCtpmmdGraph(const char* name)
    : WARMGUI::IDataGraph_summer(name, false, false, false)
    , _approx_low_price(0) 
    , _approx_he_price(0)  
    , _approx_volume(0)    
    , _approx_interest(0)  
    , _inflex_price(0)     
    , _inflex_interest(0)
    , _days_result(0)
    , _ext_points(0)
{
    SetClass();
}


CDaysCtpmmdGraph::~CDaysCtpmmdGraph(void)
{
}


HRESULT CDaysCtpmmdGraph::init()
{
    _approx_low_price  = new WARMGUI::CCurveGraph_summer("approx-low-price", false, false, false); append_glyph(_approx_low_price ); _approx_low_price ->init();
    _approx_he_price   = new WARMGUI::CCurveGraph_summer("approx-he-price" , false, false, false); append_glyph(_approx_he_price  ); _approx_he_price  ->init();
    _approx_volume     = new WARMGUI::CCurveGraph_summer("approx-volume"   , false, false, false); append_glyph(_approx_volume    ); _approx_volume    ->init();
    _approx_interest   = new WARMGUI::CCurveGraph_summer("approx-interest" , false, false, false); append_glyph(_approx_interest  ); _approx_interest  ->init();

    _inflex_price      = new CInflexionLines("inflex-price"   ); append_glyph(_inflex_price     ); _inflex_price     ->init();
    _inflex_interest   = new CInflexionLines("inflex-interest"); append_glyph(_inflex_interest  ); _inflex_interest  ->init();

    _ext_points        = new CExtremumPoints("ext-price");
    append_glyph(_ext_points);
    _ext_points->init();

	_approx_low_price ->set_size(_config->getInt("memfile-setting.maxdata-size"));																											
	_approx_he_price  ->set_size(_config->getInt("memfile-setting.maxdata-size"));																											
	_approx_volume    ->set_size(_config->getInt("memfile-setting.maxdata-size"));																											
	_approx_interest  ->set_size(_config->getInt("memfile-setting.maxdata-size"));																											                 

    _approx_low_price ->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);
    _approx_he_price  ->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);
    _approx_volume    ->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);
    _approx_interest  ->set_update_method(WARMGUI::CCurveGraph_summer::UPDATE_METHOD_RENEW);

    return (0);
}

void CDaysCtpmmdGraph::set_pvi_world(WARMGUI::CWorld* price_world, WARMGUI::CWorld* volume_world, WARMGUI::CWorld* interest_world)
{
    _approx_low_price->set_world(price_world);
    _approx_he_price->set_world(price_world);
    _inflex_price->set_world(price_world);
    _ext_points->set_world(price_world);

    _approx_volume->set_world(volume_world);

    _approx_interest->set_world(interest_world);
    _inflex_interest->set_world(interest_world);
}

void CDaysCtpmmdGraph::set_pvi_rect(RECT& price_rect, RECT& volume_rect, RECT& interest_rect)
{
    _approx_low_price->set_rect(price_rect);
    _approx_he_price->set_rect(price_rect);
    _inflex_price->set_rect(price_rect);
    _ext_points->set_rect(price_rect);

    _approx_volume->set_rect(volume_rect);

    _approx_interest->set_rect(interest_rect);
    _inflex_interest->set_rect(interest_rect);
}

#define DAYSTS _days_result->_ts
#define DAYSAMPLER _days_result->_sampler
#define LOW_APPX_PRICE _days_result->_low_approx_price
#define INDEX_X        DAYSTS->_findex
#define HE_APPX_PRICE  _days_result->_he_approx_price
#define VOLUME_APPX    _days_result->_approx_volume
#define INTEREST_APPX  _days_result->_approx_interest

void CDaysCtpmmdGraph::update_data()
{
    {
        CriticalLock::Scoped scope_ext_price(*(_ext_points->get_change_lock()));
        _ext_points->set_extremum(_days_result->_ext_price);
    }

    {
        CriticalLock::Scoped scope_1(*(_approx_low_price->get_change_lock()));
        CriticalLock::Scoped scope_2(*(_approx_he_price->get_change_lock()));
        CriticalLock::Scoped scope_3(*(_approx_volume->get_change_lock()));
        CriticalLock::Scoped scope_4(*(_approx_interest->get_change_lock()));

        _approx_low_price->begin_set_data(*INDEX_X, *LOW_APPX_PRICE);
        _approx_he_price->begin_set_data(*INDEX_X, *HE_APPX_PRICE);
        _approx_volume->begin_set_data(*INDEX_X, *VOLUME_APPX);
        _approx_interest->begin_set_data(*INDEX_X, *INTEREST_APPX);

        for (int i = 1; i <= DAYSAMPLER->get_curpos(); i++) {
            _approx_low_price->add_data_to_path_geometry(INDEX_X[i], LOW_APPX_PRICE[i]);
            _approx_he_price->add_data_to_path_geometry(INDEX_X[i], HE_APPX_PRICE[i]);
            _approx_volume->add_data_to_path_geometry(INDEX_X[i], VOLUME_APPX[i]);
            _approx_interest->add_data_to_path_geometry(INDEX_X[i], INTEREST_APPX[i]);
        }

        _approx_interest->end_set_data();
        _approx_volume->end_set_data();
        _approx_he_price->end_set_data();
        _approx_low_price->end_set_data();

        _approx_he_price->predraw();
        _approx_low_price->predraw();
        _approx_volume->predraw();
        _approx_interest->predraw();
    }
}
