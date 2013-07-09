#include "StdAfx.h"
#include "summer.h"


CCtpmdDispatcher_summer::CCtpmdDispatcher_summer(const char* name)
    : IDispatcher_summer(name)
    , _curpos(0)
    , _ctp_time_sec(0)
    , _codes(0)
    , _count(0)
    , _first_interest(0)
    , _ctpmmd(0)
    , _max_len(0)
{
}


CCtpmdDispatcher_summer::~CCtpmdDispatcher_summer(void)
{
    clear();
}

void CCtpmdDispatcher_summer::go()
{
    DataBufferStatus* dbs = _reader.get_status();
    MyMarketData**    mydata = _reader.get_mydata();
    _stop = 0;
    const int code_num = _config->getInt("code-setting.amount");

    for (;;) {
        if (_stop || dbs->nCount < 0)
            break;

        for (int i = 0; i < code_num; i++) {
            if (_curpos[i] < dbs[i].nCount) {
                CTPMMD mmd;
                if (!_curpos[i]) {
                    _ctp_time_sec[i] = TimeTrans::get_zeor_of_theday((mydata[i] + dbs[i].nCount - 1)->m_time);
                    ConvertMmd2Ctpmmd((mydata[i] + dbs[i].nCount - 1), &mmd);
                    mmd.fIndex = Time2Index(_ctp_time_sec[i], mmd.m_time);
                    _first_interest[i] = (float)(mydata[i]->OpenInterest);
                } else {
                    ConvertMmd2Ctpmmd((mydata[i] + dbs[i].nCount - 1), _ctp_time_sec[i], &mmd);
                    mmd.relInterest = (float)mmd.OpenInterest - _first_interest[i];
                    mmd.difInterest = (float)(mmd.OpenInterest - (mydata[i] + dbs[i].nCount - 2)->OpenInterest);
                    mmd.difPrice    = mmd.LastPrice    - (float)(mydata[i] + dbs[i].nCount - 2)->LastPrice;
                    mmd.relVolume   = (float)(mmd.Volume       - (mydata[i] + dbs[i].nCount - 2)->Volume);
                    mmd.open_intr   = (float)((mmd.relVolume + mmd.difInterest) / 2);
                    mmd.cover_intr  = (float)((mmd.relVolume - mmd.difInterest) / 2);
                    mmd.weightPrice = (mmd.BidPrice1 * mmd.BidVolume1 + mmd.AskPrice1 * mmd.AskVolume1) / (mmd.BidVolume1 + mmd.AskVolume1);
                }

                if (_count[i] < _max_len) {
                    memcpy(_ctpmmd[i] + _count[i], &mmd, sizeof(CTPMMD));
#ifdef _DEBUG
                    /*
                    TCHAR code[MAX_PATH];
                    CChineseCodeLib::Gb2312ToUnicode(code, MAX_PATH, _codes[i]);
                    MYTRACE(L"%d, %s: %.02f %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
                        _count[i],
                        code,
                        (_ctpmmd[i] + _count[i])->difPrice,
                        (_ctpmmd[i] + _count[i])->LastPrice - (_ctpmmd[i] + _count[i])->weightPrice,
                        (_ctpmmd[i] + _count[i])->relVolume,
                        (_ctpmmd[i] + _count[i])->relInterest,
                        (_ctpmmd[i] + _count[i])->difInterest,
                        (_ctpmmd[i] + _count[i])->open_intr,
                        (_ctpmmd[i] + _count[i])->cover_intr,
                        (_ctpmmd[i] + _count[i])->m_time);
                    */
#endif //_DEBUG

                    if (!i) //send first code
                        dispatch_data(_ctpmmd[i] + _count[i]);

                    _count[i]++;
                }
                _curpos[i] = dbs[i].nCount;
            }
            Sleep(10);
        }
    }
    _stop = 1;
}

void CCtpmdDispatcher_summer::stop()
{
    _stop = 1;
}

int CCtpmdDispatcher_summer::get_code_num()
{
    if (!_config)
        return 0;
    else
        return _config->getInt("code-setting.amount");
}

void CCtpmdDispatcher_summer::clear()
{
    _reader.clear();

    SafeDelete(_curpos);
    SafeDelete(_count);
    SafeDelete(_ctp_time_sec);

    if (_codes) {
        for (int i = 0; i < _config->getInt("code-setting.amount"); i++) {
            SafeDelete(_codes[i]);
        }
        delete _codes;
        _codes = 0;
    }

    SafeDelete(_first_interest);

    if (_ctpmmd) {
        for (int i = 0; i < _config->getInt("code-setting.amount"); i++)
            SafeDelete(_ctpmmd[i]);
        delete _ctpmmd;
        _ctpmmd = 0;
    }
}

bool CCtpmdDispatcher_summer::init()
{
    if (!_config)
        return false;

    clear();

    if (_reader.init((Poco::Util::LayeredConfiguration*)_config) < 0) {
        return false;
    }

    _curpos = new int[_config->getInt("code-setting.amount")];
    memset(_curpos, 0, sizeof(int) * _config->getInt("code-setting.amount"));
    
    _ctp_time_sec = new CTPTIMESEC[_config->getInt("code-setting.amount")];
    memset(_ctp_time_sec, 0, sizeof(CTPTIMESEC) * _config->getInt("code-setting.amount"));

    _count = new int[_config->getInt("code-setting.amount")];
    memset(_count, 0, sizeof(int) * _config->getInt("code-setting.amount"));

    _first_interest = new float[_config->getInt("code-setting.amount")];
    *_first_interest = 0;

    _ctpmmd = new CTPMMD*[_config->getInt("code-setting.amount")];
    memset(_ctpmmd, 0, sizeof(CTPMMD*) * _config->getInt("code-setting.amount"));

    _max_len = _config->getInt("memfile-setting.maxdata-size");

    _codes = new char*[_config->getInt("code-setting.amount")];
    for (int i = 0; i < _config->getInt("code-setting.amount"); i++)
    {
         char code_conf[MAX_PATH];
         _snprintf_s(code_conf, MAX_PATH, _TRUNCATE, "code-setting.code-%d.code", i + 1);
         _codes[i] = strdup(_config->getString(code_conf).c_str());

        _ctpmmd[i] = new CTPMMD[_max_len];
        memset(_ctpmmd[i], 0, sizeof(CTPMMD) * _max_len);
    }

    return (true);
}

