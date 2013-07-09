#ifndef __tickdata_grid_canvas__
#define __tickdata_grid_canvas__

class CTickdataCanvas : public WARMGUI::ICanvas_summer
{
public:
    CTickdataCanvas(const char * name);
    ~CTickdataCanvas(void);

    virtual HRESULT init();
    virtual void    set_rect(RECT& rect);

protected:
    WARMGUI::CGridCtrl* _data_grid;
};

#endif //__tickdata_grid_canvas__