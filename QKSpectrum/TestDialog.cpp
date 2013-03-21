#include "StdAfx.h"
#include "dxut9dialog.h"
#include "TestDialog.h"


CTestDialog::CTestDialog(void)
{
}


CTestDialog::~CTestDialog(void)
{
}


void CTestDialog::InitDialog()
{
    // Static
    _SampleUI.AddStatic( IDC_STATIC_DXUT, L"This is a static control.", 0, 0, 200, 30 );
    _SampleUI.AddStatic( IDC_OUTPUT,
                          L"This static control provides feedback for your action.  It will change as you interact with the UI controls.", 20, 50, 620, 300 );
    _SampleUI.GetStatic( IDC_OUTPUT )->SetTextColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) ); // Change color to red
    _SampleUI.GetStatic( IDC_STATIC_DXUT )->SetTextColor( D3DCOLOR_ARGB( 255, 0, 255, 0 ) ); // Change color to green
    _SampleUI.GetControl( IDC_OUTPUT )->GetElement( 0 )->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;
    _SampleUI.GetControl( IDC_OUTPUT )->GetElement( 0 )->iFont = 2;
    _SampleUI.GetControl( IDC_STATIC_DXUT )->GetElement( 0 )->dwTextFormat = DT_CENTER | DT_VCENTER | DT_WORDBREAK;

    // Buttons
    _SampleUI.AddButton( IDC_ENABLEIME, L"Enable (I)ME", 30, 390, 80, 35, L'I' );
    _SampleUI.AddButton( IDC_DISABLEIME, L"Disable I(M)E", 30, 430, 80, 35, L'M' );

    // Edit box
    _SampleUI.AddEditBox( IDC_EDITBOX1, L"Edit control with default styles. Type text here and press Enter", 20, 440,
                           600, 32 );

    // IME-enabled edit box
    CDXUTIMEEditBox* pIMEEdit;
    CDXUTIMEEditBox::InitDefaultElements( &_SampleUI );
    if( SUCCEEDED( CDXUTIMEEditBox::CreateIMEEditBox( &_SampleUI, IDC_EDITBOX2,
                                                      L"IME-capable edit control with custom styles. Type and press Enter", 20, 390, 600, 45, false, &pIMEEdit ) ) )
    {
        _SampleUI.AddControl( pIMEEdit );
        pIMEEdit->GetElement( 0 )->iFont = 1;
        pIMEEdit->GetElement( 1 )->iFont = 1;
        pIMEEdit->GetElement( 9 )->iFont = 1;
        pIMEEdit->GetElement( 0 )->TextureColor.Init( D3DCOLOR_ARGB( 128, 255, 255, 255 ) );  // Transparent center
        pIMEEdit->SetBorderWidth( 7 );
        pIMEEdit->SetTextColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
        pIMEEdit->SetCaretColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
        pIMEEdit->SetSelectedTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
        pIMEEdit->SetSelectedBackColor( D3DCOLOR_ARGB( 255, 40, 72, 72 ) );
    }

    // Slider
    _SampleUI.AddSlider( IDC_SLIDER, 200, 450, 200, 24, 0, 100, 50, false );

    // Checkbox
    _SampleUI.AddCheckBox( IDC_CHECKBOX, L"This is a checkbox with hotkey. Press 'C' to toggle the check state.",
                            170, 450, 350, 24, false, L'C', false );
    _SampleUI.AddCheckBox( IDC_CLEAREDIT,
                            L"This checkbox controls whether edit control text is cleared when Enter is pressed. (T)",
                            170, 460, 450, 24, false, L'T', false );

    // Combobox
    CDXUTComboBox* pCombo;
    _SampleUI.AddComboBox( IDC_COMBOBOX, 0, 0, 200, 24, L'O', false, &pCombo );
    if( pCombo )
    {
        pCombo->SetDropHeight( 100 );
        pCombo->AddItem( L"Combobox item (O)", ( LPVOID )0x11111111 );
        pCombo->AddItem( L"Placeholder (O)", ( LPVOID )0x12121212 );
        pCombo->AddItem( L"One more (O)", ( LPVOID )0x13131313 );
        pCombo->AddItem( L"I can't get enough (O)", ( LPVOID )0x14141414 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", ( LPVOID )0x15151515 );
    }

    // Radio buttons
    _SampleUI.AddRadioButton( IDC_RADIO1A, 1, L"Radio group 1 Amy (1)", 0, 50, 220, 24, false, L'1' );
    _SampleUI.AddRadioButton( IDC_RADIO1B, 1, L"Radio group 1 Brian (2)", 0, 50, 220, 24, false, L'2' );
    _SampleUI.AddRadioButton( IDC_RADIO1C, 1, L"Radio group 1 Clark (3)", 0, 50, 220, 24, false, L'3' );

    _SampleUI.AddRadioButton( IDC_RADIO2A, 2, L"Single (4)", 0, 50, 90, 24, false, L'4' );
    _SampleUI.AddRadioButton( IDC_RADIO2B, 2, L"Double (5)", 0, 50, 90, 24, false, L'5' );
    _SampleUI.AddRadioButton( IDC_RADIO2C, 2, L"Triple (6)", 0, 50, 90, 24, false, L'6' );

    // List box
    _SampleUI.AddListBox( IDC_LISTBOX, 30, 400, 200, 150, 0 );
    for( int i = 0; i < 15; ++i )
    {
        WCHAR wszText[50];
        swprintf_s( wszText, 50, L"Single-selection listbox item %d", i );
        _SampleUI.GetListBox( IDC_LISTBOX )->AddItem( wszText, ( LPVOID )( size_t )i );
    }
    _SampleUI.AddListBox( IDC_LISTBOXM, 30, 400, 200, 150, CDXUTListBox::MULTISELECTION );
    for( int i = 0; i < 30; ++i )
    {
        WCHAR wszText[50];
        swprintf_s( wszText, 50, L"Multi-selection listbox item %d", i );
        _SampleUI.GetListBox( IDC_LISTBOXM )->AddItem( wszText, ( LPVOID )( size_t )i );
    }
}

