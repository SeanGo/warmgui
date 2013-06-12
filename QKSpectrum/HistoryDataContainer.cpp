#include "StdAfx.h"
#include "qks_incs.h"

void CHistoryDataContainer::SetConfig(CWarmguiConfig* config, const char* cnf_pos)
{
    WARMGUI::IDataContainer::SetConfig(config, cnf_pos);

    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.rt-path", cnf_pos);
    CChineseCodeLib::Gb2312ToUnicode(_rt_path, MAX_PATH, _config->getString(temp).c_str());

    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.m1-path", cnf_pos);
    CChineseCodeLib::Gb2312ToUnicode(_1m_path, MAX_PATH, _config->getString(temp).c_str());

    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.m5-path", cnf_pos);
    CChineseCodeLib::Gb2312ToUnicode(_5m_path, MAX_PATH, _config->getString(temp).c_str());

    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.m15-path", cnf_pos);
    CChineseCodeLib::Gb2312ToUnicode(_15m_path, MAX_PATH, _config->getString(temp).c_str());
}

int CHistoryDataContainer::OpenTickData(const TCHAR* filepath)
{
    return (int)_rtdata.open_data(filepath, 0);
}

int CHistoryDataContainer::OpenTickData(const TCHAR* code, const TCHAR* date)
{
    TCHAR filepath[MAX_PATH];
    _sntprintf_s(filepath, MAX_PATH, _TRUNCATE, L"%s\\%s-%s.dat", _rt_path, code, date);

    return (int)_rtdata.open_data(filepath, 0);
}

int CHistoryDataContainer::OpenHistoryData(int down_sample_level)
{
    return (0);
}
