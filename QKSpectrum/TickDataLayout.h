#ifndef __qks_zit_layout_h_include__
#define __qks_zit_layout_h_include__

class CTickDataLayout : public WARMGUI::ILayout
{
public:
    CTickDataLayout(void);
    ~CTickDataLayout(void);

    virtual void Disposal(WARMGUI::IAtelier* atelier, RECT& rect);
    virtual void SetConfig(CWarmguiConfig* config, const char* conf_str);
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CTickDataLayout"); }
};

#endif //__qks_zit_layout_h_include__