#ifndef __qks_zen_in_twining_layout_h__
#define __qks_zen_in_twining_layout_h__

class CZenInTwiningLayout : public WARMGUI::ILayout
{
public:
    CZenInTwiningLayout(void);
    ~CZenInTwiningLayout(void);

    virtual void Disposal(WARMGUI::IAtelier* atelier, RECT& rect);

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CZenInTwiningLayout"); }
};

#endif //__qks_zen_in_twining_layout_h__