#ifndef __tick_data_grid_ctrl_h__
#define __tick_data_grid_ctrl_h__

class CTickDataGridCtrl :  public WARMGUI::CGridCtrl
{
public:
                       CTickDataGridCtrl(const char* name, int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
                      ~CTickDataGridCtrl(void);

    GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype);
    virtual HRESULT    Init();

protected:
    TCHAR              head[MAX_PATH];

    inline void        SetCtpData(const CTPMMD* pctpmmd, GV_ITEM& item);
};

#endif //__tick_data_grid_ctrl_h__