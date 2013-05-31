#ifndef __eulicd_layout_h__
#define __eulicd_layout_h__

class CEuclidLayout : public WARMGUI::ILayout
{
public:
    CEuclidLayout(void);
    ~CEuclidLayout(void);

    virtual void Disposal(WARMGUI::IAtelier* atelier, RECT& rect);
    virtual void SetConfig(CWarmguiConfig* config, const char* conf_str);

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CEuclidLayout"); }
};

#endif //__eulicd_layout_h__