#include "StdAfx.h"
#include "qks_incs.h"

namespace WARMGUI {


CCtpDispatcher::CCtpDispatcher(const char* name)
    : IDataDispatcher(name)
    , _count(0)
    , _prev_interest(0)
    , _prev_volume(0)
    , _mfStatus(0)
    , _mfData(0)
{
}

CCtpDispatcher::~CCtpDispatcher(void)
{
}

bool CCtpDispatcher::init()
{
    try {
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "ctp-market.ctpmdif-config-file");
        std::string str = _config->getString(temp);
        const char* config_path = str.c_str();
        if (get_configure(config_path, &_profile, "ctp_dispatcher"))
            return false;
    }catch(...){
        MYTRACE(L"CCtpDispatcher::init()\n");
        return false;
    }

    if (OpenMemSharedFile()) {
        return false;
    }

	MyMarketData   * pdata = (MyMarketData   * )_mfData->GetDataPtr();
	DataBufferStatus * dbs = ((DataBufferStatus*)_mfStatus->GetDataPtr());
	if (!pdata || !dbs)
		return false;

    return true;
}


void CCtpDispatcher::go()
{
	MyMarketData   * pdata = (MyMarketData   * )_mfData->GetDataPtr();
	DataBufferStatus * dbs = ((DataBufferStatus*)_mfStatus->GetDataPtr());

	int nIndex = 0, _count = 0;
	for (;;) {
		if (_stop || ! _mfStatus || dbs->nCount < 0)
            break;

		if (nIndex < dbs->nCount - 1) {
			nIndex = dbs->nCount - 1;
            if (!IGotANewData(pdata + nIndex))
                break;
		}
        ++_count;
		Sleep(10);
	}
}

bool CCtpDispatcher::IGotANewData(MyMarketData* mmd)
{
    if (!_count) {
	    g_ctpsec.st1 = TimeTrans::make_time_in_same_day((time_t)mmd->m_time,  9, 15),
	    g_ctpsec.et1 = TimeTrans::make_time_in_same_day((time_t)mmd->m_time, 11, 30),
	    g_ctpsec.st2 = TimeTrans::make_time_in_same_day((time_t)mmd->m_time, 13, 00),
	    g_ctpsec.et2 = TimeTrans::make_time_in_same_day((time_t)mmd->m_time, 15, 15);

        _prev_interest = (float)mmd->OpenInterest;
        _prev_volume   = (float)mmd->Volume;
    }

    CTPMMD ctpmmd;
    ConvertMmd2Ctpmmd(mmd, g_ctpsec, &ctpmmd);
    ctpmmd.relVolume = ctpmmd.Volume - _prev_volume;
    ctpmmd.relInterest = ctpmmd.OpenInterest - _prev_interest;
    _prev_volume = ctpmmd.Volume, _prev_interest = ctpmmd.OpenInterest;

    DataObject data(sizeof(CTPMMD) + sizeof(int), MARKET_DATA_TYPE_CTPMMD);
    char* buf = (char*)data.GetData();
    *(int*)buf = (int)MARKET_DATA_TYPE_CTPMMD;
    memcpy(buf + sizeof(int), &ctpmmd, sizeof(CTPMMD));
    
    //CTPMMD* pmmd = (CTPMMD*)(buf+sizeof(int));
    //MYTRACE(L"%.02f, %.02f, %.02f\n", pmmd->fIndex, pmmd->LastPrice, pmmd->m_time);
    
    DataObjectPtr dop = DataObjectPtr(&data, true);
    GLYPH_CHANGED_TYPE change = GLYPH_CHANGED_TYPE_NONE;
    for (DCConstIter iter = _dcs.begin(); iter != _dcs.end(); iter++) {
        //TCHAR name[MAX_PATH];
        //CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, (*iter)->getName());
        //MYTRACE(L"container name is %s\n", name);
        GLYPH_CHANGED_TYPE c = (*iter)->AddData(dop);
        change = (GLYPH_CHANGED_TYPE)((int)change | (int)c);
    }

    ///if you do not want crash the program, delete this line
    for (size_t i = 0; i < this->_hwnd_array.size(); i++)
        SendMessage(_hwnd_array[i], WM_RENEW_WINDOW, (WPARAM)change, 0);
    ++_count;
    return true;
}


/*    if (OpenMemSharedFile()) {
		printf("open memory file was bad\n");
		return;
	}

	_rtc.init((MyMarketData*)_mfData->GetDataPtr());

	if (!_ts.init_server(config())) {
		
	} else {*/

int CCtpDispatcher::CloseMemSharedFiles()
{
	if (_mfData) {
		_mfData->CloseFile();
		delete _mfData;
		_mfData = 0;
	}

	if (_mfStatus) {
		_mfStatus->CloseFile();
		delete _mfStatus;
		_mfStatus = 0;
	}

	return (0);
}

int CCtpDispatcher::OpenMemSharedFile()
{
	_mfData = new CMemShareFile;
	if (!_mfData)
		return (-1);

	char msfile[MAX_PATH];
	_snprintf_s(msfile, MAX_PATH, _TRUNCATE,
		"%s-%s",
		_profile.m_strMemSharedPrefix,
		_profile.m_strCodes[0]);

	if (!(_mfData->OpenReadFile(msfile))) {
		CloseMemSharedFiles();
		return (-2);
	}

	//mylog(_ctp_profile.m_strMDLogfile, L_INFO, "Open %s to read %s data.", msfile, _ctp_profile.m_strCodes[0]);

	_mfStatus = new CMemShareFile;
	if (!_mfStatus) {
		CloseMemSharedFiles();
		return (-3);
	}

	if (!(_mfStatus->OpenReadFile(_profile.m_strMemSharedStatusFile))) {
		//mylog(_ctp_profile.m_strMDLogfile, L_INFO, "Can Not Open %s to read data.", _ctp_profile.m_strMemSharedStatusFile);
		CloseMemSharedFiles();
		return (-4);
	}
	//mylog(_ctp_profile.m_strMDLogfile, L_INFO, "Open %s to read status and conters.", _ctp_profile.m_strMemSharedStatusFile);

	return (0);
}


void CCtpDispatcher::stop()
{
    _stop = 1;
    try {
        _thread.join(1000);
    }catch(...){
        MYTRACE(L"CCtpDispatcher::stop catch\n");
    }
    CloseMemSharedFiles();
}

int  CCtpDispatcher::MessageProcess(MSG msg)
{
    return (0);
}

void CCtpDispatcher::SetConfig(CWarmguiConfig* config, const char* cnf_pos)
{
    IDataDispatcher::SetConfig(config, cnf_pos);
    //set other 
}


} //namespace WARMGUI