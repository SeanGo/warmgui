#include "StdAfx.h"
#include "qks_incs.h"


CPVI_Canvas::CPVI_Canvas(const char* name)
    :  IDataCanvas(name)
    , _prc_chart(0)
    , _vol_chart(0)
    , _itr_chart(0)
    , _prc_graph_changed(GLYPH_CHANGED_TYPE_NONE)
    , _vol_graph_changed(GLYPH_CHANGED_TYPE_NONE)
    , _itr_graph_changed(GLYPH_CHANGED_TYPE_NONE)
    , _data_cont(0)
{
    setClass();
}

CPVI_Canvas::~CPVI_Canvas(void)
{
}

const HRESULT CPVI_Canvas::Init(const char* name/*=0*/)
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.blind", _strconf);
    _bHasBackground = _config->getBool(temp);

    IGlyph* g = 0;
    if (_bHasBackground) {
        _blind = new CBlind(_name, BGR(0, 0, 0), 0.5);
        g = AppendChild(_gt.begin(), _blind);
    }

    try {
        //_my_disp;
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, "%s.containers", this->_strconf);
        strcpy_s(_my_cont, MAX_PATH, _config->getString(temp).c_str());

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.rect-percentage", _strconf);
        _config->getTripleFloat(_tt_layout, temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.margin",          _strconf);
        _config->getRect(_margin, temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.chart-interval", _strconf);
        _chart_interval = _config->getInt(temp);
        _prc_chart = new CPriceTickChart("price-chart");
        _snprintf_s(temp, MAX_PATH, "%s.price-chart", _strconf);
        _prc_chart->setConfig(_config, temp);
        if (!g)
            g = AppendChild(_gt.begin(), _prc_chart);
        else
            g = InsertNext(g->GetGlyphTreeIter(), _prc_chart);
        _prc_chart->AddGraphs();

        _vol_chart = new CVolumeTickChart("volume-chart");
        _snprintf_s(temp, MAX_PATH, "%s.volume-chart", _strconf);
        _vol_chart->setConfig(_config, temp);
        g = InsertNext(g->GetGlyphTreeIter(), _vol_chart);
        _vol_chart->AddGraphs();

        _itr_chart = new CInterestTickChart("interest-chart");
        _snprintf_s(temp, MAX_PATH, "%s.interest-chart", _strconf);
        _itr_chart->setConfig(_config, temp);
        g = InsertNext(g->GetGlyphTreeIter(), _itr_chart);
        _itr_chart->AddGraphs();
    } catch(...) {
        return (-1);
    }

    return S_OK;
}


void CPVI_Canvas::SetGlyphRect()
{
	if (_blind)
		_blind->SetRect(_rect);
    RECT tmp = {0, 0, RectWidth(_rect), (long)(fRectHeight(_rect) * this->_tt_layout.f1)};
    //MYTRACE("CPVI_Canvas::price %d %d %d %d\n", tmp.left, tmp.top, tmp.right, tmp.bottom);
    if (_prc_chart) _prc_chart->SetRect(tmp);

    tmp.top = (long)(fRectHeight(_rect) * this->_tt_layout.f1), tmp.bottom = (long)(fRectHeight(_rect) * (_tt_layout.f1 + _tt_layout.f2));
    //MYTRACE("CPVI_Canvas::volume %d %d %d %d\n", tmp.left, tmp.top, tmp.right, tmp.bottom);
    if (_vol_chart) _vol_chart->SetRect(tmp);

    tmp.top = (long)(fRectHeight(_rect) * (_tt_layout.f1 + _tt_layout.f2)), tmp.bottom = RectHeight(_rect);
    //MYTRACE("CPVI_Canvas::interest %d %d %d %d\n", tmp.left, tmp.top, tmp.right, tmp.bottom);
    if (_itr_chart) _itr_chart->SetRect(tmp);

    if (_data_cont) {
        _prc_graph_changed = _vol_graph_changed = _itr_graph_changed = GLYPH_CHANGED_TYPE_COORDFRAME;
       NewDataForCtpmmd(_data_cont, DataObject::MARKET_DATA_TYPE_CTPMMD);
       _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CHANGED | (int)GLYPH_CHANGED_TYPE_COORDFRAME);
    }
}

