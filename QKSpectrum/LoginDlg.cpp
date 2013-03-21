#include "StdAfx.h"
#include "utility.h"
#include "ChineseCodeLib.h"
#include "Dxut9Dialog.h"
#include "LoginDlg.h"


CLoginDlg::CLoginDlg(void)
{
	*_username = *_password = '\0';
}


CLoginDlg::~CLoginDlg(void)
{
}

HRESULT CLoginDlg::OnResetDevice ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{
    HRESULT hr;

    V_RETURN( _DialogResourceManager.OnD3D9ResetDevice() );

    if( _pFont )
        V_RETURN( _pFont->OnResetDevice() );
    if( _pEffect )
        V_RETURN( _pEffect->OnResetDevice() );

    // Create a sprite to help batch calls when drawing many lines of text
    V_RETURN( D3DXCreateSprite( pd3dDevice, &_pTextSprite ) );

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    _Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
    _Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

    _SampleUI.SetLocation( 0, 0 );
    _SampleUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

	_SampleUI.GetControl( IDC_STATIC_COMMENTS )->SetSize( pBackBufferSurfaceDesc->Width, _nFontSize2 );

	//const TCHAR* static_username = L"username : ";
	//const TCHAR* static_password = L"password : ";

	//_SampleUI.GetControl( IDC_STATIC_USERNAME )->SetSize( _nFontSize2 * (_tcslen(static_username) + 2), _nFontSize2 );

    /*
	_SampleUI.GetControl( IDC_OUTPUT )->SetSize( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height /
                                                  4 );
    _SampleUI.GetControl( IDC_EDITBOX1 )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 230 );
    _SampleUI.GetControl( IDC_EDITBOX1 )->SetSize( pBackBufferSurfaceDesc->Width - 40, 32 );
    if( _SampleUI.GetControl( IDC_EDITBOX2 ) )
    {
        _SampleUI.GetControl( IDC_EDITBOX2 )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 280 );
        _SampleUI.GetControl( IDC_EDITBOX2 )->SetSize( pBackBufferSurfaceDesc->Width - 40, 45 );
    }
    _SampleUI.GetControl( IDC_ENABLEIME )->SetLocation( 130, pBackBufferSurfaceDesc->Height - 80 );
    _SampleUI.GetControl( IDC_DISABLEIME )->SetLocation( 220, pBackBufferSurfaceDesc->Height - 80 );
    _SampleUI.GetControl( IDC_SLIDER )->SetLocation( 10, pBackBufferSurfaceDesc->Height - 140 );
    _SampleUI.GetControl( IDC_CHECKBOX )->SetLocation( 120, pBackBufferSurfaceDesc->Height - 50 );
    _SampleUI.GetControl( IDC_CLEAREDIT )->SetLocation( 120, pBackBufferSurfaceDesc->Height - 25 );
    _SampleUI.GetControl( IDC_COMBOBOX )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 180 );
    _SampleUI.GetControl( IDC_RADIO1A )->SetLocation( pBackBufferSurfaceDesc->Width - 160, 100 );
    _SampleUI.GetControl( IDC_RADIO1B )->SetLocation( pBackBufferSurfaceDesc->Width - 160, 124 );
    _SampleUI.GetControl( IDC_RADIO1C )->SetLocation( pBackBufferSurfaceDesc->Width - 160, 148 );
    _SampleUI.GetControl( IDC_RADIO2A )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 100 );
    _SampleUI.GetControl( IDC_RADIO2B )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 76 );
    _SampleUI.GetControl( IDC_RADIO2C )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 52 );
    _SampleUI.GetControl( IDC_LISTBOX )->SetLocation( pBackBufferSurfaceDesc->Width - 400,
                                                       pBackBufferSurfaceDesc->Height - 180 );
    _SampleUI.GetControl( IDC_LISTBOX )->SetSize( 190, 96 );
    _SampleUI.GetControl( IDC_LISTBOXM )->SetLocation( pBackBufferSurfaceDesc->Width - 200,
                                                        pBackBufferSurfaceDesc->Height - 180 );
    _SampleUI.GetControl( IDC_LISTBOXM )->SetSize( 190, 124 );
    */
    _Mesh.RestoreDeviceObjects( pd3dDevice );

    return S_OK;
}

void CLoginDlg::InitDialog()
{
    // Static
    _SampleUI.AddStatic ( IDC_STATIC_COMMENTS, L"Please Input Username and Password:", 0, 32, 200, 32 );
	const UINT left = (320 - 260 - 10) / 2;
    _SampleUI.AddStatic ( IDC_STATIC_USERNAME, L"Username: ", left, 64, 60, 32);
	_SampleUI.AddEditBox( IDC_EDIT_USERNAME  , L"", left + 60 + 10, 64, 200, 32);
    _SampleUI.AddStatic ( IDC_STATIC_PASSWORD, L"Password: ", left, 96 + 10, 60, 32);
	_SampleUI.AddEditBox( IDC_EDIT_PASSWORD  , L"", left + 60 + 10, 96 + 10, 200, 32);
	const UINT kkk = (320 - 120 - 10) / 2;
	_SampleUI.AddButton ( IDC_BTN_LOGIN      , L"Login" , kkk, 128 + 20, 60, 32);
	_SampleUI.AddButton ( IDC_BTN_CANCEL     , L"Cancel", kkk + 60 + 10, 128 + 20, 60, 32);

    _SampleUI.GetEditBox( IDC_EDIT_USERNAME )->SetText(L"guoyaoqiang");
    _SampleUI.GetEditBox( IDC_EDIT_PASSWORD )->SetText(L"123456");
}

void CLoginDlg::KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown)
{
}

void CLoginDlg::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl)
{
    switch( nControlID )
    {
        case IDC_BTN_LOGIN:
			{
				CChineseCodeLib::UnicodeToGB2312(_username, sizeof(_username), _SampleUI.GetEditBox( IDC_EDIT_USERNAME )->GetText());
				CChineseCodeLib::UnicodeToGB2312(_password, sizeof(_password), _SampleUI.GetEditBox( IDC_EDIT_PASSWORD )->GetText());
				_btn_value = 1;
				SendMessage(this->_hwnd, WM_CLOSE, 0, 0);
			}
            break;
        case IDC_BTN_CANCEL:
			{
				_btn_value = 0;
				SendMessage(this->_hwnd, WM_CLOSE, 0, 0);
			}
            break;
    }
}
