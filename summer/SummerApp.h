#ifndef __summer_application__
#define __summer_application__

class CSummerApp : public WARMGUI::CWndApp
{
public:
    CSummerApp(void);
    ~CSummerApp(void);

	virtual int InitialApp(HINSTANCE hInstance, int nCmdShow);
	virtual void CleanupApp();

};

#endif //__summer_application__