#ifndef __interface_data_dispatcher_h__
#define __interface_data_dispatcher_h__


#define MSG_EXIT_DISPATCHER 0x0FFFFFF

namespace WARMGUI {

class WARMGUI_API IDispatcher_summer : public Poco::Runnable
{
public:
    IDispatcher_summer(const char* name);
    virtual ~IDispatcher_summer(void);

	Poco::Thread::TID GetThreadId() {
		for(;;) {
			if (!_tid)
				Poco::Thread::sleep(100);
			else
				return _tid;
		}
	}

    inline void  reg_data_graph(IDataGraph_summer* dg)   { register_atelier(dg->get_atelier()); _dgraph.push_back(dg); }
    inline void  reg_calculator(ICalculator_summer* cal)     {_cals.push_back(cal);}

    //inline void  remove_dc(IDataContainer* dc);
    //inline void  clear_dc();
    //inline bool  SetDataContainer(CWarmguiConfig* config);
    //inline bool  RegisterCanvasToContainer(IDataCanvas* canvas, int container_no);

    virtual void set_config(CWarmguiConfig* config, const char* cnf_pos)
                 {_config = config; strcpy_s(_cnf_pos, MAX_PATH, cnf_pos);}

    virtual void start() {
        if (_stop)
            _thread.start(*(this));
    }

    virtual void run() {
        if (!init()) return;
      	_tid = Poco::Thread::currentTid();
        _stop = 0;
        go();
    }
    virtual void go()   = 0;
    virtual void stop() = 0;
    virtual bool init() {return true;}
    bool         isme(const char* nemas) 
                 { return (0 != strstr(nemas, _name)); }


    inline void  dispatch_data(DataObjectPtr dop);
    inline void  dispatch_data(dataptr data);

    AtelierArray_summer& get_atlier_array() {return _atelier_array;}
    void register_atelier(IAtelier_summer* as);
    void remove_atelier(IAtelier_summer* as) { 
        for (AtelierConstIter_summer iter = _atelier_array.begin(); iter != _atelier_array.end(); iter++) {
            if ((*iter) == as) {
                _atelier_array.erase(iter);
                break;
            }
        }
    }

protected:
	DWORD                  _tid;
    CalculatorArray_summer _cals;
    DataGraphArray_summer  _dgraph;

    CWarmguiConfig*        _config;
    char                   _cnf_pos[MAX_PATH];
	Poco::Thread           _thread;
    int                    _stop;
    AtelierArray_summer    _atelier_array;

public:
    char    _name[MAX_PATH];
};

EXPORT_STL_VECTOR(WARMGUI_API, IDispatcher_summer*)
typedef std::vector<IDispatcher_summer*>   DispatcherArray;
typedef DispatcherArray::iterator       DispatcherIter;
typedef DispatcherArray::const_iterator DispatcherConstIter;

class WARMGUI_API CDispatchers: public DispatcherArray
{
public:
    CDispatchers() {}
    ~CDispatchers() { ReleaseAll(); }

    IDispatcher_summer* getDispatcher(const char* name){
        for (DispatcherConstIter iter = begin(); iter != end(); iter++) {
            if (!strcmp((*iter)->_name, name))
                return (*iter);
        }
        return (0);
    }

    void ReleaseAll()
    {
        for (DispatcherConstIter iter = begin(); iter != end(); iter++)
             delete (*iter);
        clear();
    }

    void start()
    {
        for (DispatcherConstIter iter = begin(); iter != end(); iter++)
             (*iter)->start();
    }

    void stop()
    {
        for (DispatcherConstIter iter = begin(); iter != end(); iter++)
             (*iter)->stop();
    }

    bool reg_graph(IDataGraph_summer* graph, const char* name)
    {
        for (DispatcherConstIter iter = begin(); iter != end(); iter++)
             if ((*iter)->isme(name)) {
                 (*iter)->reg_data_graph(graph);
                 return true;
             }
        return false;
    }
};

class WARMGUI_API IDataDispatcherCreator
{
public:
                             IDataDispatcherCreator() : _config(0) {}
                            ~IDataDispatcherCreator(){}

    void                     SetConfigure(CWarmguiConfig* config) { _config = config; }
    virtual IDispatcher_summer* CreateDispathcer(const char* conf_pos) = 0;

protected:
    CWarmguiConfig* _config;
};


} //namespace WARMGUI

#endif //__interface_data_dispatcher_h__