int  CTestDialog::DestroyDialog()
{
	return (0);
}

HRESULT CTestDialog::OnResetDevice ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
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

    _SampleUI.GetControl( IDC_STATIC_DXUT )->SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height * 6 /
                                                  10 );
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
    
    _Mesh.RestoreDeviceObjects( pd3dDevice );

    return S_OK;
}

void CTestDialog::KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown)
{
}

void CTestDialog::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl)
{
    WCHAR wszOutput[1024];

    switch( nControlID )
    {
        case IDC_ENABLEIME:
            CDXUTIMEEditBox::SetImeEnableFlag( true );
            _SampleUI.GetStatic( IDC_OUTPUT )->SetText(
                L"You clicked the 'Enable IME' button.\nIME text input is enabled for IME-capable edit controls." );
            break;
        case IDC_DISABLEIME:
            CDXUTIMEEditBox::SetImeEnableFlag( false );
            _SampleUI.GetStatic( IDC_OUTPUT )->SetText(
                L"You clicked the 'Disable IME' button.\nIME text input is disabled for IME-capable edit controls." );
            break;
        case IDC_EDITBOX1:
        case IDC_EDITBOX2:
            switch( nEvent )
            {
                case EVENT_EDITBOX_STRING:
                {
                    swprintf_s( wszOutput, 1024,
                                     L"You have pressed Enter in edit control (ID %u).\nThe content of the edit control is:\n\"%s\"",
                                     nControlID, ( ( CDXUTEditBox* )pControl )->GetText() );
                    _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );

                    // Clear the text if needed
                    if( _SampleUI.GetCheckBox( IDC_CLEAREDIT )->GetChecked() )
                        ( ( CDXUTEditBox* )pControl )->SetText( L"" );
                    break;
                }

                case EVENT_EDITBOX_CHANGE:
                {
                    swprintf_s( wszOutput, 1024,
                                     L"You have changed the content of an edit control (ID %u).\nIt is now:\n\"%s\"",
                                     nControlID, ( ( CDXUTEditBox* )pControl )->GetText() );
                    _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );

                    break;
                }
            }
            break;
        case IDC_SLIDER:
            swprintf_s( wszOutput, 1024, L"You adjusted the slider control.\nThe new value reported is %d",
                             ( ( CDXUTSlider* )pControl )->GetValue() );
            _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            break;
        case IDC_CHECKBOX:
            swprintf_s( wszOutput, 1024, L"You %s the upper check box.",
                             ( ( CDXUTCheckBox* )pControl )->GetChecked() ? L"checked" : L"cleared" );
            _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            break;
        case IDC_CLEAREDIT:
            swprintf_s( wszOutput, 1024, L"You %s the lower check box.\nNow edit controls will %s",
                             ( ( CDXUTCheckBox* )pControl )->GetChecked() ? L"checked" : L"cleared",
                             ( ( CDXUTCheckBox* )pControl )->GetChecked() ?
                             L"be cleared when you press Enter to send the text" :
                             L"retain the text context when you press Enter to send the text" );
            _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            break;
        case IDC_COMBOBOX:
        {
            DXUTComboBoxItem* pItem = ( ( CDXUTComboBox* )pControl )->GetSelectedItem();
            if( pItem )
            {
                swprintf_s( wszOutput, 1024,
                                 L"You selected a new item in the combobox.\nThe new item is \"%s\" and the associated data value is 0x%p",
                                 pItem->strText, pItem->pData );
                _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            }
            break;
        }
        case IDC_RADIO1A:
        case IDC_RADIO1B:
        case IDC_RADIO1C:
            swprintf_s( wszOutput, 1024,
                             L"You selected a new radio button in the UPPER radio group.\nThe new button is \"%s\"",
                             ( ( CDXUTRadioButton* )pControl )->GetText() );
            _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            break;
        case IDC_RADIO2A:
        case IDC_RADIO2B:
        case IDC_RADIO2C:
            swprintf_s( wszOutput, 1024,
                             L"You selected a new radio button in the LOWER radio group.\nThe new button is \"%s\"",
                             ( ( CDXUTRadioButton* )pControl )->GetText() );
            _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            break;

        case IDC_LISTBOX:
            switch( nEvent )
            {
                case EVENT_LISTBOX_ITEM_DBLCLK:
                {
                    DXUTListBoxItem* pItem = ( ( CDXUTListBox* )pControl )->GetItem(
                        ( ( CDXUTListBox* )pControl )->GetSelectedIndex( -1 ) );

                    swprintf_s( wszOutput, 1024,
                                     L"You double clicked an item in the left list box.  The item is\n\"%s\"",
                                     pItem ? pItem->strText : L"" );
                    _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
                    break;
                }

                case EVENT_LISTBOX_SELECTION:
                {
                    swprintf_s( wszOutput, 1024,
                                     L"You changed the selection in the left list box.  The selected item is %d",
                                     ( ( CDXUTListBox* )pControl )->GetSelectedIndex() );
                    _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
                    break;
                }
            }
            break;

        case IDC_LISTBOXM:
            switch( nEvent )
            {
                case EVENT_LISTBOX_ITEM_DBLCLK:
                {
                    DXUTListBoxItem* pItem = ( ( CDXUTListBox* )pControl )->GetItem(
                        ( ( CDXUTListBox* )pControl )->GetSelectedIndex( -1 ) );

                    swprintf_s( wszOutput, 1024,
                                     L"You double clicked an item in the right list box.  The item is\n\"%s\"",
                                     pItem ? pItem->strText : L"" );
                    _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
                    break;
                }

                case EVENT_LISTBOX_SELECTION:
                {
                    swprintf_s( wszOutput, 1024,
                                     L"You changed the selection in the right list box.  The selected item(s) are\n" );
                    int nSelected = -1;
                    while( ( nSelected = ( ( CDXUTListBox* )pControl )->GetSelectedIndex( nSelected ) ) != -1 )
                    {
                        swprintf_s( wszOutput + lstrlenW( wszOutput ), 1024 - lstrlenW( wszOutput ), L"%d,",
                                         nSelected );
                    }
                    // Remove the trailing comma if one exists.
                    if( wszOutput[lstrlenW( wszOutput ) - 1] == L',' )
                        wszOutput[lstrlenW( wszOutput ) - 1] = L'\0';
                    _SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
                    break;
                }
            }
            break;
    }
}


