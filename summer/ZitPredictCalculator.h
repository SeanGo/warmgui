#ifndef __zit_predict_calculator__
#define __zit_predict_calculator__


typedef struct ZIT_PREDICT_RESULT {
    CTPMMD_TIME_SERIES* _ts;
    EUCLID::CCtpmmdDataSampler* _sampler;

    double*         _low_approx_price;
    double*         _he_approx_price;
    double*         _prdct_low;
    double*         _prdct_he;

    double*         _wvlt_zit_approx;
	double*         _low_ma_filter;
	double*         _he_ma_filter;
    EXTRINFO*       _ext_zit;
    INFLINFO*       _inf_zit;
    CENTRAL_SERIES* _centrals;
	LHAREA*         _areas;


    ZIT_PREDICT_RESULT()
    {zero();}

    void zero()
    {
		_ts = (0),              
        _sampler = (0),
		_low_approx_price = (0),
		_he_approx_price = (0),
		_prdct_low = (0),      
		_prdct_he = (0),       
		_wvlt_zit_approx = (0),
		_low_ma_filter = (0),  
		_he_ma_filter = (0),   
		_ext_zit = (0),        
		_inf_zit = (0),        
		_centrals = (0),       
		_areas = (0);
    }
        
} ZIT_PREDICT_RESULT;

class CZitPredictCalculator : public WARMGUI::ICalculator_summer
{
public:
    CZitPredictCalculator(const char * name);
    ~CZitPredictCalculator(void);

    virtual int  calculate(MSG msg);
    virtual bool init();
    virtual dataptr get_result() { return &_zpr; }

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
    //for predict
    CWaveletFilter     _wvlt_low_price;
    CWaveletFilter      _wvlt_he_price;
	EUCLID::CPredict        _prdct_low;
	EUCLID::CPredict         _prdct_he;

    //for zit
    CWaveletFilter    _wvlt_zit_approx;
	CMAFilter           _low_ma_filter;
	CMAFilter            _he_ma_filter;
    EUCLID::CExtremum         _ext_zit;
    EUCLID::CInflexion        _inf_zit;
    EUCLID::CCentral          _central;

    ZIT_PREDICT_RESULT _zpr;
};

#endif __zit_predict_calculator__
