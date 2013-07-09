#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"


CDaysCtpmmdCalculator::CDaysCtpmmdCalculator(const char * name)
    : ICalculator_summer(name)
{
    _dcr._ts = (CTPMMD_TIME_SERIES*)sampler.get_time_series();
    _dcr._sampler = &sampler;
    _result_data = &_dcr;
}


CDaysCtpmmdCalculator::~CDaysCtpmmdCalculator(void)
{
}

bool CDaysCtpmmdCalculator::init()
{
    sampler.set_length(32400, 256, 1);
    _wvlt_low_price.SetWavelet(WAVELET_TYPE_SYM, 7);
    _wvlt_he_price.SetWavelet(WAVELET_TYPE_SYM, 9);
    _wvlt_interest.SetWavelet(WAVELET_TYPE_SYM, 6);
    _wvlt_volume.SetWavelet(WAVELET_TYPE_COI, 6);

    _ext_low_price.SetSlopeScale(PRICE_SLOPE_SCALING);
    _ext_volume.SetSlopeScale(PRICE_SLOPE_SCALING);
    _ext_interest.SetSlopeScale(PRICE_SLOPE_SCALING);

    return (true);
}

int  CDaysCtpmmdCalculator::calculate(MSG msg)
{
    CTPMMD* mmd = (CTPMMD*)msg.wParam;
    _dcr._low_approx_price = (0),
    _dcr._he_approx_price  = (0),
    _dcr._approx_volume    = (0),
    _dcr._approx_interest  = (0),
    _dcr._ext_price        = (0),
    _dcr._ext_volume       = (0),
    _dcr._ext_interest     = (0),
    _dcr._infl_price       = (0),
    _dcr._infl_interest    = (0);

    //MYTRACE(L"cal rcvd %.02f %.02f %.02f\n", mmd->fIndex, mmd->LastPrice, mmd->m_time);
    if (sampler.fill_data(mmd)) {
        int datalen = sampler.get_curpos() + 1;
        //MYTRACE(L"i will cal %d length data\n", datalen);


        ////////////////////////////////////////////////////////////////
        // for price, low level 
        _wvlt_low_price.SetSignal(
            sampler.get_time_series()->_fprice,
            datalen);
        _dcr._low_approx_price = _wvlt_low_price.DoFilter();
        if (!_dcr._low_approx_price) MYTRACE(L"error _wvlt_low_price\n");

        _ext_low_price.SetData(
            _dcr._low_approx_price,
            sampler.get_time_series()->_fprice,
            sampler.get_time_series()->_ftime,
            sampler.get_time_series()->_findex,
            datalen);

        _dcr._ext_price =_ext_low_price.CalExtremum();
        if (_dcr._ext_price && _dcr._ext_price->nAllNum) {
            //MYTRACE(L"price ext -- %d : %d, %.02f %.02f\n",
            //    _dcr._ext_price->nAllNum,
            //    _dcr._ext_price->extremum->nIndex,
            //    _dcr._ext_price->extremum->fValue,
            //    _dcr._ext_price->extremum->fOrngValue);

            _inf_low_price.SetData(
                sampler.get_time_series()->_fprice,
                _dcr._low_approx_price,
                sampler.get_time_series()->_ftime,
                sampler.get_time_series()->_findex,
                datalen,
                &_ext_low_price);

            _dcr._infl_price = _inf_low_price.GetInflexion();
        }

        if (!_dcr._infl_price) MYTRACE(L"price infl -- error\n");

        //////////////////////////////////////////////////////////////////////////
        // he level for price
        _wvlt_he_price.SetSignal(sampler.get_time_series()->_fprice, datalen);
        _dcr._he_approx_price = _wvlt_he_price.DoFilter();
        if (!_dcr._he_approx_price) MYTRACE(L"error _wvlt_he_price\n");


        //////////////////////////////////////////////////////////////////////////
        // volume
        _wvlt_volume.SetSignal(sampler.get_time_series()->_f_rel_volume, datalen);
        _dcr._approx_volume = _wvlt_volume.DoFilter();
        if (!_dcr._approx_volume) MYTRACE(L"error _wvlt_volume\n");
        _ext_volume.SetData(
            _dcr._approx_volume,
            sampler.get_time_series()->_f_rel_volume,
            sampler.get_time_series()->_ftime,
            sampler.get_time_series()->_findex,
            datalen);
        _dcr._ext_volume = _ext_volume.CalExtremum();
        //if (_dcr._ext_volume && _dcr._ext_volume->nAllNum)
        //    MYTRACE(L"volume ext %d\n", _dcr._ext_volume->nAllNum);


        ////////////////////////////////////////////////////////////////////////////
        // interest
        _wvlt_interest.SetSignal(sampler.get_time_series()->_f_rel_interest, datalen);
        _dcr._approx_interest = _wvlt_interest.DoFilter();
        if (!_dcr._approx_interest) MYTRACE(L"error _wvlt_interest\n");
        _ext_interest.SetData(
            _dcr._approx_interest,
            sampler.get_time_series()->_f_rel_interest,
            sampler.get_time_series()->_ftime,
            sampler.get_time_series()->_findex,
            datalen);
        _dcr._ext_interest = _ext_interest.CalExtremum();
        if (_dcr._ext_interest && _dcr._ext_interest->nAllNum) {
            //MYTRACE(L"interest ext %d\n", _dcr._ext_interest->nAllNum);
            _inf_interest.SetData(
                sampler.get_time_series()->_f_rel_interest,
                _dcr._approx_interest,
                sampler.get_time_series()->_ftime,
                sampler.get_time_series()->_findex,
                datalen,
                &_ext_interest);

            //_dcr._infl_interest = _inf_interest.GetInflexion();
        }
        //if (_dcr._infl_interest && _dcr._infl_interest->m_igStb.nNum)
        //    MYTRACE(L"interest infl -- %d\n", _dcr._infl_interest->m_igStb.nNum);

        return datalen;
    }
    return (0);
}

