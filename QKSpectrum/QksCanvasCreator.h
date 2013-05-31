#ifndef __qks_canvas_factory_h_include__
#define __qks_canvas_factory_h_include__

class CQksCanvasCreator : public WARMGUI::ICanvasCreator
{
public:
    CQksCanvasCreator(void);
    ~CQksCanvasCreator(void);

    virtual ICanvas* CreateCanvas(CWarmguiConfig* config, const char* type, const char* canvas_config);
};


#endif //__qks_canvas_factory_h_include__