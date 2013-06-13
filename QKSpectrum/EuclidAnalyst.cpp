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

            ap->zit_len                = _config->getInt("zit.len");
            ap->zit_down_sample        = _config->getInt("zit.down-sample");
            ap->short_appx_len         = _config->getInt("short-term.appx.len");
            ap->short_appx_down_sample = _config->getInt("short-term.appx.down-sample");
            ap->short_appx_low_appx    = _config->getInt("short-term.appx.low-appx");
            ap->short_appx_high_appx   = _config->getInt("short-term.appx.high-appx");
            ap->short_prdt_feture_len  = _config->getInt("short-term.predict.feture-len");
            ap->short_prdt_coff_len    = _config->getInt("short-term.predict.coff-len");
            ap->short_wavelet          = (WAVELET_TYPE)_config->getWaveletType("short-term.appx.wavelet");

            ap->long_appx_len          = _config->getInt("long-term.appx.len");
            ap->long_appx_down_sample  = _config->getInt("long-term.appx.down-sample");
            ap->long_appx_low_appx     = _config->getInt("long-term.appx.low-appx");
            ap->long_appx_high_appx    = _config->getInt("long-term.appx.high-appx");
            ap->long_prdt_feture_len   = _config->getInt("long-term.predict.feture-len");
            ap->long_prdt_coff_len     = _config->getInt("long-term.predict.coff-len");
            ap->long_wavelet           = (WAVELET_TYPE)_config->getWaveletType("long-term.appx.wavelet");

            ap->volume_min_len         = _config->getInt("volume.min-len");
            ap->volume_max_len         = _config->getInt("volume.max-len");
            ap->volume_level           = _config->getInt("volume.level");
            ap->volume_wavelet         = (WAVELET_TYPE)_config->getWaveletType("volume.wavelet");

            ap->interest_min_len       = _config->getInt("interest.min-len");
            ap->interest_max_len       = _config->getInt("interest.max-len");
            ap->interest_level         = _config->getInt("interest.level");
            ap->interest_wavelet       = (WAVELET_TYPE)_config->getWaveletType("interest.wavelet");
        }
    } catch(...) {
        return false;
    }

    return true;
}
