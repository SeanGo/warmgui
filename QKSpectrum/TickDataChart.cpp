#include "StdAfx.h"
#include "qks_incs.h"


////////////////////////////////////////////////////////////////////
// class CPriceTickChart
CPriceTickChart::CPriceTickChart(const char* name, bool world_own_type/* = true*/, bool data_own_type/*  = false*/, bool own_artist/* = false*/)
    : CDataLineChart(name, world_own_type, data_own_type, own_artist)
    ,             _dlg_price(0)
{
}


CPriceTickChart::~CPriceTickChart(void)
{
}


HRESULT CPriceTickChart::AddGraphs()
{
    CDataLineChart::Init();

    char key[MAX_PATH];
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.coord-grid.name", _strconf);
    _coord = new WARMGUI::CCoordFrame(_config->getString(key).c_str());
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.coord-grid", _strconf);
    _coord->setConfig(_config, key);

    IGlyph* g = _canvas->AppendChild(_iter, _coord);

    _dlg_price      = new WARMGUI::CDataLineGraph("price_tick", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, false, true); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_price     );
    /*
    _dlg_lt_prcappr = new WARMGUI::CDataLineGraph("prc-lt-app", false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_lt_prcappr);
    _dlg_st_prcappr = new WARMGUI::CDataLineGraph("prc-st-app", false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_st_prcappr);
    _dlg_lt_predict = new WARMGUI::CDataLineGraph("prc-lt-prd", false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_lt_predict);
    _dlg_st_predict = new WARMGUI::CDataLineGraph("prc-st-prd", false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_st_predict);
    */

    _dlg_price->setWorld(_coord->getWorld());
    _dlg_price->SetStrokeWidth(_stroke_width);

    /*
    _dlg_lt_prcappr->SetWorld(_coord->getWorld());
    _dlg_st_prcappr->SetWorld(_coord->getWorld());
    _dlg_lt_predict->SetWorld(_coord->getWorld());
    _dlg_st_predict->SetWorld(_coord->getWorld());
    */

    CTPMMD ctpmmd;
    _dlg_price->SetDataOffset((int)&ctpmmd.fIndex - (int)&ctpmmd, (int)&ctpmmd.LastPrice - (int)&ctpmmd);
    return S_OK;
}

GLYPH_CHANGED_TYPE CPriceTickChart::NewCtpData(CCtpmmdContainer* pdc, bool redraw_cood/* = false*/, bool b_first_data/* = false*/)
{
    GLYPH_CHANGED_TYPE change = GLYPH_CHANGED_TYPE_NONE;
    const CTPMMD* ctpmmd = pdc->getCurrentData();
    change = _coord->renew_world(ctpmmd->fIndex, ctpmmd->LastPrice, redraw_cood, b_first_data);

    _dlg_price->Changed(change);
    change = (GLYPH_CHANGED_TYPE)((int)change | (int)_dlg_price->NewData((dataptr)pdc->getCurrentData()));
    return change;
}

void CPriceTickChart::ResetWorldRect(IDataContainer* data_cont)
{
    CCtpmmdContainer* pdc = (CCtpmmdContainer*)data_cont;
    WORLD_RECT wr;
    CTPMMD* curdata = (CTPMMD*)pdc->getCurrentData();
    CTPMMD* dataptr = pdc->getDataPtr();
    /*
    CTPMMD* x0data = curdata - (int)(getCoordWorld()->_zero_world.width()) + 1;
    if (x0data < dataptr)
        x0data = dataptr;

    wr.xn = curdata->fIndex, wr.yn = curdata->LastPrice,
        wr.x0 = curdata->fIndex - getCoordWorld()->_zero_world.width() + 1, wr.y0 = x0data->LastPrice;

    */

    CTPMMD* x0data = curdata - (int)(getCoordWorld()->_vi._left_shirft) + 1;
    if (x0data < dataptr)
        x0data = dataptr;
    wr.xn = curdata->fIndex + (getCoordWorld()->_zero_world.width() - getCoordWorld()->_vi._left_shirft),
        wr.yn = curdata->LastPrice,
        wr.x0 = curdata->fIndex - getCoordWorld()->_vi._left_shirft + 1, wr.y0 = x0data->LastPrice;
    //MYTRACE(L"%.02f %.02f\n", wr.x0, wr.xn);
    //lu yi bian
    for (CTPMMD* ctpmmd = (CTPMMD*)curdata; ctpmmd >= x0data; --ctpmmd)
        wr.renew_min_max(ctpmmd->fIndex, ctpmmd->LastPrice);

    getCoordWorld()->SetWorldRect(wr);
    //MoveBitmapToLeft();
}






