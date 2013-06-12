#ifndef __price_tickdata_chart_h__
#define __price_tickdata_chart_h__

////////////////////////////////////////////////////////////////////
// class CPriceTickChart
class CPriceTickChart : public WARMGUI::CDataLineChart
{
public:
    CPriceTickChart(const char* name, bool world_own_type = false, bool data_own_type  = false, bool own_artist = false);
    ~CPriceTickChart(void);

    virtual HRESULT     AddGraphs();
    GLYPH_CHANGED_TYPE  NewCtpData(CCtpmmdContainer* pdc, bool redraw_cood = false, bool b_first_data = false);
    virtual void        ResetWorldRect(IDataContainer* data_cont);
private:
    WARMGUI::CDataLineGraph*             _dlg_price;
};


class CPriceAnalysChart : public WARMGUI::CDataLineChart
{
public:
    CPriceAnalysChart(const char* name, bool world_own_type = false, bool data_own_type  = false, bool own_artist = false);
    ~CPriceAnalysChart(void);

    virtual HRESULT     AddGraphs();
    GLYPH_CHANGED_TYPE  NewCtpData(CCtpmmdContainer* pdc, bool redraw_cood = false, bool b_first_data = false);
    virtual void        ResetWorldRect(IDataContainer* data_cont);
private:
    WARMGUI::CDataLineGraph*             _dlg_price;

    WARMGUI::CDataLineGraph*  _short_low_prc_approx;
    WARMGUI::CDataLineGraph* _short_high_prc_approx;
    WARMGUI::CDataLineGraph*        _short_low_prdt;
    WARMGUI::CDataLineGraph*       _short_high_prdt;

    WARMGUI::CDataLineGraph*   _long_low_prc_approx;
    WARMGUI::CDataLineGraph*  _long_high_prc_approx;
    WARMGUI::CDataLineGraph*         _long_low_prdt;
    WARMGUI::CDataLineGraph*        _long_high_prdt;
};


////////////////////////////////////////////////////////////////////
// class CVolumeTickChart
class CVolumeTickChart : public WARMGUI::CDataLineChart
{
public:
    CVolumeTickChart(const char* name, bool world_own_type = false, bool data_own_type  = false, bool own_artist = false);
    ~CVolumeTickChart(void);

    virtual HRESULT     AddGraphs();
    GLYPH_CHANGED_TYPE  NewCtpData(CCtpmmdContainer* pdc, bool redraw_cood = false, bool b_first_data = false);
    virtual void        ResetWorldRect(IDataContainer* data_cont);

private:
    WARMGUI::CDataLineGraph*        _dlg_relvol;
    WARMGUI::CDataLineGraph*        _dlg_volume;
    WARMGUI::CDataLineGraph*    _dlg_vol_approx;   ///approx of volume
};



////////////////////////////////////////////////////////////////////
// class CInterestTickChart
class CInterestTickChart : public WARMGUI::CDataLineChart
{
public:
    CInterestTickChart(const char* name, bool world_own_type = false, bool data_own_type  = false, bool own_artist = false);
    ~CInterestTickChart(void);

    virtual HRESULT     AddGraphs();
    GLYPH_CHANGED_TYPE  NewCtpData(CCtpmmdContainer* pdc, bool redraw_cood = false, bool b_first_data = false);
    virtual void        ResetWorldRect(IDataContainer* data_cont);

private:
    WARMGUI::CDataLineGraph*      _dlg_interest;
    WARMGUI::CDataLineGraph*  _dlg_rel_interest;
    WARMGUI::CDataLineGraph*    _dlg_ite_approx;   ///approx of iterest
};



#endif //__price_tickdata_chart_h__