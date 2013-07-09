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
    WARMGUI::CToolbar_summer*   _toolbar;
    CCtpmdCanvas_summer*        _ctp_canvas;
    CTickdataCanvas*            _tick_canvas;
};

#endif //__zit_predict_atelier__