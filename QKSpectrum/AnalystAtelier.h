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

protected:
    ///canvases
    WARMGUI::CBkgCanvas* _canvas_imgbkg;
    CAnalystCanvas*         _rt_analyst;
    CAnalystCanvas*         _1m_analyst;
    CAnalystCanvas*         _5m_analyst;
    CAnalystCanvas*        _15m_analyst;
    WARMGUI::CToolbar*         _toolbar;
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CAnalystAtelier"); }
};

#endif //__analyst_atelier_h__