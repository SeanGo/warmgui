#ifndef __interface_warmgui_factory_h__
#define __interface_warmgui_factory_h__

namespace WARMGUI {

class WARMGUI_API IGuiFactory
{
public:
    IGuiFactory(void)
        : _atelier(0)
        , _hwnd(0)
        , _dispatcher(0)
        , _layout(0)
        , _config(0)
    {
    }

    virtual ~IGuiFactory(void) 
    {
        if (_dispatcher) {
            PostThreadMessage(_dispatcher->GetThreadId(), MSG_EXIT_DISPATCHER, 0, 0);
            Sleep(1000);
            SafeDelete(_dispatcher);
        }
        SafeDelete(_layout);
        SafeDelete(_atelier);
    }

    bool CreateGui(HWND hwnd, CWarmguiConfig* config)
    {
        if (!config) return false;
        SafeDelete(_atelier);
        SafeDelete(_layout);
        SafeDelete(_dispatcher);

        _atelier = 0, _hwnd = hwnd, _config = config;
        return CreateEverething();
    }

    CAtelier*        GetAtelier()        {return _atelier;}
    ILayout*         GetLayout()         {return _layout; }
    IDataDispatcher* GetDataDispathcer() {return _dispatcher;}

protected:
    virtual bool CreateEverething() {
        bool r = true;

        if (r) r = CreateDispatcher();
        if (r) r = CreateAtelier();       
        if (r) r = CreateLayout();
        if (r) r = CreateCanvas();

        return (r);
    }
    virtual bool CreateAtelier()       = 0;
    virtual bool CreateDispatcher()    = 0;
    virtual bool CreateCanvas()        = 0;
    virtual bool CreateLayout()        = 0;

protected:
    CAtelier*           _atelier;
    IDataDispatcher* _dispatcher;
    ILayout*             _layout;
    HWND                   _hwnd;
    CWarmguiConfig*      _config;
};


} //namespace WARMGUI

#endif //__interface_warmgui_factory_h__