#ifndef __login_dialog_h_include__
#define __login_dialog_h_include__

class CLoginDlg : public IDxut9Dialog
{
public:
	CLoginDlg(void);
	~CLoginDlg(void);


	virtual void    InitDialog();

	virtual HRESULT OnResetDevice ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
	virtual void    KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown);
	virtual void    OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl);

protected:
	enum UI_CONTROL_IDS {
		IDC_EDIT_USERNAME   = 11,
		IDC_EDIT_PASSWORD   = 12,
		IDC_STATIC_USERNAME = 13,
		IDC_STATIC_PASSWORD = 14,
		IDC_BTN_LOGIN       = 15,
		IDC_BTN_CANCEL      = 16,
		IDC_STATIC_COMMENTS = 17,
	};

public:
	char _username[64];
	char _password[64];

};



#endif //__login_dialog_h_include__