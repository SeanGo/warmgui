#ifndef __interface_warmgui_factory_h__
#define __interface_warmgui_factory_h__

namespace WARMGUI {

class WARMGUI_API IGuiFactory
{
public:
    IGuiFactory(void)
        : _atelier(0)
        , _layout(0)
        , _config(0)
        , _layout_creator(0)
        , _canvas_creator(0)
        , _atelier_creator(0)
    {
    }

    virtual ~IGuiFactory(void) 
    {
        DeleteAll();
    }


    const IAtelier*            GetAtelier()   {return _atelier;}
    const ILayout*             GetLayout()    {return _layout; }

    virtual const bool         init_factory() = 0;

public:
    const bool CreateGui(CWarmguiConfig* config, const char* atelier_name, HWND hwnd)
    {
        if (!config) return false;
        DeleteAll();

        _config = config;

        if (init_factory())
            return CreateEverething(hwnd, atelier_name);
        else
            return false;
    }

protected:
    void DeleteAll()
    {
        SafeDelete(_layout_creator);
        SafeDelete(_canvas_creator);
        SafeDelete(_atelier_creator);
    }

    virtual const bool CreateEverething(HWND hwnd, const char* atelier_name);

    void AttachToAtelier(ICanvas* canvas, const char* canvas_config)
    {
        canvas->SetConfig(_config);
        canvas->SetConfigStr(canvas_config);
	    canvas->SetAtelier(_atelier);
        canvas->SetCommonArtist(_atelier->GetCommonArtist());
        canvas->SetBitmap(_atelier->GetAppBitmap());
	    canvas->Init();
        _atelier->Insert(canvas);
    }

    const bool CreateCanvas(const char* canvas_conf)      ;

protected:
    IAtelierCreator*             _atelier_creator;
    ICanvasCreator*               _canvas_creator;
    ILayoutCreator*               _layout_creator;

    IAtelier*                            _atelier;
    ILayout*                              _layout;
    CWarmguiConfig*                       _config;
};



} //namespace WARMGUI

#endif //__interface_warmgui_factory_h__