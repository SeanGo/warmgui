#ifndef __zit_container_h__
#define __zit_container_h__


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class
class CZitdataContainer : public WARMGUI::IDataContainer
{
public:
                    CZitdataContainer(const char* name)
                        : IDataContainer(name)
                        , _down_sampler(0), _ts_length(0), _ts_add_len(0){}
                   ~CZitdataContainer(){}

    virtual void    SetConfig(CWarmguiConfig* config, const char* cnf_pos) {}
    const ZitData*  GetZitData()    { return &_zit_data; }

protected:
    virtual bool    NewData(DataObjectPtr dop);

    ZitData  _zit_data;
    int  _down_sampler;
    int     _ts_length;
    int    _ts_add_len;
};



#endif //__zit_container_h__