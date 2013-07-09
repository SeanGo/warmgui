#ifndef __tickdata_grid_ctrl__
#define __tickdata_grid_ctrl__

class CTickdataGrid : public WARMGUI::CGridCtrl
{
public:
    CTickdataGrid(const char* name);
    ~CTickdataGrid(void);

    virtual GLYPH_CHANGED_TYPE update(dataptr data);
    virtual HRESULT init();

protected:
    TCHAR           _head[MAX_PATH];
    CTPMMD          _ctpmmd;
    size_t          _count;

    inline void     SetCtpData(GV_ITEM& item);

private:
    void SetClass() { SetMyClass("CTickdataGrid"); }
};

#endif//__tickdata_grid_ctrl__