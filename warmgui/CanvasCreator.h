#ifndef __warmgui_canvas_factory_h_include__
#define __warmgui_canvas_factory_h_include__

namespace WARMGUI {

class WARMGUI_API ICanvasCreator
{
public:
    ICanvasCreator(void);
    virtual ~ICanvasCreator(void);


    void SetConfigure(CWarmguiConfig* config) { _config = config; }
    virtual ICanvas* CreateCanvas(const char* canvas_config);


private:
    CBkgCanvas* create_bkgcanvas(const char* name);

protected:
    CWarmguiConfig* _config;
};


} //namespace WARMGUI

#endif //__warmgui_canvas_factory_h_include__
