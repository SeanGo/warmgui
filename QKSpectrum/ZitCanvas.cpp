#include "StdAfx.h"
#include "qks_incs.h"


CZitCanvas::CZitCanvas(const char* name)
    : IDataCanvas(name)
    , _prc_chart(0)
    , _zit_graph(0)
    , _short_prdt(0)
    , _long_prdt(0)
    , _data_cont(0)
    , _first_appx(0)
{
}


CZitCanvas::~CZitCanvas(void)
{
}


const HRESULT CZitCanvas::Init(const char* name/* = 0*/)
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

        _prc_chart = new CPriceTickChart("price-chart");
        _snprintf_s(temp, MAX_PATH, "%s.price-chart", _strconf);
        _prc_chart->setConfig(_config, temp);
        if (!g)
            g = AppendChild(_gt.begin(), _prc_chart);
        else
            g = InsertNext(g->GetGlyphTreeIter(), _prc_chart);
        _prc_chart->AddGraphs();

        _zit_graph = new CZITGraph("zit-graph");
        _zit_graph->setWorld(_prc_chart->getCoordWorld());
        g = InsertNext(g->GetGlyphTreeIter(), _zit_graph);

        _first_appx = new CDataLineGraph("_first_appx", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, true, false);
        _first_appx->setWorld(_prc_chart->getCoordWorld());
        _first_appx->GetGraphData()->SetSize(200);
        _first_appx->SetStrokeWidth(.02f);
        g = InsertNext(g->GetGlyphTreeIter(), _first_appx);

        _short_prdt = new CDataLineGraph("short-prdt", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, true, false);
        _short_prdt->setWorld(_prc_chart->getCoordWorld());
        g = InsertNext(g->GetGlyphTreeIter(), _short_prdt);

        _long_prdt = new CDataLineGraph("long-prdt", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, true, false);
        _long_prdt->setWorld(_prc_chart->getCoordWorld());
        g = InsertNext(g->GetGlyphTreeIter(), _long_prdt);
    } catch(...) {
        return (-1);
    }

    return S_OK;
}

void CZitCanvas::SetGlyphRect()
{
	if (_blind)
		_blind->SetRect(_rect);
    if (_prc_chart)
        _prc_chart->SetRect(_rect);
    {

        RULER_WIDTH& rw = (_prc_chart->getCoordFrame())->getRulerWidth();
        RECT rect = _rect;
        rect.left += rw.left, rect.right -= rw.right, rect.top += rw.top, rect.bottom -= rw.bottom;
        if (_zit_graph)   _zit_graph->SetRect(rect);
        if (_first_appx) _first_appx->SetRect(rect);
    }
}

void CZitCanvas::setDataOffset(int pxo, int pyo)
{
}

GLYPH_CHANGED_TYPE CZitCanvas::NewData(DataObjectPtr data)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    return changed;
}

GLYPH_CHANGED_TYPE CZitCanvas::NewData(dataptr data, DataObject::MARKET_DATA_TYPE datatype)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    if (datatype == DataObject::MARKET_DATA_TYPE_ZITDATA) {
#       ifdef _DEBUG
        //ZitData* zit = (ZitData*)data;
        //MYTRACE(L"CZitCanvas::NewData extremum %d, infl %d, central %d, area %d\n",
            //zit->extCentral.nAllNum, 
            //zit->inflBest.nNum,
            //zit->central.nNum,
            //zit->lha.nNum);
#       endif //_DEBUG

        if (_zit_graph) {
            changed = (GLYPH_CHANGED_TYPE)((int)(changed) | (int)(_zit_graph->NewData(data)));
            //MYTRACE(L" changed 0x%x\n", changed);
        }

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
        }

        /*if (_short_prdt)
            changed = (GLYPH_CHANGED_TYPE)((int)(changed) | (int)(_short_prdt->NewData(data)));
        if (_long_prdt)
            changed = (GLYPH_CHANGED_TYPE)((int)(changed) | (int)(_long_prdt->NewData(data)));

        bool b_first_data = (_data_cont->getCount() == 1);
        _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)(_prc_chart->NewCtpData(_data_cont, (_changed_type), b_first_data)));
        if (!b_first_data && ((int)_changed_type & (int)GLYPH_CHANGED_TYPE_COORDFRAME))
        {
            NewDataForCtpmmd(_data_cont, datatype);
            _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CHANGED);
        }*/
    }
    return changed;
}

GLYPH_CHANGED_TYPE CZitCanvas::NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype)
{
    _data_cont = (CCtpmmdContainer*)data_cont;
    //return GLYPH_CHANGED_TYPE_NONE;
    //MYTRACE(L"CZitCanvas::NewData\n");
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


GLYPH_CHANGED_TYPE CZitCanvas::NewDataForCtpmmd(CCtpmmdContainer* czc, DataObject::MARKET_DATA_TYPE datatype)
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
