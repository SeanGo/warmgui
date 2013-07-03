#ifndef __warmgui_scroll_bar_h__
#define __warmgui_scroll_bar_h__


namespace WARMGUI {
class WARMGUI_API CScrollBar_summer : public IGlyph_summer
{
public:
    CScrollBar_summer(void);
    virtual ~CScrollBar_summer(void);
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CScrollBar_summer"); }
};

}//namespace WARMGUI

#endif //__warmgui_scroll_bar_h__