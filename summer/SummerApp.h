#ifndef __summer_application__
#define __summer_application__

class CSummerApp : public WARMGUI::CWndApp
{
public:
    CSummerApp(void);
    ~CSummerApp(void);

	virtual int InitialApp(HINSTANCE hInstance, int nCmdShow);
	virtual void CleanupApp();

    CTestDispatcher_summer* get_test_dispatcher() { return dispatcher; }
protected:
    CTestDispatcher_summer* dispatcher;
};

extern CSummerApp the_app;

#endif //__summer_application__