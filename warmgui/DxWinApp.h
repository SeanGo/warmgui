#ifndef __DX_WINDOW_APPLICATION_H_INCLUDE__
#define __DX_WINDOW_APPLICATION_H_INCLUDE__


namespace WARMGUI {
class WARMGUI_API CDxWinApp : public CWndApp
{
public:
	CDxWinApp(void);
	~CDxWinApp(void);

	virtual int InitialApp(HINSTANCE hInstance, int nCmdShow);
	virtual void CleanupApp();
};
}


#endif //__DX_WINDOW_APPLICATION_H_INCLUDE__
