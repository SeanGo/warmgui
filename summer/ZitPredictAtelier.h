#ifndef __zit_predict_atelier__
#define __zit_predict_atelier__

class CZitPredictAtelier : public WARMGUI::IAtelier_summer
{
public:
    CZitPredictAtelier(void);
    CZitPredictAtelier(const char* name);
    ~CZitPredictAtelier(void);

    virtual HRESULT init(HWND hwnd);
    virtual void    disposal(RECT& rect);
    void Changed(GLYPH_CHANGED_TYPE change_type);
private:
    void SetClass() { SetMyClass("CZitPredictAtelier"); }

    WARMGUI::CBkgCanvas_summer* _bkg_canvas;
    WARMGUI::CToolbar_summer*      _toolbar;
    CZitPredictCanvas *         _canvas_zp1;
};

#endif //__zit_predict_atelier__