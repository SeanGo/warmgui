#ifndef __zenintwining_gui_factory_h__
#define __zenintwining_gui_factory_h__

class CQksGuiFactory :  public WARMGUI::IGuiFactory
{
protected:
    virtual const bool CreateEverething()                       ;
    virtual const bool CreateAtelier(int num)                   ;
    virtual const bool CreateDispatcher(const char* atelier_conf);
    virtual const bool CreateLayout(const char* atelier_conf)    ;
};

#endif //__zenintwining_gui_factory_h__