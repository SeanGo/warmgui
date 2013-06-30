#include "StdAfx.h"
#include "qks_incs.h"


CPredictorCalculator::CPredictorCalculator(const char* name)
    : ICalculator(name)
    , _ctpmmd(0)
    ,   _hwnd(0)
{
    strcpy_s(_name, MAX_PATH, name);
}


CPredictorCalculator::~CPredictorCalculator(void)
{
}

int CPredictorCalculator::calculate(MSG msg)
{
    if ((DataObject::MARKET_DATA_TYPE)msg.lParam == MARKET_DATA_TYPE_CTPMMD) {
        CCtpmmdContainer* cc = (CCtpmmdContainer*)msg.wParam;
        if (!cc)
            return (-1);

#   ifdef _DEBUG
        //const CTPMMD* pctpmmd = cc->getCurrentData();
        //MYTRACE(L"predict: %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
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
            _prdtor.SetMyMarketDataPtr(_ctpmmd, 32400);
        }

        int npos = cc->getCount() - 1;
        if (_prdtor.is_trade_time(npos)) {
            _prdtor.CalucShortAnalyse(npos);
            _prdtor.CalucLongAnalyse(npos);
            _prdtor.AddDataCount();
    		
            DataObject* dataobj = _prdtor.CopyResult();
            if (dataobj) {
                //MYTRACE(L"Hello %d\n", npos);
                DataObjectPtr dop(dataobj);
                {
                    size_t len = dop->GetDataLength() - (size_t)FIXED_COMM_HEAD;
                    if (len) {
                        char* buf = (char*)dop->GetData() + FIXED_COMM_HEAD;
                        GLYPH_CHANGED_TYPE change = AddData(buf, len, DataObject::EUCLID_CALUC_DBL_FILTER_PREDICTOR);
                        
                        for (size_t i = 0; i < this->_hwnd_array.size(); i++)
                            SendMessage(_hwnd_array[i], WM_RENEW_WINDOW, (WPARAM)change, 0);
                    }
                }
            }
		}

    }
    return (0);
}

void CPredictorCalculator::start()
{
    if (_stop)
        _thread.start(*(this));
}

void CPredictorCalculator::stop()
{
    _stop = 1;
    try {
        _thread.join(1000);
    }catch(...){
        MYTRACE(L"CCtpDispatcher::stop catch\n");
    }
}

bool CPredictorCalculator::init()
{
    _prdtor.SetConfig(WAVELET_TYPE_SYM, 7, 9, 1024, 180, 40);
    _prdtor.SetDataReceiver(1024, 2048, 2, 2.0f);
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

