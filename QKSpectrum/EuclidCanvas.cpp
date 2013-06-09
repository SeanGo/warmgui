#include "StdAfx.h"
#include "qks_incs.h"


CEuclidCanvas::CEuclidCanvas(const char* name)
    : IDataCanvas(name)
    , _bHasBackground(true)
    , _blind(0)
    , _prc_chart    (0)
    , _approx_short_lo (0)
    , _approx_short_he (0)
    , _approx_long_lo  (0)
    , _approx_long_he  (0)
    , _predict_short_lo(0)
    , _predict_short_he(0)
    , _predict_long_lo (0)
    , _predict_long_he (0)
    , _data_cont(0)
{
    setClass();
}


CEuclidCanvas::~CEuclidCanvas(void)
{
}

const HRESULT CEuclidCanvas::Init(const char* name/* = 0*/)
{
    IGlyph* g = 0;
    if (_bHasBackground) {
        _blind = new CBlind(_name, BGR(0, 0, 0), 0.8);
        g = AppendChild(_gt.begin(), _blind);
    }

    try {
        //_my_disp;
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, "%s.containers", this->_strconf);
        strcpy_s(_my_cont, MAX_PATH, _config->getString(temp).c_str());

        _prc_chart = new CPriceTickChart("price-chart");
        _snprintf_s(temp, MAX_PATH, "%s.price-chart", _strconf);
        _prc_chart->setConfig(_config, temp);
        if (!g)
            g = AppendChild(_gt.begin(), _prc_chart);
        else
            g = InsertNext(g->GetGlyphTreeIter(), _prc_chart);
        _prc_chart->AddGraphs();

        _approx_short_lo = new CDataLineGraph("rt-appx_short_lo", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, true, false);
        _approx_short_lo->setWorld(_prc_chart->getCoordWorld());
        _approx_short_lo->SetLineColor(BGR(255, 0, 0), 1.0f);
        _approx_short_lo->SetStrokeWidth(.02f);
        _approx_short_lo->GetGraphData()->SetSize(1024);
        g = InsertNext(g->GetGlyphTreeIter(), _approx_short_lo);

        _approx_short_he = new CDataLineGraph("rt-appx_short_he", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, true, false);
        _approx_short_he->setWorld(_prc_chart->getCoordWorld());
        _approx_short_he->SetLineColor(BGR(0, 255, 0), 1.0f);
        _approx_short_he->SetStrokeWidth(.02f);
        _approx_short_he->GetGraphData()->SetSize(1024);
        g = InsertNext(g->GetGlyphTreeIter(), _approx_short_he);

        _approx_long_lo = new CDataLineGraph("rt-appx_long_lo", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, true, false);
        _approx_long_lo->setWorld(_prc_chart->getCoordWorld());
        _approx_long_lo->SetLineColor(BGR(255, 0, 0), 1.0f);
        _approx_long_lo->SetStrokeWidth(.1f);
        _approx_long_lo->GetGraphData()->SetSize(1024);
        g = InsertNext(g->GetGlyphTreeIter(), _approx_long_lo);

        _approx_long_he = new CDataLineGraph("rt-appx_long_he", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, true, false);
        _approx_long_he->setWorld(_prc_chart->getCoordWorld());
        _approx_long_he->SetLineColor(BGR(0, 255, 0), 1.0f);
        _approx_long_he->SetStrokeWidth(.1f);
        _approx_long_he->GetGraphData()->SetSize(1024);
        g = InsertNext(g->GetGlyphTreeIter(), _approx_long_he);

        _predict_short_lo = new CDataLineGraph("rt-prdt_short_lo", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, true, false);
        _predict_short_lo->setWorld(_prc_chart->getCoordWorld());
        _predict_short_lo->SetLineColor(BGR(255, 0, 0), 1.0f);
        _predict_short_lo->SetStrokeWidth(.2f);
        _predict_short_lo->GetGraphData()->SetSize(180);
        g = InsertNext(g->GetGlyphTreeIter(), _predict_short_lo);

        _predict_short_he = new CDataLineGraph("rt-prdt_short_he", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, true, false);
        _predict_short_he->setWorld(_prc_chart->getCoordWorld());
        _predict_short_he->SetLineColor(BGR(0, 255, 0), 1.0f);
        _predict_short_he->SetStrokeWidth(.2f);
        _predict_short_he->GetGraphData()->SetSize(180);
        g = InsertNext(g->GetGlyphTreeIter(), _predict_short_he);

        _predict_long_lo = new CDataLineGraph("rt-prdt_long_lo", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, true, false);
        _predict_long_lo->setWorld(_prc_chart->getCoordWorld());
        _predict_long_lo->SetLineColor(BGR(255, 0, 0), 1.0f);
        _predict_long_lo->SetStrokeWidth(.2f);
        _predict_long_lo->GetGraphData()->SetSize(180);
        g = InsertNext(g->GetGlyphTreeIter(), _predict_long_lo);

        _predict_long_he = new CDataLineGraph("rt-prdt_long_he", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, true, false);
        _predict_long_he->setWorld(_prc_chart->getCoordWorld());
        _predict_long_he->SetLineColor(BGR(255, 0, 0), 1.0f);
        _predict_long_he->SetStrokeWidth(.2f);
        _predict_long_he->GetGraphData()->SetSize(180);
        g = InsertNext(g->GetGlyphTreeIter(), _predict_long_he);
    } catch(...) {
        return (-1);
    }
    return S_OK;
}

