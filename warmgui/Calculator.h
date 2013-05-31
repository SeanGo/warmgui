#ifndef __interface_calculator_h__
#define __interface_calculator_h__

#define WM_CALCULATOR_MESSAGE WM_USER + 7890

namespace WARMGUI {

class ICalculator;
EXPORT_STL_VECTOR(WARMGUI_API, ICalculator*)
typedef std::vector<ICalculator*>    CalculatorArray;
typedef CalculatorArray::iterator            CalIter;
typedef CalculatorArray::const_iterator CalConstIter;


class WARMGUI_API ICalculator : public Poco::Runnable
{
public:
    ICalculator(const char* name)
        : _tid(0), _stop(1), _busy(false)
    { strcpy_s(_name, MAX_PATH, name); *_my_cont = 0; }

    virtual ~ICalculator(void) {}

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
                    _busy = false;
                    if (r < 0)
                        return;
                }
			    DispatchMessage(&msg);
		    }
	    }
    }

    virtual int  calculate(MSG msg) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
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

    bool NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype)
    {
        ///multi-thread
        /*
        if (!_busy) {
            PostThreadMessage(_tid, WM_CALCULATOR_MESSAGE, (WPARAM)data_cont, (LPARAM)datatype);
            return true;
        } else 
            return false;
        */
        MSG msg;
        msg.message = WM_CALCULATOR_MESSAGE;
        msg.wParam  = (WPARAM)data_cont;
        msg.lParam  = (LPARAM)datatype;
        calculate(msg);
        return true;
    }

    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr dop) { return GLYPH_CHANGED_TYPE_NONE;}
    virtual GLYPH_CHANGED_TYPE NewData(dataptr pdata, DataObject::MARKET_DATA_TYPE datatype) { return GLYPH_CHANGED_TYPE_NONE;}

    void RegisterCalculator(ICalculator* calculator)
    {
        _cals.push_back(calculator);
    }

    void RegisterCanvas(IDataCanvas* canvas)
    {
#       ifdef _DEBUG
        TCHAR name1[MAX_PATH], name2[MAX_PATH];
        CChineseCodeLib::Gb2312ToUnicode(name1, MAX_PATH, canvas->getName());
        CChineseCodeLib::Gb2312ToUnicode(name2, MAX_PATH, _name);
        MYTRACE(L"IDataCanvas %s regestered to calculator %s\n", name1, name2);
#       endif //_DEBUG
        _canvases.push_back(canvas);
    }

    void RegisterDataGraph(IDataGraph* graph)
    {
        _dgraph.push_back(graph);
    }
    const char* getContainerName() {return _my_cont;}
    const char* getName() {return _name;}

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
    Poco::Thread::TID    _tid;
    int                 _stop;
    bool                _busy;

    CWarmguiConfig*   _config;
    char      _name[MAX_PATH];
    char   _strconf[MAX_PATH];
    char   _my_cont[MAX_PATH];
    CalculatorArray     _cals;
    DataCanvasArray _canvases;
    DataGraphArray    _dgraph;
	Poco::Thread      _thread;

    HwndArray     _hwnd_array;
};



/////////////////////////////////////////////////////////////////////////////////////////////////////
//interface ICalculatorCreator
class WARMGUI_API ICalculatorCreator
{
public:
    ICalculatorCreator(void) {}
    virtual ~ICalculatorCreator(void) {}


    void SetConfigure(CWarmguiConfig* config) { _config = config; }
    virtual ICalculator* CreateCalculator(const char* str_config) = 0;

protected:
    CWarmguiConfig* _config;
};



}   //namespace WARMGUI

#endif //__interface_calculator_h__
