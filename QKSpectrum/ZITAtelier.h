#ifndef __qks_zit_atelier_h_include__
#define __qks_zit_atelier_h_include__

class CZITLayout;
class CZITAtelier :  public WARMGUI::IAtelier
{
public:
    CZITAtelier(const char* name);

    friend class CZITLayout;

    virtual HRESULT      InitAtelier(HWND hwnd, WARMGUI::CWarmguiConfig* config);

	void                 ToggleToolbar();

    inline virtual bool  GetAllCanvas();

protected:
    CZITLayout*    _layoutOne;

    ///canvases
    WARMGUI::CBkgCanvas* _canvasImgbkg;
	WARMGUI::CToolbar*        _toolbar;
    CPVI_Canvas*            _pvicanvas;
    CTickdataCanvas*  _tickdata_canvas;
    CZitCanvas*            _zit_canvas;
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CZITAtelier"); }
};

#endif //__qks_zit_atelier_h_include__