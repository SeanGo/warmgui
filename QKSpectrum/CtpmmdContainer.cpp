#include "StdAfx.h"
#include "qks_incs.h"


//////////////////////////////////////////////////////////////////////////////////
//
// class CCtpmmdContainer
//

bool CCtpmmdContainer::NewData(DataObjectPtr dop)
{
    char* buf = (char*)dop->GetData();
    size_t len = dop->GetDataLength();

    if ((DataObject::MARKET_DATA_TYPE)(*(int*)buf) == MARKET_DATA_TYPE_CTPMMD) {
        buf += sizeof(int), len -= sizeof(int);
        CTPMMD* ctpmmd = (CTPMMD*)buf;

        //check buffer length
        if (_data_len > (size_t)ctpmmd->fIndex) {
            if (!_count) {
                _fist_data_pos = _curpos = (size_t)ctpmmd->fIndex;
                memcpy(_ctpmmd + _curpos, ctpmmd, sizeof(CTPMMD));
            } else {
                add_curpos(ctpmmd);
            }
#           ifdef _DEBUG
            //CTPMMD* pctpmmd = _ctpmmd + _curpos;
            //MYTRACE(L"CTPMMD %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
            //    pctpmmd->fIndex,
            //    pctpmmd->m_time,
            //    pctpmmd->LastPrice,
            //    pctpmmd->Volume,
            //    pctpmmd->relVolume,
            //    pctpmmd->OpenInterest,
            //    pctpmmd->relInterest);
#           endif //_DEBUG
            add_count();
        } else {
            //shift to left
            //but not now
            //if the extremum shift out the window, re-scan the extremum
            //Lu Yi Bian
        }
        return true;
    }
    return false;
}



///get the configureation of data-container
void CCtpmmdContainer::SetConfig(CWarmguiConfig* config, const char* cnf_pos)
{
    IDataContainer::SetConfig(config, cnf_pos);

    //get configuration
    try{
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.data-length", _cnf_pos);
        _data_len     = _config->getInt(temp);

        //SetContainerSize(sizeof(CTPMMD), _data_len, _ts_length);
        {
            _ctpmmd = new CTPMMD[_data_len];
            _count = 0, _sizeofdata = sizeof(CTPMMD);
        }

    }catch(...){
        MYTRACE(L"CCtpmmdContainer::SetConfig\n");
    }
}


