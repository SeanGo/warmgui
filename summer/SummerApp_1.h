#ifndef __summer_application__
#define __summer_application__

class CSummerApp_1 : public WARMGUI::CWndApp
{
public:
    CSummerApp_1(void);
    ~CSummerApp_1(void);

	virtual int InitialApp(HINSTANCE hInstance, int nCmdShow);
	virtual void CleanupApp();

    void regester_graph(WARMGUI::IDataGraph_summer* graph);
    void dispathcer_start();

protected:
    CTestDispatcher_summer* dispatcher_1;
    CTestDispatcher_summer* dispatcher_2;
    CTestDispatcher_summer* dispatcher_3;
    CTestDispatcher_summer* dispatcher_4;

    CtpmdDispatcher_summer*    _ctp_disp;
};

extern CSummerApp_1 the_app;

#endif //__summer_application__