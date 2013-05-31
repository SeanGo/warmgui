#include "StdAfx.h"
#include "warmgui.h"

namespace WARMGUI {


ICanvasCreator::ICanvasCreator(void)
{
}


ICanvasCreator::~ICanvasCreator(void)
{
}


ICanvas* ICanvasCreator::CreateCanvas(const char* canvas_config)
{
    try {
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.canvas-type", canvas_config);
        const std::string strtype = _config->getString(temp);
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.canvas-name", canvas_config);
        const std::string strname = _config->getString(temp);
        const char* type = strtype.c_str();
        const char* name = strname.c_str();

        ICanvas* canvas = 0;
        if (!strcmp(type, "background-canvas")) {
            canvas = new CBkgCanvas(name);
        } if (!strcmp(type, "grid")) {
            canvas = new CGridCanvas(name);
        } if (!strcmp(type, "toolbar")) {
            canvas = new CToolbar(name);
        }

        return canvas;
    }catch(...){
        return (0);
    }
}

} //namespace warmgui

