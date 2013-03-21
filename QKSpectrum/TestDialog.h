#pragma once

class CTestDialog :	public IDxut9Dialog
{
public:
	CTestDialog(void);
	~CTestDialog(void);

protected:
	virtual void    InitDialog();
	virtual int     DestroyDialog();

	virtual HRESULT OnResetDevice ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
	virtual void    KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown);
	virtual void    OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl);

protected:
	enum UI_CONTROL_IDS {
		IDC_TOGGLEFULLSCREEN = 1,
		IDC_TOGGLEREF        = 3,
		IDC_CHANGEDEVICE     = 4,
		IDC_EDITBOX1            ,
		IDC_EDITBOX2            ,
		IDC_ENABLEIME           ,
		IDC_DISABLEIME          ,
		IDC_COMBOBOX            ,
		IDC_STATIC_DXUT         ,
		IDC_OUTPUT          = 12,
		IDC_SLIDER              ,
		IDC_CHECKBOX            ,
		IDC_CLEAREDIT           ,
		IDC_RADIO1A             ,
		IDC_RADIO1B             ,
		IDC_RADIO1C             ,
		IDC_RADIO2A             ,
		IDC_RADIO2B             ,
		IDC_RADIO2C             ,
		IDC_LISTBOX             ,
		IDC_LISTBOXM            ,
	};
};

