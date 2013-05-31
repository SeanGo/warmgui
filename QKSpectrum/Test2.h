#ifndef __qks_test_2_h_include__
#define __qks_test_2_h_include__


class CTest2 : public WARMGUI::CWindow
{
public:
	CTest2(void);
	virtual ~CTest2(void);

	virtual BOOL PreCreateWindow(LPCREATESTRUCT cs);
	msgfun  int OnCreate(LPCREATESTRUCT /*cs*/);
	msgfun  void OnSize(UINT nType, int cx, int cy);
	msgfun  void OnDraw();
	msgfun  int  OnEraseBkgnd(HDC);
	msgfun  void OnDestroy();

    msgfun  void OnLButtonUp(UINT, int , int);
    msgfun  void OnRButtonUp(UINT, int , int);

	//msgfun int  OnRenewBitmap(ICanvas * canvas);
	inline RECT * GetClientRect();

	virtual int InitAtelier();

    void ReDraw();
	void SetConfig(CWarmguiConfig*  config) {_config = config;}
protected:
	RECT         _rectClient;
	CWarmguiConfig*  _config;

private:
    MyMarketData* _pdata;
    int      _nDataCount;
    CZenInTwiningAtelier*  _atelier;
    CZenInTwiningLayout*    _layout;

public:
    HANDLE hSimulator;
    DWORD    dwSimuId;
    bool         stop;
    static DWORD WINAPI simulator_sending(LPVOID);
};



#endif //__qks_test_2_h_include__