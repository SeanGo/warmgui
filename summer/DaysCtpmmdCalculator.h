#ifndef __euclid_approx_calculator__
#define __euclid_approx_calculator__


typedef struct DAYS_CTPMMD_RESULT {
    CTPMMD_TIME_SERIES* _ts;
    EUCLID::CCtpmmdDataSampler* _sampler;
    double*             _low_approx_price;
    double*             _he_approx_price;
    double*             _approx_volume;
    double*             _approx_interest;
    EXTRINFO*           _ext_price;
    EXTRINFO*           _ext_volume;
    EXTRINFO*           _ext_interest;
    INFLINFO*           _infl_price;
    INFLINFO*           _infl_interest;

    DAYS_CTPMMD_RESULT()
		: _ts(0)              
        , _sampler(0)
		, _low_approx_price(0)
		, _he_approx_price(0) 
		, _approx_volume(0)   
		, _approx_interest(0) 
		, _ext_price(0)       
		, _ext_volume(0)      
		, _ext_interest(0)    
		, _infl_price(0)      
		, _infl_interest(0)
    {}
        
} DAYS_CTPMMD_RESULT;

class CDaysCtpmmdCalculator : public WARMGUI::ICalculator_summer
{
public:
    CDaysCtpmmdCalculator(const char * name);
    ~CDaysCtpmmdCalculator(void);

    virtual int  calculate(MSG msg);
    virtual bool init();
    virtual dataptr get_result() { return &_dcr; }

    virtual void run() {
        //if (!init()) return;
      	_tid = Poco::Thread::currentTid();
    	_hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, GetCurrentThreadId());
        _stop = 0;

	    DWORD dwThreadPri = GetThreadPriority(_hThread);
	    if (dwThreadPri != THREAD_PRIORITY_TIME_CRITICAL) {
		    if (!SetThreadPriority(_hThread, THREAD_PRIORITY_TIME_CRITICAL))
                return; //error
	    }

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
                    if (r > 0)
                        _update_other();
                    _busy = false;
                    if (r < 0)
                        return;
                }
			    DispatchMessage(&msg);
		    }
	    }
    }
private:
    EUCLID::CCtpmmdDataSampler sampler;
    CWaveletFilter     _wvlt_low_price;
    CWaveletFilter      _wvlt_he_price;
    CWaveletFilter        _wvlt_volume;
    CWaveletFilter      _wvlt_interest;
    EUCLID::CExtremum   _ext_low_price;
    EUCLID::CInflexion  _inf_low_price;
    EUCLID::CExtremum      _ext_volume;
    EUCLID::CExtremum    _ext_interest;
    EUCLID::CInflexion   _inf_interest;

    DAYS_CTPMMD_RESULT _dcr;
};

#endif __euclid_approx_calculator__