////////////////////////////////////////////////////////////////////
// class CVolumeTickChart
CVolumeTickChart::CVolumeTickChart(const char* name, bool world_own_type/* = true*/, bool data_own_type/*  = false*/, bool own_artist/* = false*/)
	: CDataLineChart(name, world_own_type, data_own_type, own_artist)
    ,     _dlg_relvol(0)
    ,     _dlg_volume(0)
    , _dlg_vol_approx(0)
{
}


CVolumeTickChart::~CVolumeTickChart(void)
{
}


HRESULT CVolumeTickChart::AddGraphs()
{
    CDataLineChart::Init();

    char key[MAX_PATH];
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.coord-grid.name", _strconf);
    _coord = new WARMGUI::CCoordFrame(_config->getString(key).c_str());
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.coord-grid", _strconf);
    _coord->setConfig(_config, key);

    IGlyph* g = _canvas->AppendChild(_iter, _coord);

    _dlg_relvol     = new WARMGUI::CDataLineGraph("volrel_tick", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, false, true); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_relvol    );
    //_dlg_volume     = new WARMGUI::CDataLineGraph("vol_tickdata", false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_volume    );
    //_dlg_vol_approx = new WARMGUI::CDataLineGraph("vol_approx"  , false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_vol_approx);


    _dlg_relvol->setWorld(_coord->getWorld());
    _dlg_relvol->SetStrokeWidth(_stroke_width);
    //_dlg_volume    ->SetWorld(_coord->getWorld());
    //_dlg_vol_approx->SetWorld(_coord->getWorld());

    CTPMMD ctpmmd;
    _dlg_relvol->SetDataOffset((int)&ctpmmd.fIndex - (int)&ctpmmd, (int)&ctpmmd.relVolume - (int)&ctpmmd);
    //_dlg_volume->SetDataOffset((int)&ctpmmd.fIndex - (int)&ctpmmd, (int)&ctpmmd.Volume - (int)&ctpmmd);
    return S_OK;
}


GLYPH_CHANGED_TYPE CVolumeTickChart::NewCtpData(CCtpmmdContainer* pdc, bool redraw_cood/* = false*/, bool b_first_data/* = false*/)
{
    GLYPH_CHANGED_TYPE change = GLYPH_CHANGED_TYPE_NONE;
    /*
    if (pdc->getCurrentData()->relVolume > 100) {
        if (pppp < pdc->getCurrentData()->relVolume)
            pppp = pdc->getCurrentData()->relVolume;
            MYTRACE(L"%.02f\n", pppp);
    }
    */
    const CTPMMD* ctpmmd = pdc->getCurrentData();
    change = _coord->renew_world(ctpmmd->fIndex, ctpmmd->relVolume, redraw_cood,  b_first_data);

    _dlg_relvol->Changed(change);
    change = (GLYPH_CHANGED_TYPE)((int)change | (int)_dlg_relvol->NewData((dataptr)pdc->getCurrentData()));
    return change;
}


void CVolumeTickChart::ResetWorldRect(IDataContainer* data_cont)
{
    CCtpmmdContainer* pdc = (CCtpmmdContainer*)data_cont;
    WORLD_RECT wr;
    const CTPMMD* curdata = pdc->getCurrentData();
    CTPMMD* dataptr = pdc->getDataPtr();
    //MYTRACE(L"getCoordWorld()->_zero_world.width() %.02f\n", getCoordWorld()->_zero_world.width());
    CTPMMD* x0data = dataptr + (int)(getCoordWorld()->_zero_world.width()) + 1;
    if (x0data < dataptr)
        x0data = dataptr;

    wr.xn = curdata->fIndex, wr.yn = curdata->relVolume,
        wr.x0 = x0data->fIndex, wr.y0 = x0data->relVolume;
    //lu yi bian
    for (CTPMMD* ctpmmd = (CTPMMD*)curdata; ctpmmd >= x0data; --ctpmmd)
        wr.renew_min_max(ctpmmd->fIndex, ctpmmd->relVolume);

    getCoordWorld()->SetWorldRect(wr);
    //MoveBitmapToLeft();
}




