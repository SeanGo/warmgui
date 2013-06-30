#ifndef __coord_frame_warmgui_summer__
#define __coord_frame_warmgui_summer__

namespace WARMGUI {

class CCoordFrame_summer : public IDataGraph_summer
{
public:
    CCoordFrame_summer(void);
    ~CCoordFrame_summer(void);

    virtual GLYPH_CHANGED_TYPE update(dataptr data);
    virtual GLYPH_CHANGED_TYPE new_data(DataObjectPtr dop);

};

}

#endif //__coord_frame_warmgui_summer__