#ifndef __grid_canvas_h__
#define __grid_canvas_h__

namespace WARMGUI {
class WARMGUI_API CGridCanvas : public ICanvas
{
public:
    CGridCanvas(void);
	CGridCanvas(const char* name);
    ~CGridCanvas(void);

	virtual const HRESULT Init(const char* name = 0);
	virtual void SetGlyphRect();

protected:
    CGridCtrl* _gc;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CGridCanvas"); }
};

}

#endif //__grid_canvas_h__
