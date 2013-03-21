#ifndef __warmgui_toolbar_h_include__
#define __warmgui_toolbar_h_include__

namespace WARMGUI {
class WARMGUI_API CToolbar : public ICanvas
{
public:
	CToolbar(void);
	CToolbar(const TCHAR* name, UINT nFirstCommand);
	~CToolbar(void);

	virtual HRESULT Init();
	virtual void    SetGlyphRect(void);
	virtual int     OnMouseMove(int x, int y);
    virtual int     OnLButtonUp(int x, int y);

public:
	TOOLBAR_POSITION _pos;
	SIZE        _btn_size;
	int         _interval;	///the interval of button
    UINT       _first_cmd;

protected:
	CSharedImage* _imgToolbar;

    UINT GetCommandID(int x);
};

}
#endif //__warmgui_toolbar_h_include__

