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

private:
    virtual HRESULT            _predraw();
    virtual HRESULT            _draw(bool redraw_all = false);
    virtual void               _update_data();

    //WARMGUI::CCurveGraph_summer* _infl;
};

#endif //__inflexion_lines__