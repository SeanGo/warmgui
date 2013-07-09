#ifndef __zit_predict_view__
#define __zit_predict_view__

class CZitPredictView :  public WARMGUI::CWindow
{
public:
    CZitPredictView(void);
    ~CZitPredictView(void);

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
};


#endif //__zit_predict_view__