#include "StdAfx.h"
#include "qks_incs.h"

CEuclidCalculator::CEuclidCalculator(const char* name)
    : ICalculator(name)
    , _ctpmmd(0)
    ,   _hwnd(0)
{
    strcpy_s(_name, MAX_PATH, name);
}


CEuclidCalculator::~CEuclidCalculator(void)
{
}


int CEuclidCalculator::calculate(MSG msg)
{
    if ((DataObject::MARKET_DATA_TYPE)msg.lParam == DataObject::MARKET_DATA_TYPE_CTPMMD) {
        CCtpmmdContainer* cc = (CCtpmmdContainer*)msg.wParam;
        if (!cc)
            return (-1);

#   ifdef _DEBUG
        //const CTPMMD* pctpmmd = cc->getCurrentData();
        //MYTRACE(L"euclid: %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
        //    pctpmmd->fIndex,
        //    pctpmmd->m_time,
        //    pctpmmd->LastPrice,
        //    pctpmmd->Volume,
        //    pctpmmd->relVolume,
        //    pctpmmd->OpenInterest,
        //    pctpmmd->relInterest);
#   endif //_DEBUG
        if (!_ctpmmd) {
            _ctpmmd = cc->getDataPtr();
            _rtc.SetMyMarketDataPtr(_ctpmmd, 32400);
        }

        int npos = cc->getCount() - 1;
        if (_rtc.is_trade_time(npos)) {
            _rtc.CalucCentral(npos);
            _rtc.AddDataCount();
    		
            DataObject* dataobj = _rtc.CopyResult();
            DataObjectPtr dop(dataobj);
            {
                char* buf = (char*)dop->GetData();
                size_t len = dop->GetDataLength();
                //MYTRACE(L"zit-data %d\n", npos);
                if (_zit_data.AppendData(buf, len))
                {
                    if (_zit_data.extCentral.nAllNum)
                    {
                        //MYTRACE(L"zit-data GOT ZIT %d\n", npos);
                        GLYPH_CHANGED_TYPE change = AddData(&_zit_data, len, DataObject::MARKET_DATA_TYPE_ZITDATA);
                        
                        for (size_t i = 0; i < this->_hwnd_array.size(); i++)
                            SendMessage(_hwnd_array[i], WM_RENEW_WINDOW, (WPARAM)change, 0);
                    }
                }
            }
		}

    }
    return (0);
}

void CEuclidCalculator::start()
{
    if (_stop)
        _thread.start(*(this));
}

void CEuclidCalculator::stop()
{
    _stop = 1;
    try {
        _thread.join(1000);
    }catch(...){
        MYTRACE(L"CCtpDispatcher::stop catch\n");
    }
}

bool CEuclidCalculator::init()
{
    _rtc.InitObjects();
    return true;
}



#define move_to_next_data(l) \
{\
    len -= (l);\
    if (len > 0)\
        buf += (l);\
    else\
        return false;\
}


#define move_to_next_with_result(l) \
{\
    len -= (l);\
    if (len > 0)\
        buf += (l);\
    else if (!len)\
        return true;\
    else\
        return false;\
}


///////////////////////////////////////////////////////////////////////////////////////////
// struct ZitData


bool ZitData::AppendData(dataptr zitdata, size_t length)
{
    reset();
    bool result = false;

    if (length < FIXED_COMM_HEAD + sizeof(int) + sizeof(int))
        return result;

    //MYTRACE(L"ZitData::AppendData 1\n");
    size_t len = length;
    char*  buf = (char*)zitdata;

    //DataObject::EUCLID_CALUC_TO_ZITDATA
#ifdef _DEBUG
//    DataObject::MARKET_DATA_TYPE the_datatype = (DataObject::MARKET_DATA_TYPE)(*(int*)buf);
#endif //_DEBUG
    //MYTRACE(L"ZitData::AppendData 2\n");
    move_to_next_data(sizeof(int));

    //MYTRACE(L"ZitData::AppendData 3\n");
    move_to_next_data(FIXED_COMM_HEAD);

    //down sampler
    data_ds = *(int*)buf;
    //MYTRACE(L"ZitData::AppendData 4\n");
    move_to_next_with_result(sizeof(int));

    //get the number of extreme
    extCentral.nAllNum = *(int*)buf;
	if (extCentral.nAllNum < 0 || extCentral.nAllNum > 200)
        return result;

    //MYTRACE(L"ZitData::AppendData 5\n");
    //get the extremes
    move_to_next_data(sizeof(int));
    memcpy(extCentral.extremum, buf, sizeof(EXTREMUM) * extCentral.nAllNum);

    //MYTRACE(L"ZitData::AppendData 6\n");
    //get the number of inflexion
    move_to_next_data(sizeof(EXTREMUM) * extCentral.nAllNum);
    inflBest.nNum = *(int*) buf;
    if (inflBest.nNum < 0 || inflBest.nNum > 100)
        return result;

    //MYTRACE(L"ZitData::AppendData 7\n");
    //get the inflexions
    move_to_next_data(sizeof(int));
    memcpy(inflBest.infl, buf, sizeof(INFLEXION) * inflBest.nNum);
    
    //MYTRACE(L"ZitData::AppendData 8\n");
    //get the number of central 
    move_to_next_data(sizeof(INFLEXION) * inflBest.nNum);
    central.nNum = *(int*)buf;
    if (central.nNum < 0 || central.nNum > 200)
        return result;
    
    //MYTRACE(L"ZitData::AppendData 9\n");
    //get centrals
    move_to_next_data(sizeof(int));
    memcpy(central.central, buf, sizeof(CENTRAL) * central.nNum);
    
    //MYTRACE(L"ZitData::AppendData 10\n");
    //get number of the area
    move_to_next_data(sizeof(CENTRAL) * central.nNum);
    lha.nNum = *(int*)buf;
    if (lha.nNum < 0 || lha.nNum > 200)
        return result;

    //MYTRACE(L"ZitData::AppendData 11\n");
    //get the areas
    move_to_next_data(sizeof(int));
    memcpy(lha.lha, buf, sizeof(L_H_AREA) * lha.nNum);
    
#   ifdef _DEBUG
    //MYTRACE(L"ZIT-DATA: extremum %d, infl %d, central %d, area %d\n",
    //    extCentral.nAllNum, 
    //    inflBest.nNum,
    //    central.nNum,
    //    lha.nNum);
#   endif //_DEBUG
    move_to_next_with_result(sizeof(L_H_AREA) * lha.nNum);

    int prdt_len = *(int*)buf;
    move_to_next_data(sizeof(int));
    memcpy(this->_short_prdt, buf, sizeof(double) * prdt_len);
    //MYTRACE(L"short predict %d\n", prdt_len );
    _has_short_prdt = true;
    move_to_next_with_result(sizeof(double) * prdt_len);

    prdt_len = *(int*)buf;
    move_to_next_data(sizeof(int));
    memcpy(this->_long_prdt , buf, sizeof(double) * prdt_len);
    _has_long_prdt = true;
    //MYTRACE(L"long predict %d\n", prdt_len );
    move_to_next_with_result(sizeof(double) * prdt_len);

    return (true);
}




