#ifndef __dxut9_dialog_h_include__
#define __dxut9_dialog_h_include__

#include "DXUT.h"

#include "SDKmisc.h"
#include "SDKmesh.h"
#include "DXUTsettingsdlg.h"
#include "DXUTres.h"
#include "DXUTmisc.h"
#include "DXUTgui.h"
#include "DXUTguiIME.h"
#include "DXUTLockFreePipe.h"
#include "DXUTcamera.h"
#include "DXUTDevice9.h"
#include "DXUTDevice11.h"
#include "SDKsound.h"
#include "SDKwavefile.h"

class IDxut9Dialog
{
public:
	IDxut9Dialog(void);
	virtual ~IDxut9Dialog(void);

	int      ShowDialog(const TCHAR* szTitle,
					    const HINSTANCE hInstance = 0,
						const HICON hicon = 0,
						const HMENU hmenu = 0,
						const int cx = 0,
						const int cy = 0);

	void SetFontSize(UINT nSize1, UINT nSize2) { _nFontSize1 = nSize1, _nFontSize2 = nSize2; }
	void ShowHelper(bool bShow = true) { _show_helper = bShow; }
	UINT GetDialogValue() { return _btn_value; }

protected:
	// calback /////////////////////////////////////////
	static bool    CALLBACK cb_IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
	static bool    CALLBACK cb_ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
	static HRESULT CALLBACK cb_OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static HRESULT CALLBACK cb_OnResetDevice ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,  void* pUserContext );
	static void    CALLBACK cb_OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
	static void    CALLBACK cb_OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	static LRESULT CALLBACK cb_MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
	static void    CALLBACK cb_KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
	static void    CALLBACK cb_OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
	static void    CALLBACK cb_OnLostDevice( void* pUserContext );
	static void    CALLBACK cb_OnDestroyDevice( void* pUserContext );

protected:
	void            SetCallback();
	void            RenderText();

	virtual void    InitDialog() = 0;
	virtual int     DestroyDialog() {return (0);}

	virtual bool    IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed);
	virtual bool    ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings);
	virtual HRESULT OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
	virtual HRESULT OnResetDevice ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc) = 0;
	virtual void    OnFrameMove( double fTime, float fElapsedTime);
	virtual void    OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);
	virtual void    KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown) = 0;
	virtual void    OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl) = 0;
	virtual void    OnLostDevice();
	virtual void    OnDestroyDevice();

protected:
	ID3DXFont*                  _pFont;                // Font for drawing text
	ID3DXSprite*                _pTextSprite;          // Sprite for batching draw text calls
	ID3DXEffect*                _pEffect;              // D3DX effect interface
	CDXUTXFileMesh              _Mesh;                 // Background mesh
	D3DXMATRIXA16               _mView;
	CModelViewerCamera          _Camera;               // A model viewing camera
	CDXUTDialogResourceManager  _DialogResourceManager; // manager for shared resources of dialogs
	CDXUTDialog                 _SampleUI;             // dialog for sample specific controls

	UINT _nFontSize1, _nFontSize2;
	bool _show_helper;
	UINT _btn_value;
	HWND _hwnd;
};

#endif //__dxut_dialog_h_include__
