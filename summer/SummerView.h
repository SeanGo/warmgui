#ifndef __warmgui_summer_testing_view__
#define __warmgui_summer_testing_view__

class CSummerView :  public WARMGUI::CWindow
{
public:
    CSummerView(void);
    ~CSummerView(void);

	virtual BOOL PreCreateWindow(LPCREATESTRUCT cs);
	msgfun  int  OnCreate(LPCREATESTRUCT /*cs*/);
	msgfun  void OnSize(UINT nType, int cx, int cy);
	msgfun  void OnDraw();
	msgfun  void OnDestroy();
    msgfun  int  OnCommand(WORD nCmdId, WORD /*nSource*/, HWND /*hwnd*/);

    msgfun  int  OnMessage(UINT message, WPARAM wparam, LPARAM lparam);
    msgfun  int  OnKillfocus(WPARAM, LPARAM);
    msgfun  int  OnSetfocus(HWND);

    msgfun  void OnRButtonUp(UINT, int , int);
    msgfun  void OnLButtonUp(UINT, int , int);

protected:
    CSummerAtelier* _atelier;
    RECT         _rectClient;

    bool              _focus;

    CZitPredictView* _zit_prd_view;
    void ShowZitPredictView();
};


#endif //__warmgui_summer_testing_view__