////////////////////////////////////////////////////////////////////
// class CInterestTickChart
CInterestTickChart::CInterestTickChart(const char* name, bool world_own_type/* = true*/, bool data_own_type/*  = false*/, bool own_artist/* = false*/)
	: CDataLineChart(name, world_own_type, data_own_type, own_artist)
    ,     _dlg_interest(0)
    , _dlg_rel_interest(0)
    ,   _dlg_ite_approx(0)
{
}

CInterestTickChart::~CInterestTickChart(void)
{
}


HRESULT CInterestTickChart::AddGraphs()
{
    CDataLineChart::Init();

    char key[MAX_PATH];
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.coord-grid.name", _strconf);
    _coord = new WARMGUI::CCoordFrame(_config->getString(key).c_str());
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.coord-grid", _strconf);
    _coord->setConfig(_config, key);

    IGlyph* g = _canvas->AppendChild(_iter, _coord);

    _dlg_interest     = new WARMGUI::CDataLineGraph("interest_tick", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, false, true); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_interest    );
    //_dlg_rel_interest = new WARMGUI::CDataLineGraph("ite_tick_rel" , false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_rel_interest);
    //_dlg_ite_approx   = new WARMGUI::CDataLineGraph("ite_approx"   , false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_ite_approx  );


    _dlg_interest->setWorld(_coord->getWorld());
    _dlg_interest->SetStrokeWidth(_stroke_width);
    
    //_dlg_rel_interest->SetWorld(_coord->getWorld());
    //_dlg_ite_approx  ->SetWorld(_coord->getWorld());

    CTPMMD ctpmmd;
    _dlg_interest->SetDataOffset((int)&ctpmmd.fIndex - (int)&ctpmmd, (int)&ctpmmd.OpenInterest - (int)&ctpmmd);
    //_dlg_rel_interest->SetDataOffset((int)&ctpmmd.fIndex - (int)&ctpmmd, (int)&ctpmmd.relInterest - (int)&ctpmmd);
    return S_OK;
}



GLYPH_CHANGED_TYPE CInterestTickChart::NewCtpData(CCtpmmdContainer* pdc, bool redraw_cood/* = false*/, bool b_first_data/* = false*/)
{
    GLYPH_CHANGED_TYPE change = GLYPH_CHANGED_TYPE_NONE;
    const CTPMMD* ctpmmd = pdc->getCurrentData();
    change = _coord->renew_world(ctpmmd->fIndex, ctpmmd->OpenInterest, redraw_cood, b_first_data);
    
    _dlg_interest->Changed(change);
    change = (GLYPH_CHANGED_TYPE)((int)change | (int)_dlg_interest->NewData((dataptr)pdc->getCurrentData()));
    return change;
}

void CInterestTickChart::ResetWorldRect(IDataContainer* data_cont)
{
    CCtpmmdContainer* pdc = (CCtpmmdContainer*)data_cont;
    WORLD_RECT wr;
    const CTPMMD* curdata = pdc->getCurrentData();
    CTPMMD* dataptr = pdc->getDataPtr();
    CTPMMD* x0data = dataptr + (int)(getCoordWorld()->GetWorldRect().width()) + 1;
    if (x0data < dataptr)
        x0data = dataptr;

    wr.xn = curdata->fIndex, wr.yn = curdata->OpenInterest,
        wr.x0 = x0data->fIndex, wr.y0 = x0data->OpenInterest;
    //lu yi bian
    for (CTPMMD* ctpmmd = (CTPMMD*)curdata; ctpmmd >= x0data; --ctpmmd)
        wr.renew_min_max(ctpmmd->fIndex, ctpmmd->OpenInterest);

    getCoordWorld()->SetWorldRect(wr);
    //MoveBitmapToLeft();
}







////////////////////////////////////////////////////////////////////
// class CPriceAnalysChart
CPriceAnalysChart::CPriceAnalysChart(const char* name, bool world_own_type/* = true*/, bool data_own_type/*  = false*/, bool own_artist/* = false*/)
    : CDataLineChart(name, world_own_type, data_own_type, own_artist)
    ,             _dlg_price(0)
    ,  _short_low_prc_approx(0)
    , _short_high_prc_approx(0)
    ,        _short_low_prdt(0)
    ,       _short_high_prdt(0)
    ,   _long_low_prc_approx(0)
    ,  _long_high_prc_approx(0)
    ,         _long_low_prdt(0)
    ,        _long_high_prdt(0)
{
}