GLYPH_CHANGED_TYPE CPVI_Canvas::NewData(DataObjectPtr dop)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    return changed;
}


GLYPH_CHANGED_TYPE CPVI_Canvas::NewData(dataptr data, size_t datalen, DataObject::MARKET_DATA_TYPE datatype)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    if (datatype == DataObject::MARKET_DATA_TYPE_ZITDATA) {
        ZitData* zit = (ZitData*)data;
#       ifdef _DEBUG
        MYTRACE(L"extremum %d, infl %d, central %d, area %d\n",
            zit->extCentral.nAllNum, 
            zit->inflBest.nNum,
            zit->central.nNum,
            zit->lha.nNum);
#      endif //_DEBUG
    }
    return changed;
}

void CPVI_Canvas::SetGeometryData(dataptr pdata, int count, int datasize)
{
}



GLYPH_CHANGED_TYPE CPVI_Canvas::NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype)
{
    //return GLYPH_CHANGED_TYPE_NONE ;
    _changed_type = _prc_graph_changed = _prc_graph_changed = _prc_graph_changed = GLYPH_CHANGED_TYPE_NONE;
    if (datatype == DataObject::MARKET_DATA_TYPE_CTPMMD) {
#       ifdef _DEBUG
        //if (!((((CCtpmmdContainer*)data_cont)->getCount()) % 300)) {
        //    const CTPMMD* pctpmmd = ((CCtpmmdContainer*)data_cont)->getCurrentData();
        //    MYTRACE(L"%d, %.02f\n", ((CCtpmmdContainer*)data_cont)->getCount(), pctpmmd->fIndex);
        //}
        //MYTRACE(L"pvi %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
        //    pctpmmd->fIndex,
        //    pctpmmd->m_time,
        //    pctpmmd->LastPrice,
        //    pctpmmd->Volume,
        //    pctpmmd->relVolume,
        //    pctpmmd->OpenInterest,
        //    pctpmmd->relInterest);
#       endif //_DEBUG
        _data_cont = (CCtpmmdContainer*)data_cont;
        bool b_first_data = (_data_cont->getCount() == 1);
        if ( !b_first_data
            &&
            (
                _data_cont->getCount()
                >
                (_prc_chart->getCoordWorld()->GetWorldRect().x0 + _prc_chart->getCoordWorld()->_vi._left_shirft)
            )
           )
        {
            //lu yi bian world rect
            _prc_chart->ResetWorldRect(_data_cont);
            _vol_chart->ResetWorldRect(_data_cont);
            _itr_chart->ResetWorldRect(_data_cont);
            _prc_graph_changed = _vol_graph_changed = _itr_graph_changed = GLYPH_CHANGED_TYPE_COORDFRAME;
        }

        _prc_graph_changed = (GLYPH_CHANGED_TYPE)((int)_prc_graph_changed | (int)(_prc_chart->NewCtpData((CCtpmmdContainer*)data_cont, (_prc_graph_changed), b_first_data)));
        _vol_graph_changed = (GLYPH_CHANGED_TYPE)((int)_vol_graph_changed | (int)(_vol_chart->NewCtpData((CCtpmmdContainer*)data_cont, (_vol_graph_changed), b_first_data)));
        _itr_graph_changed = (GLYPH_CHANGED_TYPE)((int)_itr_graph_changed | (int)(_itr_chart->NewCtpData((CCtpmmdContainer*)data_cont, (_itr_graph_changed), b_first_data)));

        _changed_type = (GLYPH_CHANGED_TYPE)((int)_prc_graph_changed | (int)_vol_graph_changed | (int)_itr_graph_changed);
        if (!b_first_data && ((int)_changed_type & (int)GLYPH_CHANGED_TYPE_COORDFRAME))
        {
            NewDataForCtpmmd((CCtpmmdContainer*)data_cont, datatype);
            _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CHANGED);
        }
    }
    //_atelier->Changed(_changed_type);
    return _changed_type;
}


