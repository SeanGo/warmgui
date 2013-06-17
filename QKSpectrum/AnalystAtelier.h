#ifndef __analyst_atelier_h__
#define __analyst_atelier_h__

class CAnalystAtelier : public WARMGUI::IAtelier
{
public:
    CAnalystAtelier(const char* name);
    ~CAnalystAtelier(void);

    friend class CAnalystLayout;

    virtual HRESULT      InitAtelier(HWND hwnd, WARMGUI::CWarmguiConfig* config);
	void                 ToggleToolbar();
    inline virtual bool  GetAllCanvas();
    void                 set_analyst(EUCLID::CEuclidAnalyst* analyst);
    HRESULT              draw_data();

protected:
    ///canvases
    WARMGUI::CBkgCanvas* _canvas_imgbkg;
    CPVIAnalysisCanvas*     _rt_analyst;
    CPVIAnalysisCanvas*     _1m_analyst;
    CPVIAnalysisCanvas*     _5m_analyst;
    CPVIAnalysisCanvas*    _15m_analyst;
    WARMGUI::CToolbar*         _toolbar;

    EUCLID::CEuclidAnalyst*    _analyst;
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CAnalystAtelier"); }
};

#endif //__analyst_atelier_h__