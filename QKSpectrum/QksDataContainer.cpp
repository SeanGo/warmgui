#include "StdAfx.h"
#include "qks_incs.h"


#define move_to_next_data(l) { len -= (l); if (len < 1) return result; buf += (l); }
///////////////////////////////////////////////////////////////////////////////////////////
// struct ZitData

bool ZitData::AppendData(DataObjectPtr dopNewData)
{
    bool result = false;

    size_t len = dopNewData->GetDataLength();
    char*  buf = (char*)(dopNewData->GetData());

    //pointer to ZIT data
    move_to_next_data(sizeof(CTPMMD));

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
    //MYTRACE(L"extremum %d, infl %d, central %d, area %d\n",
    //    extCentral.nAllNum, 
    //    inflBest.nNum,
    //    central.nNum,
    //    lha.nNum);
#   endif //_DEBUG

    return (true);
}



//////////////////////////////////////////////////////////////////////////////////
//
// class CCtpmmdZitContainer
//

inline bool CCtpmmdZitContainer::NewData(DataObjectPtr dop)
{
    CTPMMD* ctpmmd = (CTPMMD*)dop->GetData();
    //char*  buf = (char*)dop->GetData();
	size_t len = dop->GetDataLength();

    if (len >= sizeof(CTPMMD)) {
        if (_count >= this->_data_len) {
            memmove(_ctpmmd, _ctpmmd + 1, sizeof(CTPMMD) * (_data_len - 1));
            memcpy(_ctpmmd + (_data_len - 1), ctpmmd, sizeof(CTPMMD));
        } else {
            memcpy(_ctpmmd + _count, ctpmmd, sizeof(CTPMMD));
        }

        len -= sizeof(CTPMMD);

        int k = _ts.FillData(_count);
        if (k < 0)
            MYTRACE(L"an error has occued\n");
        //else if (k > 0)
        //    MYTRACE(L"Fulfit Time Series\n");

        ++_count;
#ifdef _DEBUG
        bool result = _zit_data.AppendData(dop);
        
        MYTRACE(L"CTPMMD %.02f %.02f %.02f %.02f %.02f\n",
            _ctpmmd[_count - 1].fIndex,
            _ctpmmd[_count - 1].m_time,
            _ctpmmd[_count - 1].LastPrice,
            _ctpmmd[_count - 1].Volume,
            _ctpmmd[_count - 1].OpenInterest );

        if (!result)
            MYTRACE(L"error got zit data!\n");

        return result;
#else
        return _zit_data.AppendData(dop);
#endif //_DEBUG
    } else {
         return false;
    }
}


inline dataptr CCtpmmdZitContainer::getDataByName(size_t* nsize, DATA_OWN_TYPE* owntype, const char* dataname/* = 0*/)
{
    if (!strcmp(dataname, "price"   )) {
        TimeSeriesData* tsdata = new TimeSeriesData();
        tsdata->_index = _ts.GetTimeSeries()->_findex;
        tsdata->_value = _ts.GetTimeSeries()->_fprice;
        tsdata->_time  = _ts.GetTimeSeries()->_ftime ;
        *nsize = tsdata->_size  = _ts.GetTimeSeries()->_length;
        *owntype = DATA_OWN_TYPE_MINE;
        return tsdata;
    }
    if (!strcmp(dataname, "volume"  )) {
        TimeSeriesData* tsdata = new TimeSeriesData();
        tsdata->_index = _ts.GetTimeSeries()->_findex ;
        tsdata->_value = _ts.GetTimeSeries()->_fvolume;
        tsdata->_time  = _ts.GetTimeSeries()->_ftime  ;
        *nsize = tsdata->_size  = _ts.GetTimeSeries()->_length;
        *owntype = DATA_OWN_TYPE_MINE;
        return tsdata;
    }
    if (!strcmp(dataname, "interest")) {
        TimeSeriesData* tsdata = new TimeSeriesData();
        tsdata->_index = _ts.GetTimeSeries()->_findex   ;
        tsdata->_value = _ts.GetTimeSeries()->_finterest;
        tsdata->_time  = _ts.GetTimeSeries()->_ftime    ;
        *nsize = tsdata->_size  = _ts.GetTimeSeries()->_length;
        *owntype = DATA_OWN_TYPE_MINE;
        return tsdata;
    }

    if (!strcmp(dataname, "zen-in-twning")) {
        *owntype = DATA_OWN_TYPE_OTHER;
        *nsize = sizeof(_zit_data);
        return &_zit_data;
    }
    return (0);
}


inline WORLD_RECT* CCtpmmdZitContainer::getWorldByName(size_t* nsize, const char* dataname/* = 0*/)
{
    if (!strcmp(dataname, "price"   ))
        return (WORLD_RECT*)&(_ts.getFutureDataWorld().wr_price);
    if (!strcmp(dataname, "volume"  ))
        return (WORLD_RECT*)&(_ts.getFutureDataWorld().wr_volume);
    if (!strcmp(dataname, "interest"))
        return (WORLD_RECT*)&(_ts.getFutureDataWorld().wr_interest);
    return (0);
}

///get the configureation of data-container
inline void CCtpmmdZitContainer::SetConfig(CWarmguiConfig* config, const char* cnf_pos)
{
    IDataContainer::SetConfig(config, cnf_pos);

    //get configuration
    try{
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.down-sampler", _cnf_pos);
        _down_sampler = _config->getInt(temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.data-length", _cnf_pos);
        _data_len     = _config->getInt(temp);

        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.ts-length", _cnf_pos);
        _ts_length    = _config->getInt(temp);

        //SetContainerSize(sizeof(CTPMMD), _data_len, _ts_length);
        {
            _ctpmmd = new CTPMMD[_data_len];
            _ts.SetData(_ctpmmd, _data_len);
            _ts.SetLength(_ts_length);

            _pdata = &_ts;

            _count = 0, _sizeofdata = sizeof(CTPMMD);
        }

    }catch(...){
    }
}

inline void CCtpmmdZitContainer::RegisterNames()
{
    _sa_data_names.push_back("price"        );
    _sa_data_names.push_back("volume"       );
    _sa_data_names.push_back("interest"     );
    _sa_data_names.push_back("zen-in-twning");
 
    _sa_world_names.push_back("price"   );
    _sa_world_names.push_back("volume"  );
    _sa_world_names.push_back("interest");
}


//////////////////////////////////////////////////////////////////////////////////
//
// class CQksDCFactory
//
IDataContainer* CQksDCFactory::CreateContainer(const char* container_type, const char* conf_pos)
{
    if (!strcmp(container_type, "ctpmmd-zit")) {
        IDataContainer* pdc = new CCtpmmdZitContainer();
        pdc->SetConfig(_config, conf_pos);
        return pdc;
    }

    return (0);
}


