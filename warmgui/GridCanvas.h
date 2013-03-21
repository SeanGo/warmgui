#ifndef __grid_canvas_h__
#define __grid_canvas_h__

namespace WARMGUI {
class WARMGUI_API CGridCanvas : public ICanvas
{
public:
    CGridCanvas(void);
	CGridCanvas(const TCHAR * name);
    ~CGridCanvas(void);

	virtual HRESULT Init();
	virtual void SetGlyphRect();

protected:
    CWarmGridCtrl* _gc;
};

}

#endif //__grid_canvas_h__
