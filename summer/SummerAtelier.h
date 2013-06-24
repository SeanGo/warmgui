#ifndef __summer_atelier__
#define __summer_atelier__

class CSummerAtelier : public WARMGUI::IAtelier_summer
{
public:
    CSummerAtelier(void);
    CSummerAtelier(const char* name);
    ~CSummerAtelier(void);

    virtual HRESULT init(HWND hwnd);
    virtual void    disposal(RECT& rect);
private:
    void SetClass() { SetMyClass("CSummerAtelier"); }

    WARMGUI::CBkgCanvas_summer* _bkg_canvas;
    WARMGUI::CToolbar_summer*   _toolbar;
    CTestSummerCanvas*          _ts_canvas;
};

#endif //__summer_atelier__