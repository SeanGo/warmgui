#ifndef __qks_zen_in_twining_atelier_h_include__
#define __qks_zen_in_twining_atelier_h_include__

class CZenInTwiningLayout;
class CZenInTwiningAtelier :  public WARMGUI::IAtelier
{
public:
    CZenInTwiningAtelier(const char* name);

    friend class CZenInTwiningLayout;

    virtual HRESULT      InitAtelier(HWND hwnd, WARMGUI::CWarmguiConfig* config);

	void                 ToggleToolbar();
    CPVICanvas*          GetPVICanvas() {return _pvicanvas;}

    void                 ReceiveData(DataObjectPtr dop, const char* name);
    CTPTIMESEC&          GetTimeSec() { return _ctptimesec; }

    inline virtual bool  GetAllCanvas();

protected:
    CZenInTwiningLayout*    _layoutOne;

    ///canvases
    WARMGUI::CBkgCanvas* _canvasImgbkg;
    CPVICanvas*             _pvicanvas;
    CZenInTwiningCanvas*     _rtcanvas;
    CZenInTwiningCanvas*     _1mcanvas;
    CZenInTwiningCanvas*     _5mcanvas;
    CZenInTwiningCanvas*    _15mcanvas;
    WARMGUI::CGridCanvas*     _message;
    WARMGUI::CGridCanvas*    _trademsg;
	WARMGUI::CToolbar*        _toolbar;

    CTPTIMESEC             _ctptimesec;
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CZenInTwiningAtelier"); }
};

#endif //__qks_zen_in_twining_atelier_h_include__