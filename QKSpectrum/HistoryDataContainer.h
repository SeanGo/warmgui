#ifndef __history_data_container_h__
#define __history_data_container_h__


class CHistoryDataContainer : public WARMGUI::IDataContainer
{
public:
    CHistoryDataContainer(const char* name)
        : IDataContainer(name)
    {
        *_rt_path = *_1m_path = *_5m_path = *_15m_path = L'\0';
    }

    ~CHistoryDataContainer(void)
    {
    }


    ///get the configureation of data-container
    virtual void SetConfig(CWarmguiConfig* config, const char* cnf_pos);
    virtual bool NewData(DataObjectPtr dop){return true;}

    int OpenTickData(const TCHAR* code, const TCHAR* date);
    int OpenTickData(const TCHAR* filepath);
    int OpenHistoryData(int down_sample_level);

    TCHAR* get_history_data_path() { return _rt_path; }
protected:
    CTPMMD_DATA  _rtdata;
    CTPMMD_DATA  _1mdata;
    CTPMMD_DATA  _5mdata;
    CTPMMD_DATA _15mdata;
    TCHAR     _rt_path[MAX_PATH];
    TCHAR     _1m_path[MAX_PATH];
    TCHAR     _5m_path[MAX_PATH];
    TCHAR    _15m_path[MAX_PATH];
};


#endif //__history_data_container_h__