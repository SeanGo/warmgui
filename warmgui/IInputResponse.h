#ifndef __interface_warmgui_input_response__
#define __interface_warmgui_input_response__

namespace WARMGUI {

class WARMGUI_API IInputResponse
{
public:
    IInputResponse(void);
    virtual ~IInputResponse(void);

	virtual int  OnKeydown      (WPARAM wParam, LPARAM lParam);
	virtual int  OnKeyup        (WPARAM wParam, LPARAM lParam);
	//virtual int  OnMouseMove    (UINT nFlags, int x, int y);
	virtual int  OnLButtonDown  (UINT nFlags, int x, int y);
	virtual int  OnLButtonUp    (UINT nFlags, int x, int y);
	virtual int  OnLButtonDblclk(UINT nFlags, int x, int y);
	virtual int  OnRButtonDown  (UINT nFlags, int x, int y);
	virtual int  OnRButtonUp    (UINT nFlags, int x, int y);
	virtual int  OnRButtonDblclk(UINT nFlags, int x, int y);
	virtual int  OnMButtonDown  (UINT nFlags, int x, int y);
	virtual int  OnMButtonUp    (UINT nFlags, int x, int y);
	virtual int  OnMButtonDblclk(UINT nFlags, int x, int y);
	virtual BOOL OnMouseWheel   (UINT nFlags, short zDelta, int x, int y);
	virtual int  OnCut          (WPARAM wParam, LPARAM lParam);
	virtual int  OnCopy         (WPARAM wParam, LPARAM lParam);
	virtual int  OnPaste        (WPARAM wParam, LPARAM lParam);
	virtual int  OnClear        (WPARAM wParam, LPARAM lParam);
	virtual int  OnUndo         (WPARAM wParam, LPARAM lParam);

};

} //namespace WARMGUI
#endif //__interface_warmgui_input_response__