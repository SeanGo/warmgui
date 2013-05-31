#include "stdafx.h"
#include "warmgui.h"

namespace WARMGUI {
const bool IGuiFactory::CreateCanvas(const char* atelier_conf)
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.canvases.canvas-num", atelier_conf);
    int num = _config->getInt(temp);

    bool result = true;
    for (int n = 1; n <= num; n++) {
        char canvas_config[MAX_PATH];
        _snprintf_s(canvas_config, MAX_PATH, _TRUNCATE, "%s.canvases.canvas-%d", atelier_conf, n);
        try{
            ICanvas* canvas = _canvas_creator->CreateCanvas(canvas_config);
            if (canvas) {
                //attach
                AttachToAtelier(canvas, canvas_config);
            } else {
                result = false;
                break;
            }
        }catch(...){
            MYTRACE(L"IGuiFactory::CreateCanvas\n");
        }
    }

    return result;
}




const bool IGuiFactory::CreateEverething(HWND hwnd, const char* atelier_name)
{
    bool r = true;
    char key[MAX_PATH];
    _snprintf_s(key, MAX_PATH, _TRUNCATE, "atelier-%s", atelier_name);

    _atelier = _atelier_creator->CreateAtelier(key);

    if (_atelier) {
        _atelier->InitAtelier(hwnd, _config);
    } else
        r = false;

    if (r) {
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.layout", key);
        _layout = _layout_creator->CreateLayout(temp);
        r = (_layout != 0);
    }

    if (r)
        _atelier->SetLayout(_layout);

    if (r)
        r = CreateCanvas(key);
    else
        r = false;

    if (r)
        r = _atelier->GetAllCanvas();

    return (r);
}


}//namespace WARMGUI