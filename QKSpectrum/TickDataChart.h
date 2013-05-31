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
    WARMGUI::CDataLineGraph*         _dlg_price;
    WARMGUI::CDataLineGraph*    _dlg_lt_prcappr;   ///long  term approx
    WARMGUI::CDataLineGraph*    _dlg_st_prcappr;   ///short term approx
    WARMGUI::CDataLineGraph*    _dlg_lt_predict;   ///long term predcit
    WARMGUI::CDataLineGraph*    _dlg_st_predict;   ///short term predict
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