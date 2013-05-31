#include "StdAfx.h"
#include "qks_incs.h"
#include "network_inc.h"


qksapp::qksapp(void)
{
}


qksapp::~qksapp(void)
{
    ReleaseDataContainer();
    ReleaseCalculator();
    //or keep next line
    //_dispatchers.ReleaseAll();
}

int qksapp::InitialApp(HINSTANCE hInstance, int nCmdShow)
{
	TCHAR curpath[MAX_PATH];
	////::GetCurrentDirectoryW(MAX_PATH, curpath);	////something is wired
	GetModuleFileNameW(NULL, curpath, MAX_PATH);
	curpath[_tcslen(curpath) - 3] = L'\0';
	_tcscat_s(curpath, MAX_PATH, L"xml");
    if (!LoadConfigFile(curpath))
		return (-1);

    if (CDxWinApp::InitialApp(hInstance, nCmdShow))
		return (-2);

    //Poco::Net::initializeNetwork();
	//Poco::Net::initializeSSL();
    CEuclidConfig::GetInstance()->GetSng1Config(_config.getString("ctp-market.sng-config-file"));

    _calculator_creator.SetConfigure(&_config);
    _container_creator .SetConfigure(&_config);
    _dispatcher_creator.SetConfigure(&_config);

    bool r = true;
    if (r)
        r = CreateDispatcher();

    if (r)
        r = CreateContainer();

    if (r)
        r = CreateCalculator();

    return (0);
}


void qksapp::CleanupApp()
{
	//Poco::Net::uninitializeSSL();
	//Poco::Net::uninitializeNetwork();

    CDxWinApp::CleanupApp();
}

const bool qksapp::CreateCalculator()
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "calculators.num");
    int num = _config.getInt(temp);

    for (int n = 1; n < num + 1; n++) {
        char cal_conf[MAX_PATH];
        _snprintf_s(cal_conf, MAX_PATH, _TRUNCATE, "calculators.calculator-%d", n);
        ICalculator* calculator = _calculator_creator.CreateCalculator(cal_conf);
        if (calculator) {
            if (calculator->init()) {
                _calculators.push_back(calculator);
                for (DCConstIter iter = _data_containers.begin(); iter != _data_containers.end(); iter++) {
                    if ((*iter)->isme(calculator->getContainerName()))
                        (*iter)->RegisterCalculator(calculator);
                }
            } else {
                delete calculator;
                return false;
            }
        } else {
            return false;
        }
    }

    return true;
}

const bool qksapp::CreateDispatcher()
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "data-dispatcher.num");
    int num = _config.getInt(temp);

    for (int n = 1; n < num + 1; n++) {
        char disp_conf[MAX_PATH];
        _snprintf_s(disp_conf, MAX_PATH, _TRUNCATE, "data-dispatcher.dispatcher-%d", n);
        IDataDispatcher* dispatcher = _dispatcher_creator.CreateDispathcer(disp_conf);
        if (dispatcher)
            _dispatchers.push_back(dispatcher);
        else
            return false;
    }

    return true;
}

const bool qksapp::CreateContainer()
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "data-container.num");
    int num = _config.getInt(temp);

    for (int n = 1; n < num + 1; n++) {
        char container_conf[MAX_PATH];
        _snprintf_s(container_conf, MAX_PATH, _TRUNCATE, "data-container.data-container-%d", n);
        IDataContainer* container = _container_creator.CreateContainer(container_conf);

        if (container) {
            _data_containers.push_back(container);
            for (DispatcherConstIter iter1 = _dispatchers.begin(); iter1 != _dispatchers.end(); iter1++) {
                if ((*iter1)->isme(container->getDispatcherName()))
                    (*iter1)->reg_data_container(container);
            }
        } else
            return false;
    }

    return true;
}