void CEuclidCanvas::SetGlyphRect()
{
    if (_blind) _blind->SetRect(_rect);
    if (_prc_chart)
        _prc_chart->SetRect(_rect);

    {

        RULER_WIDTH& rw = (_prc_chart->getCoordFrame())->getRulerWidth();
        RECT rect = _rect;
        rect.left += rw.left, rect.right -= rw.right, rect.top += rw.top, rect.bottom -= rw.bottom;
        if (_approx_short_lo ) _approx_short_lo ->SetRect(rect);
        if (_approx_short_he ) _approx_short_he ->SetRect(rect);
        if (_approx_long_lo  ) _approx_long_lo  ->SetRect(rect);
        if (_approx_long_he  ) _approx_long_he  ->SetRect(rect);
        if (_predict_short_lo) _predict_short_lo->SetRect(rect);
        if (_predict_short_he) _predict_short_he->SetRect(rect);
        if (_predict_long_lo ) _predict_long_lo ->SetRect(rect);
        if (_predict_long_he ) _predict_long_he ->SetRect(rect);
    }

    if (_data_cont) {
       NewDataForCtpmmd(_data_cont, DataObject::MARKET_DATA_TYPE_CTPMMD);
       _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CHANGED | (int)GLYPH_CHANGED_TYPE_COORDFRAME);
    }
}


void CEuclidCanvas::setDataOffset(int pxo, int pyo)
{
}

GLYPH_CHANGED_TYPE CEuclidCanvas::NewData(DataObjectPtr data)
{
    return GLYPH_CHANGED_TYPE_NONE;
}

