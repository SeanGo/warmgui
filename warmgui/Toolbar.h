#ifndef __warmgui_toolbar_h_include__
#define __warmgui_toolbar_h_include__

namespace WARMGUI {
class WARMGUI_API CToolbar : public ICanvas
{
public:
	CToolbar(void);
    CToolbar(const char* name);
	CToolbar(const char* name, UINT nFirstCommand);
	~CToolbar(void);

	virtual const HRESULT Init(const char* name = 0);
	virtual void  SetGlyphRect(void);
	virtual int   OnMouseMove(int x, int y);
    virtual int   OnLButtonUp(int x, int y);
    RECT&         get_toolbar_rect() {return _toolbar_rect;}
    virtual void  SetRect(RECT& rect);

public:
	TOOLBAR_POSITION _pos;
	SIZE        _btn_size;
	int         _interval;	///the interval of button
    UINT       _first_cmd;
    RECT    _toolbar_rect;
    int           _margin;
protected:
	CSharedImage* _imgToolbar;

    UINT GetCommandID(int x);

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CToolbar"); }
};

}
#endif //__warmgui_toolbar_h_include__

