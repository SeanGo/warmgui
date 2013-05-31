#include "StdAfx.h"
#include "qks_incs.h"


bool CZitGuiFactory::CreateEverething()
{
    bool r = IGuiFactory::CreateEverething();
    if (r) {
    }
    return (r);
}

bool CZitGuiFactory::CreateAtelier()
{
    ///Create Zen-In-Twining Atelier
    const char* atelier_type = "first-atelier.atlier-type";
    if (!strcmp(_config->getString(atelier_type).c_str(), "ZenInTwining")) {
	    _atelier = new CZenInTwiningAtelier(L"zen-in-twining");
	    _atelier->SetConfigFile(_config);
	    if (FAILED(_atelier->CreateRenderTarget(_hwnd)))
		    return false;

        _atelier->SetHwnd(_hwnd);
        _atelier->SetConfigFile(_config);

        return true;
    }

    return false;
}

bool CZitGuiFactory::CreateDispatcher()
{
    const char* conf_dc = "first-atelier.data-dispatcher";
    char        temp[MAX_PATH];

    ///the dispatcher type must be qks
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.dispatcher-type", conf_dc);
    if (!strcmp(_config->getString(temp).c_str(), "qks")) {
        ///create qks-dispatcher
        _dispatcher = new CQksDataDispatcher();
        if (!_dispatcher) return false;
        if (!_dispatcher->SetDataContainer(_config)) return false;
    }

    return true;
}


bool CZitGuiFactory::CreateCanvas()
{
    return false;
}

bool CZitGuiFactory::CreateLayout()
{
    return false;
}
