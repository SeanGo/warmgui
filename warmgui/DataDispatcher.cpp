#include "StdAfx.h"
#include "warmgui.h"
#include "pocoinc.h"


namespace WARMGUI {
IDataDispatcher::IDataDispatcher(const char* name)
    : _stop(1)
{
    strcpy_s(_name, MAX_PATH, name);
}


IDataDispatcher::~IDataDispatcher(void)
{
}

    /*
bool IDataDispatcher::SetDataContainer(CWarmguiConfig* config)
{
    _config = config;
    int num = _config->getInt("atelier-0.data-dispatcher.data-container-number");

    for (int i = 1; i <= num; i++) {
        char conf_pos[MAX_PATH];
        _snprintf_s(conf_pos, MAX_PATH, _TRUNCATE, "atelier-0.data-dispatcher.data-container-%d", i);
        char container_type[MAX_PATH];
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.container-type", conf_pos);
        strcpy_s(container_type, MAX_PATH, _config->getString(temp).c_str());

        IDataContainer* dc = _dcf->CreateContainer(container_type, conf_pos);
        if (!dc)
            return false;
        _dcs.push_back(dc);
    }
    return true;
}
    */


/*
bool IDataDispatcher::RegisterCanvasToContainer(IDataCanvas* canvas, int container_no)
{
    if (container_no < 1 || container_no > (int)_dcs.size())
        return false;
    _dcs[container_no - 1]->RegisterCanvas(canvas);
    return true;
}
*/
} //namespace WARMGUI

