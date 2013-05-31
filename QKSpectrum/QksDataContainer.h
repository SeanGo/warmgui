#ifndef __qks_data_container_h__
#define __qks_data_container_h__


typedef struct ZitData {
   	EXTRINFO    extCentral;
	INFLGRP       inflBest;
	CENTRAL_SERIES central;
	LHAREA             lha;
    int            data_ds;


    ZitData()
        :data_ds(0)
    {
        memset(&extCentral, 0, sizeof(extCentral));
        memset(&inflBest  , 0, sizeof(inflBest  ));
        memset(&central   , 0, sizeof(central   ));
        memset(&lha       , 0, sizeof(lha       ));


        extCentral.extremum = new EXTREMUM[200];
        inflBest.infl       = new INFLEXION[100];
        central.central     = new CENTRAL[200];
        lha.lha             = new L_H_AREA[200];
    }

    ~ZitData()
    {
        SafeDelete(extCentral.extremum);
        SafeDelete(inflBest.infl);
        SafeDelete(central.central);
        SafeDelete(lha.lha);
    }

    bool AppendData(DataObjectPtr dopNewData);
} ZitData;

//////////////////////////////////////////////////////////////////////////////////
//
// class CCtpmmdZitContainer
//
class CCtpmmdZitContainer : public WARMGUI::IDataContainer
{
public:
    CCtpmmdZitContainer()
        :       _ctpmmd(0)
        ,    _ts_length(0)
        , _down_sampler(0)
    {
    }

    ~CCtpmmdZitContainer()
    {
        SafeDelete(_ctpmmd);
    }

    ///get the configureation of data-container
    virtual void                  SetConfig(CWarmguiConfig* config, const char* cnf_pos);
    virtual bool                  NewData(DataObjectPtr dop);
    virtual dataptr               getDataByName(size_t* nsize, DATA_OWN_TYPE* owntype, const char* dataname/* = 0*/);
    inline  virtual WORLD_RECT*   getWorldByName(size_t* nsize, const char* dataname/* = 0*/);
    const CTPMMD*                 GetRealtimCtpdata() { return _ctpmmd; }
    const CTPMMD_TIME_SERIES_FLT* GetTimeSeries() { return _ts.GetTimeSeries(); }
    const ZitData*                GetZitData()    { return &_zit_data; }

    const int                     GetDownSamplerInterval() { return _down_sampler; }
    inline virtual void           RegisterNames();
protected:
    CTPMMD*       _ctpmmd;
    CFutureDataSeries _ts;
    ZitData     _zit_data;
    int        _ts_length;
    int     _down_sampler;
};


//////////////////////////////////////////////////////////////////////////////////
//
// class CQksDCFactory
//
class CQksDCFactory : public WARMGUI::CContainerFactory
{
public:
    CQksDCFactory(void) {}

    virtual IDataContainer* CreateContainer(const char* container_type, const char* conf_pos);

};



#endif //__qks_data_container_h__