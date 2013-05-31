#ifndef __euclid_factories_h__
#define __euclid_factories_h__

class CEuclidGuiFactory :  public WARMGUI::IGuiFactory
{
protected:
    virtual const bool init_factory();
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CEuclidCanvasCreator : public WARMGUI::ICanvasCreator
{
public:
    virtual ICanvas* CreateCanvas(const char* config);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CEuclidDataContainerCreator : public WARMGUI::IDataContainerCreator
{
public:
    virtual IDataContainer* CreateContainer(const char* conf_pos);
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CEuclidDataDispatcherCreator : public WARMGUI::IDataDispatcherCreator
{
public:
    virtual IDataDispatcher* CreateDispathcer(const char* conf_pos);

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CEuclidAtelierCreator : public WARMGUI::IAtelierCreator
{
public:
    virtual IAtelier* CreateAtelier(const char* conf_pos);

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CEuclidLayoutCreator
class CEuclidLayoutCreator : public WARMGUI::ILayoutCreator
{
public:
    virtual ILayout* CreateLayout(const char* layout_config);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CEuclidCalculator
class CEuclidCalculatorCreator : public WARMGUI::ICalculatorCreator
{
public:
    virtual ICalculator* CreateCalculator(const char* str_conf);

};

extern CTPTIMESEC g_ctpsec;


#endif //__euclid_factories_h__