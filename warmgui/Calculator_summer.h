#ifndef __interface_calculator_h__
#define __interface_calculator_h__

#define WM_CALCULATOR_MESSAGE WM_USER + 7890

namespace WARMGUI {

class ICalculator_summer;
EXPORT_STL_VECTOR(WARMGUI_API, ICalculator_summer*)
typedef std::vector<ICalculator_summer*>    CalculatorArray_summer;
typedef CalculatorArray_summer::iterator            CalIter_summer;
typedef CalculatorArray_summer::const_iterator CalConstIter_summer;


class WARMGUI_API ICalculator_summer : public Poco::Runnable
{
public:
    ICalculator_summer(const char* name)
        : _tid(0), _stop(1), _busy(false),_hThread(0),_result_data(0)
    { strcpy_s(_name, MAX_PATH, name); *_my_cont = 0; }

    virtual ~ICalculator_summer(void) {}

	Poco::Thread::TID GetThreadId() {
		for(;;) {
			if (!_tid)
				Poco::Thread::sleep(100);
			else
				return _tid;
		}
	}

    virtual void run() {
        //if (!init()) return;
      	_tid = Poco::Thread::currentTid();
    	_hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, GetCurrentThreadId());
        _stop = 0;

        MSG msg;
	    BOOL bRet = 1;

	    while(bRet) {
		    bRet = GetMessage( &msg, (HWND)-1, 0, 0);
		    if (bRet == -1) {
			    exit(-10000);
            } else {
                if (_stop)
                    return;
                if (!_busy) {
                    _busy = true;
                    int r = calculate(msg);
                    if (!r)
                        _update_other();
                    _busy = false;
                    if (r < 0)
                        return;
                }
			    DispatchMessage(&msg);
		    }
	    }
    }

    virtual int  calculate(MSG msg) = 0;
    virtual void start() { if (init() && _stop) _thread.start(*(this)); }
    virtual void stop()  {
                            _stop = 1;
                            try {
                                _thread.join(1000);
                            }catch(...){
                                MYTRACE(L"Calculator::stop catch error\n");
                            }
                          }
    virtual bool init() = 0;
    bool         is_busy() { return _busy; }
    bool         isme(const char* nemas) 
                 { return (0 != strstr(nemas, _name)); }

    void SetConfig(CWarmguiConfig* config, const char* strconf)
    {
        _config = config;
        if (strconf)
            strcpy_s(_strconf, MAX_PATH, strconf);
        try {
            //_my_disp;
            char temp[MAX_PATH];
            _snprintf_s(temp, MAX_PATH, "%s.containers", strconf);
            strcpy_s(_my_cont, MAX_PATH, _config->getString(temp).c_str());
        } catch(...) {
        }
    }

    /*
    bool NewData(IDataContainer_summer* data_cont, MARKET_DATA_TYPE datatype)
    {
        ///multi-thread
        /*
        if (!_busy) {
            PostThreadMessage(_tid, WM_CALCULATOR_MESSAGE, (WPARAM)data_cont, (LPARAM)datatype);
            return true;
        } else 
            return false;
        */
        /*MSG msg;
        msg.message = WM_CALCULATOR_MESSAGE;
        msg.wParam  = (WPARAM)data_cont;
        msg.lParam  = (LPARAM)datatype;
        calculate(msg);
        return true;
    }*/

    virtual void update(dataptr data)
    {
        ::PostThreadMessage(this->_tid, WM_CALCULATOR_NEWDATA, (WPARAM)data, 0);
    }

    virtual dataptr get_result() = 0;

    //virtual void NewData(dataptr pdata, MARKET_DATA_TYPE datatype) { return GLYPH_CHANGED_TYPE_NONE;}

    void reg_calculator(ICalculator_summer* calculator)
    {
        _cals.push_back(calculator);
    }

    void reg_data_graph(IDataGraph_summer* graph)
    {
        _glyphs.push_back(graph);
        graph->set_user_data(_result_data);
    }

    const char* getContainerName() {return _my_cont;}
    const char* getName() {return _name;}

    AtelierArray_summer& get_atlier_array() {return _atelier_array;}
    void register_atelier(IAtelier_summer* as) { _atelier_array.push_back(as); }
    void remove_hwnd(IAtelier_summer* as) { 
        for (AtelierConstIter_summer iter = _atelier_array.begin(); iter != _atelier_array.end(); iter++) {
            if ((*iter) == as) {
                _atelier_array.erase(iter);
                break;
            }
        }
    }

    inline void _update_other()
    {
        for (int i = 0; i < _cals.size(); i++) {
            _cals[i]->update(0);
        }

        for (int i = 0; i < _glyphs.size(); i++) {
            _glyphs[i]->update_data();
        }

        for (int i = 0; i < _atelier_array.size(); i++) {
            _atelier_array[i]->redraw_window();
        }
    }


public:
    char      _name[MAX_PATH];


protected:
    Poco::Thread::TID    _tid;
    HANDLE           _hThread;
    int                 _stop;
    bool                _busy;

    CWarmguiConfig*   _config;
    char   _strconf[MAX_PATH];
    char   _my_cont[MAX_PATH];
    CalculatorArray_summer _cals;
    DataGraphArray_summer  _glyphs;
	Poco::Thread           _thread;

    AtelierArray_summer    _atelier_array;
    dataptr                _result_data;
};




EXPORT_STL_VECTOR(WARMGUI_API, ICalculator_summer*)
typedef std::vector<ICalculator_summer*> CalculatorArray;
typedef CalculatorArray::iterator        CalculatorIter;
typedef CalculatorArray::const_iterator  CalculatorConstIter;

class WARMGUI_API CCalculators: public CalculatorArray
{
public:
    CCalculators() {}
    ~CCalculators() { ReleaseAll(); }

    ICalculator_summer* getCalculator(const char* name){
        for (CalculatorConstIter iter = begin(); iter != end(); iter++) {
            if (!strcmp((*iter)->_name, name))
                return (*iter);
        }
        return (0);
    }

    void ReleaseAll()
    {
        for (CalculatorConstIter iter = begin(); iter != end(); iter++)
             delete (*iter);
        clear();
    }

    void start()
    {
        for (CalculatorConstIter iter = begin(); iter != end(); iter++)
             (*iter)->start();
    }

    void stop()
    {
        for (CalculatorConstIter iter = begin(); iter != end(); iter++)
             (*iter)->stop();
    }

    bool reg_graph(IDataGraph_summer* graph, const char* name)
    {
        for (CalculatorConstIter iter = begin(); iter != end(); iter++)
             if ((*iter)->isme(name)) {
                 (*iter)->reg_data_graph(graph);
                 return true;
             }
        return false;
    }

    bool reg_cal(ICalculator_summer* cal, const char* name)
    {
        for (CalculatorConstIter iter = begin(); iter != end(); iter++)
             if ((*iter)->isme(name)) {
                 (*iter)->reg_calculator(cal);
                 return true;
             }
        return false;
    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
//interface ICalculatorCreator
class WARMGUI_API ICalculatorCreator
{
public:
    ICalculatorCreator(void) {}
    virtual ~ICalculatorCreator(void) {}


    void SetConfigure(CWarmguiConfig* config) { _config = config; }
    virtual ICalculator_summer* CreateCalculator(const char* str_config) = 0;

protected:
    CWarmguiConfig* _config;
};



}   //namespace WARMGUI

#endif //__interface_calculator_h__
