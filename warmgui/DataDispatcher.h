#ifndef __interface_data_dispatcher_h__
#define __interface_data_dispatcher_h__


#define MSG_EXIT_DISPATCHER 0x0FFFFFF

namespace WARMGUI {

class WARMGUI_API IDataDispatcher : public Poco::Runnable
{
public:
    IDataDispatcher(const char* name);
    virtual ~IDataDispatcher(void);

	Poco::Thread::TID GetThreadId() {
		for(;;) {
			if (!_tid)
				Poco::Thread::sleep(100);
			else
				return _tid;
		}
	}

    inline void  reg_data_container(IDataContainer* dc)
                 {
#                    ifdef _DEBUG
                     TCHAR name1[MAX_PATH], name2[MAX_PATH];
                     CChineseCodeLib::Gb2312ToUnicode(name1, MAX_PATH, dc->getName());
                     CChineseCodeLib::Gb2312ToUnicode(name2, MAX_PATH, _name);
                     MYTRACE(L"contanier %s regestered to dispatcher %s\n", name1, name2);
#                    endif
                     _dcs.push_back(dc);
                 }
    inline void  reg_calculator(ICalculator* cal)     {_cals.push_back(cal);}

    //inline void  remove_dc(IDataContainer* dc);
    //inline void  clear_dc();
    //inline bool  SetDataContainer(CWarmguiConfig* config);
    //inline bool  RegisterCanvasToContainer(IDataCanvas* canvas, int container_no);

    virtual void SetConfig(CWarmguiConfig* config, const char* cnf_pos)
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

    HwndArray& get_hwnd_array() {return _hwnd_array;}
    void register_hwnd(HWND hwnd) { _hwnd_array.push_back(hwnd); }
    void remove_hwnd(HWND hwnd) { 
        for (HwndConstIter iter = _hwnd_array.begin(); iter != _hwnd_array.end(); iter++) {
            if ((*iter) == hwnd) {
                _hwnd_array.erase(iter);
                break;
            }
        }
    }

protected:
	DWORD              _tid;
    DCArray            _dcs;
    CalculatorArray   _cals;
    CWarmguiConfig* _config;
    char _cnf_pos[MAX_PATH];
	Poco::Thread    _thread;
    int               _stop;
    HwndArray   _hwnd_array;

public:
    char    _name[MAX_PATH];
};

EXPORT_STL_VECTOR(WARMGUI_API, IDataDispatcher*)
typedef std::vector<IDataDispatcher*>   DispatcherArray;
typedef DispatcherArray::iterator       DispatcherIter;
typedef DispatcherArray::const_iterator DispatcherConstIter;

class WARMGUI_API CDispatchers: public DispatcherArray
{
public:
    CDispatchers() {}
    ~CDispatchers() { ReleaseAll(); }

    IDataDispatcher* getDispatcher(const char* name){
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
};

class WARMGUI_API IDataDispatcherCreator
{
public:
                             IDataDispatcherCreator() : _config(0) {}
                            ~IDataDispatcherCreator(){}

    void                     SetConfigure(CWarmguiConfig* config) { _config = config; }
    virtual IDataDispatcher* CreateDispathcer(const char* conf_pos) = 0;

protected:
    CWarmguiConfig* _config;
};


} //namespace WARMGUI

#endif //__interface_data_dispatcher_h__
