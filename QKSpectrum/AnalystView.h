#ifndef __analyst_view_h__
#define __analyst_view_h__

class CAnalystView : public WARMGUI::CWindow
{
public:
    CAnalystView(void);
    ~CAnalystView(void);

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

    enum COMMAND_STATE {
		COMMAND_STATE_MOUSE_OVER =  0,
		COMMAND_STATE_SELECT     =  1,
		COMMAND_STATE_ZOOM       =  2,
		COMMAND_STATE_SCALE      =  4,
		COMMAND_STATE_SKEW       =  8,
		COMMAND_STATE_MOVE       = 16,
        COMMAND_STATE_PREDICTOR  = 32,
    };
private:
	RECT                _rectClient;
	CWarmguiConfig*         _config;
    CDispatchers*      _dispatchers;
    CQksGuiFactory     _qks_factory;
    CAnalystAtelier*       _atelier;
    //CNetClient            _netcli;
    CalculatorArray*   _calculators;
    bool               _b_gui_ready;


    COMMAND_STATE    _command_state;
};

#endif //__analyst_view_h__