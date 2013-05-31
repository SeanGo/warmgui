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

    bool AppendData(dataptr zitdata, size_t length);
} ZitData;

//////////////////////////////////////////////////////////////////////////////////
//
// class CCtpmmdZitContainer
//
class CCtpmmdZitContainer : public WARMGUI::IDataContainer
{
public:
                       CCtpmmdZitContainer() : _count(0), _data_len(0), _sizeofdata(0), _ctpmmd(0), _curpos(0) { }
                      ~CCtpmmdZitContainer() { SafeDelete(_ctpmmd); }

    ///get the configureation of data-container
    virtual void       SetConfig(CWarmguiConfig* config, const char* cnf_pos);
    virtual bool       NewData(DataObjectPtr dop);
    const CTPMMD*      getCurrentData() { return _ctpmmd + _curpos; }
    size_t             getCurrentIndex(){ return  _count - 1; }
    size_t             getCount()       { return      _count; }
    size_t             getDataSize()    { return _sizeofdata; }
    size_t             getDataLength()  { return   _data_len; }
    FUTURE_DATA_RANGE& getDataRange()   { return      _world; }
private:
    //just set the min and max, because the x0, y0, xn, yn will set by canvas or graph
    void               renew_world();

protected:
    CTPMMD*             _ctpmmd;
    FUTURE_DATA_RANGE    _world;
    size_t            _data_len;
    size_t               _count;
    size_t          _sizeofdata;
    size_t              _curpos;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class
class CZitdataContainer : public WARMGUI::IDataContainer
{
public:
                    CZitdataContainer() : _down_sampler(0), _ts_length(0), _ts_add_len(0){}
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



#endif //__qks_data_container_h__