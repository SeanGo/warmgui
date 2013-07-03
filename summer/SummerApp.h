#ifndef __summer_application__
#define __summer_application__

class CSummerApp : public WARMGUI::CWndApp
{
public:
    CSummerApp(void);
    ~CSummerApp(void);

	virtual int              InitialApp(HINSTANCE hInstance, int nCmdShow);
	virtual void             CleanupApp();

    WARMGUI::CDispatchers*   get_dispatchers() { return &_dispatchers; }

protected:
    CCtpmdDispatcher_summer* _ctp_disp;
    WARMGUI::CDispatchers    _dispatchers;
};

extern CSummerApp the_app;

#endif //__summer_application__