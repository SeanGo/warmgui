#ifndef __warmgui_toolbar_summer__
#define __warmgui_toolbar_summer__

namespace WARMGUI {
class WARMGUI_API CToolbar_summer : public ICanvas_summer
{
public:
	CToolbar_summer(void);
    CToolbar_summer(const char* name);
	CToolbar_summer(const char* name, UINT nFirstCommand);
	~CToolbar_summer(void);

	virtual HRESULT init();
    virtual int     OnLButtonUp(int x, int y);
    RECT&           get_toolbar_rect() {return _toolbar_rect;}
    virtual void    set_rect(RECT& rect);

public:
	TOOLBAR_POSITION _pos;
	SIZE        _btn_size;
	int         _interval;	///the interval of button
    UINT       _first_cmd;
    RECT    _toolbar_rect;
    int           _margin;
protected:
	CSharedImage_summer* _imgToolbar;
    CBlind_summer*       _blind;
    UINT GetCommandID(int x);

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CToolbar_summer"); }
};

}
#endif //__warmgui_toolbar_summer__