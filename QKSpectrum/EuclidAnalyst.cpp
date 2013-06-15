#include "StdAfx.h"
#include "qks_incs.h"
#include "EuclidAnalyst.h"

extern qksapp the_app;

CEuclid::CEuclid(void)
    : _history_dc(0)
    , _config(0)
{
}


CEuclid::~CEuclid(void)
{
    SafeDelete(_config);
}


bool CEuclid::set_config()
{
    try{
        //get the analyst's config file
        SafeDelete(_config);
        _config = new CWarmguiConfig(the_app.GetConfig()->getString("euclid-analyst-params-file"));
        if (!_config) return false;

        {
            EUCLID::ANALYST_PARAM* ap = _eudlid.get_param();

            ap->_zp.len                          = _config->getInt("zit.len"               );
            ap->_zp.down_sample                  = _config->getInt("zit.down-sample"       );
   		    ap->_zp.wavelet_level                = _config->getInt("zit.wavelet-level"     );
		    ap->_zp.hilvl_filter_type            = _config->getInt("zit.hilvl-filter-type" );
		    ap->_zp.low_filter_level             = _config->getInt("zit.low-filter-level"  );
		    ap->_zp.hi_filter_level              = _config->getInt("zit.hi-filter-level"   );
            ap->_zp.extr_len                     = _config->getInt("zit.extr-len"          );
            ap->_zp.price_slope_scaling          = _config->getDouble("zit.price-slope-scaling");

            ap->_short_pp.appx_len               = _config->getInt("short-term.appx.len");
            ap->_short_pp.appx_down_sample       = _config->getInt("short-term.appx.down-sample");
            ap->_short_pp.appx_low_appx          = _config->getInt("short-term.appx.low-appx");
            ap->_short_pp.appx_high_appx         = _config->getInt("short-term.appx.high-appx");
            ap->_short_pp.prdt_feture_len        = _config->getInt("short-term.predict.feture-len");
            ap->_short_pp.prdt_coff_len          = _config->getInt("short-term.predict.coff-len");
            ap->_short_pp.extr_len               = _config->getInt("short-term.appx.extr-len"   );
            ap->_short_pp.wavelet                = (WAVELET_TYPE)_config->getWaveletType("short-term.appx.wavelet");
            ap->_short_pp.price_slope_scaling    = _config->getDouble("short-term.price-slope-scaling");

            ap->_long_pp.appx_len                = _config->getInt("long-term.appx.len");
            ap->_long_pp.appx_down_sample        = _config->getInt("long-term.appx.down-sample");
            ap->_long_pp.appx_low_appx           = _config->getInt("long-term.appx.low-appx");
            ap->_long_pp.appx_high_appx          = _config->getInt("long-term.appx.high-appx");
            ap->_long_pp.prdt_feture_len         = _config->getInt("long-term.predict.feture-len");
            ap->_long_pp.prdt_coff_len           = _config->getInt("long-term.predict.coff-len");
            ap->_long_pp.wavelet                 = (WAVELET_TYPE)_config->getWaveletType("long-term.appx.wavelet");
            ap->_long_pp.extr_len                = _config->getInt("long-term.appx.extr-len"   );
            ap->_long_pp.price_slope_scaling     = _config->getDouble("long-term.price-slope-scaling");

            ap->_volume_pp.min_len               = _config->getInt("volume.min-len");
            ap->_volume_pp.max_len               = _config->getInt("volume.max-len");
            ap->_volume_pp.level                 = _config->getInt("volume.level");
            ap->_volume_pp.wavelet               = (WAVELET_TYPE)_config->getWaveletType("volume.wavelet");
            ap->_volume_pp.extr_len              = _config->getInt("volume.extr-len"          );
            ap->_volume_pp.price_slope_scaling   = _config->getDouble("volume.price-slope-scaling");

            ap->_interest_pp.min_len             = _config->getInt("interest.min-len");
            ap->_interest_pp.max_len             = _config->getInt("interest.max-len");
            ap->_interest_pp.level               = _config->getInt("interest.level");
            ap->_interest_pp.wavelet             = (WAVELET_TYPE)_config->getWaveletType("interest.wavelet");
            ap->_interest_pp.extr_len            = _config->getInt("interest.extr-len"          );
            ap->_interest_pp.price_slope_scaling = _config->getDouble("interest.price-slope-scaling");
        }
    } catch(...) {
        return false;
    }

    return true;
}
