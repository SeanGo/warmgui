#include "StdAfx.h"
#include "qks_incs.h"

//CTPTIMESEC g_ctpsec;

const bool CEuclidGuiFactory::init_factory()
{
    _atelier_creator    = new CEuclidAtelierCreator();
    _canvas_creator     = new CEuclidCanvasCreator();
    _layout_creator     = new CEuclidLayoutCreator();

    _atelier_creator   ->SetConfigure(_config);
    _canvas_creator    ->SetConfigure(_config);
    _layout_creator    ->SetConfigure(_config);

    return true;
}

/*
	if (SUCCEEDED(hr) && GetBitmapMaterial()) {
		_canvasImgbkg = new WARMGUI::CBkgCanvas(L"bkgcanvas");
		_canvasImgbkg->SetBitmap(&_appbmp);
		Insert(_canvasImgbkg);
	}
*/












///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CEuclidCanvasCreator
ICanvas* CEuclidCanvasCreator::CreateCanvas(const char* canvas_config)
{
    ICanvas* canvas = ICanvasCreator::CreateCanvas(canvas_config);
    if (!canvas)
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
            if (!strcmp(type, "pvi-canvas")) {
                //canvas = new CPVICanvas(name);
            } if (!strcmp(type, "zen-in-twining")) {
                canvas = new CZitCanvas(name);
            } if (!strcmp(type, "PVICanvas")) {
                canvas = new CPVI_Canvas(name);
            } if (!strcmp(type, "gridctrl")) {
                canvas = new CTickdataCanvas(name);
            } if (!strcmp(type, "euclid")) {
                canvas = new CEuclidCanvas(name);
            }
             
            return canvas;

        }catch(...){
            MYTRACE(L"CEuclidCanvasCreator::CreateCanvas\n");
        }
    }
    return canvas;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CEuclidDataContainerCreator
IDataContainer* CEuclidDataContainerCreator::CreateContainer(const char* conf_pos)
{
    try {
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.type", conf_pos);
        const std::string strtype = _config->getString(temp);
        const char* type = strtype.c_str();

        IDataContainer* container = 0;
        if (!strcmp(type, "ctpmmd-zit")) {
            _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.name", conf_pos);
            const std::string strname = _config->getString(temp);
            const char* name = strname.c_str();
            container = new CCtpmmdContainer(name);
            container->SetConfig(_config, conf_pos);
        }
        return container;
    }catch(...){
        MYTRACE(L"CEuclidDataContainerCreator::CreateContainer\n");
    }
    return (0);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CEuclidDataDispatcherCreator
IDataDispatcher* CEuclidDataDispatcherCreator::CreateDispathcer(const char* conf_pos)
{
    try {
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.type", conf_pos);
        const std::string strtype = _config->getString(temp);
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.name", conf_pos);
        const std::string strname = _config->getString(temp);
        const char* type = strtype.c_str();
        const char* name = strname.c_str();

        IDataDispatcher* dispatcher = 0;
        if (!strcmp(type, "america-network")) {
            dispatcher = new CQksDataDispatcher(name);
        } if (!strcmp(type, "ctp-futures")) {
            dispatcher = new CCtpDispatcher(name);
        }
        if (dispatcher)
            dispatcher->SetConfig(_config, conf_pos);
        return dispatcher;
    }catch(...){
        MYTRACE(L"CEuclidDataDispatcherCreator::CreateDispathcer\n");
    }
    return (0);
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CEuclidAtelierCreator
IAtelier* CEuclidAtelierCreator::CreateAtelier(const char* conf_pos)
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.atelier-type", conf_pos);

    IAtelier* atelier = (0);
    if (!strcmp(_config->getString(temp).c_str(), "EuclidAtelier")) {
	    atelier = new CEuclidAtelier("eulid-atelier");
	    if (atelier) atelier->SetConfigFile(_config, conf_pos);
    }
    return atelier;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CEuclidLayoutCreator
ILayout* CEuclidLayoutCreator::CreateLayout(const char* layout_config)
{
    try {
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.type", layout_config);

        ILayout* layout = (0);
        if (!strcmp(_config->getString(temp).c_str(), "EuclidLayout")) {
	        layout = new CEuclidLayout();
	        if (layout) layout->SetConfig(_config, layout_config);
        }
        return layout;
    } catch(...) {
        MYTRACE(L"CEuclidLayoutCreator::CreateLayout\n");
        return (0);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CEuclidCalculator
ICalculator* CEuclidCalculatorCreator::CreateCalculator(const char* str_conf)
{
    try {
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.type", str_conf);

        ICalculator* calculator = (0);
        if (!strcmp(_config->getString(temp).c_str(), "euclid-calculator")) {
            _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.name", str_conf);
            const std::string strname = _config->getString(temp);
            const char* name = strname.c_str();
	        calculator = new CEuclidCalculator(name);
	        if (calculator) calculator->SetConfig(_config, str_conf);
        }
        return calculator;
    } catch(...) {
        MYTRACE(L"CEuclidLayoutCreator::CreateLayout\n");
        return (0);
    }
}

