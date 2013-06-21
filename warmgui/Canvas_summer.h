#ifndef __warmgui_summer_interface_canvas__
#define __warmgui_summer_interface_canvas__

namespace WARMGUI {

class IAtelier_summer;
class WARMGUI_API ICanvas_summer : public IGlyph_summer {
public:
    ICanvas_summer();
    ICanvas_summer(const char *name);
    ~ICanvas_summer();

    void        inherit(IAtelier_summer* atelier, CGlyphTree_summer* tree, GlyphTreeIter_summer& tree_iter, eArtist* artist, CWarmguiConfig* config)
                {_atelier = atelier, _glyph_tree = tree, _tree_iter = tree_iter, _artist = artist, _config = config;;}

    inline void append_glyph(IGlyph_summer* glyph);

protected:
    IAtelier_summer* _atelier;
private:
    void setClass() { SetMyClass("ICanvas_summer"); }
};

} //namespace WARMGUI


#endif //__warmgui_summer_interface_canvas__