#ifndef __warmgui_scroll_bar_h__
#define __warmgui_scroll_bar_h__


namespace WARMGUI {
class WARMGUI_API CScrollBar : public IGlyph
{
public:
    CScrollBar(void);
    virtual ~CScrollBar(void);
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CScrollBar"); }
};

}//namespace WARMGUI

#endif //__warmgui_scroll_bar_h__