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
    WARMGUI::CCalculators*   get_calculators() { return &_calculators; }
protected:
    WARMGUI::CDispatchers    _dispatchers;
    WARMGUI::CCalculators    _calculators;
};

extern CSummerApp the_app;

#endif //__summer_application__