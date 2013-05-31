#ifndef __zenintwining_gui_factory_h__
#define __zenintwining_gui_factory_h__

class CZitGuiFactory :  public WARMGUI::IGuiFactory
{
protected:
    virtual bool CreateEverething();
    virtual bool CreateAtelier()   ;
    virtual bool CreateDispatcher();
    virtual bool CreateCanvas()    ;
    virtual bool CreateLayout()    ;

};

#endif //__zenintwining_gui_factory_h__