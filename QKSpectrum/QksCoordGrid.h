#ifndef __qks_coord_grid_h__
#define __qks_coord_grid_h__

class CQksCoordGrid : public WARMGUI::CCoordGrid
{
public:
    CQksCoordGrid(const char* name);
    ~CQksCoordGrid(void);

	//virtual HRESULT         DrawGraph(bool redraw/* = false*/);
    inline  virtual HRESULT Init();
    inline  virtual bool    AddNewData(DataObjectPtr dop);
private:
    int predict_len;
};


#endif //__qks_coord_grid_h__
