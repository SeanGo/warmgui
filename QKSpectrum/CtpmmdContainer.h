#ifndef __ctpmmd_data_container_h__
#define __ctpmmd_data_container_h__


//////////////////////////////////////////////////////////////////////////////////
//
// class CCtpmmdContainer
//
class CCtpmmdContainer : public WARMGUI::IDataContainer
{
public:
                       CCtpmmdContainer(const char* name)
                           : IDataContainer(name)
                           , _count(0), _data_len(0), _sizeofdata(0), _ctpmmd(0), _curpos(0), _fist_data_pos(0)
                           { }
                      ~CCtpmmdContainer() { SafeDelete(_ctpmmd); }

    ///get the configureation of data-container
    virtual void       SetConfig(CWarmguiConfig* config, const char* cnf_pos);
    virtual bool       NewData(DataObjectPtr dop);
    const CTPMMD*      getCurrentData() { CriticalLock::Scoped scope(_lockCount);  return _ctpmmd + _curpos; }
    CTPMMD*            getDataPtr()     { CriticalLock::Scoped scope(_lockCount);  return _ctpmmd + _fist_data_pos; }
    size_t             getCurrentIndex(){ CriticalLock::Scoped scope(_lockCount);  return  _count - 1; }
    size_t             getCount()       { CriticalLock::Scoped scope(_lockCount);  return      _count; }
    size_t             getDataSize()    { return _sizeofdata; }
    size_t             getDataLength()  { return   _data_len; }
private:
    //just set the min and max, because the x0, y0, xn, yn will set by canvas or graph
    void               renew_world();
    void               add_count() 
                       {
                           CriticalLock::Scoped scope(_lockCount);
                            ++_count ;
                       }
    void               add_curpos(CTPMMD* ctpmmd)
                       {
                           CriticalLock::Scoped scope(_lockCount);
                           ++_curpos;
                           memcpy(_ctpmmd + _curpos, ctpmmd, sizeof(CTPMMD));
                       }
protected:
    CTPMMD*             _ctpmmd;
    size_t            _data_len;
    size_t               _count;
    size_t          _sizeofdata;
    size_t              _curpos;
    size_t       _fist_data_pos;
	CriticalLock     _lockCount;
};



#endif //__ctpmmd_data_container_h__