GLYPH_CHANGED_TYPE CPVI_Canvas::NewDataForCtpmmd(CCtpmmdContainer* czc, DataObject::MARKET_DATA_TYPE datatype)
{
    //FUTURE_DATA_RANGE& fdr = czc->getDataRange();
    /*
    MYTRACE(L"PRICE_RANGE: %.02f %.02f %.02f %.02f\n", fdr.wr_price.minpos, fdr.wr_price.miny, fdr.wr_price.maxpos, fdr.wr_price.maxy);
    MYTRACE(L"VOLUM_RANGE: %.02f %.02f %.02f %.02f\n", fdr.wr_volume.minpos, fdr.wr_volume.miny, fdr.wr_volume.maxpos, fdr.wr_volume.maxy);
    MYTRACE(L"RELVO_RANGE: %.02f %.02f %.02f %.02f\n", fdr.wr_rel_volume.minpos, fdr.wr_rel_volume.miny, fdr.wr_rel_volume.maxpos, fdr.wr_rel_volume.maxy);
    MYTRACE(L"OPINT_RANGE: %.02f %.02f %.02f %.02f\n", fdr.wr_openinterest.minpos, fdr.wr_openinterest.miny, fdr.wr_openinterest.maxpos, fdr.wr_openinterest.maxy);
    MYTRACE(L"RELOI_RANGE: %.02f %.02f %.02f %.02f\n", fdr.wr_rel_interest.minpos, fdr.wr_rel_interest.miny, fdr.wr_rel_interest.maxpos, fdr.wr_rel_interest.maxy);
    */

    //MYTRACE(L"LU YI BIAN!\n");
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_CHANGED;
    {//begin set data
        if ((int)_prc_graph_changed & (int)GLYPH_CHANGED_TYPE_COORDFRAME)
            _prc_chart->BeginSetData((dataptr)(czc->getDataPtr()));

        if ((int)_vol_graph_changed & (int)GLYPH_CHANGED_TYPE_COORDFRAME)
            _vol_chart->BeginSetData((dataptr)(czc->getDataPtr()));

        if ((int)_itr_graph_changed & (int)GLYPH_CHANGED_TYPE_COORDFRAME)
            _itr_chart->BeginSetData((dataptr)(czc->getDataPtr()));
    }

    //set data
    size_t i = (int)_prc_chart->getCoordWorld()->GetWorldRect().x0;
    for (; i < czc->getCount() ; i++) {
        CTPMMD* ctpmmmd = (CTPMMD*)czc->getDataPtr();
        //the world of price was changed
#ifdef _DEBUG
        /*
        TCHAR name[MAX_PATH];
        CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
        MYTRACE(
            L"%s %.02f, %.02f, %.02f, %.02f, %.02f, %.02f\n",
            name,
            (ctpmmmd + i)->fIndex,
            (ctpmmmd + i)->LastPrice,
            (ctpmmmd + i)->Volume,
            (ctpmmmd + i)->relVolume,
            (ctpmmmd + i)->OpenInterest,
            (ctpmmmd + i)->relInterest);
        */
#endif //_DEBUG
        if ((int)_prc_graph_changed & (int)GLYPH_CHANGED_TYPE_COORDFRAME)
            _prc_chart->AddDataToPathGeometry(ctpmmmd + i);

        if ((int)_vol_graph_changed & (int)GLYPH_CHANGED_TYPE_COORDFRAME)
            _vol_chart->AddDataToPathGeometry(ctpmmmd + i);

        if ((int)_itr_graph_changed & (int)GLYPH_CHANGED_TYPE_COORDFRAME)
            _itr_chart->AddDataToPathGeometry(ctpmmmd + i);
    }

    {//end set data
        if ((int)_prc_graph_changed & (int)GLYPH_CHANGED_TYPE_COORDFRAME) {
            _prc_chart->EndSetData();
            _prc_chart->RenewGraph(GLYPH_CHANGED_TYPE_COORDFRAME);
        }

        if ((int)_vol_graph_changed & (int)GLYPH_CHANGED_TYPE_COORDFRAME) {
            _vol_chart->EndSetData();
            _vol_chart->RenewGraph(GLYPH_CHANGED_TYPE_COORDFRAME);
        }

        if ((int)_itr_graph_changed & (int)GLYPH_CHANGED_TYPE_COORDFRAME) {
            _itr_chart->EndSetData();
            _itr_chart->RenewGraph(GLYPH_CHANGED_TYPE_COORDFRAME);
        }
    }

    return changed;
}
