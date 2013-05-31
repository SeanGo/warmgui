#include "StdAfx.h"
#include "qks_incs.h"


#define move_to_next_data(l) { len -= (l); if (len < 1) return result; buf += (l); }
///////////////////////////////////////////////////////////////////////////////////////////
// struct ZitData

bool ZitData::AppendData(dataptr zitdata, size_t length)
{
    bool result = false;

    size_t len = length;
    char*  buf = (char*)zitdata;

    //down sampler
    data_ds = *(int*)buf;
    move_to_next_data(sizeof(int));

    //get the number of extreme
    extCentral.nAllNum = *(int*)buf;
	if (extCentral.nAllNum < 0 || extCentral.nAllNum > 200)
        return result;

    //get the extremes
    move_to_next_data(sizeof(int));
    memcpy(extCentral.extremum, buf, sizeof(EXTREMUM) * extCentral.nAllNum);

    //get the number of inflexion
    move_to_next_data(sizeof(EXTREMUM) * extCentral.nAllNum);
    inflBest.nNum = *(int*) buf;
    if (inflBest.nNum < 0 || inflBest.nNum > 100)
        return result;

    //get the inflexions
    move_to_next_data(sizeof(int));
    memcpy(inflBest.infl, buf, sizeof(INFLEXION) * inflBest.nNum);
    
    //get the number of central 
    move_to_next_data(sizeof(INFLEXION) * inflBest.nNum);
    central.nNum = *(int*)buf;
    if (central.nNum < 0 || central.nNum > 200)
        return result;
    
    //get centrals
    move_to_next_data(sizeof(int));
    memcpy(central.central, buf, sizeof(CENTRAL) * central.nNum);
    
    //get number of the area
    move_to_next_data(sizeof(CENTRAL) * central.nNum);
    lha.nNum = *(int*)buf;
    if (lha.nNum < 0 || lha.nNum > 200)
        return result;

    //get the areas
    move_to_next_data(sizeof(int));
    memcpy(lha.lha, buf, sizeof(L_H_AREA) * lha.nNum);
    
    //buf += sizeof(L_H_AREA) * lha.nNum; len -= sizeof(L_H_AREA) * lha.nNum;

#   ifdef _DEBUG
    MYTRACE(L"extremum %d, infl %d, central %d, area %d\n",
        extCentral.nAllNum, 
        inflBest.nNum,
        central.nNum,
        lha.nNum);
#   endif //_DEBUG

    return (true);
}



//////////////////////////////////////////////////////////////////////////////////
//
// class CCtpmmdZitContainer
//

bool CCtpmmdZitContainer::NewData(DataObjectPtr dop)
{
    char* buf = (char*)dop->GetData();
    size_t len = dop->GetDataLength();

    if ((DataObject::MARKET_DATA_TYPE)(*(int*)buf) == DataObject::MARKET_DATA_TYPE_CTPMMD) {
        buf += sizeof(int), len -= sizeof(int);
        CTPMMD* ctpmmd = (CTPMMD*)buf;
        if (_data_len > (size_t)ctpmmd->fIndex) {
            _curpos = (size_t)ctpmmd->fIndex;
            memcpy(_ctpmmd + _curpos, ctpmmd, sizeof(CTPMMD));
#           ifdef _DEBUG
            /*CTPMMD* pctpmmd = _ctpmmd + _curpos;
            MYTRACE(L"CTPMMD %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
                pctpmmd->fIndex,
                pctpmmd->m_time,
                pctpmmd->LastPrice,
                pctpmmd->Volume,
                pctpmmd->relVolume,
                pctpmmd->OpenInterest,
                pctpmmd->relInterest);*/
#           endif //_DEBUG
            _world.renew_world(_ctpmmd + _curpos);
            ++_count;
        } else {
            //shift to left
            //but not now
        }
        return true;
    }
    return false;
}



///get the configureation of data-container
void CCtpmmdZitContainer::SetConfig(CWarmguiConfig* config, const char* cnf_pos)
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
    }
}



bool CZitdataContainer::NewData(DataObjectPtr dop)
{
    char* buf = (char*)dop->GetData();
    size_t len = dop->GetDataLength();

    if ((DataObject::MARKET_DATA_TYPE)(*(int*)buf) == DataObject::EUCLID_CALUC_TO_TRANSMIT) {
        buf += sizeof(int), len -= sizeof(int);

        return _zit_data.AppendData(buf, len);
    }
    return false;
}
