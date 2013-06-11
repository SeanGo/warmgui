#ifndef __qks_zit_atelier_h_include__
#define __qks_zit_atelier_h_include__

class CTickDataLayout;
class CTickDataAtelier :  public WARMGUI::IAtelier
{
public:
    CTickDataAtelier(const char* name);

    friend class CTickDataLayout;

    virtual HRESULT      InitAtelier(HWND hwnd, WARMGUI::CWarmguiConfig* config);

	void                 ToggleToolbar();

    inline virtual bool  GetAllCanvas();

protected:
    ///canvases
    WARMGUI::CBkgCanvas* _canvasImgbkg;
	WARMGUI::CToolbar*        _toolbar;
    CPVI_Canvas*            _pvicanvas;
    CTickdataCanvas*  _tickdata_canvas;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CTickDataAtelier"); }
};

#endif //__qks_zit_atelier_h_include__