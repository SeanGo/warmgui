#ifndef __warmgui_directx_window_h_include__
#define __warmgui_directx_window_h_include__

namespace WARMGUI {
class WARMGUI_API CDxWindow : public CWindow
{
public:
	CDxWindow(void);
	virtual ~CDxWindow(void);

	virtual BOOL PreCreateWindow(LPCREATESTRUCT cs);
	msgfun  int OnCreate(LPCREATESTRUCT /*cs*/);
	msgfun void OnSize(UINT nType, int cx, int cy);
	msgfun void OnDraw();
	msgfun int  OnEraseBkgnd(HDC);
	msgfun void OnDestroy();
	//msgfun int  OnRenewBitmap(ICanvas * canvas);
	inline RECT * GetClientRect();

	virtual int InitAtelier();

	void SetConfig(CWarmguiConfig*  config) {_config = config;}
protected:
	CAtelier* _atelier;
	RECT         _rectClient;
	CWarmguiConfig*      _config;
};


}

#endif //__warmgui_directx_window_h_include__