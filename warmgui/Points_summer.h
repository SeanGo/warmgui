#ifndef __points_summer__
#define __points_summer__

namespace WARMGUI {
class WARMGUI_API CPoints_summer : public IDataGraph_summer
{
public:
    CPoints_summer(const char* name);
    ~CPoints_summer(void);

    enum POINT_TEXT_POSISION {
        UP,
        DOWN,
        LEFT,
        RIGHT,
    };

    HRESULT         draw(float x, float y, char c, POINT_TEXT_POSISION pos, COLORALPHA clr);
    virtual HRESULT draw(bool redraw = false) { return S_OK; }
    virtual HRESULT init() { return (0); }
    void            set_txt_format(IDWriteTextFormat* pTextFormat)
                    { _pTextFormat = pTextFormat; }
    virtual void    set_world(CWorld* world) { _world = world; }
    virtual void    set_rect(RECT& rect)
                    {
                        _abs_rect = rect;
                        _rect.left = _rect.top = 0, _rect.right = RectWidth(rect), _rect.bottom = RectHeight(rect);
                    }
    void            set_artist(eArtist* artist) { _artist = artist; }
private:
    char _c;
    POINT_TEXT_POSISION _pos;
    COLORALPHA                 _clr;
    IDWriteTextFormat* _pTextFormat;
    //D2D1_POINT_2F pnt;

private:
    void SetClass() { SetMyClass("CPoints_summer"); }
};



} //namespace WARMGUI
#endif//__points_summer__