GLYPH_CHANGED_TYPE CEuclidCanvas::NewData(dataptr data, size_t datalen, DataObject::MARKET_DATA_TYPE datatype)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    if (datatype == DataObject::MARKET_DATA_TYPE_ZITDATA) {
#       ifdef _DEBUG
        //ZitData* zit = (ZitData*)data;
        //MYTRACE(L"CEuclidCanvas::NewData extremum %d, infl %d, central %d, area %d\n",
            //zit->extCentral.nAllNum, 
            //zit->inflBest.nNum,
            //zit->central.nNum,
            //zit->lha.nNum);
#       endif //_DEBUG

        {/* fist approximate
        if (_first_appx && ((ZitData*)data)->extCentral.nAllNum) {
            _first_appx->GetGraphData()->Reset();
            for(int i = 0; i < ((ZitData*)data)->extCentral.nAllNum; i++) {
                //MYTRACE(L"%d %.02f %.02f %.02f\n",
                //    i,
                //    ((ZitData*)data)->extCentral.extremum[i].ftime,
                //    Time2Index(g_ctpsec, ((ZitData*)data)->extCentral.extremum[i].ftime),
                //    ((ZitData*)data)->extCentral.extremum[i].fValue);
                _first_appx->GetGraphData()->AddData(
                    Time2Index(g_ctpsec, ((ZitData*)data)->extCentral.extremum[i].ftime),
                    ((ZitData*)data)->extCentral.extremum[i].fOrngValue);
            }
            changed = (GLYPH_CHANGED_TYPE)((int)(changed) | (int)GLYPH_CHANGED_TYPE_CHANGED);
        }*/}

        /** for line graph 
        double fFirstPos = (_prc_chart->getCoordWorld()->GetWorldRect().x0 + _prc_chart->getCoordWorld()->_vi._left_shirft);
        if (_short_prdt && ((ZitData*)data)->_has_short_prdt) {
            _short_prdt->GetGraphData()->Reset();
            for (int i = 0; i < 180; i++) {
                _short_prdt->GetGraphData()->AddData(
                    fFirstPos + i,
                    ((ZitData*)data)->_short_prdt[i]);
            }
            changed = (GLYPH_CHANGED_TYPE)((int)(changed) | (int)GLYPH_CHANGED_TYPE_CHANGED);
        }

        if (_long_prdt && ((ZitData*)data)->_has_long_prdt) {
            _long_prdt->GetGraphData()->Reset();
            for (int i = 0; i < 180; i++) {
                _long_prdt->GetGraphData()->AddData(
                    fFirstPos + i * 2,
                    ((ZitData*)data)->_long_prdt[i]);
            }
            changed = (GLYPH_CHANGED_TYPE)((int)(changed) | (int)GLYPH_CHANGED_TYPE_CHANGED);
        }*/

        bool b_first_data = (_data_cont->getCount() == 1);
        _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)(_prc_chart->NewCtpData(_data_cont, (_changed_type), b_first_data)));
        if (!b_first_data && ((int)_changed_type & (int)GLYPH_CHANGED_TYPE_COORDFRAME))
        {
            NewDataForCtpmmd(_data_cont, datatype);
            _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CHANGED);
        }
    } else if (datatype == DataObject::EUCLID_CALUC_DBL_FILTER_PREDICTOR) {
        if (datalen > (1024 + 1024 + 1024 + 180 + 180) * sizeof(double)) {
            //have short and long term
            double fFirstPos  = _prc_chart->getCoordWorld()->GetWorldRect().x0;
            double* fshort_index    = (double*)data;
            double* short_appx_low  = fshort_index + 1024;
            double* short_appx_high = short_appx_low  + 1024;
            double* short_prdt_low  = short_appx_high + 1024;
            double* short_prdt_high = short_prdt_low  + 180;
            double* f_long_index    = short_prdt_high + 180;
            double* long_appx_low   = f_long_index    + 1024;
            double* long_appx_high  = long_appx_low   + 1024;
            double* long_prdt_low   = long_appx_high  + 1024;
            double* long_prdt_high  = long_prdt_low   + 180;
            
            _approx_short_lo->GetGraphData()->Reset();
            _approx_short_he->GetGraphData()->Reset();
            _approx_long_lo ->GetGraphData()->Reset();
            _approx_long_he ->GetGraphData()->Reset();
            _predict_short_lo->GetGraphData()->Reset();
            _predict_short_he->GetGraphData()->Reset();
            _predict_long_lo ->GetGraphData()->Reset();
            _predict_long_he ->GetGraphData()->Reset();
            for (size_t i = 0; i < 1024; i++) {
                //copy approx-data to graph
                _approx_short_lo->GetGraphData()->AddData(fshort_index[i], short_appx_low [i]);
                _approx_short_he->GetGraphData()->AddData(fshort_index[i], short_appx_high[i]);
                _approx_long_lo ->GetGraphData()->AddData(f_long_index[i], long_appx_low  [i]);
                _approx_long_he ->GetGraphData()->AddData(f_long_index[i], long_appx_high [i]);
            }

            for (size_t i = 0; i < 180; i++) {
                //copy approx-data to graph
                _predict_short_lo->GetGraphData()->AddData(fshort_index[1023] + i + 1, short_prdt_low [i]);
                _predict_short_he->GetGraphData()->AddData(fshort_index[1023] + i + 1, short_prdt_high[i]);
                _predict_long_lo ->GetGraphData()->AddData(f_long_index[1023] + i * 2 + 1, long_prdt_low  [i]);
                _predict_long_he ->GetGraphData()->AddData(f_long_index[1023] + i * 2 + 1, long_prdt_high [i]);
            }
            _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CHANGED);
        } else if (datalen) {
            //short only
            double* findex    = (double*)data;
#ifdef      _DEBUG
            //double fFirstPos  = _prc_chart->getCoordWorld()->GetWorldRect().x0;
            //MYTRACE(L"%.02f %.02f %d\n", *findex, fFirstPos, _data_cont->getCount());
#endif      //_DEBUG
            double* appx_low  = (double*)((char*)findex    + 1024 * sizeof(double));
            double* appx_high = (double*)((char*)appx_low  + 1024 * sizeof(double));
            double* prdt_low  = (double*)((char*)appx_high + 1024 * sizeof(double));
            double* prdt_high = (double*)((char*)prdt_low  +  180 * sizeof(double));

            _approx_short_lo->GetGraphData()->Reset();
            _approx_short_he->GetGraphData()->Reset();
            _predict_short_lo->GetGraphData()->Reset();
            _predict_short_he->GetGraphData()->Reset();

            for (size_t i = 0; i < 1024; i++) {
                //copy approx-data to graph
                _approx_short_lo->GetGraphData()->AddData(findex[i], appx_low [i]);
                _approx_short_he->GetGraphData()->AddData(findex[i], appx_high[i]);
                //if (appx_low [i] - appx_high[i] != 0) {
                //    MYTRACE(L"%d %.02f - %.02f = %.02f, is not zero\n", i, appx_low[i], appx_high[i], appx_low[i] - appx_high[i]);
                //}
            }

            for (size_t i = 0; i < 180; i++) {
                //copy approx-data to graph
                _predict_short_lo->GetGraphData()->AddData(findex[1023] + i + 1, prdt_low [i]);
                _predict_short_he->GetGraphData()->AddData(findex[1023] + i + 1, prdt_high[i]);
            }
            _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CHANGED);
        }
    }
    return changed;
}

