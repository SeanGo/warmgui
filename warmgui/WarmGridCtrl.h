#ifndef __grid_control_h__
#define __grid_control_h__

namespace WARMGUI {
class WARMGUI_API CWarmGridCtrl : public IGlyph
{
public:
    CWarmGridCtrl();
    CWarmGridCtrl(const TCHAR * name, bool own_artist = false);
    ~CWarmGridCtrl(void);

  	virtual HRESULT DrawGraph(bool redraw = false);


protected:
    int ncol;   ///number of columns
    int nrow;   ///number of rows
    int nrhead; ///number of head of row
    int nchead; ///number of head of row
};

}

#endif //__grid_control_h__