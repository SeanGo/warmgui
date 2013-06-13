#ifndef __analyst_view_h__
#define __analyst_view_h__

/////////////////////////////////////////////////////////////////////////////////////////////
// class CMouseState
class WARMGUI_API CMouseState
{
public:
	enum MOUSE_STATE {
		MOUSE_STATE_NONE      = 0x000,
		MOUSE_STATE_MOVE_OVER = 0x001,
		MOUSE_STATE_LBTN_DOWN = 0x002,
		MOUSE_STATE_LBTN_UP   = 0x004,
		MOUSE_STATE_LBTN_DCLK = 0x008,
		MOUSE_STATE_RBTN_DOWN = 0x010,
		MOUSE_STATE_RBTN_UP   = 0x020,
		MOUSE_STATE_RBTN_DCLK = 0x040,
		MOUSE_STATE_MBTN_DOWN = 0x080,
		MOUSE_STATE_MBTN_UP   = 0x100,
		MOUSE_STATE_MBTN_DCLK = 0x200,
		MOUSE_STATE_WHEEL     = 0x400,
	};

    CMouseState() : _state(MOUSE_STATE_NONE)
        {_old_pnt.x = _old_pnt.y = _now_pnt.x = _now_pnt.y = 0;}
	void SetPoint(POINT pnt) { _old_pnt = _now_pnt, _now_pnt = pnt; }
    void ChangeState(UINT state) { _state |=  state; }
    void RemoveState(UINT state) { _state &= ~state; }

	UINT GetState() { return _state; }

protected:
	UINT    _state;
	POINT _old_pnt;
	POINT _now_pnt;
};

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

    msgfun  void OnLButtonDown(UINT, int , int);
    msgfun  void OnRButtonDown(UINT, int , int);
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

	typedef struct key_state {
        enum KEY_STATE {
		    KEY_STATE_NONE = 0,
		    KEY_STATE_DOWN = 1,
		    KEY_STATE_UP   = 2,
        };
        KEY_STATE _ks;
        UINT     _key;

        key_state() : _ks(KEY_STATE_NONE), _key(0) {}
    };

    int open_history_data();

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
    CMouseState        _mouse_state;
    CHistoryDataContainer* _history_dc;
    CEuclid*           _analyst;
};

#endif //__analyst_view_h__