#ifndef __qks_test1_atelier_h_include__
#define __qks_test1_atelier_h_include__

class CTest1Layout : public WARMGUI::ILayout
{
public:
	CTest1Layout();
	~CTest1Layout();

	virtual void Disposal(WARMGUI::IAtelier* atelier, RECT& rect);
};


class CTest1Atelier : public WARMGUI::IAtelier
{
public:
    CTest1Atelier();

	virtual HRESULT InitAtelier(HWND hwnd, WARMGUI::CWarmguiConfig* config);
	virtual void SetLayout();
	friend class CTest1Layout;

    CPVICanvas* GetPVICanvas() {return _pvicanvas;}

private:
    WARMGUI::CBkgCanvas *_canvasImgbkg;
    CPVICanvas*             _pvicanvas;
	CTest1Layout*           _layoutOne;
};

#endif //__qks_test1_atelier_h_include__