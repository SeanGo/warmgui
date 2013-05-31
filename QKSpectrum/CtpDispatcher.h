#ifndef __ctp_dispatcher_h__
#define __ctp_dispatcher_h__

namespace WARMGUI {

class CCtpDispatcher : public WARMGUI::IDataDispatcher
{
public:
                 CCtpDispatcher(const char* name);
                ~CCtpDispatcher(void);

    virtual bool init();
    virtual void go();
    virtual void stop();

protected:
    virtual int  MessageProcess(MSG msg);
    virtual void SetConfig(CWarmguiConfig* config, const char* cnf_pos);

	int CloseMemSharedFiles();
	int OpenMemSharedFile();

    bool IGotANewData(MyMarketData* mmd);
private:
	CMemShareFile*   _mfData;
	CMemShareFile* _mfStatus;
    CtpmdifProfile  _profile;

    size_t            _count;

    float     _prev_interest;
    float       _prev_volume;

};

} //namespace WARMGUI

#endif //__ctp_dispatcher_h__