GLYPH_CHANGED_TYPE CEuclidCanvas::NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype)
{
    _data_cont = (CCtpmmdContainer*)data_cont;
    //return GLYPH_CHANGED_TYPE_NONE;
    //MYTRACE(L"CEuclidCanvas::NewData\n");
    _changed_type = GLYPH_CHANGED_TYPE_NONE;
    if (datatype == DataObject::MARKET_DATA_TYPE_CTPMMD) {
#       ifdef _DEBUG
        //if (!((((CCtpmmdContainer*)data_cont)->getCount()) % 300)) {
        //    const CTPMMD* pctpmmd = ((CCtpmmdContainer*)data_cont)->getCurrentData();
        //    MYTRACE(L"%d, %.02f\n", ((CCtpmmdContainer*)data_cont)->getCount(), pctpmmd->fIndex);
        //    MYTRACE(L"zit-canvas %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
        //        pctpmmd->fIndex,
        //        pctpmmd->m_time,
        //        pctpmmd->LastPrice,
        //        pctpmmd->Volume,
        //        pctpmmd->relVolume,
        //        pctpmmd->OpenInterest,
        //        pctpmmd->relInterest);
        //}
#       endif //_DEBUG

        bool b_first_data = ((CCtpmmdContainer*)data_cont)->getCount() == 1;
        //MYTRACE(L"My world is %.02f %.02f %.02f %.02f %.02f %.02f %.02f %.02f uuu %.02f %.02f %.02f\n",
        //    _prc_chart->getCoordWorld()->GetWorldRect().x0,
        //    _prc_chart->getCoordWorld()->GetWorldRect().y0,
        //    _prc_chart->getCoordWorld()->GetWorldRect().xn,
        //    _prc_chart->getCoordWorld()->GetWorldRect().yn,
        //    _prc_chart->getCoordWorld()->GetWorldRect().minpos,
        //    _prc_chart->getCoordWorld()->GetWorldRect().miny,
        //    _prc_chart->getCoordWorld()->GetWorldRect().maxpos,
        //    _prc_chart->getCoordWorld()->GetWorldRect().maxy,
        //    _prc_chart->getCoordWorld()->_vi._left_shirft + _prc_chart->getCoordWorld()->GetWorldRect().x0,
        //    _prc_chart->getCoordWorld()->GetWorldRect().xn - _prc_chart->getCoordWorld()->GetWorldRect().x0,
        //    ((CCtpmmdContainer*)data_cont)->getCurrentData()->fIndex
        //     - _prc_chart->getCoordWorld()->_vi._left_shirft
        //     - _prc_chart->getCoordWorld()->GetWorldRect().x0);
        //MYTRACE(L"Currrent data is %.02f\n", ((CCtpmmdContainer*)data_cont)->getCurrentData()->fIndex);

        //MYTRACE(L"My world is %.02f %.02f\n",
        //    ((CCtpmmdContainer*)data_cont)->getCurrentData()->fIndex,
        //    (_prc_chart->getCoordWorld()->GetWorldRect().x0 + _prc_chart->getCoordWorld()->_vi._left_shirft));
        if (!b_first_data
            &&
            (((CCtpmmdContainer*)data_cont)->getCurrentData()->fIndex
                 >=
                 (_prc_chart->getCoordWorld()->GetWorldRect().x0 + _prc_chart->getCoordWorld()->_vi._left_shirft)))
        {
            //MYTRACE(L"My world is %.02f %.02f %.02f %.02f %.02f %.02f %.02f %.02f uuu %.02f %.02f %.02f\n",
            //    _prc_chart->getCoordWorld()->GetWorldRect().x0,
            //    _prc_chart->getCoordWorld()->GetWorldRect().y0,
            //    _prc_chart->getCoordWorld()->GetWorldRect().xn,
            //    _prc_chart->getCoordWorld()->GetWorldRect().yn,
            //    _prc_chart->getCoordWorld()->GetWorldRect().minpos,
            //    _prc_chart->getCoordWorld()->GetWorldRect().miny,
            //    _prc_chart->getCoordWorld()->GetWorldRect().maxpos,
            //    _prc_chart->getCoordWorld()->GetWorldRect().maxy,
            //    _prc_chart->getCoordWorld()->_vi._left_shirft + _prc_chart->getCoordWorld()->GetWorldRect().x0,
            //    _prc_chart->getCoordWorld()->GetWorldRect().xn - _prc_chart->getCoordWorld()->GetWorldRect().x0,
            //    ((CCtpmmdContainer*)data_cont)->getCurrentData()->fIndex
            //     - _prc_chart->getCoordWorld()->_vi._left_shirft
            //     - _prc_chart->getCoordWorld()->GetWorldRect().x0);
            _prc_chart->ResetWorldRect(((CCtpmmdContainer*)data_cont));
            _changed_type = GLYPH_CHANGED_TYPE_COORDFRAME;
        }

        _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)(_prc_chart->NewCtpData((CCtpmmdContainer*)data_cont, (_changed_type), b_first_data)));
        if (!b_first_data && ((int)_changed_type & (int)GLYPH_CHANGED_TYPE_COORDFRAME))
        {
            NewDataForCtpmmd((CCtpmmdContainer*)data_cont, datatype);
            _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CHANGED);
        }
    }
    return _changed_type;
}


GLYPH_CHANGED_TYPE CEuclidCanvas::NewDataForCtpmmd(CCtpmmdContainer* czc, DataObject::MARKET_DATA_TYPE datatype)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    _prc_chart->BeginSetData((dataptr)(czc->getCurrentData()));
    //set data
    CTPMMD* curdata = (CTPMMD*)czc->getCurrentData();
    CTPMMD* dataptr = czc->getDataPtr();
    CTPMMD* x0data = curdata - (int)(_prc_chart->getCoordWorld()->GetWorldRect().width()) + 1;
    if (x0data < dataptr)
        x0data = dataptr;

    for (CTPMMD* ctpmmd = (CTPMMD*)curdata; ctpmmd >= x0data; --ctpmmd) {
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
        _prc_chart->AddDataToPathGeometry(ctpmmd);
    }

    _prc_chart->EndSetData();
    _prc_chart->RenewGraph(GLYPH_CHANGED_TYPE_COORDFRAME);

    return changed;
}