CPriceAnalysChart::~CPriceAnalysChart(void)
{
}


HRESULT CPriceAnalysChart::AddGraphs()
{
    CDataLineChart::Init();

    char key[MAX_PATH];
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.coord-grid.name", _strconf);
    _coord = new WARMGUI::CCoordFrame(_config->getString(key).c_str());
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.coord-grid", _strconf);
    _coord->setConfig(_config, key);

    IGlyph* g = _canvas->AppendChild(_iter, _coord);

    _dlg_price      = new WARMGUI::CDataLineGraph("price_tick", CDataLineGraph::GEOMETRY_PATH_TYPE_LINE, false, false, true); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_price     );
    /*
    _dlg_lt_prcappr = new WARMGUI::CDataLineGraph("prc-lt-app", false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_lt_prcappr);
    _dlg_st_prcappr = new WARMGUI::CDataLineGraph("prc-st-app", false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_st_prcappr);
    _dlg_lt_predict = new WARMGUI::CDataLineGraph("prc-lt-prd", false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_lt_predict);
    _dlg_st_predict = new WARMGUI::CDataLineGraph("prc-st-prd", false); g = _canvas->InsertNext(g->GetGlyphTreeIter(), _dlg_st_predict);
    */

    _dlg_price->setWorld(_coord->getWorld());
    _dlg_price->SetStrokeWidth(_stroke_width);

    /*
    _dlg_lt_prcappr->SetWorld(_coord->getWorld());
    _dlg_st_prcappr->SetWorld(_coord->getWorld());
    _dlg_lt_predict->SetWorld(_coord->getWorld());
    _dlg_st_predict->SetWorld(_coord->getWorld());
    */

    CTPMMD ctpmmd;
    _dlg_price->SetDataOffset((int)&ctpmmd.fIndex - (int)&ctpmmd, (int)&ctpmmd.LastPrice - (int)&ctpmmd);
    return S_OK;
}

GLYPH_CHANGED_TYPE CPriceAnalysChart::NewCtpData(CCtpmmdContainer* pdc, bool redraw_cood/* = false*/, bool b_first_data/* = false*/)
{
    GLYPH_CHANGED_TYPE change = GLYPH_CHANGED_TYPE_NONE;
    const CTPMMD* ctpmmd = pdc->getCurrentData();
    change = _coord->renew_world(ctpmmd->fIndex, ctpmmd->LastPrice, redraw_cood, b_first_data);

    _dlg_price->Changed(change);
    change = (GLYPH_CHANGED_TYPE)((int)change | (int)_dlg_price->NewData((dataptr)pdc->getCurrentData()));
    return change;
}

void CPriceAnalysChart::ResetWorldRect(IDataContainer* data_cont)
{
    CCtpmmdContainer* pdc = (CCtpmmdContainer*)data_cont;
    WORLD_RECT wr;
    CTPMMD* curdata = (CTPMMD*)pdc->getCurrentData();
    CTPMMD* dataptr = pdc->getDataPtr();
    /*
    CTPMMD* x0data = curdata - (int)(getCoordWorld()->_zero_world.width()) + 1;
    if (x0data < dataptr)
        x0data = dataptr;

    wr.xn = curdata->fIndex, wr.yn = curdata->LastPrice,
        wr.x0 = curdata->fIndex - getCoordWorld()->_zero_world.width() + 1, wr.y0 = x0data->LastPrice;

    */

    CTPMMD* x0data = curdata - (int)(getCoordWorld()->_vi._left_shirft) + 1;
    if (x0data < dataptr)
        x0data = dataptr;
    wr.xn = curdata->fIndex + (getCoordWorld()->_zero_world.width() - getCoordWorld()->_vi._left_shirft),
        wr.yn = curdata->LastPrice,
        wr.x0 = curdata->fIndex - getCoordWorld()->_vi._left_shirft + 1, wr.y0 = x0data->LastPrice;
    //MYTRACE(L"%.02f %.02f\n", wr.x0, wr.xn);
    //lu yi bian
    for (CTPMMD* ctpmmd = (CTPMMD*)curdata; ctpmmd >= x0data; --ctpmmd)
        wr.renew_min_max(ctpmmd->fIndex, ctpmmd->LastPrice);

    getCoordWorld()->SetWorldRect(wr);
    //MoveBitmapToLeft();
}

