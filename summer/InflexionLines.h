#ifndef __inflexion_lines__
#define __inflexion_lines__

class CInflexionLines : public WARMGUI::IDataGraph_summer
{
public:
    CInflexionLines(const char* name);
    ~CInflexionLines(void);

    virtual HRESULT            init();
    virtual GLYPH_CHANGED_TYPE update(dataptr data);
    virtual GLYPH_CHANGED_TYPE new_data(DataObjectPtr dop);
    virtual void               set_rect(RECT& rect);

    virtual HRESULT            predraw();
    virtual HRESULT            draw(bool redraw_all = false);
    virtual void               update_data();

    //WARMGUI::CCurveGraph_summer* _infl;
};

#endif //__inflexion_lines__