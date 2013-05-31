#include "StdAfx.h"
#include "qks_incs.h"


CQksCanvasCreator::CQksCanvasCreator(void)
{
}


CQksCanvasCreator::~CQksCanvasCreator(void)
{
}


ICanvas* CQksCanvasCreator::CreateCanvas(CWarmguiConfig* config, const char* type, const char* canvas_config)
{
    ICanvas* canvas = ICanvasCreator::CreateCanvas(config, type, canvas_config);
    if (!canvas)
    {      
        const char* name = 0;
        try {
            char temp[MAX_PATH];
            _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.canvas-name", canvas_config);
            const std::string str = config->getString(temp);
            name = str.c_str();

            if (!strcmp(type, "pvi-canvas")) {
                canvas = new CPVICanvas(name);
            } if (!strcmp(type, "zen-in-twining")) {
                canvas = new CZenInTwiningCanvas(name);
            } if (!strcmp(type, "PVICanvas")) {
                canvas = new CPVI_Canvas(name);
            }
        }catch(...){
        }
    }
    return canvas;
}
