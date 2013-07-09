#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"


CZitPredictCalculator::CZitPredictCalculator(const char * name)
    : ICalculator_summer(name)
    , _ext_zit(1)
{
    _zpr._ts = (CTPMMD_TIME_SERIES*)sampler.get_time_series();
    _zpr._sampler = &sampler;
    _result_data = &_zpr;
}


CZitPredictCalculator::~CZitPredictCalculator(void)
{
}

bool CZitPredictCalculator::init()
{
    sampler.set_length(1024, 1024, 1);
    _wvlt_low_price.SetWavelet(WAVELET_TYPE_SYM, 7);
    _wvlt_he_price.SetWavelet(WAVELET_TYPE_SYM, 9);
    _prdct_low.set_data_length(1024, 180);
    _prdct_he.set_data_length(1024, 180);

    _wvlt_zit_approx.SetWavelet(WAVELET_TYPE_SYM, 1);
    _low_ma_filter.SetLevel(5);
    _he_ma_filter.SetLevel(10);
    _ext_zit.SetSlopeScale(PRICE_SLOPE_SCALING);
    _inf_zit.set_maxinf_by_maxdata(32400);

    return (true);
}

int  CZitPredictCalculator::calculate(MSG msg)
{
    CTPMMD* mmd = (CTPMMD*)msg.wParam;
    _zpr.zero();

    //MYTRACE(L"cal rcvd %.02f %.02f %.02f\n", mmd->fIndex, mmd->LastPrice, mmd->m_time);
    if (sampler.fill_data(mmd)) {
        int datalen = sampler.get_curpos() + 1;
        //MYTRACE(L"i will cal %d length data\n", datalen);
        if (datalen > 1024) {
            MYTRACE(L"%d length data error\n", datalen);
            return (0);
        }


        ////////////////////////////////////////////////////////////////
        // for price, low level 
        _wvlt_low_price.SetSignal(
            sampler.get_time_series()->_fprice,
            datalen);
        _zpr._low_approx_price = _wvlt_low_price.DoFilter();
        if (!_zpr._low_approx_price) MYTRACE(L"error _wvlt_low_price\n");

        // he level for price
        _wvlt_he_price.SetSignal(sampler.get_time_series()->_fprice, datalen);
        _zpr._he_approx_price = _wvlt_he_price.DoFilter();
        if (!_zpr._he_approx_price) MYTRACE(L"error _wvlt_he_price\n");

        //predict
        if (_zpr._low_approx_price) {
            _prdct_low.SetData(_zpr._low_approx_price, datalen);
            _zpr._prdct_low = _prdct_low.Predict();
            if (!_zpr._prdct_low) MYTRACE(L"error _prdct_low\n");
        }

        if (_zpr._he_approx_price) {
            _prdct_he.SetData(_zpr._he_approx_price, datalen);
            _zpr._prdct_he = _prdct_he.Predict();
            if (!_zpr._prdct_he) MYTRACE(L"error _prdct_he\n");
        }

        ///////////////////////////////////////////////////////////////////////
        // for zit

        //wvlt approx
        _wvlt_zit_approx.SetSignal(
            sampler.get_time_series()->_fprice,
            datalen);
        _zpr._wvlt_zit_approx = _wvlt_zit_approx.DoFilter();
        if (!_zpr._wvlt_zit_approx) MYTRACE(L"error _wvlt_zit_approx\n");
        
        //low ma
        _low_ma_filter.SetSignal(
            sampler.get_time_series()->_fprice,
            datalen);
        _zpr._low_ma_filter = _low_ma_filter.DoFilter();
        if (!_zpr._low_ma_filter)  MYTRACE(L"error _low_ma_filter\n");

        //he ma
        _he_ma_filter.SetSignal(
            sampler.get_time_series()->_fprice,
            datalen);
        _zpr._he_ma_filter = _he_ma_filter.DoFilter();
        if (!_zpr._he_ma_filter)  MYTRACE(L"error _he_ma_filter\n");

        //extrumes
        if (_zpr._wvlt_zit_approx) {
            _ext_zit.SetData(
                _zpr._wvlt_zit_approx,
                sampler.get_time_series()->_fprice,
                sampler.get_time_series()->_ftime,
                sampler.get_time_series()->_findex,
                datalen);
            _zpr._ext_zit =_ext_zit.CalExtremum();
            if (_zpr._ext_zit && _zpr._ext_zit->nAllNum) {
                //MYTRACE(L"price ext -- %d : %d, %.02f %.02f\n",
                //    _zpr._ext_zit->nAllNum,
                //    _zpr._ext_zit->extremum->nIndex,
                //    _zpr._ext_zit->extremum->fValue,
                //    _zpr._ext_zit->extremum->fOrngValue);

                //inflexions
                _inf_zit.SetData(
                    sampler.get_time_series()->_fprice,
                    _zpr._wvlt_zit_approx,
                    sampler.get_time_series()->_ftime,
                    sampler.get_time_series()->_findex,
                    datalen,
                    &_ext_zit);

                _zpr._inf_zit = _inf_zit.GetInflexion();

                if (_zpr._inf_zit && _zpr._inf_zit->m_igStb.nNum) {
                    _central.SetData(
                        sampler.get_time_series()->_fprice,
                        _zpr._wvlt_zit_approx,
                        _zpr._low_ma_filter,
                        _zpr._he_ma_filter,
                        sampler.get_time_series()->_ftime,
                        &_zpr._inf_zit->m_igStb,
                        _zpr._ext_zit,
                        datalen);
                    _zpr._centrals = _central.GetCentral(datalen - 1);
                    _zpr._areas    = _central.GetCentralAreaPtr();
                    if (!_zpr._centrals || !_zpr._centrals->nNum)
                        MYTRACE(L"error central\n");
                    //else
                    //    MYTRACE(L"central num %d areas %d\n", _zpr._centrals->nNum, _zpr._areas->nNum);
                } else {
                    MYTRACE(L"error _inf_zit\n");
                }
            } else {
                MYTRACE(L"error _ext_zit\n");
            }
        } else {
            MYTRACE(L"error _wvlt_zit_approx\n");
        }

        //if (_zpr._infl_interest && _zpr._infl_interest->m_igStb.nNum)
        //    MYTRACE(L"interest infl -- %d\n", _zpr._infl_interest->m_igStb.nNum);

        return datalen;
    }
    return (0);
}

