#include "StdAfx.h"
#include "utility.h"
#include "warmgui_incs.h"
#include "ChineseCodeLib.h"
#include "Dxut9Dialog.h"
#include "OpenHistoryDataDlg.h"


COpenHistoryDataDlg::COpenHistoryDataDlg(void)
{
    *filepath  = L'\0';
    *_datapath = L'\0';
    _margin.left = _margin.right = _margin.top = _margin.bottom = 20;
}


COpenHistoryDataDlg::~COpenHistoryDataDlg(void)
{
}

void COpenHistoryDataDlg::InitDialog()
{
    RECT rect;
    ::GetClientRect(_hwnd, &rect);

    rect.left += _margin.left, rect.right -= _margin.right, rect.top += _margin.top, rect.bottom -= _margin.bottom;
    UINT edit_height = 32, edit_width = 200, row_interval = 15, col_interval = 10, lable_height = 32, lable_width = 60;

    rect.top = 32,
        rect.left = (RectWidth(rect) - edit_width -  lable_width - col_interval)/ 2;

    _SampleUI.AddStatic ( IDC_STATIC_DATA_PATH, L"Data Path:", rect.left, rect.top, lable_width, lable_height );
    _SampleUI.AddEditBox( IDC_EDIT_DATA_PATH  , L""          , rect.left + lable_width + col_interval, rect.top, edit_width, edit_height);
    rect.top += (row_interval + edit_height);
    _SampleUI.AddStatic ( IDC_STATIC_CODE     , L"Code: "    , rect.left, rect.top, lable_width, lable_height );
	_SampleUI.AddEditBox( IDC_EDIT_CODE       , L""          , rect.left + lable_width + col_interval, rect.top, edit_width, edit_height);
    rect.top += (row_interval + edit_height);
    _SampleUI.AddStatic ( IDC_STATIC_DATE     , L"Date: "    , rect.left, rect.top, lable_width, lable_height );
	_SampleUI.AddEditBox( IDC_EDIT_DATE       , L""          , rect.left + lable_width + col_interval, rect.top, edit_width, edit_height);
    rect.top += (row_interval + edit_height),
        rect.left = (RectWidth(rect) - lable_width -  lable_width - col_interval)/ 2;;
	_SampleUI.AddButton ( IDC_BTN_OPEN        , L"Open"      , rect.left, rect.top, lable_width, lable_height );
	_SampleUI.AddButton ( IDC_BTN_CANCEL      , L"Cancel"    , rect.left + lable_width + col_interval, rect.top, lable_width, lable_height );


    _SampleUI.GetEditBox( IDC_EDIT_DATA_PATH )->SetText(_datapath);
}

int COpenHistoryDataDlg::DestroyDialog()
{
    return (0);
}

HRESULT COpenHistoryDataDlg::OnResetDevice ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{
    HRESULT hr = S_OK;

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

	//_SampleUI.GetControl( IDC_STATIC_COMMENTS )->SetSize( pBackBufferSurfaceDesc->Width, _nFontSize2 );

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

void COpenHistoryDataDlg::KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown)
{
}

void COpenHistoryDataDlg::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl)
{
    switch( nControlID )
    {
        case IDC_BTN_OPEN:
			{
                _sntprintf_s(filepath,
                    MAX_PATH,
                    _TRUNCATE, 
                    L"%s\\%s-%s.dat",
                    _SampleUI.GetEditBox( IDC_EDIT_DATA_PATH )->GetText(),
                    _SampleUI.GetEditBox( IDC_EDIT_CODE )->GetText(),
                    _SampleUI.GetEditBox( IDC_EDIT_DATE )->GetText());
				_btn_value = 1;
				SendMessage(_hwnd, WM_CLOSE, 0, 0);
			}
            break;
        case IDC_BTN_CANCEL:
			{
				_btn_value = 0;
				SendMessage(_hwnd, WM_CLOSE, 0, 0);
			}
            break;
    }
}



void COpenHistoryDataDlg::set_data_path(const TCHAR* datapath)
{
    _tcscpy_s(_datapath, MAX_PATH, datapath);
}
