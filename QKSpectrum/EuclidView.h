#ifndef __euclid_view_h_include__
#define __euclid_view_h_include__


class CEuclidView : public WARMGUI::CWindow
{
public:
    CEuclidView(void);
    ~CEuclidView(void);

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

    int          ReceiveData();

private:
	RECT                _rectClient;
	CWarmguiConfig*         _config;
    CDispatchers*      _dispatchers;
    CQksGuiFactory    _qks_factory;
    CEuclidAtelier*        _atelier;
    //CNetClient            _netcli;
    CalculatorArray*   _calculators;
    bool               _b_gui_ready;
};





#endif //__euclid_view_h_include__
