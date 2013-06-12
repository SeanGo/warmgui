#include "StdAfx.h"
#include "qks_incs.h"

CTPTIMESEC g_ctpsec;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CQksGuiFactory

/*
const bool CQksGuiFactory::CreateAtelier(int num)
{

    return true;
}

const bool CQksGuiFactory::CreateDispatcher(const char* atelier_conf)
{
    /*
    char        temp[MAX_PATH];
    ///the dispatcher type must be qks
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.data-dispatcher.dispatcher-type", atelier_conf);
    if (!strcmp(_config->getString(temp).c_str(), "qks")) {
        ///create qks-dispatcher
        CQksDataDispatcher* dispatcher = new CQksDataDispatcher();
        if (!dispatcher) return false;
        this->_dispatchers.push_back(dispatcher);
    } else if (!strcmp(_config->getString(temp).c_str(), "")) {
    }

    return true;

    return this->_dispatcher_factory->CreateDispathcer();*/
/*    return true;
}


const bool CQksGuiFactory::CreateLayout(const char* atelier_conf)
{
    char        temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.layout", atelier_conf);

    if (!strcmp(_config->getString(temp).c_str(), "zit-layout")) {
       	_layout = new CZenInTwiningLayout();
        if (!_layout) return false;
    } else if (!strcmp(_config->getString(temp).c_str(), "ZITLayout")) {
       	_layout = new CTickDataLayout();
        if (!_layout) return false;
    }
    return true;
}
*/
const bool CQksGuiFactory::init_factory()
{
    _atelier_creator    = new CQksAtelierCreator();
    _canvas_creator     = new CQksCanvasCreator();
    _layout_creator     = new CQksLayoutCreator();

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
// class CQksCanvasCreator
ICanvas* CQksCanvasCreator::CreateCanvas(const char* canvas_config)
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
            } else if (!strcmp(type, "zen-in-twining")) {
                canvas = new CZitCanvas(name);
            } else if (!strcmp(type, "PVICanvas")) {
                canvas = new CPVI_Canvas(name);
            } else if (!strcmp(type, "gridctrl")) {
                canvas = new CTickdataCanvas(name);
            } else if (!strcmp(type, "euclid")) {
                canvas = new CEuclidCanvas(name);
            } else if (!strcmp(type, "analyst-canvas")) {
                canvas = new CAnalystCanvas(name);
            } else if (!strcmp(type, "pvi-analyst-canvas")) {
                canvas = new CPVIAnalysisCanvas(name);
            }

            return canvas;

        }catch(...){
            MYTRACE(L"CQksCanvasCreator::CreateCanvas\n");
        }
    }
    return canvas;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CQksDataContainerCreator
IDataContainer* CQksDataContainerCreator::CreateContainer(const char* conf_pos)
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
        } else if (!strcmp(type, "history")) {
            _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.name", conf_pos);
            const std::string strname = _config->getString(temp);
            const char* name = strname.c_str();
            container = new CHistoryDataContainer(name);
            container->SetConfig(_config, conf_pos);
        }

        return container;
    }catch(...){
        MYTRACE(L"CQksDataContainerCreator::CreateContainer\n");
    }
    return (0);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CQksDataDispatcherCreator
IDataDispatcher* CQksDataDispatcherCreator::CreateDispathcer(const char* conf_pos)
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
        MYTRACE(L"CQksDataDispatcherCreator::CreateDispathcer\n");
    }
    return (0);
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CQksAtelierCreator
IAtelier* CQksAtelierCreator::CreateAtelier(const char* conf_pos)
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.atelier-type", conf_pos);

    IAtelier* atelier = (0);
    if (!strcmp(_config->getString(temp).c_str(), "ZITAtelier")) {
	    atelier = new CTickDataAtelier("zen-in-twining");
	    if (atelier) atelier->SetConfigFile(_config, conf_pos);
    } else if (!strcmp(_config->getString(temp).c_str(), "EuclidAtelier")) {
	    atelier = new CEuclidAtelier("eulid-atelier");
	    if (atelier) atelier->SetConfigFile(_config, conf_pos);
    } else if (!strcmp(_config->getString(temp).c_str(), "analyst-atelier")) {
	    atelier = new CAnalystAtelier("analyst-atelier");
	    if (atelier) atelier->SetConfigFile(_config, conf_pos);
    }
    return atelier;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CQksLayoutCreator
ILayout* CQksLayoutCreator::CreateLayout(const char* layout_config)
{
    try {
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.type", layout_config);

        ILayout* layout = (0);
        if (!strcmp(_config->getString(temp).c_str(), "ZITLayout")) {
	        layout = new CTickDataLayout();
	        if (layout) layout->SetConfig(_config, layout_config);
        } else if (!strcmp(_config->getString(temp).c_str(), "EuclidLayout")) {
	        layout = new CEuclidLayout();
	        if (layout) layout->SetConfig(_config, layout_config);
        } else if (!strcmp(_config->getString(temp).c_str(), "analyst-layout")) {
	        layout = new CAnalystLayout();
	        if (layout) layout->SetConfig(_config, layout_config);
        }
        return layout;
    } catch(...) {
        MYTRACE(L"CQksLayoutCreator::CreateLayout\n");
        return (0);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CQksCalculator
ICalculator* CQksCalculatorCreator::CreateCalculator(const char* str_conf)
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
        } else if (!strcmp(_config->getString(temp).c_str(), "predictor-calculator")) {
            _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.name", str_conf);
            const std::string strname = _config->getString(temp);
            const char* name = strname.c_str();
	        calculator = new CPredictorCalculator(name);
	        if (calculator) calculator->SetConfig(_config, str_conf);
        }
        return calculator;
    } catch(...) {
        MYTRACE(L"CQksLayoutCreator::CreateLayout\n");
        return (0);
    }
}

