#ifndef __qks_data_dispatcher_h__
#define __qks_data_dispatcher_h__

class CQksDataDispatcher : public WARMGUI::IDataDispatcher
{
public:
                 CQksDataDispatcher(const char* name) : IDataDispatcher(name) {}

    
    virtual void go();
    virtual void stop();
    virtual void SetConfig(CWarmguiConfig* config, const char* cnf_pos);

protected:
    int  MessageProcess(MSG msg);
};


#endif //__qks_data_dispatcher_h__