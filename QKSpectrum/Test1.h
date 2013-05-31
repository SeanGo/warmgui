#ifndef __qks_test_1_h_include__
#define __qks_test_1_h_include__

class CTest1Atelier;
class CTest1 : public WARMGUI::CWindow
{
public:
	CTest1(void);
	virtual ~CTest1(void);

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

    void ReDraw(GLYPH_CHANGED_TYPE changed_type);
	void SetConfig(CWarmguiConfig*  config) {_config = config;}
protected:
	RECT         _rectClient;
	CWarmguiConfig*  _config;

private:

    MyMarketData* _pdata;
    CTPMMD*      _ctpmmd;
    int      _nDataCount;
    CTest1Atelier* _atelier;

public:
    HANDLE hSimulator;
    DWORD    dwSimuId;
    bool         stop;
    static DWORD WINAPI simulator_sending(LPVOID);
};



#endif //__qks_test_1_h_include__