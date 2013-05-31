#ifndef __qks_test_3_h_include__
#define __qks_test_3_h_include__


class CTest3 : public WARMGUI::CWindow
{
public:
	CTest3(void);
	virtual ~CTest3(void);

	virtual BOOL PreCreateWindow(LPCREATESTRUCT cs);
	msgfun  int  OnCreate(LPCREATESTRUCT /*cs*/);
	msgfun  void OnSize(UINT nType, int cx, int cy);
	msgfun  void OnDraw();
	msgfun  int  OnEraseBkgnd(HDC);
	msgfun  void OnDestroy();

    msgfun  void OnLButtonUp(UINT, int , int);
    msgfun  void OnRButtonUp(UINT, int , int);

	msgfun  void OnMouseMove(UINT nFlags, int x, int y);
	msgfun  int  OnCommand(WORD nCmdId, WORD nSource, HWND hwnd);

    msgfun  int  OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

	//msgfun int  OnRenewBitmap(ICanvas * canvas);
	inline RECT* GetClientRect();

    void         ReDraw();
	void         SetConfig(CWarmguiConfig*  config) {_config = config;}


    int          Login(const char* username, const char* password);
    int          Logout();

private:
	RECT                _rectClient;
	CWarmguiConfig*         _config;
    IDataDispatcher*    _dispatcher;
	Poco::Thread  _th_data_receiver;
    CQksGuiFactory     _zit_factory;
    CZenInTwiningAtelier*  _atelier;
    CZenInTwiningLayout*    _layout;
    CNetClient              _netcli;
    CtpRtFilters         _ctpfilter;
};



#endif //__qks_test_3_h_include__