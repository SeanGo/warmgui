#ifndef __warmgui_window_application_h_include__
#define __warmgui_window_application_h_include__

#include <Poco/Exception.h>
#include <Poco/Util/PropertyFileConfiguration.h>

namespace WARMGUI {


class WARMGUI_API CWndApp
{
public:
	CWndApp(void);
	virtual ~CWndApp(void);

	int UniqueInstance(TCHAR * appname);

	virtual int InitialApp(HINSTANCE hInstance, int nCmdShow);

	virtual void CleanupApp();

	CWarmguiConfig* GetConfig() { return &_config; }

	bool LoadConfigFile(const TCHAR* filepath);

protected:
	HANDLE _hInstanceMutex;
	HINSTANCE   _hInstance;
	//CWarmguiConfig _config;
    CWarmguiConfig _config;
};

}


#endif //__warmgui_window_application_h_include__