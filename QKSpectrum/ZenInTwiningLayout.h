#ifndef __qks_zen_in_twining_h_include__
#define __qks_zen_in_twining_h_include__

class CZenInTwiningLayout : public WARMGUI::ILayout
{
public:
    CZenInTwiningLayout(void);
    ~CZenInTwiningLayout(void);

    virtual void Disposal(WARMGUI::CAtelier* atelier, RECT& rect);

};

#endif //__qks_zen_in_twining_h_include__