#ifndef __qiankun_spectrum_application_h__
#define __qiankun_spectrum_application_h__


class qksapp : public CDxWinApp
{
public:
    qksapp(void);
    ~qksapp(void);

	virtual int InitialApp(HINSTANCE hInstance, int nCmdShow);
	virtual void CleanupApp();

    CDispatchers*    getDispatchers() {return &_dispatchers;}
    CalculatorArray* GetCalculators() {return &_calculators;}
    DCArray*         getDataContainers() {return &_data_containers;}
    IDataContainer*  getDataContainer(const char* name)
                     {
                        for (DCConstIter iter = _data_containers.begin(); iter != _data_containers.end(); iter++)
                            if ((*iter)->isme(name))
                                return (*iter);
                        return (0);
                     }

    const bool CreateCalculator();
    const bool CreateDispatcher();
    const bool CreateContainer ();

    void ReleaseDataContainer()
    {
        for (DCConstIter iter = _data_containers.begin(); iter != _data_containers.end(); iter++)
            delete (*iter);
        _data_containers.clear();
    }

    void ReleaseCalculator()
    {
        for (CalConstIter iter = _calculators.begin(); iter != _calculators.end(); iter++)
            delete (*iter);
        _calculators.clear();
    }

protected:
    CQksCalculatorCreator     _calculator_creator;
    CQksDataContainerCreator   _container_creator;
    CQksDataDispatcherCreator _dispatcher_creator;

    CDispatchers                     _dispatchers;
    DCArray                      _data_containers;
    CalculatorArray                  _calculators;
};

extern int ShowEuclidView (CWindow* parent_window);
extern int ShowAnalystView(CWindow* parent_window);

#endif //__qiankun_spectrum_application_h__