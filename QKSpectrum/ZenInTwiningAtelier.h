#ifndef __qks_zen_in_twining_atelier_h_include__
#define __qks_zen_in_twining_atelier_h_include__

class CZenInTwiningLayout;
class CZenInTwiningAtelier :  public WARMGUI::CAtelier
{
public:
    CZenInTwiningAtelier(const TCHAR* name);

    friend class CZenInTwiningLayout;

    virtual HRESULT      InitAtelier(HWND hwnd, WARMGUI::CWarmguiConfig* config);
    virtual void         SetLayout();
    void                 SetChatToDataContanier();

	void                 ToggleToolbar();
    CPVICanvas*          GetPVICanvas() {return _pvicanvas;}

    void                 ReceiveData(DataObjectPtr dop, const char* name);
    CTPTIMESEC&          GetTimeSec() { return _ctptimesec; }
protected:
    WARMGUI::CBkgCanvas* _canvasImgbkg;
    CPVICanvas*             _pvicanvas;
    CZenInTwiningLayout*    _layoutOne;
    CZenInTwiningCanvas*     _rtcanvas;
    CZenInTwiningCanvas*     _1mcanvas;
    CZenInTwiningCanvas*     _5mcanvas;
    CZenInTwiningCanvas*    _15mcanvas;
    WARMGUI::CGridCanvas*     _message;
    WARMGUI::CGridCanvas*    _trademsg;
	WARMGUI::CToolbar*        _toolbar;

    CTPTIMESEC             _ctptimesec;
};

#endif //__qks_zen_in_twining_atelier_h_include__