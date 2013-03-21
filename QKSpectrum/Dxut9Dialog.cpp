#include "StdAfx.h"
#include "Dxut9Dialog.h"


IDxut9Dialog::IDxut9Dialog(void)
	: _pFont(0)
	, _pTextSprite(0)
	, _pEffect(0)
	, _nFontSize1(24)
	, _nFontSize2(16)
	, _show_helper(false)
	, _btn_value(0)
	, _hwnd(0)
{
}


IDxut9Dialog::~IDxut9Dialog(void)
{
	DXUTShutdown(0);
}


bool CALLBACK IDxut9Dialog::cb_IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	return ((IDxut9Dialog*)pUserContext)->IsDeviceAcceptable(pCaps, AdapterFormat, BackBufferFormat, bWindowed);
}

bool    CALLBACK IDxut9Dialog::cb_ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	return ((IDxut9Dialog*)pUserContext)->ModifyDeviceSettings( pDeviceSettings );
}

HRESULT CALLBACK IDxut9Dialog::cb_OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	return ((IDxut9Dialog*)pUserContext)->OnCreateDevice( pd3dDevice, pBackBufferSurfaceDesc);
}

HRESULT CALLBACK IDxut9Dialog::cb_OnResetDevice ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,  void* pUserContext )
{
	return ((IDxut9Dialog*)pUserContext)->OnResetDevice ( pd3dDevice, pBackBufferSurfaceDesc);
}

void    CALLBACK IDxut9Dialog::cb_OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	return ((IDxut9Dialog*)pUserContext)->OnFrameMove( fTime, fElapsedTime);
}

void    CALLBACK IDxut9Dialog::cb_OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	return ((IDxut9Dialog*)pUserContext)->OnFrameRender( pd3dDevice, fTime, fElapsedTime);
}

LRESULT CALLBACK IDxut9Dialog::cb_MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	return ((IDxut9Dialog*)pUserContext)->MsgProc( hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing);
}

void    CALLBACK IDxut9Dialog::cb_KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	return ((IDxut9Dialog*)pUserContext)->KeyboardProc( nChar, bKeyDown, bAltDown);
}

void    CALLBACK IDxut9Dialog::cb_OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	return ((IDxut9Dialog*)pUserContext)->OnGUIEvent( nEvent, nControlID, pControl);
}

void    CALLBACK IDxut9Dialog::cb_OnLostDevice( void* pUserContext )
{
	return ((IDxut9Dialog*)pUserContext)->OnLostDevice( );
}

void    CALLBACK IDxut9Dialog::cb_OnDestroyDevice( void* pUserContext )
{
	return ((IDxut9Dialog*)pUserContext)->OnDestroyDevice( );
}


int IDxut9Dialog::ShowDialog(const TCHAR* szTitle,
							   const HINSTANCE hInstance,
							   const HICON hicon,
							   const HMENU hmenu,
							   const int cx,
							   const int cy)
{
	SetCallback();

    DXUTSetCursorSettings( true, true );
	_DialogResourceManager.m_SpriteBufferBytes11 = 0;

	_SampleUI.Init( &_DialogResourceManager );
    _SampleUI.SetCallback( cb_OnGUIEvent, this );

	_SampleUI.SetFont( 1, L"Comic Sans MS", _nFontSize1, FW_NORMAL );
    _SampleUI.SetFont( 2, L"Courier New",   _nFontSize2, FW_NORMAL );

	InitDialog();

	DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );
    DXUTCreateWindow( szTitle, hInstance, hicon, hmenu, cx, cy );
	_hwnd = DXUTGetHWND();
	DXUTCreateDevice (D3D_FEATURE_LEVEL_9_2, true, cx, cy );
    DXUTMainLoop();

	DestroyDialog();

	return DXUTGetExitCode();
}

void IDxut9Dialog::SetCallback()
{
    DXUTSetCallbackD3D9DeviceAcceptable( cb_IsDeviceAcceptable, this );
    DXUTSetCallbackD3D9DeviceCreated( cb_OnCreateDevice, this );
    DXUTSetCallbackD3D9DeviceReset( cb_OnResetDevice, this );
    DXUTSetCallbackD3D9FrameRender( cb_OnFrameRender, this );
    DXUTSetCallbackD3D9DeviceLost( cb_OnLostDevice, this );
    DXUTSetCallbackD3D9DeviceDestroyed( cb_OnDestroyDevice, this );
    DXUTSetCallbackMsgProc( cb_MsgProc, this );
    DXUTSetCallbackKeyboard( cb_KeyboardProc, this );
    DXUTSetCallbackFrameMove( cb_OnFrameMove, this );
    DXUTSetCallbackDeviceChanging( cb_ModifyDeviceSettings, this );
}











