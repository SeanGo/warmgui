#ifndef __analyst_layout_h__
#define __analyst_layout_h__

class CAnalystLayout : public WARMGUI::ILayout
{
public:
    CAnalystLayout(void);
    ~CAnalystLayout(void);

    virtual void Disposal(WARMGUI::IAtelier* atelier, RECT& rect);
    virtual void SetConfig(CWarmguiConfig* config, const char* conf_str);

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CAnalystLayout"); }
};

#endif //__analyst_layout_h__