#include "StdAfx.h"
#include "qks_incs.h"


CTickdataCanvas::CTickdataCanvas(const char* name)
    : IDataCanvas(name)
    , _grid_tickdata(0)
    , _blind(0)
{
    _bHasBackground = true;
}

CTickdataCanvas::~CTickdataCanvas(void)
{
}


///set children's rect
void  CTickdataCanvas::SetGlyphRect()
{
	if (_blind) _blind->SetRect(_rect);
    if (_grid_tickdata) _grid_tickdata->SetRect(_rect);
}

///initial canvas, must set the name before init
const HRESULT CTickdataCanvas::Init(const char* name/* = 0*/)
{
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

        _grid_tickdata = new CTickDataGridCtrl(_name);
        char strconf[MAX_PATH];
        _snprintf_s(strconf, MAX_PATH, "%s.grid", _strconf);
        _grid_tickdata->SetConfig(_config);
        _grid_tickdata->SetConfigStr(strconf);

        if (!g)
            g = AppendChild(_gt.begin(), _grid_tickdata);
        else
            g = InsertNext(g->GetGlyphTreeIter(), _grid_tickdata);
    } catch(...) {
        return (-1);
    }

    return S_OK;
}

GLYPH_CHANGED_TYPE CTickdataCanvas::NewData(DataObjectPtr data)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    return changed;
}


GLYPH_CHANGED_TYPE CTickdataCanvas::NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype)
{
    _changed_type = GLYPH_CHANGED_TYPE_NONE;
    if (datatype == DataObject::MARKET_DATA_TYPE_CTPMMD) {
#       ifdef _DEBUG
        //if (!((((CCtpmmdContainer*)data_cont)->getCount()) % 300)) {
        //    const CTPMMD* pctpmmd = ((CCtpmmdContainer*)data_cont)->getCurrentData();
        //    MYTRACE(L"%d, %.02f\n", ((CCtpmmdContainer*)data_cont)->getCount(), pctpmmd->fIndex);
        //}
        //const CTPMMD* pctpmmd = ((CCtpmmdContainer*)data_cont)->getCurrentData();
        //MYTRACE(L"tick %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
        //    pctpmmd->fIndex,
        //    pctpmmd->m_time,
        //    pctpmmd->LastPrice,
        //    pctpmmd->Volume,
        //    pctpmmd->relVolume,
        //    pctpmmd->OpenInterest,
        //    pctpmmd->relInterest);
#       endif //_DEBUG
        _changed_type = _grid_tickdata->NewData(data_cont, datatype);
        _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CANVAS_BKG);
    }
    //_atelier->Changed(_changed_type);
    return _changed_type;
}