//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool IDxut9Dialog::IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed)
{
    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    // Must support pixel shader 2.0
    if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool IDxut9Dialog::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings )
{
    assert( DXUT_D3D9_DEVICE == pDeviceSettings->ver );

    HRESULT hr;
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    D3DCAPS9 caps;

    V( pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal,
                            pDeviceSettings->d3d9.DeviceType,
                            &caps ) );

    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
    // then switch to SWVP.
    if( ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
        caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
    {
        pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // Debugging vertex shaders requires either REF or software vertex processing 
    // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
    if( pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF )
    {
        pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
        pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
#ifdef DEBUG_PS
    pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif

    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF )
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
    }

    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT IDxut9Dialog::OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{
    HRESULT hr;

    CDXUTIMEEditBox::Initialize( DXUTGetHWND() );

    V_RETURN( _DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
    // Initialize the font
    V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                              L"Arial", &_pFont ) );

    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
#if defined( DEBUG ) || defined( _DEBUG )
        dwShaderFlags |= D3DXSHADER_DEBUG;
    #endif
#ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
#ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    // Read the D3DX effect file
    WCHAR str[MAX_PATH];
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"\\shader\\CustomUI.fx" ) );
    V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
                                        NULL, &_pEffect, NULL ) );

    _Mesh.Create( pd3dDevice, L"\\shader\\cell.x" );

    // Setup the camera's view parameters
    D3DXVECTOR3 vecEye( 0.0f, 1.5f, -7.0f );
    D3DXVECTOR3 vecAt ( 0.0f, 0.2f, 0.0f );
    D3DXVECTOR3 vecUp ( 0.0f, 1.0f, 0.0f );
    _Camera.SetViewParams( &vecEye, &vecAt );
    D3DXMatrixLookAtLH( &_mView, &vecEye, &vecAt, &vecUp );

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void IDxut9Dialog::OnFrameMove( double fTime, float fElapsedTime)
{
    D3DXMATRIXA16 m;
    D3DXMatrixRotationY( &m, D3DX_PI * fElapsedTime / 40.0f );
    D3DXMatrixMultiply( &_mView, &m, &_mView );
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void IDxut9Dialog::OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{
    HRESULT hr;
    D3DXMATRIXA16 mWorld;
    D3DXMATRIXA16 mView;
    D3DXMATRIXA16 mProj;
    D3DXMATRIXA16 mWorldViewProjection;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
        // Get the projection & view matrix from the camera class
        mWorld = *_Camera.GetWorldMatrix();
        mProj = *_Camera.GetProjMatrix();
        mView = _mView;

        mWorldViewProjection = mWorld * mView * mProj;

        // Update the effect's variables.  Instead of using strings, it would 
        // be more efficient to cache a handle to the parameter by calling 
        // ID3DXEffect::GetParameterByName
        V( _pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
        V( _pEffect->SetMatrix( "g_mWorld", &mWorld ) );
        V( _pEffect->SetFloat( "g_fTime", ( float )fTime ) );

        _pEffect->SetTechnique( "RenderScene" );
        UINT cPasses;
        _pEffect->Begin( &cPasses, 0 );
        ID3DXMesh* pMesh = _Mesh.GetMesh();
        for( UINT p = 0; p < cPasses; ++p )
        {
            _pEffect->BeginPass( p );
            for( UINT m = 0; m < _Mesh.m_dwNumMaterials; ++m )
            {
                _pEffect->SetTexture( "g_txScene", _Mesh.m_pTextures[m] );
                _pEffect->CommitChanges();
                pMesh->DrawSubset( m );
            }
            _pEffect->EndPass();
        }
        _pEffect->End();

        if (_show_helper)
			RenderText();

        V( _SampleUI.OnRender( fElapsedTime ) );

        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void IDxut9Dialog::RenderText()
{
    CDXUTTextHelper txtHelper( _pFont, _pTextSprite, 15 );

    // Output statistics
    txtHelper.Begin();
    txtHelper.SetInsertionPos( 5, 5 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    txtHelper.DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
    txtHelper.DrawTextLine( DXUTGetDeviceStats() );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
    txtHelper.DrawTextLine( L"Press ESC to quit" );
    txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT IDxut9Dialog::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing)
{
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly
    *pbNoFurtherProcessing = CDXUTIMEEditBox::StaticMsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = _DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    *pbNoFurtherProcessing = _SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass all remaining windows messages to camera so it can respond to user input
    _Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void IDxut9Dialog::OnLostDevice()
{
    _DialogResourceManager.OnD3D9LostDevice();
    _Mesh.InvalidateDeviceObjects();

    if( _pFont )
        _pFont->OnLostDevice();
    if( _pEffect )
        _pEffect->OnLostDevice();
    SAFE_RELEASE( _pTextSprite );
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void IDxut9Dialog::OnDestroyDevice()
{
    CDXUTIMEEditBox::Uninitialize( );

    _DialogResourceManager.OnD3D9DestroyDevice();
    _Mesh.Destroy();

    SAFE_RELEASE( _pEffect );
    SAFE_RELEASE( _pFont );
}



