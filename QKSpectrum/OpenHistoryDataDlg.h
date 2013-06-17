#ifndef __open_history_data_dialog_h__
#define __open_history_data_dialog_h__

class COpenHistoryDataDlg :	public IDxut9Dialog
{
public:
    COpenHistoryDataDlg(void);
    ~COpenHistoryDataDlg(void);

    void            set_data_path(const TCHAR* datapath);
    void            set_data_path(const char* datapath);

protected:
	virtual void    InitDialog();
	virtual int     DestroyDialog();

	virtual HRESULT OnResetDevice ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
	virtual void    KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown);
	virtual void    OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl);

protected:
	enum UI_CONTROL_IDS {
		IDC_EDIT_DATA_PATH  = 11,
		IDC_EDIT_CODE           ,
		IDC_EDIT_DATE           ,
		IDC_STATIC_DATA_PATH    ,
		IDC_STATIC_CODE         ,
		IDC_STATIC_DATE         ,
		IDC_BTN_OPEN            ,
		IDC_BTN_CANCEL          ,
    };

public:
	TCHAR  filepath[MAX_PATH];
    TCHAR  code[MAX_PATH];
    TCHAR  date[MAX_PATH];

protected:
    TCHAR _datapath[MAX_PATH];
    MARGIN _margin;
};

#endif //__open_history_data_dialog_h__