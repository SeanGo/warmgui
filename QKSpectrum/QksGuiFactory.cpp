#include "StdAfx.h"
#include "qks_incs.h"


const bool CQksGuiFactory::CreateEverething()
{
    _canvas_fact = new CQksCanvasCreator();

    bool r = IGuiFactory::CreateEverething();
    if (r) {
    }
    return (r);
}

const bool CQksGuiFactory::CreateAtelier(int num)
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "atelier-%d.atelier-type", num);

    ///Create Zen-In-Twining Atelier
    //if (strcmp(_config->getString(temp).c_str(), "ZenInTwining"))
    //    return false;
    if (strcmp(_config->getString(temp).c_str(), "ZITAtelier"))
        return false;

    {
	    _atelier = new CZITAtelier("zen-in-twining");
	    _atelier->SetConfigFile(_config);
    }

    return true;
}

const bool CQksGuiFactory::CreateDispatcher(const char* atelier_conf)
{
    char        temp[MAX_PATH];
    ///the dispatcher type must be qks
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.data-dispatcher.dispatcher-type", atelier_conf);
    if (!strcmp(_config->getString(temp).c_str(), "qks")) {
        ///create qks-dispatcher
        _dispatcher = new CQksDataDispatcher();
        if (!_dispatcher) return false;
        if (!_dispatcher->SetDataContainer(_config)) return false;
    }

    return true;
}


const bool CQksGuiFactory::CreateLayout(const char* atelier_conf)
{
    char        temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.layout", atelier_conf);

    if (!strcmp(_config->getString(temp).c_str(), "zit-layout")) {
       	_layout = new CZenInTwiningLayout();
        if (!_layout) return false;
    } else if (!strcmp(_config->getString(temp).c_str(), "ZITLayout")) {
       	_layout = new CZITLayout();
        if (!_layout) return false;
    }
    return true;
}


/*
	if (SUCCEEDED(hr) && GetBitmapMaterial()) {
		_canvasImgbkg = new WARMGUI::CBkgCanvas(L"bkgcanvas");
		_canvasImgbkg->SetBitmap(&_appbmp);
		Insert(_canvasImgbkg);
	}
*/