#include "StdAfx.h"
#include "warmgui_summer.h"



// Spit out some messages as a sanity check for programmers
#ifdef GRIDCONTROL_NO_TITLETIPS
#pragma message(" -- CGridCtrl: No titletips for cells with large data")
#endif
#ifdef GRIDCONTROL_NO_DRAGDROP
#pragma message(" -- CGridCtrl: No OLE drag and drop")
#endif
#ifdef GRIDCONTROL_NO_CLIPBOARD
#pragma message(" -- CGridCtrl: No clipboard support")
#endif
#ifdef GRIDCONTROL_NO_PRINTING
#pragma message(" -- CGridCtrl: No printing support")
#endif


#ifndef ASSERT
#define ASSERT(f)
#endif//ASSERT

namespace WARMGUI {



// Get the number of lines to scroll with each mouse wheel notch
// Why doesn't windows give us this function???
UINT GetMouseScrollLines()
{
    int nScrollLines = 3;            // reasonable default

#ifndef _WIN32_WCE
    // Do things the hard way in win95
    OSVERSIONINFO VersionInfo;
    VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (!GetVersionEx(&VersionInfo) ||
        (VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && VersionInfo.dwMinorVersion == 0))
    {
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
            0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
        {
            TCHAR szData[128];
            DWORD dwKeyDataType;
            DWORD dwDataBufSize = sizeof(szData);

            if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
                (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
            {
                nScrollLines = _tcstoul(szData, NULL, 10);
            }
            RegCloseKey(hKey);
        }
    }
    // win98 or greater
    else
           SystemParametersInfo (SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0);
#endif

    return nScrollLines;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl
CGridCtrl::CGridCtrl(const char* name, int nRows/* = 0*/, int nCols/* = 0*/, int nFixedRows/* = 0*/, int nFixedCols/* = 0*/)
    : IGuiControl(name)
{
    // Store the system colours in case they change. The gridctrl uses
    // these colours, and in OnSysColorChange we can check to see if
    // the gridctrl colours have been changed from the system colours.
    // If they have, then leave them, otherwise change them to reflect
    // the new system colours.
    _nRows               = 0;
    _nCols               = 0;
    _nFixedRows          = 0;
    _nFixedCols          = 0;

    _bVirtualMode        = FALSE;
    _pfnCallback         = NULL;

    _nVScrollMax         = 0;          // Scroll position
    _nHScrollMax         = 0;
    _nRowsPerWheelNotch  = GetMouseScrollLines(); // Get the number of lines
                                                   // per mouse wheel notch to scroll
    _nBarState           = GVL_NONE;
    _MouseMode           = MOUSE_NOTHING;
    _nGridLines          = GVL_BOTH;
    _bEditable           = TRUE;
    _bListMode           = FALSE;
    _bSingleRowSelection = FALSE;
    _bSingleColSelection = FALSE;
    _bLMouseButtonDown   = FALSE;
    _bRMouseButtonDown   = FALSE;
    _bAllowDraw          = TRUE;       // allow draw updates
    _bEnableSelection    = TRUE;
    _bFixedColumnSelection = TRUE;
    _bFixedRowSelection  = TRUE;
    _bAllowRowResize     = TRUE;
    _bAllowColumnResize  = TRUE;
    _bSortOnClick        = FALSE;      // Sort on header row click
    _bHandleTabKey       = TRUE;
#ifdef _WIN32_WCE
    _bDoubleBuffer       = FALSE;      // Use double buffering to avoid flicker?
#else
    _bDoubleBuffer       = TRUE;       // Use double buffering to avoid flicker?
#endif
    _bTitleTips          = TRUE;       // show cell title tips

    _bWysiwygPrinting    = FALSE;      // use size-to-width printing

    _bHiddenColUnhide    = TRUE;       // 0-width columns can be expanded via mouse
    _bHiddenRowUnhide    = TRUE;       // 0-Height rows can be expanded via mouse

    _bAllowColHide       = TRUE;       // Columns can be contracted to 0-width via mouse
    _bAllowRowHide       = TRUE;       // Rows can be contracted to 0-height via mouse

    _bAscending          = TRUE;       // sorting stuff
    _nSortColumn         = -1;
	_pfnCompare		  = NULL;

    _nAutoSizeColumnStyle = GVS_BOTH;  // Autosize grid using header and data info

    _nTimerID            = 0;          // For drag-selection
    _nTimerInterval      = 25;         // (in milliseconds)
    _nResizeCaptureRange = 3;          // When resizing columns/row, the cursor has to be
                                        // within +/-3 pixels of the dividing line for
                                        // resizing to be possible
    ////_pImageList          = NULL;       // Images in the grid
    _bAllowDragAndDrop   = FALSE;      // for drag and drop - EFW - off by default
    _bTrackFocusCell     = TRUE;       // Track Focus cell?
    _bFrameFocus         = TRUE;       // Frame the selected cell?
    _zoom_fix            = ZOOM_FIX_BOTH;

    ////_pRtcDefault = RUNTIME_CLASS(CGridCell);

    SetupDefaultCells();
    SetGridBkColor(_uiSetting._crShadow);

    // Set up the initial grid sizef
    SetRowCount(nRows);
    SetColumnCount(nCols);
    SetFixedRowCount(nFixedRows);
    SetFixedColumnCount(nFixedCols);

    SetTitleTipTextClr(DEFAULT_COLOR_ALPHA);  //FNA
    SetTitleTipBackClr(DEFAULT_COLOR_ALPHA);

    // set initial selection range (ie. none)
    _SelectedCellMap.clear();
    _PrevSelectedCellMap.clear();

#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
    // EFW - Added to support shaded/unshaded printout and
    // user-definable margins.
    _bShadedPrintOut = TRUE;
    ////SetPrintMarginInfo(2, 2, 4, 4, 1, 1, 1);
#endif

    setClass();
}

CGridCtrl::~CGridCtrl()
{
    DeleteAllItems();

#ifndef GRIDCONTROL_NO_TITLETIPS
    if (_bTitleTips && ::IsWindow(_TitleTip.GetSafeHwnd()))
        _TitleTip.DestroyWindow();
#endif

#if !defined(GRIDCONTROL_NO_DRAGDROP) || !defined(GRIDCONTROL_NO_CLIPBOARD)
    // BUG FIX - EFW
    COleDataSource *pSource = COleDataSource::GetClipboardOwner();
    if(pSource)
        COleDataSource::FlushClipboard();
#endif
}


void CGridCtrl::SetRect(RECT& rect)
{
    _rect = rect;
    if (_zoom_fix == ZOOM_FIX_BOTH)
    {
        int width  = (int)((fRectWidth (_rect) - _margin.left - _margin.right) / (float)_nCols);
        int height = (int)((fRectHeight(_rect) - _margin.bottom - _margin.top) / (float)_nRows);

        for (int i = 0; i < _nRows - 1; i++)
            _arRowHeights[i] = height;
        for (int i = 0; i < _nCols - 1; i++)
            _arColWidths[i] = width;

        for (int i = 0; i < RectHeight(_rect) - height * _nRows - _margin.bottom - _margin.top; i++)
            _arRowHeights[i] += 1;
        _arRowHeights[_nRows - 1] = height;
        for (int i = 0; i < RectWidth (_rect) - width  * _nCols - _margin.left - _margin.right; i++)
            _arColWidths[i] += 1;
        _arColWidths[_nCols - 1] = width;
        
    } else {
        for (int i = 0; i < _nRows; i++)
            _arRowHeights[i] = _cellDefault.GetHeight();
        for (int i = 0; i < _nCols; i++)
            _arColWidths[i] = _cellDefault.GetWidth();
    }

    return;
}

BOOL CGridCtrl::Initialise()
{
    // Stop re-entry problems
    static BOOL bInProcedure = FALSE;
    if (bInProcedure)
        return FALSE;
    bInProcedure = TRUE;

#ifndef GRIDCONTROL_NO_TITLETIPS
    _TitleTip.SetParentWnd(this);
#endif

    bInProcedure = FALSE;
    return TRUE;
}

// creates the control - use like any other window create control
BOOL CGridCtrl::Create(const RECT& rect, UINT nID, DWORD dwStyle)
{
    //Initialise();

    // The number of rows and columns will only be non-zero if the constructor
    // was called with non-zero initialising parameters. If this window was created
    // using a dialog template then the number of rows and columns will be 0 (which
    // means that the code below will not be needed - which is lucky 'cause it ain't
    // gonna get called in a dialog-template-type-situation.

    try
    {
        _arRowHeights.resize(_nRows);    // initialize row heights
        _arColWidths.resize(_nCols);     // initialize column widths
    }
    catch(...)
    {
        return FALSE;
    }

    int i;
    for (i = 0; i < _nRows; i++)
        _arRowHeights[i] = _cellDefault.GetHeight();
    for (i = 0; i < _nCols; i++)
        _arColWidths[i] = _cellDefault.GetWidth();

    ////ResetScrollBars(); //- called in OnSize anyway
    return TRUE;
}

void CGridCtrl::SetupDefaultCells()
{
    _cellDefault.SetGrid(this);            // Normal editable cell
    _cellFixedColDef.SetGrid(this);        // Cell for fixed columns
    _cellFixedRowDef.SetGrid(this);        // Cell for fixed rows
    _cellFixedRowColDef.SetGrid(this);     // Cell for area overlapped by fixed columns/rows

    _cellDefault.SetTextClr(_uiSetting._crDefaultText);
    _cellDefault.SetBackClr(_uiSetting._crDefaultBkgText);
    _cellFixedColDef.SetTextClr(_uiSetting._crFixColText);
    _cellFixedColDef.SetBackClr(_uiSetting._crFixColBkgText);
    _cellFixedRowDef.SetTextClr(_uiSetting._crFixRowText);
    _cellFixedRowDef.SetBackClr(_uiSetting._crFixRowBkgText);
    _cellFixedRowColDef.SetTextClr(_uiSetting._crFixRowColText);
    _cellFixedRowColDef.SetBackClr(_uiSetting._crFixRowColBkgText);
}

// Sends a message to the parent in the form of a WM_NOTIFY message with
// a NM_GRIDVIEW structure attached
LRESULT CGridCtrl::SendMessageToParent(int nRow, int nCol, int nMessage) const
{
    NM_GRIDVIEW nmgv;
    nmgv.iRow         = nRow;
    nmgv.iColumn      = nCol;
    nmgv.hdr.hwndFrom = 0;
    nmgv.hdr.idFrom   = 0;       ////GetDlgCtrlID();
    nmgv.hdr.code     = nMessage;

    return 0;
}

// Send a request to the parent to return information on a given cell
LRESULT CGridCtrl::SendDisplayRequestToParent(GV_DISPINFO* pDisplayInfo) const
{
    // Fix up the message headers
    pDisplayInfo->hdr.hwndFrom = 0;
    pDisplayInfo->hdr.idFrom   = 0;       ////GetDlgCtrlID();
    pDisplayInfo->hdr.code     = GVN_GETDISPINFO;

    return 0;
}

// Send a hint to the parent about caching information
LRESULT CGridCtrl::SendCacheHintToParent(const CCellRange& range) const
{
    GV_CACHEHINT CacheHint;

    // Fix up the message headers
    CacheHint.hdr.hwndFrom = 0;
    CacheHint.hdr.idFrom   = 0;       ////GetDlgCtrlID();
    CacheHint.hdr.code     = GVN_ODCACHEHINT;

    CacheHint.range = range;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl message handlers

void CGridCtrl::OnPaint()
{
}

BOOL CGridCtrl::OnEraseBkgnd()
{
    return TRUE;    // Don't erase the background.
}

// Custom background erasure. This gets called from within the OnDraw function,
// since we will (most likely) be using a memory DC to stop flicker. If we just
// erase the background normally through OnEraseBkgnd, and didn't fill the memDC's
// selected bitmap with colour, then all sorts of vis problems would occur
void CGridCtrl::EraseBkgnd()
{
    /*
    RECT  VisRect, ClipRect, rect;
    CBrush FixedRowColBack(GetDefaultCell(TRUE, TRUE)->GetBackClr()),
           FixedRowBack(GetDefaultCell(TRUE, FALSE)->GetBackClr()),
           FixedColBack(GetDefaultCell(FALSE, TRUE)->GetBackClr()),
           TextBack(GetDefaultCell(FALSE, FALSE)->GetBackClr());
    CBrush Back(GetGridBkColor());
    //CBrush Back(GetTextBkColor());

    if (pDC->GetClipBox(ClipRect) == ERROR)
        return;
    GetVisibleNonFixedCellRange(VisRect);

    int nFixedColumnWidth = GetFixedColumnWidth();
    int nFixedRowHeight = GetFixedRowHeight();

    // Draw Fixed row/column background
    if (ClipRect.left < nFixedColumnWidth && ClipRect.top < nFixedRowHeight)
        pDC->FillRect(RECT(ClipRect.left, ClipRect.top,
                      nFixedColumnWidth, nFixedRowHeight),
                      &FixedRowColBack);

    // Draw Fixed columns background
    if (ClipRect.left < nFixedColumnWidth && ClipRect.top < VisRect.bottom)
        pDC->FillRect(RECT(ClipRect.left, ClipRect.top,
                      nFixedColumnWidth, VisRect.bottom),
                      &FixedColBack);

    // Draw Fixed rows background
    if (ClipRect.top < nFixedRowHeight &&
        ClipRect.right > nFixedColumnWidth && ClipRect.left < VisRect.right)
        pDC->FillRect(RECT(nFixedColumnWidth-1, ClipRect.top,
                      VisRect.right, nFixedRowHeight),
                      &FixedRowBack);

    // Draw non-fixed cell background
    if (rect.IntersectRect(VisRect, ClipRect))
    {
        RECT CellRect(max(nFixedColumnWidth, rect.left),
                       max(nFixedRowHeight, rect.top),
                       rect.right, rect.bottom);
        pDC->FillRect(CellRect, &TextBack);
    }

    // Draw right hand side of window outside grid
    if (VisRect.right < ClipRect.right)
        pDC->FillRect(RECT(VisRect.right, ClipRect.top,
                      ClipRect.right, ClipRect.bottom),
                      &Back);

    // Draw bottom of window below grid
    if (VisRect.bottom < ClipRect.bottom && ClipRect.left < VisRect.right)
        pDC->FillRect(RECT(ClipRect.left, VisRect.bottom,
                      VisRect.right, ClipRect.bottom),
                      &Back);
    */
}

void CGridCtrl::OnSize(UINT nType, int cx, int cy)
{
    static BOOL bAlreadyInsideThisProcedure = FALSE;
    if (bAlreadyInsideThisProcedure)
        return;


    // This hurts - but I need a place for initialisation for the drop target,
    // and the window must be created. "Create" is a good place - if one is
    // creating the window dynamically. If one is using a dialog template then
    // this is no good. Oh well...
#   ifndef GRIDCONTROL_NO_DRAGDROP
        _DropTarget.Register(this);
#   endif

    // Start re-entry blocking
    bAlreadyInsideThisProcedure = TRUE;

    EndEditing();        // destroy any InPlaceEdit's

    ////////CWnd::OnSize(nType, cx, cy);
    ////ResetScrollBars();

    // End re-entry blocking
    bAlreadyInsideThisProcedure = FALSE;
}

UINT CGridCtrl::OnGetDlgCode()
{
    UINT nCode = DLGC_WANTARROWS | DLGC_WANTCHARS; // DLGC_WANTALLKEYS; //

    if (_bHandleTabKey && !IsCTRLpressed())
        nCode |= DLGC_WANTTAB;

    return nCode;
}

HRESULT CGridCtrl::Init()
{
    try {
        char key[MAX_PATH];
        _snprintf_s(key, MAX_PATH, "%s.row", _strconf);
        int row = _config->getInt(key);
        _snprintf_s(key, MAX_PATH, "%s.col", _strconf);
        int col = _config->getInt(key);

        _snprintf_s(key, MAX_PATH, "%s.margin", _strconf);
        _config->getRect(_margin, key);

        _snprintf_s(key, MAX_PATH, "%s.grid-bkg-color-alpha", _strconf);
        _config->getColorAlpha(_uiSetting._crGridBkColour, key);

        _snprintf_s(key, MAX_PATH, "%s.border-color-alpha", _strconf);
        _config->getColorAlpha(_uiSetting._crGridLineColour, key);

        _snprintf_s(key, MAX_PATH, "%s.face3d-color-alpha", _strconf);
        _config->getColorAlpha(_uiSetting._cr3DFace, key);

        _snprintf_s(key, MAX_PATH, "%s.shadow3d-color-alpha", _strconf);
        _config->getColorAlpha(_uiSetting._crShadow, key);

        _snprintf_s(key, MAX_PATH, "%s.ttip-bak-color-alpha", _strconf);
        _config->getColorAlpha(_uiSetting._crTTipBackClr, key);

        _snprintf_s(key, MAX_PATH, "%s.ttip-txt-color-alpha", _strconf);
        _config->getColorAlpha(_uiSetting._crTTipTextClr, key);

        _snprintf_s(key, MAX_PATH, "%s.highlight-color-alpha", _strconf);
        _config->getColorAlpha(_uiSetting._crHighLightBkg, key);

        _snprintf_s(key, MAX_PATH, "%s.highlight-txt-color-alpha", _strconf);
        _config->getColorAlpha(_uiSetting._crHighLightTxt, key);


        {///Font and Color
            //set fonts
            FONT font;
            _snprintf_s(key, MAX_PATH, "%s.txt-font", _strconf);
            _config->getFontSetting(font, key);
            CDxFactorys::GetInstance()->CreateTextFormat(font, &_uiSetting._pDefaultFont);

            _snprintf_s(key, MAX_PATH, "%s.fixcol-font", _strconf);
            _config->getFontSetting(font, key);
            CDxFactorys::GetInstance()->CreateTextFormat(font, &_uiSetting._pFixedColFont);

            _snprintf_s(key, MAX_PATH, "%s.fixrow-font", _strconf);
            _config->getFontSetting(font, key);
            CDxFactorys::GetInstance()->CreateTextFormat(font, &_uiSetting._pFixedRowFont);

            _snprintf_s(key, MAX_PATH, "%s.fixrowcol-font", _strconf);
            _config->getFontSetting(font, key);
            CDxFactorys::GetInstance()->CreateTextFormat(font, &_uiSetting._pFixedRowColFont);

            //set colors
            _snprintf_s(key, MAX_PATH, "%s.txt-color-alpha", _strconf);
            _config->getColorAlpha(_uiSetting._crDefaultText, key);
            _snprintf_s(key, MAX_PATH, "%s.txtbkg-color-alpha", _strconf);
            _config->getColorAlpha(_uiSetting._crDefaultBkgText, key);

            _snprintf_s(key, MAX_PATH, "%s.fixcol-color-alpha", _strconf);
            _config->getColorAlpha(_uiSetting._crFixColText, key);
            _snprintf_s(key, MAX_PATH, "%s.fixcol-bkg-color-alpha", _strconf);
            _config->getColorAlpha(_uiSetting._crFixColBkgText, key);

            _snprintf_s(key, MAX_PATH, "%s.fixrow-color-alpha", _strconf);
            _config->getColorAlpha(_uiSetting._crFixRowText, key);
            _snprintf_s(key, MAX_PATH, "%s.fixrow-bkg-color-alpha", _strconf);
            _config->getColorAlpha(_uiSetting._crFixRowBkgText, key);

            _snprintf_s(key, MAX_PATH, "%s.fixrowcol-color-alpha", _strconf);
            _config->getColorAlpha(_uiSetting._crFixRowColText, key);
            _snprintf_s(key, MAX_PATH, "%s.fixrowcol-bkg-color-alpha", _strconf);
            _config->getColorAlpha(_uiSetting._crFixRowColBkgText, key);

            //set cell's font and color
            _cellDefault.SetFont(_uiSetting._pDefaultFont);
            _cellDefault.SetTextClr(_uiSetting._crDefaultText);
            _cellDefault.SetBackClr(_uiSetting._crDefaultBkgText);

            _cellFixedColDef.SetFont(_uiSetting._pFixedColFont);
            _cellFixedColDef.SetTextClr(_uiSetting._crFixColText);
            _cellFixedColDef.SetBackClr(_uiSetting._crFixColBkgText);

            _cellFixedRowDef.SetFont(_uiSetting._pFixedRowFont);
            _cellFixedRowDef.SetTextClr(_uiSetting._crFixRowText);
            _cellFixedRowDef.SetBackClr(_uiSetting._crFixRowBkgText);

            _cellFixedRowColDef.SetFont(_uiSetting._pFixedRowColFont);
            _cellFixedRowColDef.SetTextClr(_uiSetting._crFixRowColText);
            _cellFixedRowColDef.SetBackClr(_uiSetting._crFixRowColBkgText);

            SetColumnCount(col);
            SetRowCount(row);

            {//test first cell
                //GV_ITEM item;
                //item.mask = GVIF_TEXT;
    
                //item.col = item.row = 0, item.strText = L"FUCK";
                //SetItem(&item);
            }
        }
    } catch(...) {
        return (-1);
    }

    return S_OK;
}

#ifndef _WIN32_WCE
// If system colours change, then redo colours
void CGridCtrl::OnSysColorChange()
{
    /*
    if (GetDefaultCell(FALSE, FALSE)->GetTextClr() == _crWindowText)                   // Still using system colours
        GetDefaultCell(FALSE, FALSE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(FALSE, FALSE)->GetBackClr() == _crWindowColour)
        GetDefaultCell(FALSE, FALSE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(TRUE, FALSE)->GetTextClr() == _crWindowText)                   // Still using system colours
        GetDefaultCell(TRUE, FALSE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(TRUE, FALSE)->GetBackClr() == _crWindowColour)
        GetDefaultCell(TRUE, FALSE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(FALSE, TRUE)->GetTextClr() == _crWindowText)                   // Still using system colours
        GetDefaultCell(FALSE, TRUE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(FALSE, TRUE)->GetBackClr() == _crWindowColour)
        GetDefaultCell(FALSE, TRUE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(TRUE, TRUE)->GetTextClr() == _crWindowText)                   // Still using system colours
        GetDefaultCell(TRUE, TRUE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(TRUE, TRUE)->GetBackClr() == _crWindowColour)
        GetDefaultCell(TRUE, TRUE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetGridBkColor() == _crShadow)
        SetGridBkColor(::GetSysColor(COLOR_3DSHADOW));

    _crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
    _crWindowColour = ::GetSysColor(COLOR_WINDOW);
    _cr3DFace       = ::GetSysColor(COLOR_3DFACE);
    _crShadow       = ::GetSysColor(COLOR_3DSHADOW);
    */
}
#endif

#ifndef _WIN32_WCE_NO_CURSOR
// If we are drag-selecting cells, or drag and dropping, stop now
void CGridCtrl::OnCaptureChanged()
{
    // kill timer if active
    if (_nTimerID != 0)
    {
        ////KillTimer(_nTimerID);
        _nTimerID = 0;
    }

#ifndef GRIDCONTROL_NO_DRAGDROP
    // Kill drag and drop if active
    if (_MouseMode == MOUSE_DRAGGING)
        _MouseMode = MOUSE_NOTHING;
#endif
}
#endif


// For drag-selection. Scrolls hidden cells into view
// TODO: decrease timer interval over time to speed up selection over time
void CGridCtrl::OnTimer(UINT_PTR nIDEvent)
{
    ASSERT(nIDEvent == WM_LBUTTONDOWN);
    if (nIDEvent != WM_LBUTTONDOWN)
        return;

    POINT_u pt, origPt;

#ifdef _WIN32_WCE
    if (_MouseMode == MOUSE_NOTHING)
        return;
    origPt = GetMessagePos();
#else
    ////if (!GetCursorPos(&origPt))
    ////    return;
#endif

    ////ScreenToClient(&origPt);

    RECT rect = _rect;
    ////GetClientRect(&rect);

    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth = GetFixedColumnWidth();

    pt = origPt;
    if (pt.y > rect.bottom)
    {
        //////SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        ////SendMessage(WM_KEYDOWN, VK_DOWN, 0);

        if (pt.x < rect.left)
            pt.x = rect.left;
        if (pt.x > rect.right)
            pt.x = rect.right;
        pt.y = rect.bottom;
        OnSelecting(GetCellFromPt(pt));
    }
    else if (pt.y < nFixedRowHeight)
    {
        //////SendMessage(WM_VSCROLL, SB_LINEUP, 0);
        ////SendMessage(WM_KEYDOWN, VK_UP, 0);

        if (pt.x < rect.left)
            pt.x = rect.left;
        if (pt.x > rect.right)
            pt.x = rect.right;
        pt.y = nFixedRowHeight + 1;
        OnSelecting(GetCellFromPt(pt));
    }

    pt = origPt;
    if (pt.x > rect.right)
    {
        // ////SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        ////SendMessage(WM_KEYDOWN, VK_RIGHT, 0);

        if (pt.y < rect.top)
            pt.y = rect.top;
        if (pt.y > rect.bottom)
            pt.y = rect.bottom;
        pt.x = rect.right;
        OnSelecting(GetCellFromPt(pt));
    }
    else if (pt.x < nFixedColWidth)
    {
        //////SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
        ////SendMessage(WM_KEYDOWN, VK_LEFT, 0);

        if (pt.y < rect.top)
            pt.y = rect.top;
        if (pt.y > rect.bottom)
            pt.y = rect.bottom;
        pt.x = nFixedColWidth + 1;
        OnSelecting(GetCellFromPt(pt));
    }
}

// move about with keyboard
void CGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (!IsValid(_idCurrentCell))
    {
        return;
    }

    CCellID next = _idCurrentCell;
    BOOL bChangeLine = FALSE;
	BOOL bHorzScrollAction = FALSE;
	BOOL bVertScrollAction = FALSE;

    if (IsCTRLpressed())
    {
        switch (nChar)
        {
        case 'A':
            OnEditSelectAll();
            break;
#ifndef GRIDCONTROL_NO_CLIPBOARD
        case 'X':
            OnEditCut();
            break;
        case VK_INSERT:
        case 'C':
            OnEditCopy();
            break;
        case 'V':
            OnEditPaste();
            break;
#endif
        }
    }

#ifndef GRIDCONTROL_NO_CLIPBOARD
    if (IsSHIFTpressed() &&(nChar == VK_INSERT))
        OnEditPaste();
#endif

    BOOL bFoundVisible;
    int iOrig;

    if (nChar == VK_DELETE)
    {
        ValidateAndModifyCellContents(_idCurrentCell.row, _idCurrentCell.col, _T(""));
    }
    else if (nChar == VK_DOWN)
    {
        // don't let user go to a hidden row
        bFoundVisible = FALSE;
        iOrig = next.row;
        next.row++;
        while( next.row < GetRowCount())
        {
            if( GetRowHeight( next.row) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.row++;
        }
        if( !bFoundVisible)
            next.row = iOrig;
    }
    else if (nChar == VK_UP)
    {
        // don't let user go to a hidden row
        bFoundVisible = FALSE;
        iOrig = next.row;
        next.row--;
        while( next.row >= _nFixedRows)
        {
            if( GetRowHeight( next.row) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.row--;
        }
        if( !bFoundVisible)
            next.row = iOrig;
    }
    else if (nChar == VK_RIGHT || (nChar == VK_TAB && !IsSHIFTpressed()) )
    {
        // don't let user go to a hidden column
        bFoundVisible = FALSE;
        iOrig = next.col;
        next.col++;

        if (nChar == VK_TAB)
        {
            if (next.col == (GetColumnCount()) && next.row < (GetRowCount() - 1))
            {
                next.row++;
                next.col = _nFixedCols;
                bChangeLine = TRUE;
            }
            else {
                ////CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
            }
        }

        while( next.col < GetColumnCount())
        {
            if( GetColumnWidth( next.col) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.col++;
        }
        if( !bFoundVisible)
            next.col = iOrig;
    }
    else if (nChar == VK_LEFT || (nChar == VK_TAB && IsSHIFTpressed()) )
    {
        // don't let user go to a hidden column
        bFoundVisible = FALSE;
        iOrig = next.col;
        next.col--;

        if (nChar == VK_TAB)
        {
            if (next.col == (GetFixedColumnCount()-1) && next.row > GetFixedRowCount())
            {
                next.row--;
                next.col = GetColumnCount() - 1;
                bChangeLine = TRUE;
            }
            else {
                ////CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
            }
        }

        while( next.col >= _nFixedCols)
        {
            if( GetColumnWidth( next.col) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.col--;
        }
        if( !bFoundVisible)
            next.col = iOrig;
    }
    else if (nChar == VK_NEXT)
    {
        CCellID idOldTopLeft = GetTopleftNonFixedCell();
        ////SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
		bVertScrollAction = TRUE;
        CCellID idNewTopLeft = GetTopleftNonFixedCell();

        int increment = idNewTopLeft.row - idOldTopLeft.row;
        if (increment)
        {
            next.row += increment;
            if (next.row >(GetRowCount() - 1))
                next.row = GetRowCount() - 1;
        }
        else
            next.row = GetRowCount() - 1;
    }
    else if (nChar == VK_PRIOR)
    {
        CCellID idOldTopLeft = GetTopleftNonFixedCell();
        ////SendMessage(WM_VSCROLL, SB_PAGEUP, 0);
		bVertScrollAction = TRUE;
        CCellID idNewTopLeft = GetTopleftNonFixedCell();

        int increment = idNewTopLeft.row - idOldTopLeft.row;
        if (increment)
        {
            next.row += increment;
            if (next.row < _nFixedRows)
                next.row = _nFixedRows;
        }
        else
            next.row = _nFixedRows;
    }
    else if (nChar == VK_HOME)
    {
        // Home and Ctrl-Home work more like Excel
        //  and don't let user go to a hidden cell
        if (IsCTRLpressed())
        {
            ////SendMessage(WM_VSCROLL, SB_TOP, 0);
            ////SendMessage(WM_HSCROLL, SB_LEFT, 0);
			bVertScrollAction = TRUE;
			bHorzScrollAction = TRUE;
            next.row = _nFixedRows;
            next.col = _nFixedCols;
        }
        else
        {
            ////SendMessage(WM_HSCROLL, SB_LEFT, 0);
 			bHorzScrollAction = TRUE;
            next.col = _nFixedCols;
        }
        // adjust column to avoid hidden columns and rows
        while( next.col < GetColumnCount() - 1)
        {
            if( GetColumnWidth( next.col) > 0)
                break;
            next.col++;
        }
        while( next.row < GetRowCount() - 1)
        {
            if( GetRowHeight( next.row) > 0)
                break;
            next.row++;
        }
    }
    else if (nChar == VK_END)
    {
        // End and Ctrl-End work more like Excel
        //  and don't let user go to a hidden cell
        if (IsCTRLpressed())
        {
            ////SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
            ////SendMessage(WM_HSCROLL, SB_RIGHT, 0);
			bHorzScrollAction = TRUE;
			bVertScrollAction = TRUE;
            next.row = GetRowCount() - 1;
            next.col = GetColumnCount() - 1;
        }
        else
        {
            ////SendMessage(WM_HSCROLL, SB_RIGHT, 0);
			bHorzScrollAction = TRUE;
            next.col = GetColumnCount() - 1;
        }
        // adjust column to avoid hidden columns and rows
        while( next.col > _nFixedCols + 1)
        {
            if( GetColumnWidth( next.col) > 0)
                break;
            next.col--;
        }
        while( next.row > _nFixedRows + 1)
        {
            if( GetRowHeight( next.row) > 0)
                break;
            next.row--;
        }
    }
    else if (nChar == VK_F2)
    {
        OnEditCell(_idCurrentCell.row, _idCurrentCell.col, POINT_u( -1, -1), VK_LBUTTON);
    }
    else
    {
        ////CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }

    if (next != _idCurrentCell)
    {
        // While moving with the Cursorkeys the current ROW/CELL will get selected
        // OR Selection will get expanded when SHIFT is pressed
        // Cut n paste from OnLButtonDown - Franco Bez
        // Added check for NULL mouse mode - Chris Maunder.
        if (_MouseMode == MOUSE_NOTHING)
        {
            _PrevSelectedCellMap.clear();
            _MouseMode = _bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
            if (!IsSHIFTpressed() || nChar == VK_TAB)
                _SelectionStartCell = next;
            OnSelecting(next);
            _MouseMode = MOUSE_NOTHING;
        }

        SetFocusCell(next);

        if (!IsCellVisible(next))
        {

            switch (nChar)
            {
            case VK_RIGHT:
                ////SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
				bHorzScrollAction = TRUE;
                break;

            case VK_LEFT:
                ////SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
				bHorzScrollAction = TRUE;
                break;

            case VK_DOWN:
                ////SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
				bVertScrollAction = TRUE;
                break;

            case VK_UP:
                ////SendMessage(WM_VSCROLL, SB_LINEUP, 0);
				bVertScrollAction = TRUE;
                break;

            case VK_TAB:
                if (IsSHIFTpressed())
                {
                    if (bChangeLine)
                    {
                        ////SendMessage(WM_VSCROLL, SB_LINEUP, 0);
						bVertScrollAction = TRUE;
                        ////SetScrollPos32(SB_HORZ, _nHScrollMax);
                        break;
                    }
                    else
					{
                        ////SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
						bHorzScrollAction = TRUE;
					}
                }
                else
                {
                    if (bChangeLine)
                    {
                        ////SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
						bVertScrollAction = TRUE;
                        ////SetScrollPos32(SB_HORZ, 0);
                        break;
                    }
                    else
					{
						////SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
						bHorzScrollAction = TRUE;
					}
                }
                break;
            }
            EnsureVisible(next); // Make sure cell is visible
            ////SEAN SEAN SEAN Invalidate();
        }
        EnsureVisible(next); // Make sure cell is visible

		if (bHorzScrollAction) {
			////SendMessage(WM_HSCROLL, SB_ENDSCROLL, 0);
        }
		if (bVertScrollAction) {
			////SendMessage(WM_VSCROLL, SB_ENDSCROLL, 0);
        }
    }
}

void CGridCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    ////CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGridCtrl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#ifdef GRIDCONTROL_USE_TITLETIPS
    _TitleTip.Hide();  // hide any titletips
#endif

    ////CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

// Instant editing of cells when keys are pressed
void CGridCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // EFW - BUG FIX
    if (!IsCTRLpressed() && _MouseMode == MOUSE_NOTHING && nChar != VK_ESCAPE)
    {
        if (!_bHandleTabKey || (_bHandleTabKey && nChar != VK_TAB))
            OnEditCell(_idCurrentCell.row, _idCurrentCell.col, POINT_u( -1, -1), nChar);
    }

    ////CWnd::OnChar(nChar, nRepCnt, nFlags);
}

// Added by KiteFly
LRESULT CGridCtrl::OnImeChar(WPARAM wCharCode, LPARAM)
{
    // EFW - BUG FIX
    if (!IsCTRLpressed() && _MouseMode == MOUSE_NOTHING && wCharCode != VK_ESCAPE)
        OnEditCell(_idCurrentCell.row, _idCurrentCell.col, POINT_u(-1, -1), wCharCode);
    return 0;
}

// Callback from any CInPlaceEdits that ended. This just calls OnEndEditCell,
// refreshes the edited cell and moves onto next cell if the return character
// from the edit says we should.
void CGridCtrl::OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
    GV_ITEM     *pgvItem = &pgvDispInfo->item;


    OnEndEditCell(pgvItem->row, pgvItem->col, pgvItem->strText);
    //InvalidateCellRect(CCellID(pgvItem->row, pgvItem->col));

    switch (pgvItem->lParam)
    {
    case VK_TAB:
    case VK_DOWN:
    case VK_UP:
    case VK_RIGHT:
    case VK_LEFT:
    case VK_NEXT:
    case VK_PRIOR:
    case VK_HOME:
    case VK_END:
        OnKeyDown(pgvItem->lParam, 0, 0);
        OnEditCell(_idCurrentCell.row, _idCurrentCell.col, POINT_u( -1, -1), pgvItem->lParam);
    }

    *pResult = 0;
}

// Handle horz scrollbar notifications
void CGridCtrl::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar_summer* /*pScrollBar*/)
{
    EndEditing();

#ifndef GRIDCONTROL_NO_TITLETIPS
    _TitleTip.Hide();  // hide any titletips
#endif

    int scrollPos = 0;  ////GetScrollPos32(SB_HORZ);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    RECT rect;
    ////GetClientRect(&rect);

    switch (nSBCode)
    {
    case SB_LINERIGHT:
        if (scrollPos < _nHScrollMax)
        {
            // may have contiguous hidden columns.  Blow by them
            while (idTopLeft.col < (GetColumnCount()-1)
                    && GetColumnWidth( idTopLeft.col) < 1 )
            {
                idTopLeft.col++;
            }
            int xScroll = GetColumnWidth(idTopLeft.col);
            ////SetScrollPos32(SB_HORZ, scrollPos + xScroll);
            ////if (GetScrollPos32(SB_HORZ) == scrollPos)
            ////    break;          // didn't work

            rect.left = GetFixedColumnWidth();
            //rect.left = GetFixedColumnWidth() + xScroll;
            //ScrollWindow(-xScroll, 0, rect);
            //rect.left = rect.right - xScroll;
            ////SEAN SEAN SEAN InvalidateRect(rect);
        }
        break;

    case SB_LINELEFT:
        if (scrollPos > 0 && idTopLeft.col > GetFixedColumnCount())
        {
            int iColToUse = idTopLeft.col-1;
            // may have contiguous hidden columns.  Blow by them
            while(  iColToUse > GetFixedColumnCount()
                    && GetColumnWidth( iColToUse) < 1 )
            {
                iColToUse--;
            }

            int xScroll = GetColumnWidth(iColToUse);
            ////SetScrollPos32(SB_HORZ, max(0, scrollPos - xScroll));
            rect.left = GetFixedColumnWidth();
            //ScrollWindow(xScroll, 0, rect);
            //rect.right = rect.left + xScroll;
            ////SEAN SEAN SEAN InvalidateRect(rect);
        }
        break;

    case SB_PAGERIGHT:
        if (scrollPos < _nHScrollMax)
        {
            rect.left = GetFixedColumnWidth();
            int offset = RectWidth(rect);
            int pos = min(_nHScrollMax, scrollPos + offset);
            ////SetScrollPos32(SB_HORZ, pos);
            rect.left = GetFixedColumnWidth();
            ////SEAN SEAN SEAN InvalidateRect(rect);
        }
        break;

    case SB_PAGELEFT:
        if (scrollPos > 0)
        {
            rect.left = GetFixedColumnWidth();
            int offset = -RectWidth(rect);
            int pos = max(0, scrollPos + offset);
            ////SetScrollPos32(SB_HORZ, pos);
            rect.left = GetFixedColumnWidth();
            ////SEAN SEAN SEAN InvalidateRect(rect);
        }
        break;

    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        {
            ////SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
            _idTopLeftCell.row = -1;
            CCellID idNewTopLeft = GetTopleftNonFixedCell();
            if (idNewTopLeft != idTopLeft)
            {
                rect.left = GetFixedColumnWidth();
                ////SEAN SEAN SEAN InvalidateRect(rect);
            }
        }
        break;

    case SB_LEFT:
        if (scrollPos > 0)
        {
            ////SetScrollPos32(SB_HORZ, 0);
            ////SEAN SEAN SEAN Invalidate();
        }
        break;

    case SB_RIGHT:
        if (scrollPos < _nHScrollMax)
        {
            ////SetScrollPos32(SB_HORZ, _nHScrollMax);
            ////SEAN SEAN SEAN Invalidate();
        }
        break;


    default:
        break;
    }
}

// Handle vert scrollbar notifications
void CGridCtrl::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar_summer* /*pScrollBar*/)
{
    EndEditing();

#ifndef GRIDCONTROL_NO_TITLETIPS
    _TitleTip.Hide();  // hide any titletips
#endif

    // Get the scroll position ourselves to ensure we get a 32 bit value
    int scrollPos = 0;  ////GetScrollPos32(SB_VERT);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    RECT rect;
    ////GetClientRect(&rect);

    switch (nSBCode)
    {
    case SB_LINEDOWN:
        if (scrollPos < _nVScrollMax)
        {
            // may have contiguous hidden rows.  Blow by them
            while(  idTopLeft.row < (GetRowCount()-1)
                    && GetRowHeight( idTopLeft.row) < 1 )
            {
                idTopLeft.row++;
            }

            int yScroll = GetRowHeight(idTopLeft.row);
            ////SetScrollPos32(SB_VERT, scrollPos + yScroll);
            ////if (GetScrollPos32(SB_VERT) == scrollPos)
            ////    break;          // didn't work

            rect.top = GetFixedRowHeight();
            //rect.top = GetFixedRowHeight() + yScroll;
            //ScrollWindow(0, -yScroll, rect);
            //rect.top = rect.bottom - yScroll;
            ////SEAN SEAN SEAN InvalidateRect(rect);
        }
        break;

    case SB_LINEUP:
        if (scrollPos > 0 && idTopLeft.row > GetFixedRowCount())
        {
            int iRowToUse = idTopLeft.row-1;
            // may have contiguous hidden rows.  Blow by them
            while(  iRowToUse > GetFixedRowCount()
                    && GetRowHeight( iRowToUse) < 1 )
            {
                iRowToUse--;
            }

            int yScroll = GetRowHeight( iRowToUse);
            ////SetScrollPos32(SB_VERT, max(0, scrollPos - yScroll));
            rect.top = GetFixedRowHeight();
            //ScrollWindow(0, yScroll, rect);
            //rect.bottom = rect.top + yScroll;
            ////SEAN SEAN SEAN InvalidateRect(rect);
        }
        break;

    case SB_PAGEDOWN:
        if (scrollPos < _nVScrollMax)
        {
            rect.top = GetFixedRowHeight();
            scrollPos = min(_nVScrollMax, scrollPos + RectHeight(rect));
            ////SetScrollPos32(SB_VERT, scrollPos);
            rect.top = GetFixedRowHeight();
            ////SEAN SEAN SEAN InvalidateRect(rect);
        }
        break;

    case SB_PAGEUP:
        if (scrollPos > 0)
        {
            rect.top = GetFixedRowHeight();
            int offset = -RectHeight(rect);
            int pos = max(0, scrollPos + offset);
            ////SetScrollPos32(SB_VERT, pos);
            rect.top = GetFixedRowHeight();
            ////SEAN SEAN SEAN InvalidateRect(rect);
        }
        break;

    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        {
            ////SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
            _idTopLeftCell.row = -1;
            CCellID idNewTopLeft = GetTopleftNonFixedCell();
            if (idNewTopLeft != idTopLeft)
            {
                rect.top = GetFixedRowHeight();
                ////SEAN SEAN SEAN InvalidateRect(rect);
            }
        }
        break;

    case SB_TOP:
        if (scrollPos > 0)
        {
            ////SetScrollPos32(SB_VERT, 0);
            ////SEAN SEAN SEAN Invalidate();
        }
        break;

    case SB_BOTTOM:
        if (scrollPos < _nVScrollMax)
        {
            ////SetScrollPos32(SB_VERT, _nVScrollMax);
            ////SEAN SEAN SEAN Invalidate();
        }

    default:
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl implementation functions

HRESULT CGridCtrl::DrawGraph(bool redraw/* = false*/)
{
    if (!_bAllowDraw)
        return S_OK;

	D2D1_ANTIALIAS_MODE am = _artist->GetHwndRT()->GetAntialiasMode();
	_artist->GetUsingRT()->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	_artist->SetStrokeStyle(D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_LINE_JOIN_ROUND,
		10.0f,
		D2D1_DASH_STYLE_SOLID);

    RECT clipRect;
    clipRect.left = _margin.left,
        clipRect.top = _margin.top,
        clipRect.bottom = RectHeight(_rect) - _margin.top - _margin.bottom,
        clipRect.right  = RectWidth(_rect)  - _margin.left - _margin.right;

    int row, col;
    CGridCellBase* pCell;

    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth  = GetFixedColumnWidth();

    CCellID idTopLeft = GetTopleftNonFixedCell();
    int minVisibleRow = idTopLeft.row,
        minVisibleCol = idTopLeft.col;

    RECT VisRect;
    CCellRange VisCellRange = GetVisibleNonFixedCellRange(&VisRect);
    int maxVisibleRow = VisCellRange.GetMaxRow(),
        maxVisibleCol = VisCellRange.GetMaxCol();

    if (GetVirtualMode())
        SendCacheHintToParent(VisCellRange);

    // draw top-left cells 0.._nFixedRows-1, 0.._nFixedCols-1
    RECT rect;
    rect.bottom = -1;
    for (row = 0; row < _nFixedRows; row++)
    {
        //MYTRACE(L"1\n");
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;
        rect.right = -1;

        for (col = 0; col < _nFixedCols; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            pCell = GetCell(row, col);
            if (pCell)
                pCell->Draw(row, col, rect, FALSE);
        }
    }

    // draw fixed column cells:  _nFixedRows..n, 0.._nFixedCols-1
    rect.bottom = nFixedRowHeight-1;
    for (row = minVisibleRow; row <= maxVisibleRow; row++)
    {
        //MYTRACE(L"4\n");
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom)
            break;                // Gone past cliprect
        if (rect.bottom < clipRect.top)
            continue;             // Reached cliprect yet?

        rect.right = -1;
        for (col = 0; col < _nFixedCols; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right)
                break;            // gone past cliprect
            if (rect.right < clipRect.left)
                continue;         // Reached cliprect yet?

            pCell = GetCell(row, col);
            if (pCell)
                pCell->Draw(row, col, rect, FALSE);
        }
    }

    // draw fixed row cells  0.._nFixedRows, _nFixedCols..n
    rect.bottom = -1;
    for (row = 0; row < _nFixedRows; row++)
    {
        //MYTRACE(L"2\n");
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom)
            break;                // Gone past cliprect
        if (rect.bottom < clipRect.top)
            continue;             // Reached cliprect yet?

        rect.right = nFixedColWidth-1;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right)
                break;        // gone past cliprect
            if (rect.right < clipRect.left)
                continue;     // Reached cliprect yet?

            pCell = GetCell(row, col);
            if (pCell)
                pCell->Draw(row, col, rect, FALSE);
        }
    }

    // draw rest of non-fixed cells
    rect.bottom = nFixedRowHeight-1;
    for (row = minVisibleRow; row <= maxVisibleRow; row++)
    {
        //MYTRACE(L"3\n");
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom)
            break;                // Gone past cliprect
        if (rect.bottom < clipRect.top)
            continue;             // Reached cliprect yet?

        rect.right = nFixedColWidth-1;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right)
                break;        // gone past cliprect
            if (rect.right < clipRect.left)
                continue;     // Reached cliprect yet?

            pCell = GetCell(row, col);
            // TRACE(_T("Cell %d,%d type: %s\n"), row, col, pCell->GetRuntimeClass()->_lpszClassName);
            if (pCell)
                pCell->Draw(row, col, rect, FALSE);
        }
    }

    // draw vertical lines (drawn at ends of cells)
    if (_nGridLines == GVL_BOTH || _nGridLines == GVL_VERT)
    {
        int x = nFixedColWidth;
        _artist->SetSolidColorBrush(_uiSetting._crGridLineColour);
        _artist->DrawLine(x, nFixedRowHeight, x, VisRect.bottom);

        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {
            x += GetColumnWidth(col);
            _artist->SetSolidColorBrush(_uiSetting._crGridLineColour);
            _artist->DrawLine(x, nFixedRowHeight, x, VisRect.bottom);
        }
    }

    // draw horizontal lines (drawn at bottom of each cell)
    if (_nGridLines == GVL_BOTH || _nGridLines == GVL_HORZ)
    {
        int y = nFixedRowHeight;
        _artist->SetSolidColorBrush(_uiSetting._crGridLineColour);
        _artist->DrawLine(nFixedColWidth, y, VisRect.right,  y);

        for (row = minVisibleRow; row <= maxVisibleRow; row++)
        {
            y += GetRowHeight(row);
            _artist->SetSolidColorBrush(_uiSetting._crGridLineColour);
            _artist->DrawLine(nFixedColWidth, y, VisRect.right,  y);
        }
    }

    // Let parent know it can discard it's data if it needs to.
    if (GetVirtualMode())
       SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    _artist->GetUsingRT()->SetAntialiasMode(am);
    return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////
// CGridCtrl Cell selection stuff

// Is a given cell designation valid (ie within the bounds of our number
// of columns/rows)?
BOOL CGridCtrl::IsValid(int nRow, int nCol) const
{
    return (nRow >= 0 && nRow < _nRows && nCol >= 0 && nCol < _nCols);
}

BOOL CGridCtrl::IsValid(const CCellID& cell) const
{
    return IsValid(cell.row, cell.col);
}

// Is a given cell range valid (ie within the bounds of our number
// of columns/rows)?
BOOL CGridCtrl::IsValid(const CCellRange& range) const
{
    return (range.GetMinRow() >= 0 && range.GetMinCol() >= 0 &&
        range.GetMaxRow() >= 0 && range.GetMaxCol() >= 0 &&
        range.GetMaxRow() < _nRows && range.GetMaxCol() < _nCols &&
        range.GetMinRow() <= range.GetMaxRow() && range.GetMinCol() <= range.GetMaxCol());
}

// Enables/Disables redraw for certain operations like columns auto-sizing etc,
// but not for user caused things such as selection changes.
void CGridCtrl::SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars /* = FALSE */)
{
//    TRACE(_T("%s: Setting redraw to %s\n"),
//             GetRuntimeClass()->_lpszClassName, bAllowDraw? _T("TRUE") : _T("FALSE"));

    if (bAllowDraw && !_bAllowDraw)
    {
        _bAllowDraw = TRUE;
        Refresh();
    }

    _bAllowDraw = bAllowDraw;
    ////if (bResetScrollBars)
        ////ResetScrollBars();
}

// Forces a redraw of a cell immediately (using a direct DC construction,
// or the supplied dc)
BOOL CGridCtrl::RedrawCell(const CCellID& cell)
{
    return RedrawCell(cell.row, cell.col);
}

BOOL CGridCtrl::RedrawCell(int nRow, int nCol)
{
    BOOL bResult = TRUE;
    BOOL bMustReleaseDC = FALSE;

    if (!_bAllowDraw || !IsCellVisible(nRow, nCol))
        return FALSE;

    RECT rect;
    if (!GetCellRect(nRow, nCol, &rect))
        return FALSE;
    /****
    if (!pDC)
    {
        pDC = GetDC();
        if (pDC)
            bMustReleaseDC = TRUE;
    }

    if (pDC)
    {
        // Redraw cells directly
        if (nRow < _nFixedRows || nCol < _nFixedCols)
        {
            CGridCellBase* pCell = GetCell(nRow, nCol);
            if (pCell)
                bResult = pCell->Draw(pDC, nRow, nCol, rect, TRUE);
        }
        else
        {
            CGridCellBase* pCell = GetCell(nRow, nCol);
            if (pCell)
                bResult = pCell->Draw(pDC, nRow, nCol, rect, TRUE);

            // Since we have erased the background, we will need to redraw the gridlines
            /**** SEAN SEAN SEAN
            ////draw
            CPen pen;
            pen.CreatePen(PS_SOLID, 0, _crGridLineColour);

            CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);
            if (_nGridLines == GVL_BOTH || _nGridLines == GVL_HORZ)
            {
                pDC->MoveTo(rect.left,    rect.bottom);
                pDC->LineTo(rect.right + 1, rect.bottom);
            }
            if (_nGridLines == GVL_BOTH || _nGridLines == GVL_VERT)
            {
                pDC->MoveTo(rect.right, rect.top);
                pDC->LineTo(rect.right, rect.bottom + 1);
            }
            pDC->SelectObject(pOldPen);
        }
    } else
        InvalidateRect(rect, TRUE);     // Could not get a DC - invalidate it anyway
    // and hope that OnPaint manages to get one

    if (bMustReleaseDC)
        ReleaseDC(pDC);

    ****/
    return bResult;
}

// redraw a complete row
BOOL CGridCtrl::RedrawRow(int row)
{
    BOOL bResult = TRUE;
    /****
    CDC* pDC = GetDC();
    for (int col = 0; col < GetColumnCount(); col++)
        bResult = RedrawCell(row, col, pDC) && bResult;
    if (pDC)
        ReleaseDC(pDC);
    ****/
    return bResult;
}

// redraw a complete column
BOOL CGridCtrl::RedrawColumn(int col)
{
    BOOL bResult = TRUE;

    /****
    CDC* pDC = GetDC();
    for (int row = 0; row < GetRowCount(); row++)
        bResult = RedrawCell(row, col, pDC) && bResult;
    if (pDC)
        ReleaseDC(pDC);
    ****/

    return bResult;
}


// Sets the currently selected cell, returning the previous current cell
CCellID CGridCtrl::SetFocusCell(int nRow, int nCol)
{
    return SetFocusCell(CCellID(nRow, nCol));
}

CCellID CGridCtrl::SetFocusCell(CCellID cell)
{
    if (cell == _idCurrentCell)
        return _idCurrentCell;

    CCellID idPrev = _idCurrentCell;

    // EFW - Bug Fix - Force focus to be in a non-fixed cell
    if(cell.row != -1 && cell.row < GetFixedRowCount())
        cell.row = GetFixedRowCount();
    if(cell.col != -1 && cell.col < GetFixedColumnCount())
        cell.col = GetFixedColumnCount();

    _idCurrentCell = cell;

    if (IsValid(idPrev))
    {

        SetItemState(idPrev.row, idPrev.col,
            GetItemState(idPrev.row, idPrev.col) & ~GVIS_FOCUSED);
        RedrawCell(idPrev); // comment to reduce flicker

        if (GetTrackFocusCell() && idPrev.col != _idCurrentCell.col)
            for (int row = 0; row < _nFixedRows; row++)
                RedrawCell(row, idPrev.col);
        if (GetTrackFocusCell() && idPrev.row != _idCurrentCell.row)
            for (int col = 0; col < _nFixedCols; col++)
                RedrawCell(idPrev.row, col);
    }

    if (IsValid(_idCurrentCell))
    {
        SetItemState(_idCurrentCell.row, _idCurrentCell.col,
            GetItemState(_idCurrentCell.row, _idCurrentCell.col) | GVIS_FOCUSED);

        RedrawCell(_idCurrentCell); // comment to reduce flicker

        if (GetTrackFocusCell() && idPrev.col != _idCurrentCell.col)
            for (int row = 0; row < _nFixedRows; row++)
                RedrawCell(row, _idCurrentCell.col);
        if (GetTrackFocusCell() && idPrev.row != _idCurrentCell.row)
            for (int col = 0; col < _nFixedCols; col++)
                RedrawCell(_idCurrentCell.row, col);

        // EFW - New addition.  If in list mode, make sure the selected
        // row highlight follows the cursor.
        // Removed by C Maunder 27 May
        //if (_bListMode)
        //{
        //    _PrevSelectedCellMap.clear();
        //    _MouseMode = MOUSE_SELECT_ROW;
        //    OnSelecting(_idCurrentCell);

            // Leave this off so that you can still drag the highlight around
            // without selecting rows.
            // _MouseMode = MOUSE_NOTHING;
        //}

	}

    return idPrev;
}

// Sets the range of currently selected cells
void CGridCtrl::SetSelectedRange(const CCellRange& Range,
                                 BOOL bForceRepaint /* = FALSE */, BOOL bSelectCells/*=TRUE*/)
{
    SetSelectedRange(Range.GetMinRow(), Range.GetMinCol(),
                     Range.GetMaxRow(), Range.GetMaxCol(),
                     bForceRepaint, bSelectCells);
}

void CGridCtrl::SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
                                 BOOL bForceRepaint /* = FALSE */, BOOL bSelectCells/*=TRUE*/)
{
    if (!_bEnableSelection)
        return;

    /****
    CDC* pDC = NULL;
    if (bForceRepaint)
        pDC = GetDC();

	// Only redraw visible cells
    CCellRange VisCellRange;
	if (IsWindow(GetSafeHwnd()))
		VisCellRange = GetVisibleNonFixedCellRange();

    // EFW - Bug fix - Don't allow selection of fixed rows
    if(nMinRow >= 0 && nMinRow < GetFixedRowCount())
        nMinRow = GetFixedRowCount();
    if(nMaxRow >= 0 && nMaxRow < GetFixedRowCount())
        nMaxRow = GetFixedRowCount();
    if(nMinCol >= 0 && nMinCol < GetFixedColumnCount())
        nMinCol = GetFixedColumnCount();
    if(nMaxCol >= 0 && nMaxCol < GetFixedColumnCount())
        nMaxCol = GetFixedColumnCount();

    // If we are selecting cells, then first clear out the list of currently selected cells, then
    if (bSelectCells)
    {
        POSITION pos;

        // Unselect all previously selected cells
        for (pos = _SelectedCellMap.GetStartPosition(); pos != NULL; )
        {
            DWORD key;
            CCellID cell;
            _SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

            // Reset the selection flag on the cell
            if (IsValid(cell))
            {
                // This will remove the cell from the _SelectedCellMap map
                SetItemState(cell.row, cell.col,
                    GetItemState(cell.row, cell.col) & ~GVIS_SELECTED);

                // If this is to be reselected, continue on past the redraw
                if (nMinRow <= cell.row && cell.row <= nMaxRow &&
                    nMinCol <= cell.col && cell.col <= nMaxCol)
                    continue;

				if ( VisCellRange.IsValid() && VisCellRange.InRange( cell ) )
				{
					if (bForceRepaint && pDC)                    // Redraw NOW
						RedrawCell(cell.row, cell.col);
					else
						InvalidateCellRect(cell);                // Redraw at leisure
				}
            }
            else
            {
                _SelectedCellMap.RemoveKey( key);  // if it's not valid, get rid of it!
            }
        }

        // if we are selecting cells, and there are previous selected cells to be retained
        // (eg Ctrl is being held down) then copy them to the newly created list, and mark
        // all these cells as selected
        // Note that if we are list mode, single row selection, the we won't be adding
        // the previous cells. Only the current row of cells will be added (see below)
        if (!GetSingleRowSelection() &&
            nMinRow >= 0 && nMinCol >= 0 && nMaxRow >= 0 && nMaxCol >= 0)
        {
            for (pos = _PrevSelectedCellMap.GetStartPosition(); pos != NULL;)
            {
                DWORD key;
                CCellID cell;
                _PrevSelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

                if (!IsValid(cell))
                    continue;

                int nState = GetItemState(cell.row, cell.col);

                // Set state as Selected. This will add the cell to _SelectedCellMap
                SetItemState(cell.row, cell.col, nState | GVIS_SELECTED);

				if ( VisCellRange.IsValid() && VisCellRange.InRange( cell ) )
				{
					// Redraw (immediately or at leisure)
					if (bForceRepaint && pDC)
					    RedrawCell(cell.row, cell.col, pDC);
					else
					    InvalidateCellRect(cell);
				}
            }
        }
    }

    // Now select/deselect all cells in the cell range specified. If selecting, and the cell
    // has already been marked as selected (above) then ignore it. If we are deselecting and
    // the cell isn't selected, then ignore
    if (nMinRow >= 0 && nMinCol >= 0 && nMaxRow >= 0 && nMaxCol >= 0 &&
        nMaxRow < _nRows && nMaxCol < _nCols &&
        nMinRow <= nMaxRow && nMinCol <= nMaxCol)
    {
        for (int row = nMinRow; row <= nMaxRow; row++)
            for (int col = nMinCol; col <= nMaxCol; col++)
            {
                int nState = GetItemState(row, col);

                // Why is there no XOR operator in C??? We have bitwise XOR ^, but what about ^^??
                if ( (bSelectCells && (nState & GVIS_SELECTED)) ||
                     (!bSelectCells && !(nState & GVIS_SELECTED)) )
                    continue;    // Already selected or deselected - ignore

                // Set the selected state. This will add/remove the cell to _SelectedCellMap
                if (bSelectCells)
                    SetItemState(row, col, nState | GVIS_SELECTED);
                else
                    SetItemState(row, col, GetItemState(row, col) & ~GVIS_SELECTED);

				if ( VisCellRange.IsValid() && VisCellRange.InRange(row, col) )
				{
	                // Redraw (immediately or at leisure)
	                if (bForceRepaint && pDC)
	                    RedrawCell(row, col, pDC);
	                else
	                    InvalidateCellRect(row, col);
				}
            }
    }
    //    TRACE(_T("%d cells selected.\n"), _SelectedCellMap.GetCount());

    if (pDC != NULL)
        ReleaseDC(pDC);
    ****/
}

// selects all cells
void CGridCtrl::SelectAllCells()
{
    if (!_bEnableSelection)
        return;

    SetSelectedRange(_nFixedRows, _nFixedCols, GetRowCount()-1, GetColumnCount()-1);
}

// selects columns
void CGridCtrl::SelectColumns(CCellID currentCell,
                              BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
    if (!_bEnableSelection)
        return;

    //if (currentCell.col == _idCurrentCell.col) return;
    if (currentCell.col < _nFixedCols)
        return;
    if (!IsValid(currentCell))
        return;

    if (GetSingleColSelection())
        SetSelectedRange(GetFixedRowCount(), currentCell.col,
                         GetRowCount()-1,    currentCell.col,
                         bForceRedraw, bSelectCells);
    else
        SetSelectedRange(GetFixedRowCount(),
                         min(_SelectionStartCell.col, currentCell.col),
                         GetRowCount()-1,
                         max(_SelectionStartCell.col, currentCell.col),
                         bForceRedraw, bSelectCells);
}

// selects rows
void CGridCtrl::SelectRows(CCellID currentCell,
                           BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
    if (!_bEnableSelection)
        return;

    //if (currentCell.row; == _idCurrentCell.row) return;
    if (currentCell.row < _nFixedRows)
        return;
    if (!IsValid(currentCell))
        return;

    if (GetSingleRowSelection())
        SetSelectedRange(currentCell.row, GetFixedColumnCount(),
                         currentCell.row, GetColumnCount()-1,
                         bForceRedraw, bSelectCells);
    else
        SetSelectedRange(min(_SelectionStartCell.row, currentCell.row),
                         GetFixedColumnCount(),
                         max(_SelectionStartCell.row, currentCell.row),
                         GetColumnCount()-1,
                         bForceRedraw, bSelectCells);
}

// selects cells
void CGridCtrl::SelectCells(CCellID currentCell,
                            BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
    if (!_bEnableSelection)
        return;

    int row = currentCell.row;
    int col = currentCell.col;
    if (row < _nFixedRows || col < _nFixedCols)
        return;
    if (!IsValid(currentCell))
        return;

    // Prevent unnecessary redraws
    //if (currentCell == _LeftClickDownCell)  return;
    //else if (currentCell == _idCurrentCell) return;

    SetSelectedRange(min(_SelectionStartCell.row, row),
                     min(_SelectionStartCell.col, col),
                     max(_SelectionStartCell.row, row),
                     max(_SelectionStartCell.col, col),
                     bForceRedraw, bSelectCells);
}

// Called when mouse/keyboard selection is a-happening.
void CGridCtrl::OnSelecting(const CCellID& currentCell)
{
    if (!_bEnableSelection)
        return;

    switch (_MouseMode)
    {
    case MOUSE_SELECT_ALL:
        SelectAllCells();
        break;
    case MOUSE_SELECT_COL:
        SelectColumns(currentCell, FALSE);
        break;
    case MOUSE_SELECT_ROW:
        SelectRows(currentCell, FALSE);
        break;
    case MOUSE_SELECT_CELLS:
        SelectCells(currentCell, FALSE);
        break;
    }

    // EFW - Bug fix [REMOVED CJM: this will cause infinite loop in list mode]
    // SetFocusCell(max(currentCell.row, _nFixedRows), max(currentCell.col, _nFixedCols));
}

void CGridCtrl::ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText)
{
    if (!IsCellEditable(nRow, nCol))
        return;

    if (SendMessageToParent(nRow, nCol, GVN_BEGINLABELEDIT) >= 0)
    {
        tstring strCurrentText = GetItemText(nRow, nCol);
        if (strCurrentText != strText)
        {
            SetItemText(nRow, nCol, strText);
            if (ValidateEdit(nRow, nCol, strText) &&
                SendMessageToParent(nRow, nCol, GVN_ENDLABELEDIT) >= 0)
            {
                SetModified(TRUE, nRow, nCol);
                RedrawCell(nRow, nCol);
            }
            else
            {
                SetItemText(nRow, nCol, strCurrentText.c_str());
            }
        }
    }
}

void CGridCtrl::ClearCells(CCellRange Selection)
{
    for (int row = Selection.GetMinRow(); row <= Selection.GetMaxRow(); row++)
    {
        for (int col = Selection.GetMinCol(); col <= Selection.GetMaxCol(); col++)
        {
            // don't clear hidden cells
            if ( _arRowHeights[row] > 0 && _arColWidths[col] > 0 )
            {
                ValidateAndModifyCellContents(row, col, _T(""));
            }
		}
	}
    Refresh();
}

#ifndef GRIDCONTROL_NO_CLIPBOARD

////////////////////////////////////////////////////////////////////////////////////////
// Clipboard functions

// Deletes the contents from the selected cells
void CGridCtrl::CutSelectedText()
{
    if (!IsEditable())
        return;

    CCellRange Selection = GetSelectedCellRange();
    if (!IsValid(Selection))
        return;

	ClearCells(Selection);
}

// Copies text from the selected cells to the clipboard
COleDataSource* CGridCtrl::CopyTextFromGrid()
{
    USES_CONVERSION;

    CCellRange Selection = GetSelectedCellRange();
    if (!IsValid(Selection))
        return NULL;

    if (GetVirtualMode())
        SendCacheHintToParent(Selection);

    // Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
    CSharedFile sf(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);

    // Get a tab delimited string to copy to cache
    tstring str;
    CGridCellBase *pCell;
    for (int row = Selection.GetMinRow(); row <= Selection.GetMaxRow(); row++)
    {
        // don't copy hidden cells
        if( _arRowHeights[row] <= 0 )
            continue;

        str.Empty();
        for (int col = Selection.GetMinCol(); col <= Selection.GetMaxCol(); col++)
        {
            // don't copy hidden cells
            if( _arColWidths[col] <= 0 )
                continue;

            pCell = GetCell(row, col);
            if (pCell &&(pCell->GetState() & GVIS_SELECTED))
            {
                // if (!pCell->GetText())
                //    str += _T(" ");
                // else
                str += pCell->GetText();
            }
            if (col != Selection.GetMaxCol())
                str += _T("\t");
        }
        if (row != Selection.GetMaxRow())
            str += _T("\n");

        sf.Write(T2A(str.GetBuffer(1)), str.GetLength());
        str.ReleaseBuffer();
    }

    char c = '\0';
    sf.Write(&c, 1);

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    DWORD dwLen = (DWORD) sf.GetLength();
    HGLOBAL hMem = sf.Detach();
    if (!hMem)
        return NULL;

    hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
    if (!hMem)
        return NULL;

    // Cache data
    COleDataSource* pSource = new COleDataSource();
    pSource->CacheGlobalData(CF_TEXT, hMem);

    return pSource;
}

// Pastes text from the clipboard to the selected cells
BOOL CGridCtrl::PasteTextToGrid(CCellID cell, COleDataObject* pDataObject)
{
    if (!IsValid(cell) || !IsCellEditable(cell) || !pDataObject->IsDataAvailable(CF_TEXT))
        return FALSE;

    // Get the text from the COleDataObject
    HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
    CMemFile sf((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

    // CF_TEXT is ANSI text, so we need to allocate a char* buffer
    // to hold this.
    LPSTR szBuffer = new char[::GlobalSize(hmem)];
    if (!szBuffer)
        return FALSE;

    sf.Read(szBuffer, ::GlobalSize(hmem));
    ::GlobalUnlock(hmem);

    // Now store in generic TCHAR form so we no longer have to deal with
    // ANSI/UNICODE problems
    tstring strText(szBuffer);
    delete szBuffer;

    // Parse text data and set in cells...
    strText.LockBuffer();
    tstring strLine = strText;
    int nLine = 0;

    // Find the end of the first line
    int nIndex;
    do
    {
        int nColumn = 0;
        nIndex = strLine.Find(_T("\n"));

        // Store the remaining chars after the newline
        tstring strNext = (nIndex < 0)? _T("")  : strLine.Mid(nIndex + 1);

        // Remove all chars after the newline
        if (nIndex >= 0)
            strLine = strLine.Left(nIndex);

        int nLineIndex = strLine.FindOneOf(_T("\t,"));
        tstring strCellText = (nLineIndex >= 0)? strLine.Left(nLineIndex) : strLine;

        // skip hidden rows
        int iRowVis = cell.row + nLine;
        while( iRowVis < GetRowCount())
        {
            if( GetRowHeight( iRowVis) > 0)
                break;
            nLine++;
            iRowVis++;
        }

        while (!strLine.IsEmpty())
        {
            // skip hidden columns
            int iColVis = cell.col + nColumn;
            while( iColVis < GetColumnCount())
            {
                if( GetColumnWidth( iColVis) > 0)
                    break;
                nColumn++;
                iColVis++;
            }

            CCellID TargetCell(iRowVis, iColVis);
            if (IsValid(TargetCell))
            {
                strCellText.TrimLeft();
                strCellText.TrimRight();

                ValidateAndModifyCellContents(TargetCell.row, TargetCell.col, strCellText);

                // Make sure cell is not selected to avoid data loss
                SetItemState(TargetCell.row, TargetCell.col,
                    GetItemState(TargetCell.row, TargetCell.col) & ~GVIS_SELECTED);
            }

            strLine = (nLineIndex >= 0)? strLine.Mid(nLineIndex + 1) : _T("");
            nLineIndex = strLine.FindOneOf(_T("\t,"));
            strCellText = (nLineIndex >= 0)? strLine.Left(nLineIndex) : strLine;

            nColumn++;
        }

        strLine = strNext;
        nLine++;
    } while (nIndex >= 0);

    strText.UnlockBuffer();
    Refresh();

    return TRUE;
}
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP

// Start drag n drop
void CGridCtrl::OnBeginDrag()
{
    if (!_bAllowDragAndDrop)
        return;

    COleDataSource* pSource = CopyTextFromGrid();
    if (pSource)
    {
        SendMessageToParent(GetSelectedCellRange().GetTopLeft().row,
            GetSelectedCellRange().GetTopLeft().col,
            GVN_BEGINDRAG);

        _MouseMode = MOUSE_DRAGGING;
        _bLMouseButtonDown = FALSE;

        DROPEFFECT dropEffect = pSource->DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);

        if (dropEffect & DROPEFFECT_MOVE)
            CutSelectedText();

        if (pSource)
            delete pSource;    // Did not pass source to clipboard, so must delete
    }
}

// Handle drag over grid
DROPEFFECT CGridCtrl::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState,
                                 POINT_u point)
{
    // Any text data available for us?
    if (!_bAllowDragAndDrop || !IsEditable() || !pDataObject->IsDataAvailable(CF_TEXT))
        return DROPEFFECT_NONE;

    // Find which cell we are over and drop-highlight it
    CCellID cell = GetCellFromPt(point, FALSE);

    // If not valid, set the previously drop-highlighted cell as no longer drop-highlighted
    if (!IsValid(cell))
    {
        OnDragLeave();
        _LastDragOverCell = CCellID(-1,-1);
        return DROPEFFECT_NONE;
    }

    if (!IsCellEditable(cell))
        return DROPEFFECT_NONE;

    // Have we moved over a different cell than last time?
    if (cell != _LastDragOverCell)
    {
        // Set the previously drop-highlighted cell as no longer drop-highlighted
        if (IsValid(_LastDragOverCell))
        {
            UINT nState = GetItemState(_LastDragOverCell.row, _LastDragOverCell.col);
            SetItemState(_LastDragOverCell.row, _LastDragOverCell.col,
                nState & ~GVIS_DROPHILITED);
            RedrawCell(_LastDragOverCell);
        }

        _LastDragOverCell = cell;

        // Set the new cell as drop-highlighted
        if (IsValid(_LastDragOverCell))
        {
            UINT nState = GetItemState(_LastDragOverCell.row, _LastDragOverCell.col);
            SetItemState(_LastDragOverCell.row, _LastDragOverCell.col,
                nState | GVIS_DROPHILITED);
            RedrawCell(_LastDragOverCell);
        }
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    if (dwKeyState & MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;
}

// Something has just been dragged onto the grid
DROPEFFECT CGridCtrl::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState,
                                  POINT_u point)
{
    // Any text data available for us?
    if (!_bAllowDragAndDrop || !pDataObject->IsDataAvailable(CF_TEXT))
        return DROPEFFECT_NONE;

    // Find which cell we are over and drop-highlight it
    _LastDragOverCell = GetCellFromPt(point, FALSE);
    if (!IsValid(_LastDragOverCell))
        return DROPEFFECT_NONE;

    if (!IsCellEditable(_LastDragOverCell))
        return DROPEFFECT_NONE;

    if (IsValid(_LastDragOverCell))
    {
        UINT nState = GetItemState(_LastDragOverCell.row, _LastDragOverCell.col);
        SetItemState(_LastDragOverCell.row, _LastDragOverCell.col,
            nState | GVIS_DROPHILITED);
        RedrawCell(_LastDragOverCell);
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    if (dwKeyState & MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;
}

// Something has just been dragged away from the grid
void CGridCtrl::OnDragLeave()
{
    // Set the previously drop-highlighted cell as no longer drop-highlighted
    if (IsValid(_LastDragOverCell))
    {
        UINT nState = GetItemState(_LastDragOverCell.row, _LastDragOverCell.col);
        SetItemState(_LastDragOverCell.row, _LastDragOverCell.col,
            nState & ~GVIS_DROPHILITED);
        RedrawCell(_LastDragOverCell);
    }
}

// Something has just been dropped onto the grid
BOOL CGridCtrl::OnDrop(COleDataObject* pDataObject, DROPEFFECT /*dropEffect*/,
                       POINT_u /* point */)
{
    _MouseMode = MOUSE_NOTHING;

    if (!_bAllowDragAndDrop || !IsCellEditable(_LastDragOverCell))
        return FALSE;

    OnDragLeave();

    return PasteTextToGrid(_LastDragOverCell, pDataObject);
}
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CGridCtrl::OnEditCut()
{
    if (!IsEditable())
        return;

    COleDataSource* pSource = CopyTextFromGrid();
    if (!pSource)
        return;

    pSource->SetClipboard();
    CutSelectedText();
}

void CGridCtrl::OnEditCopy()
{
    COleDataSource* pSource = CopyTextFromGrid();
    if (!pSource)
        return;

    pSource->SetClipboard();
}

void CGridCtrl::OnEditPaste()
{
    if (!IsEditable())
        return;

    // Get the Focus cell, or if none, get the topleft (non-fixed) cell
    CCellID cell = GetFocusCell();
    if (!IsValid(cell))
        cell = GetTopleftNonFixedCell();
    if (!IsValid(cell))
        return;

	// If a cell is being edited, then call it's edit window paste function.
    if ( IsItemEditing(cell.row, cell.col) )
    {
        CGridCellBase* pCell = GetCell(cell.row, cell.col);
        ASSERT(pCell);
        if (!pCell) return;

		CWnd* pEditWnd = pCell->GetEditWnd();
		if ( pEditWnd && pEditWnd->IsKindOf(RUNTIME_CLASS(CEdit)) )
		{
			((CEdit*)pEditWnd)->Paste();
			return;
		}
    }

    // Attach a COleDataObject to the clipboard and paste the data to the grid
    COleDataObject obj;
    if (obj.AttachClipboard())
        PasteTextToGrid(cell, &obj);
}
#endif

void CGridCtrl::OnEditSelectAll()
{
    SendMessageToParent(_LeftClickDownCell.row, _LeftClickDownCell.col, GVN_SELCHANGING);
    SelectAllCells();
    SendMessageToParent(_idCurrentCell.row, _idCurrentCell.col, GVN_SELCHANGED);
}

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CGridCtrl::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
    CCellRange Selection = GetSelectedCellRange();
    pCmdUI->Enable(Selection.Count() && IsValid(Selection));
}

void CGridCtrl::OnUpdateEditCut(CCmdUI* pCmdUI)
{
    CCellRange Selection = GetSelectedCellRange();
    pCmdUI->Enable(IsEditable() && Selection.Count() && IsValid(Selection));
}

void CGridCtrl::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
    CCellID cell = GetFocusCell();

    BOOL bCanPaste = IsValid(cell) && IsCellEditable(cell) &&
        ::IsClipboardFormatAvailable(CF_TEXT);

    pCmdUI->Enable(bCanPaste);
}
#endif

void CGridCtrl::OnUpdateEditSelectAll()
{
}

////////////////////////////////////////////////////////////////////////////////////////
// hittest-like functions

// TRUE if the mouse is over a row resize area
BOOL CGridCtrl::MouseOverRowResizeArea(POINT_u& point)
{
    if (point.x >= GetFixedColumnWidth())
        return FALSE;

    CCellID idCurrentCell = GetCellFromPt(point);
    POINT_u start;
    if (!GetCellOrigin(idCurrentCell, &start))
        return FALSE;

    int endy = start.y + GetRowHeight(idCurrentCell.row);

    if ((point.y - start.y < _nResizeCaptureRange && idCurrentCell.row != 0) ||
        endy - point.y < _nResizeCaptureRange)
    {
        return TRUE;
    }
    else
        return FALSE;
}

// TRUE if the mouse is over a column resize area. point is in Client coords
BOOL CGridCtrl::MouseOverColumnResizeArea(POINT_u& point)
{
    if (point.y >= GetFixedRowHeight())
        return FALSE;

    CCellID idCurrentCell = GetCellFromPt(point);
    POINT_u start;
    if (!GetCellOrigin(idCurrentCell, &start))
        return FALSE;

    int endx = start.x + GetColumnWidth(idCurrentCell.col);

    if ((point.x - start.x < _nResizeCaptureRange && idCurrentCell.col != 0) ||
        endx - point.x < _nResizeCaptureRange)
    {
        return TRUE;
    }
    else
        return FALSE;
}

// Get cell from point.
// point - client coordinates
// bAllowFixedCellCheck - if TRUE then fixed cells are checked
CCellID CGridCtrl::GetCellFromPt(POINT_u point, BOOL bAllowFixedCellCheck /*=TRUE*/)
{
    CCellID cellID; // return value

    CCellID idTopLeft = GetTopleftNonFixedCell();
	if (!bAllowFixedCellCheck && !IsValid(idTopLeft))
		return cellID;

    // calculate column index
    int fixedColWidth = GetFixedColumnWidth();

    if (point.x < 0 || (!bAllowFixedCellCheck && point.x < fixedColWidth)) // not in window
        cellID.col = -1;
    else if (point.x < fixedColWidth) // in fixed col
    {
        int xpos = 0;
        int col = 0;
        while (col < _nFixedCols)
        {
            xpos += GetColumnWidth(col);
            if (xpos > point.x)
                break;
			col++;
        }
        cellID.col = col;
    }
    else    // in non-fixed col
    {
        int xpos = fixedColWidth;
		int col = idTopLeft.col; //_nFixedCols;
        while ( col < GetColumnCount())
        {
            xpos += GetColumnWidth(col);
            if (xpos > point.x)
                break;
			col++;
        }

        if (col >= GetColumnCount())
            cellID.col = -1;
        else
            cellID.col = col;
    }

    // calculate row index
    int fixedRowHeight = GetFixedRowHeight();
    if (point.y < 0 || (!bAllowFixedCellCheck && point.y < fixedRowHeight)) // not in window
        cellID.row = -1;
    else if (point.y < fixedRowHeight) // in fixed col
    {
        int ypos = 0;
        int row = 0;
        while (row < _nFixedRows)
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y)
                break;
			row++;
        }
        cellID.row = row;
    }
    else
    {
        int ypos = fixedRowHeight;
		int row = idTopLeft.row; //_nFixedRows;
        while ( row < GetRowCount() )
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y)
                break;
			row++;
        }

        if (row >= GetRowCount())
            cellID.row = -1;
        else
            cellID.row = row;
    }

    return cellID;
}

////////////////////////////////////////////////////////////////////////////////
// CGridCtrl cellrange functions

// Gets the first non-fixed cell ID
CCellID CGridCtrl::GetTopleftNonFixedCell(BOOL bForceRecalculation /*=FALSE*/)
{
    // Used cached value if possible
    if (_idTopLeftCell.IsValid() && !bForceRecalculation)
        return _idTopLeftCell;
    int nVertScroll = 0, /**** GetScrollPos(SB_VERT), ****/
        nHorzScroll = 0; /**** GetScrollPos(SB_HORZ)  ****/

    _idTopLeftCell.col = _nFixedCols;
    int nRight = 0;
    while (nRight < nHorzScroll && _idTopLeftCell.col < (GetColumnCount()-1))
        nRight += GetColumnWidth(_idTopLeftCell.col++);

    _idTopLeftCell.row = _nFixedRows;
    int nTop = 0;
    while (nTop < nVertScroll && _idTopLeftCell.row < (GetRowCount()-1))
        nTop += GetRowHeight(_idTopLeftCell.row++);

    //TRACE2("TopLeft cell is row %d, col %d\n",_idTopLeftCell.row, _idTopLeftCell.col);
    return _idTopLeftCell;
}

// This gets even partially visible cells
CCellRange CGridCtrl::GetVisibleNonFixedCellRange(LPRECT pRect /*=NULL*/,
                                                  BOOL bForceRecalculation /*=FALSE*/)
{
    CCellID idTopLeft = GetTopleftNonFixedCell(bForceRecalculation);

    // calc bottom
    int bottom = GetFixedRowHeight();
	int i;
    for (i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= _rect.bottom - _margin.bottom)
        {
            bottom = _rect.bottom - _margin.bottom;
            break;
        }
    }
    int maxVisibleRow = min(i, GetRowCount() - 1);

    // calc right
    int right = GetFixedColumnWidth();
    for (i = idTopLeft.col; i < GetColumnCount(); i++)
    {
        right += GetColumnWidth(i);
        if (right >= _rect.right - _margin.right)
        {
            right = _rect.right - _margin.right;
            break;
        }
    }
    int maxVisibleCol = min(i, GetColumnCount() - 1);
    if (pRect)
    {
        pRect->left = pRect->top = 0;
        pRect->right = right;
        pRect->bottom = bottom;
    }

    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

// used by ResetScrollBars() - This gets only fully visible cells
CCellRange CGridCtrl::GetUnobstructedNonFixedCellRange(BOOL bForceRecalculation /*=FALSE*/)
{
    RECT rect;
    ////GetClientRect(&rect);

    CCellID idTopLeft = GetTopleftNonFixedCell(bForceRecalculation);

    // calc bottom
    int bottom = GetFixedRowHeight();
	int i;
    for (i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom)
            break;
    }
    int maxVisibleRow = min(i, GetRowCount() - 1);
    if (maxVisibleRow > 0 && bottom > rect.bottom)
        maxVisibleRow--;

    // calc right
    int right = GetFixedColumnWidth();
    for (i = idTopLeft.col; i < GetColumnCount(); i++)
    {
        right += GetColumnWidth(i);
        if (right >= rect.right)
            break;
    }
    int maxVisibleCol = min(i, GetColumnCount() - 1);
    if (maxVisibleCol > 0 && right > rect.right)
        maxVisibleCol--;


    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

// Returns the minimum bounding range of the current selection
// If no selection, then the returned CCellRange will be invalid
CCellRange CGridCtrl::GetSelectedCellRange() const
{
    CCellRange Selection(GetRowCount(), GetColumnCount(), -1,-1);

    CellIDMapConstIter iter = _SelectedCellMap.begin();
    for (; iter != _SelectedCellMap.end(); iter++)
    {
        DWORD key = iter->first;
        const CCellID* cell = iter->second;

        Selection.SetMinRow( min(Selection.GetMinRow(), cell->row) );
        Selection.SetMinCol( min(Selection.GetMinCol(), cell->col) );
        Selection.SetMaxRow( max(Selection.GetMaxRow(), cell->row) );
        Selection.SetMaxCol( max(Selection.GetMaxCol(), cell->col) );
    }

    return Selection;
}

// Returns ALL the cells in the grid
CCellRange CGridCtrl::GetCellRange() const
{
    return CCellRange(0, 0, GetRowCount() - 1, GetColumnCount() - 1);
}

// Resets the selected cell range to the empty set.
void CGridCtrl::ResetSelectedRange()
{
    _PrevSelectedCellMap.clear();
    SetSelectedRange(-1,-1,-1,-1);
    SetFocusCell(-1,-1);
}
////////////////////////////////////////////////////////////////////////////////////
// Row/Column position functions

// returns the top left point of the cell. Returns FALSE if cell not visible.
BOOL CGridCtrl::GetCellOrigin(int nRow, int nCol, POINT_u* p)
{
    int i;

    if (!IsValid(nRow, nCol))
        return FALSE;

    CCellID idTopLeft;
    if (nCol >= _nFixedCols || nRow >= _nFixedRows)
        idTopLeft = GetTopleftNonFixedCell();

    if ((nRow >= _nFixedRows && nRow < idTopLeft.row) ||
        (nCol>= _nFixedCols && nCol < idTopLeft.col))
        return FALSE;

    p->x = 0;
    if (nCol < _nFixedCols)                      // is a fixed column
        for (i = 0; i < nCol; i++)
            p->x += GetColumnWidth(i);
        else
        {                                        // is a scrollable data column
            for (i = 0; i < _nFixedCols; i++)
                p->x += GetColumnWidth(i);
            for (i = idTopLeft.col; i < nCol; i++)
                p->x += GetColumnWidth(i);
        }

        p->y = 0;
        if (nRow < _nFixedRows)                      // is a fixed row
            for (i = 0; i < nRow; i++)
                p->y += GetRowHeight(i);
            else
            {                                        // is a scrollable data row
                for (i = 0; i < _nFixedRows; i++)
                    p->y += GetRowHeight(i);
                for (i = idTopLeft.row; i < nRow; i++)
                    p->y += GetRowHeight(i);
            }

            return TRUE;
}

BOOL CGridCtrl::GetCellOrigin(const CCellID& cell, POINT_u* p)
{
    return GetCellOrigin(cell.row, cell.col, p);
}

// Returns the bounding box of the cell
BOOL CGridCtrl::GetCellRect(const CCellID& cell, LPRECT pRect)
{
    return GetCellRect(cell.row, cell.col, pRect);
}

BOOL CGridCtrl::GetCellRect(int nRow, int nCol, LPRECT pRect)
{
    POINT_u CellOrigin;
    if (!GetCellOrigin(nRow, nCol, &CellOrigin))
        return FALSE;

    pRect->left   = CellOrigin.x;
    pRect->top    = CellOrigin.y;
    pRect->right  = CellOrigin.x + GetColumnWidth(nCol)-1;
    pRect->bottom = CellOrigin.y + GetRowHeight(nRow)-1;

    //TRACE("Row %d, col %d: L %d, T %d, W %d, H %d:  %d,%d - %d,%d\n",
    //      nRow,nCol, CellOrigin.x, CellOrigin.y, GetColumnWidth(nCol), GetRowHeight(nRow),
    //      pRect->left, pRect->top, pRect->right, pRect->bottom);

    return TRUE;
}

BOOL CGridCtrl::GetTextRect(const CCellID& cell, LPRECT pRect)
{
    return GetTextRect(cell.row, cell.col, pRect);
}

BOOL CGridCtrl::GetTextRect(int nRow, int nCol, LPRECT pRect)
{
    CGridCellBase* pCell = GetCell( nRow, nCol);
    if( pCell == NULL)
        return FALSE;

    if( !GetCellRect( nRow, nCol, pRect) )
        return FALSE;

    return pCell->GetTextRect( pRect);
}

// Returns the bounding box of a range of cells
BOOL CGridCtrl::GetCellRangeRect(const CCellRange& cellRange, LPRECT lpRect)
{
    POINT_u MinOrigin,MaxOrigin;

    if (!GetCellOrigin(cellRange.GetMinRow(), cellRange.GetMinCol(), &MinOrigin))
        return FALSE;
    if (!GetCellOrigin(cellRange.GetMaxRow(), cellRange.GetMaxCol(), &MaxOrigin))
        return FALSE;

    lpRect->left   = MinOrigin.x;
    lpRect->top    = MinOrigin.y;
    lpRect->right  = MaxOrigin.x + GetColumnWidth(cellRange.GetMaxCol()) - 1;
    lpRect->bottom = MaxOrigin.y + GetRowHeight(cellRange.GetMaxRow()) - 1;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Grid attribute functions

LRESULT CGridCtrl::OnSetFont(WPARAM hFont, LPARAM /*lParam */)
{
    /****
    LRESULT result = Default();

    // Get the logical font
    LOGFONT lf;
    if (!GetObject((HFONT) hFont, sizeof(LOGFONT), &lf))
        return result;

    _cellDefault.SetFont(&lf);
    _cellFixedColDef.SetFont(&lf);
    _cellFixedRowDef.SetFont(&lf);
    _cellFixedRowColDef.SetFont(&lf);
    ****/
    Refresh();

    return 0;
}

LRESULT CGridCtrl::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    //LOGFONT    lf;
    //_cellDefault.GetFontObject()->GetLogFont(&lf);

    return (LRESULT) _cellDefault.GetFontObject();
}

#ifndef _WIN32_WCE_NO_CURSOR
BOOL CGridCtrl::OnSetCursor(UINT nHitTest, UINT message)
{
    /****
    if (nHitTest == HTCLIENT)
    {
        switch (_MouseMode)
        {
        case MOUSE_OVER_COL_DIVIDE:
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
            break;
        case MOUSE_OVER_ROW_DIVIDE:
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
            break;
#ifndef GRIDCONTROL_NO_DRAGDROP
        case MOUSE_DRAGGING:
            break;
#endif
        default:
            if (!GetVirtualMode())
            {
                POINT_u pt(GetMessagePos());
                ScreenToClient(&pt);
                CCellID cell = GetCellFromPt(pt);
                if (IsValid(cell))
                {
                    CGridCellBase* pCell = GetCell(cell.row, cell.col);
                    if (pCell)
                        return pCell->OnSetCursor();
                }
            }

            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        }
        return TRUE;
    }

    return ////CWnd::OnSetCursor(pWnd, nHitTest, message);
    ****/
    return TRUE;
}
#endif

////////////////////////////////////////////////////////////////////////////////////
// Row/Column count functions

BOOL CGridCtrl::SetFixedRowCount(int nFixedRows)
{
    if (_nFixedRows == nFixedRows)
        return TRUE;

    ASSERT(nFixedRows >= 0);

    ResetSelectedRange();

    // Force recalculation
    _idTopLeftCell.col = -1;

    if (nFixedRows > GetRowCount())
        if (!SetRowCount(nFixedRows))
            return FALSE;

        if (_idCurrentCell.row < nFixedRows)
            SetFocusCell(-1, - 1);

        if (!GetVirtualMode())
        {
            if (nFixedRows > _nFixedRows)
            {
                for (int i = _nFixedRows; i < nFixedRows; i++)
                    for (int j = 0; j < GetColumnCount(); j++)
                    {
                        SetItemState(i, j, GetItemState(i, j) | GVIS_FIXED | GVIS_FIXEDROW);
                        SetItemBkColour(i, j, _uiSetting._crFixRowText    );
                        SetItemFgColour(i, j, _uiSetting._crFixRowBkgText );
                    }
            }
            else
            {
                for (int i = nFixedRows; i < _nFixedRows; i++)
                {
                    int j;
                    for (j = 0; j < GetFixedColumnCount(); j++)
                        SetItemState(i, j, GetItemState(i, j) & ~GVIS_FIXEDROW );

                    for (j = GetFixedColumnCount(); j < GetColumnCount(); j++)
                    {
                        SetItemState(i, j, GetItemState(i, j) & ~(GVIS_FIXED | GVIS_FIXEDROW) );
                        SetItemBkColour(i, j, _uiSetting._crFixRowText    );
                        SetItemFgColour(i, j, _uiSetting._crFixRowBkgText );
                    }
                }
            }
        }

        _nFixedRows = nFixedRows;

        Refresh();

        return TRUE;
}

BOOL CGridCtrl::SetFixedColumnCount(int nFixedCols)
{
    if (_nFixedCols == nFixedCols)
        return TRUE;

    ASSERT(nFixedCols >= 0);

    if (nFixedCols > GetColumnCount())
        if (!SetColumnCount(nFixedCols))
            return FALSE;

    if (_idCurrentCell.col < nFixedCols)
        SetFocusCell(-1, - 1);

    ResetSelectedRange();

    // Force recalculation
    _idTopLeftCell.col = -1;

    if (!GetVirtualMode())
    {
        if (nFixedCols > _nFixedCols)
        {
            for (int i = 0; i < GetRowCount(); i++)
                for (int j = _nFixedCols; j < nFixedCols; j++)
                {
                    SetItemState(i, j, GetItemState(i, j) | GVIS_FIXED | GVIS_FIXEDCOL);
                    SetItemBkColour(i, j, _uiSetting._crFixColText    );
                    SetItemFgColour(i, j, _uiSetting._crFixColBkgText );
                }
        }
        else
        {
            int i;
            for (i = 0; i < GetFixedRowCount(); i++)
                for (int j = nFixedCols; j < _nFixedCols; j++)
                    SetItemState(i, j, GetItemState(i, j) & ~GVIS_FIXEDCOL );

            for (i = GetFixedRowCount(); i < GetRowCount(); i++)
                for (int j = nFixedCols; j < _nFixedCols; j++)
                {
                    SetItemState(i, j, GetItemState(i, j) & ~(GVIS_FIXED | GVIS_FIXEDCOL) );
                    SetItemBkColour(i, j, _uiSetting._crFixColText    );
                    SetItemFgColour(i, j, _uiSetting._crFixColBkgText );
                }
        }
    }

    _nFixedCols = nFixedCols;

    Refresh();

    return TRUE;
}

BOOL CGridCtrl::SetRowCount(int nRows)
{
    BOOL bResult = TRUE;

    ASSERT(nRows >= 0);
    if (nRows == GetRowCount())
        return bResult;

    // Force recalculation
    _idTopLeftCell.col = -1;

    if (nRows < _nFixedRows)
        _nFixedRows = nRows;

    if (_idCurrentCell.row >= nRows)
        SetFocusCell(-1, - 1);

    int addedRows = nRows - GetRowCount();

    // If we are about to lose rows, then we need to delete the GridCell objects
    // in each column within each row
    if (addedRows < 0)
    {
        if (!GetVirtualMode())
        {
            for (int row = nRows; row < _nRows; row++)
            {
                // Delete cells
                for (int col = 0; col < _nCols; col++)
                    DestroyCell(row, col);

                // Delete rows
                GridRow* pRow = _RowData[row];
                if (pRow)
                    delete pRow;
            }
        }
        _nRows = nRows;
    }

    try
    {
        _arRowHeights.resize(nRows);

        if (GetVirtualMode())
        {
            _nRows = nRows;
            if (addedRows > 0)
            {
                int startRow = nRows - addedRows;
                for (int row = startRow; row < nRows; row++)
                    _arRowHeights[row] = _cellDefault.GetHeight();
            }
        }
        else
        {
            // Change the number of rows.
            _RowData.resize(nRows);

            // If we have just added rows, we need to construct new elements for each cell
            // and set the default row height
            if (addedRows > 0)
            {
                // initialize row heights and data
                int startRow = nRows - addedRows;
                for (int row = startRow; row < nRows; row++)
                {
                    _arRowHeights[row] = _cellDefault.GetHeight();

                    _RowData[row] = new CGridRow;
                    _RowData[row]->resize(_nCols);
                    for (int col = 0; col < _nCols; col++)
                    {
                        GridRow* pRow = _RowData[row];
                        if (pRow && !GetVirtualMode())
                            (*pRow)[col] = CreateCell(row, col);
                    }
                    _nRows++;
                }
            }
        }
    }
    catch (...)
    {
        bResult = FALSE;
    }

    SetModified();
    ////ResetScrollBars();
    Refresh();

    return bResult;
}

BOOL CGridCtrl::SetColumnCount(int nCols)
{
    BOOL bResult = TRUE;

    ASSERT(nCols >= 0);

    if (nCols == GetColumnCount())
        return bResult;

    // Force recalculation
    _idTopLeftCell.col = -1;

    if (nCols < _nFixedCols)
        _nFixedCols = nCols;

    if (_idCurrentCell.col >= nCols)
        SetFocusCell(-1, - 1);

    int addedCols = nCols - GetColumnCount();

    // If we are about to lose columns, then we need to delete the GridCell objects
    // within each column
    if (addedCols < 0 && !GetVirtualMode())
    {
        for (int row = 0; row < _nRows; row++)
            for (int col = nCols; col < GetColumnCount(); col++)
                DestroyCell(row, col);
    }

    try
    {
        // Change the number of columns.
        _arColWidths.resize(nCols);

        // Change the number of columns in each row.
        if (!GetVirtualMode())
            for (int i = 0; i < _nRows; i++)
                if (_RowData[i])
                    _RowData[i]->resize(nCols);

        // If we have just added columns, we need to construct new elements for each cell
        // and set the default column width
        if (addedCols > 0)
        {
            // initialized column widths
            int startCol = nCols - addedCols;
            for (int col = startCol; col < nCols; col++)
                _arColWidths[col] = _cellFixedColDef.GetWidth();

            // initialise column data
            if (!GetVirtualMode())
            {
                for (int row = 0; row < _nRows; row++)
                    for (int col = startCol; col < nCols; col++)
                    {
                        GridRow* pRow = _RowData[row];
                        if (pRow)
                            (*pRow)[col] = CreateCell(row, col);
                    }
            }
        }
        // else    // check for selected cell ranges
        //    ResetSelectedRange();
    }
    catch (...)
    {
        bResult = FALSE;
    }

    _nCols = nCols;

    SetModified();
    ////ResetScrollBars();
    Refresh();

    return bResult;
}

// Insert a column at a given position, or add to end of columns (if nColumn = -1)
int CGridCtrl::InsertColumn(LPCTSTR strHeading,
                            UINT nFormat /* = DT_CENTER|DT_VCENTER|DT_SINGLELINE */,
                            int nColumn  /* = -1 */)
{
    if (nColumn >= 0 && nColumn < _nFixedCols)
    {

        // TODO: Fix it so column insertion works for in the fixed column area
        
        return -1;
    }

    // If the insertion is for a specific column, check it's within range.
    if (nColumn >= 0 && nColumn > GetColumnCount())
        return -1;

    // Force recalculation
    _idTopLeftCell.col = -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the column.
    if (_nRows < 1)
        SetRowCount(1);

    // Allow the user to insert after the last of the columns, but process it as a
    // "-1" column, meaning it gets flaged as being the last column, and not a regular
    // "insert" routine.
    if (nColumn == GetColumnCount())
        nColumn = -1;

    try
    {
        if (nColumn < 0)
        {
            nColumn = _nCols;
            _arColWidths.push_back(0);
            if (!GetVirtualMode())
            {
                for (int row = 0; row < _nRows; row++)
                {
                    GridRow* pRow = _RowData[row];
                    if (!pRow)
                        return -1;
                    pRow->push_back(CreateCell(row, nColumn));
                }
            }
        }
        else
        {
            _arColWidths.InsertAt(nColumn, (UINT)0);
            if (!GetVirtualMode())
            {
                for (int row = 0; row < _nRows; row++)
                {
                    CGridRow* pRow = _RowData[row];
                    if (!pRow)
                        return -1;

                    pRow->InsertAt(nColumn, CreateCell(row, nColumn));
                }
            }
        }
    }
    catch (...)
    {
        return FALSE;
    }

    _nCols++;

    // Initialise column data
    SetItemText(0, nColumn, strHeading);
    for (int row = 0; row < _nRows; row++)
        SetItemFormat(row, nColumn, nFormat);

    // initialized column width
    _arColWidths[nColumn] = GetTextExtent(0, nColumn, strHeading).width;

    if (_idCurrentCell.col != -1 && nColumn < _idCurrentCell.col)
        _idCurrentCell.col++;

    ////ResetScrollBars();

    SetModified();

    return nColumn;
}

// Insert a row at a given position, or add to end of rows (if nRow = -1)
int CGridCtrl::InsertRow(LPCTSTR strHeading, int nRow /* = -1 */)
{
    if (nRow >= 0 && nRow < _nFixedRows)
    {
        // TODO: Fix it so column insertion works for in the fixed row area
        
        return -1;
    }

    // If the insertion is for a specific row, check it's within range.
    if (nRow >= 0 && nRow >= GetRowCount())
        return -1;

    // Force recalculation
    _idTopLeftCell.col = -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the row.
    if (_nCols < 1)
        SetColumnCount(1);

    try
    {
        // Adding a row to the bottom
        if (nRow < 0)
        {
            nRow = _nRows;
            _arRowHeights.push_back(0);
            if (!GetVirtualMode())
                _RowData.push_back(new CGridRow);
        }
        else
        {
            _arRowHeights.InsertAt(nRow, (UINT)0);
            if (!GetVirtualMode())
                _RowData.InsertAt(nRow, new CGridRow);
        }

        if (!GetVirtualMode())
            _RowData[nRow]->resize(_nCols);
    }
    catch(...)
    {
        return FALSE;
    }


    _nRows++;

    // Initialise cell data
    if (!GetVirtualMode())
    {
        for (int col = 0; col < _nCols; col++)
        {
            GridRow* pRow = _RowData[nRow];
            if (!pRow)
                return -1;
            (*pRow)[col] = CreateCell(nRow, col);
        }
    }

    // Set row title
    SetItemText(nRow, 0, strHeading);

    // initialized row height
    if (strHeading && strHeading[0])
        _arRowHeights[nRow] = GetTextExtent(nRow, 0, strHeading).height;
    else
        _arRowHeights[nRow] = _cellFixedRowDef.GetHeight();

    if (_idCurrentCell.row != -1 && nRow < _idCurrentCell.row)
        _idCurrentCell.row++;

    ////ResetScrollBars();

    SetModified();

    return nRow;
}

///////////////////////////////////////////////////////////////////////////////
// Cell creation stuff

BOOL CGridCtrl::SetCellType(int nRow, int nCol)
{
    if (GetVirtualMode())
        return FALSE;

    if (!IsValid(nRow, nCol))
        return FALSE;

    CGridCellBase* pNewCell = (CGridCellBase*)new CGridCell;

    CGridCellBase* pCurrCell = GetCell(nRow, nCol);
    if (pCurrCell)
        *pNewCell = *pCurrCell;

    SetCell(nRow, nCol, pNewCell);
    delete pCurrCell;

    return TRUE;
}

BOOL CGridCtrl::SetDefaultCellType()
{
    return TRUE;
}

// Creates a new grid cell and performs any necessary initialisation
/*virtual*/ CGridCellBase* CGridCtrl::CreateCell(int nRow, int nCol)
{
    GetVirtualMode();

    CGridCellBase* pCell = (CGridCellBase*)new CGridCell;
    if (!pCell)
        return NULL;

    pCell->SetGrid(this);
    pCell->SetCoords(nRow, nCol);

    if (nCol < _nFixedCols)
        pCell->SetState(pCell->GetState() | GVIS_FIXED | GVIS_FIXEDCOL);
    if (nRow < _nFixedRows)
        pCell->SetState(pCell->GetState() | GVIS_FIXED | GVIS_FIXEDROW);

    pCell->SetFormat(pCell->GetDefaultCell()->GetFormat());

    return pCell;
}

// Performs any cell cleanup necessary to maintain grid integrity
/*virtual*/ void CGridCtrl::DestroyCell(int nRow, int nCol)
{
    // Should NEVER get here in virtual mode.
    GetVirtualMode();

    // Set the cells state to 0. If the cell is selected, this
    // will remove the cell from the selected list.
    SetItemState(nRow, nCol, 0);

    delete GetCell(nRow, nCol);
}

BOOL CGridCtrl::DeleteColumn(int nColumn)
{
    if (nColumn < 0 || nColumn >= GetColumnCount())
        return FALSE;

    ResetSelectedRange();

    if (!GetVirtualMode())
    {
        for (int row = 0; row < GetRowCount(); row++)
        {
            CGridRow* pRow = _RowData[row];
            if (!pRow)
                return FALSE;

            DestroyCell(row, nColumn);

            pRow->RemoveAt(nColumn);
        }
    }
    _arColWidths.RemoveAt(nColumn);
    _nCols--;
    if (nColumn < _nFixedCols)
        _nFixedCols--;

    if (nColumn == _idCurrentCell.col)
        _idCurrentCell.row = _idCurrentCell.col = -1;
    else if (nColumn < _idCurrentCell.col)
        _idCurrentCell.col--;

    ////ResetScrollBars();

    SetModified();

    return TRUE;
}

BOOL CGridCtrl::DeleteRow(int nRow)
{
    if (nRow < 0 || nRow >= GetRowCount())
        return FALSE;

    ResetSelectedRange();

    if (!GetVirtualMode())
    {
        GridRow* pRow = _RowData[nRow];
        if (!pRow)
            return FALSE;

        for (int col = 0; col < GetColumnCount(); col++)
            DestroyCell(nRow, col);

        delete pRow;
        _RowData.RemoveAt(nRow);
    }

    _arRowHeights.RemoveAt(nRow);

    _nRows--;
    if (nRow < _nFixedRows)
        _nFixedRows--;

    if (nRow == _idCurrentCell.row)
        _idCurrentCell.row = _idCurrentCell.col = -1;
    else if (nRow < _idCurrentCell.row)
        _idCurrentCell.row--;

    ////ResetScrollBars();

    SetModified();

    return TRUE;
}

// Handy function that removes all non-fixed rows
BOOL CGridCtrl::DeleteNonFixedRows()
{
    ResetSelectedRange();

    int nFixed = GetFixedRowCount();
    int nCount = GetRowCount();

    // Delete all data rows
    for (int nRow = nCount; nRow >= nFixed; nRow--)
        DeleteRow(nRow);

    return TRUE;
}

// Removes all rows, columns and data from the grid.
BOOL CGridCtrl::DeleteAllItems()
{
    ResetSelectedRange();

    _arColWidths.clear();
    _arRowHeights.clear();

    // Delete all cells in the grid
    if (!GetVirtualMode())
    {
        for (int row = 0; row < _nRows; row++)
        {
            for (int col = 0; col < _nCols; col++)
                DestroyCell(row, col);

            GridRow* pRow = _RowData[row];
            delete pRow;
        }

        // Remove all rows
        _RowData.clear();
    }

    _idCurrentCell.row = _idCurrentCell.col = -1;
    _nRows = _nFixedRows = _nCols = _nFixedCols = 0;

    ////ResetScrollBars();

    SetModified();

    return TRUE;
}

void CGridCtrl::AutoFill()
{
    if (!::IsWindow(0))
        return;

    RECT rect;
    ////GetClientRect(&rect);

    SetColumnCount(RectWidth(rect) / _cellDefault.GetWidth() + 1);
    SetRowCount(RectHeight(rect) / _cellDefault.GetHeight() + 1);
    SetFixedRowCount(1);
    SetFixedColumnCount(1);
    ExpandToFit();
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

// Set CListCtrl::GetNextItem for details
CCellID CGridCtrl::GetNextItem(CCellID& cell, int nFlags) const
{
    if ((nFlags & GVNI_ALL) == GVNI_ALL)
    {    // GVNI_ALL Search whole Grid beginning from cell
        //          First row (cell.row) -- ONLY Columns to the right of cell
        //          following rows       -- ALL  Columns
        int row = cell.row , col = cell.col + 1;
        if (row <= 0)
            row = GetFixedRowCount();
        for (; row < GetRowCount(); row++)
        {
            if (col <= 0)
                col = GetFixedColumnCount();
            for (; col < GetColumnCount(); col++)
            {
                int nState = GetItemState(row, col);
                if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) ||
                    (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                    (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                    (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                    (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                    (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                    return CCellID(row, col);
            }
            // go to First Column
            col = GetFixedColumnCount();
        }
    }
    else if ((nFlags & GVNI_BELOW) == GVNI_BELOW &&
             (nFlags & GVNI_TORIGHT) == GVNI_TORIGHT)
    {   // GVNI_AREA Search Grid beginning from cell to Lower-Right of Grid
        //           Only rows starting with  cell.row and below
        //           All rows   -- ONLY Columns to the right of cell
        int row = cell.row;
        if (row <= 0)
            row = GetFixedRowCount();
        for (; row < GetRowCount(); row++)
        {
            int col = cell.col + 1;
            if (col <= 0)
                col = GetFixedColumnCount();
            for (; col < GetColumnCount(); col++)
            {
                int nState = GetItemState(row, col);
                if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) ||
                    (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                    (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                    (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                    (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                    (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                    return CCellID(row, col);
            }
        }
    }
    else if ((nFlags & GVNI_ABOVE) == GVNI_ABOVE)
    {
        for (int row = cell.row - 1; row >= GetFixedRowCount(); row--)
        {
            int nState = GetItemState(row, cell.col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) ||
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(row, cell.col);
        }
    }
    else if ((nFlags & GVNI_BELOW) == GVNI_BELOW)
    {
        for (int row = cell.row + 1; row < GetRowCount(); row++)
        {
            int nState = GetItemState(row, cell.col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) ||
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(row, cell.col);
        }
    }
    else if ((nFlags & GVNI_TOLEFT) == GVNI_TOLEFT)
    {
        for (int col = cell.col - 1; col >= GetFixedColumnCount(); col--)
        {
            int nState = GetItemState(cell.row, col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) ||
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(cell.row, col);
        }
    }
    else if ((nFlags & GVNI_TORIGHT) == GVNI_TORIGHT)
    {
        for (int col = cell.col + 1; col < GetColumnCount(); col++)
        {
            int nState = GetItemState(cell.row, col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) ||
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(cell.row, col);
        }
    }

    return CCellID(-1, -1);
}

// Sorts on a given column using the cell text
BOOL CGridCtrl::SortTextItems(int nCol, BOOL bAscending, LPARAM data /* = 0 */)
{
    SetSortColumn(nCol);
    SetSortAscending(bAscending);
    ResetSelectedRange();
    SetFocusCell(-1, - 1);
    return CGridCtrl::SortItems(pfnCellTextCompare, nCol, bAscending, data);
}

void CGridCtrl::SetCompareFunction(PFNLVCOMPARE pfnCompare)
{
	_pfnCompare = pfnCompare;
}

// Sorts on a given column using the cell text and using the specified comparison
// function
BOOL CGridCtrl::SortItems(int nCol, BOOL bAscending, LPARAM data /* = 0 */)
{
    SetSortColumn(nCol);
    SetSortAscending(bAscending);
    ResetSelectedRange();
    SetFocusCell(-1, - 1);

	if (_pfnCompare == NULL)
		return CGridCtrl::SortItems(pfnCellTextCompare, nCol, bAscending, data);
	else
	    return CGridCtrl::SortItems(_pfnCompare, nCol, bAscending, data);
}

// Sorts on a given column using the supplied compare function (see CListCtrl::SortItems)
BOOL CGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending,
                          LPARAM data /* = 0 */)
{
    SetSortColumn(nCol);
    SetSortAscending(bAscending);
    ResetSelectedRange();
    SetFocusCell(-1, -1);
    return SortItems(pfnCompare, nCol, bAscending, data, GetFixedRowCount(), -1);
}

int CALLBACK CGridCtrl::pfnCellTextCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGridCellBase* pCell1 = (CGridCellBase*) lParam1;
	CGridCellBase* pCell2 = (CGridCellBase*) lParam2;
	if (!pCell1 || !pCell2) return 0;

	return _tcscmp(pCell1->GetText(), pCell2->GetText());
}

int CALLBACK CGridCtrl::pfnCellNumericCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGridCellBase* pCell1 = (CGridCellBase*) lParam1;
	CGridCellBase* pCell2 = (CGridCellBase*) lParam2;
	if (!pCell1 || !pCell2) return 0;

	int nValue1 = _ttol(pCell1->GetText());
	int nValue2 = _ttol(pCell2->GetText());

	if (nValue1 < nValue2)
		return -1;
	else if (nValue1 == nValue2)
		return 0;
	else
		return 1;
}

// private recursive sort implementation
BOOL CGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data,
                          int low, int high)
{
    if (nCol >= GetColumnCount())
        return FALSE;

    if (high == -1)
        high = GetRowCount() - 1;

    int lo = low;
    int hi = high;

    if (hi <= lo)
        return FALSE;

    //LPARAM midItem = GetItemData((lo + hi)/2, nCol);
	LPARAM pMidCell = (LPARAM) GetCell((lo + hi)/2, nCol);

    // loop through the list until indices cross
    while (lo <= hi)
    {
        // Find the first element that is greater than or equal to the partition
        // element starting from the left Index.
        if (bAscending)
            while (lo < high  && pfnCompare( (LPARAM)GetCell(lo, nCol), (LPARAM) pMidCell, data) < 0)
                ++lo;
        else
            while (lo < high && pfnCompare((LPARAM)GetCell(lo, nCol), pMidCell, data) > 0)
				++lo;

		// Find an element that is smaller than or equal to  the partition
		// element starting from the right Index.
		if (bAscending)
			while (hi > low && pfnCompare((LPARAM)GetCell(hi, nCol), pMidCell, data) > 0)
				--hi;
		else
			while (hi > low && pfnCompare((LPARAM)GetCell(hi, nCol), pMidCell, data) < 0)
				--hi;

        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
			// swap only if the items are not equal
            if (pfnCompare((LPARAM)GetCell(lo, nCol), (LPARAM)GetCell(hi, nCol), data) != 0)
            {
				for (int col = 0; col < GetColumnCount(); col++)
                {
					CGridCellBase *pCell = GetCell(lo, col);
                    SetCell(lo, col, GetCell(hi, col));
                    SetCell(hi, col, pCell);
                }
                UINT nRowHeight = _arRowHeights[lo];
                _arRowHeights[lo] = _arRowHeights[hi];
                _arRowHeights[hi] = nRowHeight;
            }

            ++lo;
            --hi;
         }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if (low < hi)
        SortItems(pfnCompare, nCol, bAscending, data, low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if (lo < high)
        SortItems(pfnCompare, nCol, bAscending, data, lo, high);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

BOOL CGridCtrl::SetItem(const GV_ITEM* pItem)
{
    if (!pItem || GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(pItem->row, pItem->col);
    if (!pCell)
        return FALSE;

    SetModified(TRUE, pItem->row, pItem->col);

    if (pItem->mask & GVIF_TEXT)
        pCell->SetText(pItem->strText.c_str());
    if (pItem->mask & GVIF_PARAM)
        pCell->SetData(pItem->lParam);
    if (pItem->mask & GVIF_IMAGE)
        pCell->SetImage(pItem->iImage);
    if (pItem->mask & GVIF_STATE)
        SetItemState(pItem->row, pItem->col, pItem->nState);
    if (pItem->mask & GVIF_FORMAT)
        pCell->SetFormat(pItem->nFormat);
    if (pItem->mask & GVIF_BKCLR)
        pCell->SetBackClr(pItem->crBkClr);
    if (pItem->mask & GVIF_FGCLR)
        pCell->SetTextClr(pItem->crFgClr);
    if (pItem->mask & GVIF_FONT)
        pCell->SetFont(pItem->lfFont);
    if( pItem->mask & GVIF_MARGIN)
        pCell->SetMargin( pItem->nMargin);

    return TRUE;
}

BOOL CGridCtrl::GetItem(GV_ITEM* pItem)
{
    if (!pItem)
        return FALSE;
    CGridCellBase* pCell = GetCell(pItem->row, pItem->col);
    if (!pCell)
        return FALSE;

    if (pItem->mask & GVIF_TEXT)
        pItem->strText = GetItemText(pItem->row, pItem->col);
    if (pItem->mask & GVIF_PARAM)
        pItem->lParam  = pCell->GetData();;
    if (pItem->mask & GVIF_IMAGE)
        pItem->iImage  = pCell->GetImage();
    if (pItem->mask & GVIF_STATE)
        pItem->nState  = pCell->GetState();
    if (pItem->mask & GVIF_FORMAT)
        pItem->nFormat = pCell->GetFormat();
    if (pItem->mask & GVIF_BKCLR)
        pItem->crBkClr = pCell->GetBackClr();
    if (pItem->mask & GVIF_FGCLR)
        pItem->crFgClr = pCell->GetTextClr();
    if (pItem->mask & GVIF_FONT)
        memcpy(&(pItem->lfFont), pCell->GetFont(), sizeof(LOGFONT));
    if( pItem->mask & GVIF_MARGIN)
        pItem->nMargin = pCell->GetMargin();

    return TRUE;
}

BOOL CGridCtrl::SetItemText(int nRow, int nCol, LPCTSTR str)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    pCell->SetText(str);

    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

#if !defined(_WIN32_WCE) || (_WIN32_WCE >= 210)
// EFW - 06/13/99 - Added to support printf-style formatting codes
BOOL CGridCtrl::SetItemTextFmt(int nRow, int nCol, LPCTSTR szFmt, ...)
{
    if (GetVirtualMode())
        return FALSE;

    tstring strText;

    va_list argptr;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    // Format the message text
    va_start(argptr, szFmt);
    ////trText.FormatV(szFmt, argptr);
    va_end(argptr);

    pCell->SetText(strText.c_str());

    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

// EFW - 06/13/99 - Added to support string resource ID.  Supports
// a variable argument list too.
BOOL CGridCtrl::SetItemTextFmtID(int nRow, int nCol, UINT nID, ...)
{
    if (GetVirtualMode())
        return FALSE;

    tstring strFmt, strText;
    va_list argptr;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    // Format the message text
    va_start(argptr, nID);
    ////strFmt.LoadString(nID);
    ////strText.FormatV(strFmt, argptr);
    va_end(argptr);

    pCell->SetText(strText.c_str());

    SetModified(TRUE, nRow, nCol);
    return TRUE;
}
#endif

BOOL CGridCtrl::SetItemData(int nRow, int nCol, LPARAM lParam)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    pCell->SetData(lParam);
    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

LPARAM CGridCtrl::GetItemData(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return (LPARAM) 0;

    return pCell->GetData();
}

BOOL CGridCtrl::SetItemImage(int nRow, int nCol, int iImage)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    pCell->SetImage(iImage);
    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

int CGridCtrl::GetItemImage(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return -1;

    return pCell->GetImage();
}

BOOL CGridCtrl::SetItemState(int nRow, int nCol, UINT state)
{
    BOOL bSelected = IsCellSelected(nRow, nCol);

    // If the cell is being unselected, remove it from the selected list
    if (bSelected && !(state & GVIS_SELECTED))
    {
        _SelectedCellMap.Remove(nRow, nCol);
    }

    // If cell is being selected, add it to the list of selected cells
    else if (!bSelected && (state & GVIS_SELECTED))
    {
        CCellID cell(nRow, nCol);
        _SelectedCellMap.Add(&cell);
    }

    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    // Set the cell's state
    pCell->SetState(state);

    return TRUE;
}

UINT CGridCtrl::GetItemState(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->GetState();
}

BOOL CGridCtrl::SetItemFormat(int nRow, int nCol, UINT nFormat)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    pCell->SetFormat(nFormat);
    return TRUE;
}

UINT CGridCtrl::GetItemFormat(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->GetFormat();
}

BOOL CGridCtrl::SetItemBkColour(int nRow, int nCol, COLORALPHA   cr /* = DEFAULT_COLOR_ALPHA */)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    pCell->SetBackClr(cr);
    return TRUE;
}

COLORALPHA CGridCtrl::GetItemBkColour(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return DEFAULT_COLOR_ALPHA;

    return pCell->GetBackClr();
}

BOOL CGridCtrl::SetItemFgColour(int nRow, int nCol, COLORALPHA   cr /* = DEFAULT_COLOR_ALPHA */)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    pCell->SetTextClr(cr);
    return TRUE;
}

COLORALPHA CGridCtrl::GetItemFgColour(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return DEFAULT_COLOR_ALPHA;

    return pCell->GetTextClr();
}

BOOL CGridCtrl::SetItemFont(int nRow, int nCol, const IDWriteTextFormat* plf)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    pCell->SetFont(plf);

    return TRUE;
}

const IDWriteTextFormat* CGridCtrl::GetItemFont(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return GetDefaultCell(nRow < GetFixedRowCount(), nCol < GetFixedColumnCount())->GetFont();

    return pCell->GetFont();
}

BOOL CGridCtrl::IsItemEditing(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    return pCell->IsEditing();
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column size functions

long CGridCtrl::GetVirtualWidth() const
{
    long lVirtualWidth = 0;
    int iColCount = GetColumnCount();
    for (int i = 0; i < iColCount; i++)
        lVirtualWidth += _arColWidths[i];

    return lVirtualWidth;
}

long CGridCtrl::GetVirtualHeight() const
{
    long lVirtualHeight = 0;
    int iRowCount = GetRowCount();
    for (int i = 0; i < iRowCount; i++)
        lVirtualHeight += _arRowHeights[i];

    return lVirtualHeight;
}

int CGridCtrl::GetRowHeight(int nRow) const
{
    ASSERT(nRow >= 0 && nRow < _nRows);
    if (nRow < 0 || nRow >= _nRows)
        return -1;

    return _arRowHeights[nRow];
}

int CGridCtrl::GetColumnWidth(int nCol) const
{
    ASSERT(nCol >= 0 && nCol < _nCols);
    if (nCol < 0 || nCol >= _nCols)
        return -1;

    return _arColWidths[nCol];
}

BOOL CGridCtrl::SetRowHeight(int nRow, int height)
{
    ASSERT(nRow >= 0 && nRow < _nRows && height >= 0);
    if (nRow < 0 || nRow >= _nRows || height < 0)
        return FALSE;

    _arRowHeights[nRow] = height;
    ////ResetScrollBars();

    return TRUE;
}

BOOL CGridCtrl::SetColumnWidth(int nCol, int width)
{
    ASSERT(nCol >= 0 && nCol < _nCols && width >= 0);
    if (nCol < 0 || nCol >= _nCols || width < 0)
        return FALSE;

    _arColWidths[nCol] = width;
    ////ResetScrollBars();

    return TRUE;
}

int CGridCtrl::GetFixedRowHeight() const
{
    int nHeight = _margin.top;
    for (int i = 0; i < _nFixedRows; i++)
        nHeight += GetRowHeight(i);

    return nHeight;
}

int CGridCtrl::GetFixedColumnWidth() const
{
    int nWidth = _margin.left;
    for (int i = 0; i < _nFixedCols; i++)
        nWidth += GetColumnWidth(i);

    return nWidth;
}

BOOL CGridCtrl::AutoSizeColumn(int nCol, UINT nAutoSizeStyle /*=GVS_DEFAULT*/,
                               BOOL bResetScroll /*=TRUE*/)
{
    /****
    ASSERT(nCol >= 0 && nCol < _nCols);
    if (nCol < 0 || nCol >= _nCols)
        return FALSE;

    //  Skip hidden columns when autosizing
    if( GetColumnWidth( nCol) <=0 )
        return FALSE;

    SIZE_u size;
    CDC* pDC = GetDC();
    if (!pDC)
        return FALSE;

    int nWidth = 0;

    ASSERT(GVS_DEFAULT <= nAutoSizeStyle && nAutoSizeStyle <= GVS_BOTH);
    if (nAutoSizeStyle == GVS_DEFAULT)
        nAutoSizeStyle = GetAutoSizeStyle();

    int nStartRow = (nAutoSizeStyle & GVS_HEADER)? 0 : GetFixedRowCount();
    int nEndRow   = (nAutoSizeStyle & GVS_DATA)? GetRowCount()-1 : GetFixedRowCount()-1;

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(nStartRow, nCol, nEndRow, nCol));

    for (int nRow = nStartRow; nRow <= nEndRow; nRow++)
    {
        CGridCellBase* pCell = GetCell(nRow, nCol);
        if (pCell)
            size = pCell->GetCellExtent(pDC);
        if (size.cx > nWidth)
            nWidth = size.cx;
    }

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    _arColWidths[nCol] = nWidth;

    ReleaseDC(pDC);
    if (bResetScroll)
        ////ResetScrollBars();
    ****/
    return TRUE;
}

BOOL CGridCtrl::AutoSizeRow(int nRow, BOOL bResetScroll /*=TRUE*/)
{
    /****
    ASSERT(nRow >= 0 && nRow < _nRows);
    if (nRow < 0 || nRow >= _nRows)
        return FALSE;

    //  Skip hidden rows when autosizing
    if( GetRowHeight( nRow) <=0 )
        return FALSE;

    SIZE_u size;
    CDC* pDC = GetDC();
    if (!pDC)
        return FALSE;

    int nHeight = 0;
    int nNumColumns = GetColumnCount();

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(nRow, 0, nRow, nNumColumns));

    for (int nCol = 0; nCol < nNumColumns; nCol++)
    {
        CGridCellBase* pCell = GetCell(nRow, nCol);
        if (pCell)
            size = pCell->GetCellExtent(pDC);
        if (size.cy > nHeight)
            nHeight = size.cy;
    }
    _arRowHeights[nRow] = nHeight;

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    ReleaseDC(pDC);
    if (bResetScroll)
        ////ResetScrollBars();
    ****/
    return TRUE;
}

void CGridCtrl::AutoSizeColumns(UINT nAutoSizeStyle /*=GVS_DEFAULT*/)
{
    int nNumColumns = GetColumnCount();
    for (int nCol = 0; nCol < nNumColumns; nCol++)
    {
        //  Skip hidden columns when autosizing
        if( GetColumnWidth( nCol) > 0 )
            AutoSizeColumn(nCol, nAutoSizeStyle, FALSE);
    }
    ////ResetScrollBars();
}

void CGridCtrl::AutoSizeRows()
{
    int nNumRows = GetRowCount();
    for (int nRow = 0; nRow < nNumRows; nRow++)
    {
        //  Skip hidden rows when autosizing
        if( GetRowHeight( nRow) > 0 )
            AutoSizeRow(nRow, FALSE);
    }
    ////ResetScrollBars();
}

// sizes all rows and columns
// faster than calling both AutoSizeColumns() and AutoSizeRows()
void CGridCtrl::AutoSize(UINT nAutoSizeStyle /*=GVS_DEFAULT*/)
{
    /****
    CDC* pDC = GetDC();
    if (!pDC)
        return;

    int nNumColumns = GetColumnCount();

    int nCol, nRow;

    ASSERT(GVS_DEFAULT <= nAutoSizeStyle && nAutoSizeStyle <= GVS_BOTH);
    if (nAutoSizeStyle == GVS_DEFAULT)
        nAutoSizeStyle = GetAutoSizeStyle();

    int nStartRow = (nAutoSizeStyle & GVS_HEADER)? 0 : GetFixedRowCount();
    int nEndRow   = (nAutoSizeStyle & GVS_DATA)? GetRowCount()-1 : GetFixedRowCount()-1;

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(nStartRow, 0, nEndRow, nNumColumns));

    // Row initialisation - only work on rows whose height is > 0
    for (nRow = nStartRow; nRow <= nEndRow; nRow++)
    {
        if( GetRowHeight( nRow) > 0 )
            _arRowHeights[nRow] = 1;
    }

    SIZE_u size;
    for (nCol = 0; nCol < nNumColumns; nCol++)
    {
        //  Don't size hidden columns or rows
        if( GetColumnWidth( nCol) > 0 )
        {
            // Skip columns that are hidden, but now initialize
            _arColWidths[nCol] = 0;
            for (nRow = nStartRow; nRow <= nEndRow; nRow++)
            {
                if( GetRowHeight( nRow) > 0 )
                {
                    CGridCellBase* pCell = GetCell(nRow, nCol);
                    if (pCell)
                        size = pCell->GetCellExtent(pDC);
                    if (size.cx >(int) _arColWidths[nCol])
                        _arColWidths[nCol] = size.cx;
                    if (size.cy >(int) _arRowHeights[nRow])
                        _arRowHeights[nRow] = size.cy;
                }
            }
        }
    }

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    ReleaseDC(pDC);
    ****/
    ////ResetScrollBars();
    Refresh();
}

// Expands the columns to fit the screen space. If bExpandFixed is FALSE then fixed
// columns will not be affected
void CGridCtrl::ExpandColumnsToFit(BOOL bExpandFixed /*=TRUE*/)
{
    if (bExpandFixed)
	{
		if (GetColumnCount() <= 0) return;
	}
	else
	{
		if (GetColumnCount() <= GetFixedColumnCount()) return;
	}

    ////EnableScrollBars(SB_HORZ, FALSE);

    RECT rect;
    ////GetClientRect(&rect);

    int nFirstColumn = (bExpandFixed)? 0 : GetFixedColumnCount();

    int nNumColumnsAffected = 0;
    int col;
    for (col = nFirstColumn; col < GetColumnCount(); col++)
    {
        if (_arColWidths[col] > 0)
            nNumColumnsAffected++;
    }

    if (nNumColumnsAffected <= 0)
        return;

    long virtualWidth = GetVirtualWidth();
    int nDifference = RectWidth(rect) -(int) virtualWidth;
    int nColumnAdjustment = nDifference / nNumColumnsAffected;

    for (col = nFirstColumn; col < GetColumnCount(); col++)
    {
        if (_arColWidths[col] > 0)
            _arColWidths[col] += nColumnAdjustment;
    }

    if (nDifference > 0)
    {
        int leftOver = nDifference % nNumColumnsAffected;
        for (int nCount = 0, col = nFirstColumn;
             (col < GetColumnCount()) && (nCount < leftOver); col++, nCount++)
        {
            if (_arColWidths[col] > 0)
                _arColWidths[col] += 1;
        }
    }
    else
    {
        int leftOver = (-nDifference) % nNumColumnsAffected;
        for (int nCount = 0, col = nFirstColumn;
             (col < GetColumnCount()) && (nCount < leftOver); col++, nCount++)
        {
            if (_arColWidths[col] > 0)
                _arColWidths[col] -= 1;
        }
    }

    Refresh();

    ////ResetScrollBars();
}

void CGridCtrl::ExpandLastColumn()
{
    if (GetColumnCount() <= 0)
        return;

    // Search for last non-hidden column
    int nLastColumn = GetColumnCount()-1;
    while (_arColWidths[nLastColumn] <= 0)
        nLastColumn--;

    if (nLastColumn <= 0)
        return;

    ////EnableScrollBars(SB_HORZ, FALSE);

    RECT rect;
    ////GetClientRect(&rect);

    long virtualWidth = GetVirtualWidth();
    int nDifference = RectWidth(rect) -(int) virtualWidth;

    if (nDifference > 0)
    {
        //if (GetVirtualHeight() > RectHeight(rect))
        //    nDifference -= GetSystemMetrics(SM_CXVSCROLL);

        _arColWidths[ nLastColumn ] += nDifference;
        Refresh();
    }

    ////ResetScrollBars();
}

// Expands the rows to fit the screen space. If bExpandFixed is FALSE then fixed
// rows will not be affected
void CGridCtrl::ExpandRowsToFit(BOOL bExpandFixed /*=TRUE*/)
{
    if (bExpandFixed)
	{
		if (GetRowCount() <= 0) return;
	}
	else
	{
		if (GetRowCount() <= GetFixedRowCount()) return;
	}

    ////EnableScrollBars(SB_VERT, FALSE);

    RECT rect;
    ////GetClientRect(&rect);

    int nFirstRow = (bExpandFixed)? 0 : GetFixedRowCount();

    int nNumRowsAffected = 0;
    int row;
    for (row = nFirstRow; row < GetRowCount(); row++)
    {
        if (_arRowHeights[row] > 0)
            nNumRowsAffected++;
    }

    if (nNumRowsAffected <= 0)
        return;

    long virtualHeight = GetVirtualHeight();
    int nDifference = RectHeight(rect) -(int) virtualHeight;
    int nRowAdjustment = nDifference / nNumRowsAffected;

    for (row = nFirstRow; row < GetRowCount(); row++)
    {
        if (_arRowHeights[row] > 0)
            _arRowHeights[row] += nRowAdjustment;
    }

    if (nDifference > 0)
    {
        int leftOver = nDifference % nNumRowsAffected;
        for (int nCount = 0, row = nFirstRow;
             (row < GetRowCount()) && (nCount < leftOver); row++, nCount++)
        {
            if (_arRowHeights[row] > 0)
                _arRowHeights[row] += 1;
        }
    }
    else
    {
        int leftOver = (-nDifference) % nNumRowsAffected;
        for (int nCount = 0, row = nFirstRow;
             (row < GetRowCount()) && (nCount < leftOver); row++, nCount++)
        {
            if (_arRowHeights[row] > 0)
                _arRowHeights[row] -= 1;
        }
    }

    Refresh();

    ////ResetScrollBars();
}

// Expands the cells to fit the screen space. If bExpandFixed is FALSE then fixed
// cells  will not be affected
void CGridCtrl::ExpandToFit(BOOL bExpandFixed /*=TRUE*/)
{
    ExpandColumnsToFit(bExpandFixed);   // This will remove any existing horz scrollbar
    ExpandRowsToFit(bExpandFixed);      // This will remove any existing vert scrollbar
    ExpandColumnsToFit(bExpandFixed);   // Just in case the first adjustment was with a vert
                                        // scrollbar in place
    Refresh();
}

/////////////////////////////////////////////////////////////////////////////////////
// Attributes

void CGridCtrl::SetVirtualMode(BOOL bVirtual)
{
    DeleteAllItems();
    _bVirtualMode = bVirtual;

    // Force some defaults here.
    if (_bVirtualMode)
    {
        SetEditable(FALSE);
        SetHeaderSort(FALSE);
        SetAutoSizeStyle(GVS_HEADER);
        SetFixedColumnSelection(FALSE);
        SetFixedRowSelection(FALSE);
    }
}

void CGridCtrl::SetGridLines(int nWhichLines /*=GVL_BOTH*/)
{
    _nGridLines = nWhichLines;
    Refresh();
}

void CGridCtrl::SetListMode(BOOL bEnableListMode /*=TRUE*/)
{
    ResetSelectedRange();
    SetSortColumn(-1);
    _bListMode = bEnableListMode;
    SetFixedRowSelection(FALSE);
    Refresh();
}

void CGridCtrl::SetSortColumn(int nCol)
{
    if (_nSortColumn >= 0)
        InvalidateCellRect(0, _nSortColumn);
    _nSortColumn = nCol;
    if (nCol >= 0)
        InvalidateCellRect(0, nCol);
}

BOOL CGridCtrl::IsCellFixed(int nRow, int nCol)
{
    return (nRow < GetFixedRowCount() || nCol < GetFixedColumnCount());
}

void CGridCtrl::SetModified(BOOL bModified /*=TRUE*/, int nRow /*=-1*/, int nCol /*=-1*/)
{
    // Cannot guarantee sorting anymore...
    if (nCol < 0 || nCol == GetSortColumn())
        SetSortColumn(-1);

    if (nRow >= 0 && nCol >= 0)
    {
        if (bModified)
        {
            SetItemState(nRow, nCol, GetItemState(nRow, nCol) | GVIS_MODIFIED);
            _bModified = TRUE;
        }
        else
            SetItemState(nRow, nCol, GetItemState(nRow, nCol) & ~GVIS_MODIFIED);
    }
    else
        _bModified = bModified;

    if (!_bModified)
    {
        for (int row = 0; row < GetRowCount(); row++)
            for (int col = 0; col < GetColumnCount(); col++)
                SetItemState(row, col, GetItemState(row, col) & ~GVIS_MODIFIED);
    }
}

BOOL CGridCtrl::GetModified(int nRow /*=-1*/, int nCol /*=-1*/)
{
    if (nRow >= 0 && nCol >= 0)
        return ( (GetItemState(nRow, nCol) & GVIS_MODIFIED) == GVIS_MODIFIED );
    else
        return _bModified;
}

/////////////////////////////////////////////////////////////////////////////////////
// GridCtrl cell visibility tests and invalidation/redraw functions

void CGridCtrl::Refresh()
{
    ////if (GetSafeHwnd() && _bAllowDraw)
        ////SEAN SEAN SEAN Invalidate();
}

// EnsureVisible supplied by Roelf Werkman
void CGridCtrl::EnsureVisible(int nRow, int nCol)
{
    if (!_bAllowDraw)
        return;

    RECT rectWindow;
    /*
    // set the scroll to the approximate position of row (Nigel Page-Jones)
    int nPos = (int)((float)nRow / GetRowCount() * 1000);
    float fPos = (float)nPos / 1000;
    SCROLLINFO scrollInfo;
    GetScrollInfo(SB_VERT, &scrollInfo);
    scrollInfo.nPos = (int)(scrollInfo.nMax * fPos);
    SetScrollInfo(SB_VERT, &scrollInfo, FALSE);

    GetClientRect(rectWindow);

    // redraw cells    if necessary (Nigel Page-Jones)
    CCellID idTopLeft = GetTopleftNonFixedCell(FALSE);
    CCellID idNewTopLeft = GetTopleftNonFixedCell(TRUE);
    if (idNewTopLeft != idTopLeft)
    {
        rectWindow.top = GetFixedRowHeight();
        InvalidateRect(rectWindow);
    }
    */

    // We are gonna send some scroll messages, which will steal the focus
    // from it's rightful owner. Squirrel it away ourselves so we can give
    // it back. (Damir)

    /****
    CWnd* pFocusWnd = GetFocus();

    CCellRange VisibleCells = GetVisibleNonFixedCellRange();

    int right = nCol - VisibleCells.GetMaxCol();
    int left  = VisibleCells.GetMinCol() - nCol;
    int down  = nRow - VisibleCells.GetMaxRow();
    int up    = VisibleCells.GetMinRow() - nRow;

    int iColumnStart;
    int iRowStart;

    iColumnStart = VisibleCells.GetMaxCol() + 1;
    while( right > 0 )
    {
        if( GetColumnWidth( iColumnStart ) > 0 )
            ////SendMessage( WM_HSCROLL, SB_LINERIGHT, 0 );

        right--;
        iColumnStart++;
    }

    iColumnStart = VisibleCells.GetMinCol() - 1;
    while( left > 0 )
    {
        if( GetColumnWidth( iColumnStart ) > 0 )
            ////SendMessage( WM_HSCROLL, SB_LINELEFT, 0 );
        left--;
        iColumnStart--;
    }

    iRowStart = VisibleCells.GetMaxRow() + 1;
    while( down > 0 )
    {
        if( GetRowHeight( iRowStart ) > 0 )
            ////SendMessage( WM_VSCROLL, SB_LINEDOWN, 0 );
        down--;
        iRowStart++;
    }

    iRowStart = VisibleCells.GetMinRow() - 1;
    while( up > 0 )
    {
        if( GetRowHeight( iRowStart ) > 0 )
            ////SendMessage( WM_VSCROLL, SB_LINEUP, 0 );
        up--;
        iRowStart--;
    }

    // Move one more if we only see a snall bit of the cell
    RECT rectCell;
    if (!GetCellRect(nRow, nCol, rectCell))
    {
        ////pFocusWnd->SetFocus();
        return;
    }

    GetClientRect(rectWindow);

    // The previous fix was fixed properly by Martin Richter <martin.richter@grutzeck.de>
    while (rectCell.right > rectWindow.right
           && rectCell.left > GetFixedColumnWidth())
    {
        ////SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        if (!GetCellRect(nRow, nCol, rectCell))
        {
            pFocusWnd->SetFocus();
            return;
        }
    }

    while (rectCell.bottom > rectWindow.bottom
           && rectCell.top > GetFixedRowHeight())
    {
        ////SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        if (!GetCellRect(nRow, nCol, rectCell))
        {
            pFocusWnd->SetFocus();
            return;
        }
    }

    // restore focus to whoever owned it
    pFocusWnd->SetFocus();
    ****/
}

BOOL CGridCtrl::IsCellEditable(CCellID &cell) const
{
    return IsCellEditable(cell.row, cell.col);
}

BOOL CGridCtrl::IsCellEditable(int nRow, int nCol) const
{
    return IsEditable() && ((GetItemState(nRow, nCol) & GVIS_READONLY) != GVIS_READONLY);
}

BOOL CGridCtrl::IsCellSelected(CCellID &cell) const
{
    return IsCellSelected(cell.row, cell.col);
}

BOOL CGridCtrl::IsCellSelected(int nRow, int nCol) const
{
    if (GetVirtualMode())
    {
        if (!IsSelectable())
            return FALSE;

        return (_SelectedCellMap.Find(nRow, nCol) != (0));
    }
    else
        return IsSelectable() && ((GetItemState(nRow, nCol) & GVIS_SELECTED) == GVIS_SELECTED);
}

BOOL CGridCtrl::IsCellVisible(CCellID cell)
{
    return IsCellVisible(cell.row, cell.col);
}

BOOL CGridCtrl::IsCellVisible(int nRow, int nCol)
{
    if (!IsWindow(0))
        return FALSE;

    int x, y;

    CCellID TopLeft;
    if (nCol >= GetFixedColumnCount() || nRow >= GetFixedRowCount())
    {
        TopLeft = GetTopleftNonFixedCell();
        if (nCol >= GetFixedColumnCount() && nCol < TopLeft.col)
            return FALSE;
        if (nRow >= GetFixedRowCount() && nRow < TopLeft.row)
            return FALSE;
    }

    RECT rect;
    ////GetClientRect(&rect);
    if (nCol < GetFixedColumnCount())
    {
        x = 0;
        for (int i = 0; i <= nCol; i++)
        {
            if (x >= rect.right)
                return FALSE;
            x += GetColumnWidth(i);
        }
    }
    else
    {
        x = GetFixedColumnWidth();
        for (int i = TopLeft.col; i <= nCol; i++)
        {
            if (x >= rect.right)
                return FALSE;
            x += GetColumnWidth(i);
        }
    }

    if (nRow < GetFixedRowCount())
    {
        y = 0;
        for (int i = 0; i <= nRow; i++)
        {
            if (y >= rect.bottom)
                return FALSE;
            y += GetRowHeight(i);
        }
    }
    else
    {
        if (nRow < TopLeft.row)
            return FALSE;
        y = GetFixedRowHeight();
        for (int i = TopLeft.row; i <= nRow; i++)
        {
            if (y >= rect.bottom)
                return FALSE;
            y += GetRowHeight(i);
        }
    }

    return TRUE;
}

BOOL CGridCtrl::InvalidateCellRect(const CCellID& cell)
{
    return InvalidateCellRect(cell.row, cell.col);
}

BOOL CGridCtrl::InvalidateCellRect(const int row, const int col)
{
    return TRUE;
}

BOOL CGridCtrl::InvalidateCellRect(const CCellRange& cellRange)
{
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl Mouse stuff

// Handles mouse wheel notifications
// Note - if this doesn't work for win95 then use OnRegisteredMouseWheel instead
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
BOOL CGridCtrl::OnMouseWheel(UINT nFlags, short zDelta, POINT_u pt)
{
    // A _nRowsPerWheelNotch value less than 0 indicates that the mouse
    // wheel scrolls whole pages, not just lines.
    if (_nRowsPerWheelNotch == -1)
    {
        int nPagesScrolled = zDelta / 120;

        if (nPagesScrolled > 0)
            for (int i = 0; i < nPagesScrolled; i++)
                PostMessage(WM_VSCROLL, SB_PAGEUP, 0);
        else
            for (int i = 0; i > nPagesScrolled; i--)
                PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
    }
    else
    {
        int nRowsScrolled = _nRowsPerWheelNotch * zDelta / 120;

        if (nRowsScrolled > 0)
            for (int i = 0; i < nRowsScrolled; i++)
                PostMessage(WM_VSCROLL, SB_LINEUP, 0);
        else
            for (int i = 0; i > nRowsScrolled; i--)
                PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
    }

    return ////CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
#endif // !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)

void CGridCtrl::OnMouseMove(UINT /*nFlags*/, POINT_u point)
{
    /****
    RECT rect;
    GetClientRect(&rect);

#ifndef GRIDCONTROL_NO_DRAGDROP
    // If outside client area, return (unless we are drag n dropping)
    if (_MouseMode != MOUSE_DRAGGING && !rect.PtInRect(point))
        return;
#endif

    // Sometimes a MOUSEMOVE message can come after the left buttons
    // has been let go, but before the BUTTONUP message hs been processed.
    // We'll keep track of mouse buttons manually to avoid this.
    // All bMouseButtonDown's have been replaced with the member _bLMouseButtonDown
    // BOOL bMouseButtonDown = ((nFlags & MK_LBUTTON) == MK_LBUTTON);

    // If the left mouse button is up, then test to see if row/column sizing is imminent
    if (!_bLMouseButtonDown ||
        (_bLMouseButtonDown && _MouseMode == MOUSE_NOTHING))
    {
        if (_bAllowColumnResize && MouseOverColumnResizeArea(point))
        {
            if (_MouseMode != MOUSE_OVER_COL_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
#endif
                _MouseMode = MOUSE_OVER_COL_DIVIDE;
            }
        }
        else if (_bAllowRowResize && MouseOverRowResizeArea(point))
        {
            if (_MouseMode != MOUSE_OVER_ROW_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
#endif
                _MouseMode = MOUSE_OVER_ROW_DIVIDE;
            }
        }
        else if (_MouseMode != MOUSE_NOTHING)
        {
#ifndef _WIN32_WCE_NO_CURSOR
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif
            _MouseMode = MOUSE_NOTHING;
        }

        if (_MouseMode == MOUSE_NOTHING)
        {
            CGridCellBase* pCell = NULL;
            CCellID idCurrentCell;
            if (!GetVirtualMode() || _bTitleTips)
            {
                // Let the cell know that a big fat cursor is currently hovering
                // over it.
                idCurrentCell = GetCellFromPt(point);
                pCell = GetCell(idCurrentCell.row, idCurrentCell.col);
                if (pCell)
                    pCell->OnMouseOver();
            }

#ifndef GRIDCONTROL_NO_TITLETIPS
            // Titletips anyone? anyone?
            if (_bTitleTips)
            {
                RECT TextRect, CellRect;
                if (pCell)
                {
                    LPCTSTR szTipText = pCell->GetTipText();
                    if (!_bRMouseButtonDown
						&& szTipText && szTipText[0]
                        && !pCell->IsEditing()
                        && GetCellRect( idCurrentCell.row, idCurrentCell.col, &TextRect)
                        && pCell->GetTipTextRect( &TextRect)
                        && GetCellRect(idCurrentCell.row, idCurrentCell.col, CellRect) )
                    {
						MYTRACE(L"Showing TitleTip\n");
						_TitleTip.Show(TextRect, pCell->GetTipText(),  0, CellRect,
                                        pCell->GetFont(),  GetTitleTipTextClr(), GetTitleTipBackClr());
                    }
                }
            }
#endif
        }

        _LastMousePoint = point;
        return;
    }

    if (!IsValid(_LeftClickDownCell))
    {
        _LastMousePoint = point;
        return;
    }

    // If the left mouse button is down, then process appropriately
    if (_bLMouseButtonDown)
    {
        switch (_MouseMode)
        {
        case MOUSE_SELECT_ALL:
            break;

        case MOUSE_SELECT_COL:
        case MOUSE_SELECT_ROW:
        case MOUSE_SELECT_CELLS:
            {
                CCellID idCurrentCell = GetCellFromPt(point);
                if (!IsValid(idCurrentCell))
                    return;

                if (idCurrentCell != GetFocusCell())
                {
                    OnSelecting(idCurrentCell);

                    // EFW - BUG FIX - Keep the appropriate cell row and/or
                    // column focused.  A fix in SetFocusCell() will place
                    // the cursor in a non-fixed cell as needed.
                    if((idCurrentCell.row >= _nFixedRows &&
                      idCurrentCell.col >= _nFixedCols) ||
                      _MouseMode==MOUSE_SELECT_COL ||
                      _MouseMode==MOUSE_SELECT_ROW)
                    {
                        SetFocusCell(idCurrentCell);
                    }
                }
                break;
            }

        case MOUSE_SIZING_COL:
            {
                /****
                CDC* pDC = GetDC();
                if (!pDC)
                    break;

                RECT oldInvertedRect(_LastMousePoint.x, rect.top,
                    _LastMousePoint.x + 2, rect.bottom);
                pDC->InvertRect(&oldInvertedRect);
                RECT newInvertedRect(point.x, rect.top,
                    point.x + 2, rect.bottom);
                pDC->InvertRect(&newInvertedRect);
                ReleaseDC(pDC);
                ****/
/****
            }
            break;

        case MOUSE_SIZING_ROW:
            {
                /****
                CDC* pDC = GetDC();
                if (!pDC)
                    break;

                RECT oldInvertedRect(rect.left, _LastMousePoint.y,
                    rect.right, _LastMousePoint.y + 2);
                pDC->InvertRect(&oldInvertedRect);
                RECT newInvertedRect(rect.left, point.y,
                    rect.right, point.y + 2);
                pDC->InvertRect(&newInvertedRect);
                ReleaseDC(pDC);
                ****/
/****
            }
            break;

#ifndef GRIDCONTROL_NO_DRAGDROP
        case MOUSE_PREPARE_EDIT:
        case MOUSE_PREPARE_DRAG:
            _MouseMode = MOUSE_PREPARE_DRAG;
            OnBeginDrag();
            break;
#endif
        }
    }
    ****/
    _LastMousePoint = point;
}

// Returns the point inside the cell that was clicked (coords relative to cell top left)
POINT_u CGridCtrl::GetPointClicked(int nRow, int nCol, const POINT_u& point)
{
    POINT_u PointCellOrigin;
    if( !GetCellOrigin( nRow, nCol, &PointCellOrigin)  )
        return POINT_u( 0, 0);

    POINT_u PointClickedCellRelative(point);
    PointClickedCellRelative -= PointCellOrigin;
    return PointClickedCellRelative;
}

void CGridCtrl::OnLButtonDblClk(UINT nFlags, POINT_u point)
{
    MYTRACE(L"CGridCtrl::OnLButtonDblClk\n");

    CCellID cell = GetCellFromPt(point);
    if( !IsValid( cell) )
    {
        //
        return;
    }

#ifdef _WIN32_WCE
    if (MouseOverColumnResizeArea(point))
#else
    if (_MouseMode == MOUSE_OVER_COL_DIVIDE)
#endif
    {
        POINT_u start;
        if (!GetCellOrigin(0, cell.col, &start))
            return;

        if (point.x - start.x < _nResizeCaptureRange)     // Clicked right of border
            cell.col--;

        //  ignore columns that are hidden and look left towards first visible column
        BOOL bFoundVisible = FALSE;
        while( cell.col >= 0)
        {
            if( GetColumnWidth( cell.col) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            cell.col--;
        }
        if( !bFoundVisible)
            return;

        AutoSizeColumn(cell.col, GetAutoSizeStyle());
        ////SEAN SEAN SEAN Invalidate();
    }
#ifdef _WIN32_WCE
    else if (MouseOverRowResizeArea(point))
#else
    else if (_MouseMode == MOUSE_OVER_ROW_DIVIDE)
#endif
    {
        POINT_u start;
        if (!GetCellOrigin(0, cell.col, &start))
            return;

        if (point.y - start.y < _nResizeCaptureRange)     // Clicked below border
            cell.row--;

        //  ignore rows that are hidden and look up towards first visible row
        BOOL bFoundVisible = FALSE;
        while( cell.row >= 0)
        {
            if( GetRowHeight( cell.row) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            cell.row--;
        }
        if( !bFoundVisible)
            return;

        AutoSizeRow(cell.row);
        ////SEAN SEAN SEAN Invalidate();
    }
    else if (_MouseMode == MOUSE_NOTHING)
    {
        POINT_u pointClickedRel;
        pointClickedRel = GetPointClicked( cell.row, cell.col, point);

        CGridCellBase* pCell = NULL;
        if (IsValid(cell))
            pCell = GetCell(cell.row, cell.col);

        // Clicked in the text area? Only then will cell selection work
        BOOL bInTextArea = FALSE;
        if (pCell)
        {
            RECT rectCell;
            if (GetCellRect(cell.row, cell.col, &rectCell) && pCell->GetTextRect(&rectCell))
                bInTextArea = pt_in_rect(rectCell, point);
        }

        if (cell.row >= _nFixedRows && IsValid(_LeftClickDownCell) &&
            cell.col >= _nFixedCols && bInTextArea)
        {
            OnEditCell(cell.row, cell.col, pointClickedRel, VK_LBUTTON);
        }
        else if (_bListMode)
        {
            if (!IsValid(cell))
                return;
            if (cell.row >= _nFixedRows && cell.col >= _nFixedCols && bInTextArea)
                OnEditCell(cell.row, cell.col, pointClickedRel, VK_LBUTTON);
        }

        if (IsValid(cell))
        {
            CGridCellBase* pCell = GetCell(cell.row, cell.col);
            if (pCell)
                pCell->OnDblClick(pointClickedRel);
            SendMessageToParent(cell.row, cell.col, NM_DBLCLK);
        }
    }

    ////CWnd::OnLButtonDblClk(nFlags, point);
}

void CGridCtrl::OnLButtonDown(UINT nFlags, POINT_u point)
{
    /****
#ifdef GRIDCONTROL_USE_TITLETIPS
    // EFW - Bug Fix
    _TitleTip.Hide();  // hide any titletips
#endif

    // MYTRACE(L"CGridCtrl::OnLButtonDown\n");
    // ////CWnd::OnLButtonDown(nFlags, point);

    SetFocus();

    _bLMouseButtonDown   = TRUE;
    _LeftClickDownPoint = point;
    _LeftClickDownCell  = GetCellFromPt(point);
    if (!IsValid(_LeftClickDownCell))
        return;

    // If the SHIFT key is not down, then the start of the selection area should be the
    // cell just clicked. Otherwise, keep the previous selection-start-cell so the user
    // can add to their previous cell selections in an intuitive way. If no selection-
    // start-cell has been specified, then set it's value here and now.
    if ((nFlags & MK_SHIFT) != MK_SHIFT)
        _SelectionStartCell = _LeftClickDownCell;
    else
    {
        if (!IsValid(_SelectionStartCell))
            _SelectionStartCell = _idCurrentCell;
    }

    EndEditing();

    // tell the cell about it
    CGridCellBase* pCell = GetCell(_LeftClickDownCell.row, _LeftClickDownCell.col);
    if (pCell)
        pCell->OnClickDown(GetPointClicked( _LeftClickDownCell.row, _LeftClickDownCell.col, point));

    // Clicked in the text area? Only then will cell selection work
    BOOL bInTextArea = FALSE;
    if (pCell)
    {
        RECT rectCell;
        if (GetCellRect(_LeftClickDownCell.row, _LeftClickDownCell.col, rectCell) &&
            pCell->GetTextRect(rectCell))
        {
            bInTextArea = rectCell.PtInRect(point);
        }
    }

    // If the user clicks on the current cell, then prepare to edit it.
    // (If the user moves the mouse, then dragging occurs)
    if (_LeftClickDownCell == _idCurrentCell &&
        !(nFlags & MK_CONTROL) && bInTextArea &&
        IsCellEditable(_LeftClickDownCell))
    {
        _MouseMode = MOUSE_PREPARE_EDIT;
        return;
    }
    // If the user clicks on a selected cell, then prepare to drag it.
    // (If the user moves the mouse, then dragging occurs)
    else if (IsCellSelected(_LeftClickDownCell))
    {
        // If control is pressed then unselect the cell or row (depending on the list mode)
        if (nFlags & MK_CONTROL)
        {
            SetFocusCell(_LeftClickDownCell);
            if (GetListMode())
                SelectRows(_LeftClickDownCell, TRUE, FALSE);
            else
                SelectCells(_LeftClickDownCell, TRUE, FALSE);
            return;
        }
#ifndef GRIDCONTROL_NO_DRAGDROP
        else if (_bAllowDragAndDrop)
            _MouseMode = MOUSE_PREPARE_DRAG;
#endif
    }
    else if (_MouseMode != MOUSE_OVER_COL_DIVIDE &&
             _MouseMode != MOUSE_OVER_ROW_DIVIDE)
    {
        SetFocusCell(-1, - 1);
        if (GetRowCount() > GetFixedRowCount() &&
            GetColumnCount() > GetFixedColumnCount())
            SetFocusCell(max(_LeftClickDownCell.row, _nFixedRows),
                         max(_LeftClickDownCell.col, _nFixedCols));
    }

    SetCapture();

    if (_MouseMode == MOUSE_NOTHING)
    {
        if (_bAllowColumnResize && MouseOverColumnResizeArea(point))
        {
            if (_MouseMode != MOUSE_OVER_COL_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
#endif
                _MouseMode = MOUSE_OVER_COL_DIVIDE;
            }
        }
        else if (_bAllowRowResize && MouseOverRowResizeArea(point))
        {
            if (_MouseMode != MOUSE_OVER_ROW_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
#endif
                _MouseMode = MOUSE_OVER_ROW_DIVIDE;
            }
        }
        // else if (_MouseMode != MOUSE_NOTHING)
        //{
        //    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        //    _MouseMode = MOUSE_NOTHING;
        //}
    }

    if (_MouseMode == MOUSE_OVER_COL_DIVIDE) // sizing column
    {
        _MouseMode = MOUSE_SIZING_COL;

        // Kludge for if we are over the last column...
        if (GetColumnWidth(GetColumnCount()-1) < _nResizeCaptureRange)
        {
            RECT VisRect;
            GetVisibleNonFixedCellRange(VisRect);
            if (abs(point.x - VisRect.right) < _nResizeCaptureRange)
                _LeftClickDownCell.col = GetColumnCount()-1;
        }

        POINT_u start;
        if (!GetCellOrigin(0, _LeftClickDownCell.col, &start))
            return;

        if( !_bHiddenColUnhide)
        {
            //  ignore columns that are hidden and look left towards first visible column
            BOOL bLookForVisible = TRUE;
            BOOL bIsCellRightBorder = point.x - start.x >= _nResizeCaptureRange;

            if( bIsCellRightBorder
                && _LeftClickDownCell.col + 1 >= GetColumnCount() )
            {
                // clicked on last column's right border

                // if last column is visible, don't do anything
                if( _LeftClickDownCell.col >= 0)
                    bLookForVisible = FALSE;
            }

            if( bLookForVisible)
            {
                // clicked on column divider other than last right border
                BOOL bFoundVisible = FALSE;
                int iOffset = 1;

                if( bIsCellRightBorder)
                    iOffset = 0;

                while( _LeftClickDownCell.col - iOffset >= 0)
                {
                    if( GetColumnWidth( _LeftClickDownCell.col - iOffset) > 0)
                    {
                        bFoundVisible = TRUE;
                        break;
                    }
                    _LeftClickDownCell.col--;
                }
                if( !bFoundVisible)
                    return;
            }
        }


        RECT rect;
        GetClientRect(&rect);
        RECT invertedRect(point.x, rect.top, point.x + 2, rect.bottom);

        CDC* pDC = GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        // If we clicked to the right of the colimn divide, then reset the click-down cell
        // as the cell to the left of the column divide - UNLESS we clicked on the last column
        // and the last column is teensy (kludge fix)
        if (point.x - start.x < _nResizeCaptureRange)
        {
            if (_LeftClickDownCell.col < GetColumnCount()-1 ||
                GetColumnWidth(GetColumnCount()-1) >= _nResizeCaptureRange)
            {
                if (!GetCellOrigin(0, --_LeftClickDownCell.col, &start))
                    return;
            }
        }

		// Allow a cell resize width no greater than that which can be viewed within
		// the grid itself
		int nMaxCellWidth = RectWidth(rect)-GetFixedColumnWidth();
        rect.left  = start.x + 1;
		rect.right = rect.left + nMaxCellWidth;

        ClientToScreen(rect);
#ifndef _WIN32_WCE_NO_CURSOR
        ClipCursor(rect);
#endif
    }
    else if (_MouseMode == MOUSE_OVER_ROW_DIVIDE) // sizing row
    {
        _MouseMode = MOUSE_SIZING_ROW;

        // Kludge for if we are over the last column...
        if (GetRowHeight(GetRowCount()-1) < _nResizeCaptureRange)
        {
            RECT VisRect;
            GetVisibleNonFixedCellRange(VisRect);
            if (abs(point.y - VisRect.bottom) < _nResizeCaptureRange)
                _LeftClickDownCell.row = GetRowCount()-1;
        }

        POINT_u start;
        if (!GetCellOrigin(_LeftClickDownCell, &start))
            return;

        if( !_bHiddenRowUnhide)
        {
            //  ignore rows that are hidden and look up towards first visible row
            BOOL bLookForVisible = TRUE;
            BOOL bIsCellBottomBorder = point.y - start.y >= _nResizeCaptureRange;

            if( bIsCellBottomBorder
                && _LeftClickDownCell.row + 1 >= GetRowCount() )
            {
                // clicked on last row's bottom border

                // if last row is visible, don't do anything
                if( _LeftClickDownCell.row >= 0)
                    bLookForVisible = FALSE;
            }

            if( bLookForVisible)
            {
                // clicked on row divider other than last bottom border
                BOOL bFoundVisible = FALSE;
                int iOffset = 1;

                if( bIsCellBottomBorder)
                    iOffset = 0;

                while( _LeftClickDownCell.row - iOffset >= 0)
                {
                    if( GetRowHeight( _LeftClickDownCell.row - iOffset) > 0)
                    {
                        bFoundVisible = TRUE;
                        break;
                    }
                    _LeftClickDownCell.row--;
                }
                if( !bFoundVisible)
                    return;
            }
        }

        RECT rect;
        GetClientRect(&rect);
        RECT invertedRect(rect.left, point.y, rect.right, point.y + 2);

        CDC* pDC = GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        // If we clicked below the row divide, then reset the click-down cell
        // as the cell above the row divide - UNLESS we clicked on the last row
        // and the last row is teensy (kludge fix)
        if (point.y - start.y < _nResizeCaptureRange)            // clicked below border
        {
            if (_LeftClickDownCell.row < GetRowCount()-1 ||
                GetRowHeight(GetRowCount()-1) >= _nResizeCaptureRange)
            {
                if (!GetCellOrigin(--_LeftClickDownCell.row, 0, &start))
                    return;
            }
        }

		int nMaxCellHeight = RectHeight(rect)-GetFixedRowHeight();
        rect.top = start.y + 1;
		rect.bottom = rect.top + nMaxCellHeight;

        ClientToScreen(rect);

#ifndef _WIN32_WCE_NO_CURSOR
        ClipCursor(rect);
#endif
    }
    else
#ifndef GRIDCONTROL_NO_DRAGDROP
    if (_MouseMode != MOUSE_PREPARE_DRAG) // not sizing or editing -- selecting
#endif
    {
	    SendMessageToParent(_LeftClickDownCell.row, _LeftClickDownCell.col, GVN_SELCHANGING);

        // If Ctrl pressed, save the current cell selection. This will get added
        // to the new cell selection at the end of the cell selection process
        _PrevSelectedCellMap.clear();
        if (nFlags & MK_CONTROL)
        {
            for (POSITION pos = _SelectedCellMap.GetStartPosition(); pos != NULL; )
            {
                DWORD key;
                CCellID cell;
                _SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
                _PrevSelectedCellMap.SetAt(key, cell);
            }
        }

        if (_LeftClickDownCell.row < GetFixedRowCount())
            OnFixedRowClick(_LeftClickDownCell);
        else if (_LeftClickDownCell.col < GetFixedColumnCount())
            OnFixedColumnClick(_LeftClickDownCell);
        else
        {
            _MouseMode = _bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
            OnSelecting(_LeftClickDownCell);

            _nTimerID = SetTimer(WM_LBUTTONDOWN, _nTimerInterval, 0);
        }
    }
    ****/
    _LastMousePoint = point;
}

void CGridCtrl::OnLButtonUp(UINT nFlags, POINT_u point)
{
    // MYTRACE(L"CGridCtrl::OnLButtonUp\n");

    ////CWnd::OnLButtonUp(nFlags, point);

    /****
    _bLMouseButtonDown = FALSE;

#ifndef _WIN32_WCE_NO_CURSOR
    ClipCursor(NULL);
#endif

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
    {
        ReleaseCapture();
        KillTimer(_nTimerID);
        _nTimerID = 0;
    }

    POINT_u pointClickedRel;
    pointClickedRel = GetPointClicked( _idCurrentCell.row, _idCurrentCell.col, point);

    // _MouseMode == MOUSE_PREPARE_EDIT only if user clicked down on current cell
    // and then didn't move mouse before clicking up (releasing button)
    if (_MouseMode == MOUSE_PREPARE_EDIT)
    {
        OnEditCell(_idCurrentCell.row, _idCurrentCell.col, pointClickedRel, VK_LBUTTON);
    }
#ifndef GRIDCONTROL_NO_DRAGDROP
    // _MouseMode == MOUSE_PREPARE_DRAG only if user clicked down on a selected cell
    // and then didn't move mouse before clicking up (releasing button)
    else if (_MouseMode == MOUSE_PREPARE_DRAG)
    {
        CGridCellBase* pCell = GetCell(_idCurrentCell.row, _idCurrentCell.col);
        if (pCell)
            pCell->OnClick( GetPointClicked( _idCurrentCell.row, _idCurrentCell.col, point) );
        SendMessageToParent(_LeftClickDownCell.row, _LeftClickDownCell.col, NM_CLICK);
	    SendMessageToParent(_LeftClickDownCell.row, _LeftClickDownCell.col, GVN_SELCHANGING);
        ResetSelectedRange();
	    SendMessageToParent(_idCurrentCell.row, _idCurrentCell.col, GVN_SELCHANGED);
    }
#endif
    else if (_MouseMode == MOUSE_SIZING_COL)
    {
        RECT rect;
        GetClientRect(&rect);
        RECT invertedRect(_LastMousePoint.x, rect.top, _LastMousePoint.x + 2, rect.bottom);

        CDC* pDC = GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (_LeftClickDownPoint != point && (point.x != 0 || point.y != 0)) // 0 pt fix by email1@bierling.net
        {
            POINT_u start;
            if (!GetCellOrigin(_LeftClickDownCell, &start))
                return;

            int nColumnWidth = max(point.x - start.x, _bAllowColHide? 0 : 1);

            SetColumnWidth(_LeftClickDownCell.col, nColumnWidth);
            ////ResetScrollBars();
            ////SEAN SEAN SEAN Invalidate();
        }
    }
    else if (_MouseMode == MOUSE_SIZING_ROW)
    {
        RECT rect;
        GetClientRect(&rect);
        RECT invertedRect(rect.left, _LastMousePoint.y, rect.right, _LastMousePoint.y + 2);

        CDC* pDC = GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (_LeftClickDownPoint != point  && (point.x != 0 || point.y != 0)) // 0 pt fix by email1@bierling.net
        {
            POINT_u start;
            if (!GetCellOrigin(_LeftClickDownCell, &start))
                return;

            int nRowHeight = max(point.y - start.y, _bAllowRowHide? 0 : 1);

            SetRowHeight(_LeftClickDownCell.row, nRowHeight);
            ////ResetScrollBars();
            ////SEAN SEAN SEAN Invalidate();
        }
    }
    else
    {
	    SendMessageToParent(_idCurrentCell.row, _idCurrentCell.col, GVN_SELCHANGED);

        CGridCellBase* pCell = GetCell(_idCurrentCell.row, _idCurrentCell.col);
        if (pCell)
            pCell->OnClick( GetPointClicked( _idCurrentCell.row, _idCurrentCell.col, point) );
        SendMessageToParent(_LeftClickDownCell.row, _LeftClickDownCell.col, NM_CLICK);
    }

    _MouseMode = MOUSE_NOTHING;

#ifndef _WIN32_WCE_NO_CURSOR
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif

    if (!IsValid(_LeftClickDownCell))
        return;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->0))
        pOwner->PostMessage(WM_COMMAND, MAKELONG(0;       ////GetDlgCtrlID(), BN_CLICKED),
        (LPARAM) GetSafeHwnd());
    ****/
}

#ifndef _WIN32_WCE
void CGridCtrl::OnRButtonDown(UINT nFlags, POINT_u point)
{
    ////CWnd::OnRButtonDown(nFlags, point);
	_bRMouseButtonDown = TRUE;

#ifdef GRIDCONTROL_USE_TITLETIPS
	MYTRACE(L"Hiding TitleTip\n");
    _TitleTip.Hide();  // hide any titletips
#endif
}

// EFW - Added to forward right click to parent so that a context
// menu can be shown without deriving a new grid class.
void CGridCtrl::OnRButtonUp(UINT nFlags, POINT_u point)
{
    ////CWnd::OnRButtonUp(nFlags, point);
    /****
	_bRMouseButtonDown = FALSE;

    CCellID FocusCell;
    FocusCell = GetCellFromPt(point);

    EndEditing();        // Auto-destroy any InPlaceEdit's

    // If not a valid cell, pass -1 for row and column
    if(!IsValid(FocusCell))
        SendMessageToParent(-1, -1, NM_RCLICK);
    else
    {
        SetFocusCell(-1,-1);
        SetFocusCell(max(FocusCell.row, _nFixedRows),
            max(FocusCell.col, _nFixedCols));

        // tell the cell about it
        CGridCellBase* pCell = GetCell(FocusCell.row, FocusCell.col);
        if (pCell)
            pCell->OnRClick( GetPointClicked( FocusCell.row, FocusCell.col, point) );

        SendMessageToParent(FocusCell.row, FocusCell.col, NM_RCLICK);
    }
    ****/
}
#endif



#ifndef _WIN32_WCE
/////////////////////////////////////////////////////////////////////////////
// CGridCtrl persistance

BOOL CGridCtrl::Save(LPCTSTR filename, TCHAR chSeparator/*=_T(',')*/)
{
    /****
    CStdioFile File;
    CFileException ex;
	tstring strSeparator(chSeparator);

    if (!File.Open(filename, CFile::modeWrite | CFile::modeCreate| CFile::typeText, &ex))
    {
        ex.ReportError();
        return FALSE;
    }

    try
    {
        int nNumColumns = GetColumnCount();
        for (int i = 0; i < GetRowCount(); i++)
        {
            for (int j = 0; j < nNumColumns; j++)
            {
                File.WriteString(GetItemText(i,j));
                File.WriteString((j==(nNumColumns-1))? _T("\n"): strSeparator);
            }
        }

        File.Close();
    }

    CATCH (CFileException, e)
    {
        AfxMessageBox(_T("Unable to save grid list"));
        return FALSE;
    }

    ****/
    return TRUE;
}

BOOL CGridCtrl::Load(LPCTSTR filename, TCHAR chSeparator/*=_T(',')*/)
{
    /****
    if (GetVirtualMode())
        return FALSE;

    TCHAR *token, *end;
    TCHAR buffer[1024];
    CStdioFile File;
    CFileException ex;

    if (!File.Open(filename, CFile::modeRead | CFile::typeText))
    {
        ex.ReportError();
        return FALSE;
    }

    DeleteAllItems();

    try
    {
        // Read Header off file
        File.ReadString(buffer, 1024);

        // Get first token
        for (token=buffer, end=buffer;
             *end && (*end != chSeparator) && (*end != _T('\n'));
             end++)
            ;

        if ((*end == _T('\0')) && (token == end))
            token = NULL;

        *end = _T('\0');

        while (token)
        {
            InsertColumn(token);

            // Get next token
            for (token=++end; *end && (*end != chSeparator) && (*end != _T('\n'));
               end++)
                ;

            if ((*end == _T('\0')) && (token == end))
                token = NULL;

            *end = _T('\0');
        }

        // Read in rest of data
        int nItem = 0;
        while (File.ReadString(buffer, 1024))
        {
            // Get first token
            for (token=buffer, end=buffer;
              *end && (*end != chSeparator) && (*end != _T('\n')); end++)
                ;

            if ((*end == _T('\0')) && (token == end))
                token = NULL;

            *end = _T('\0');

            int nSubItem = 0;
            while (token)
            {
                if (!nSubItem)
                    InsertRow(token);
                else
                    SetItemText(nItem, nSubItem, token);

                // Get next token
                for (token=++end; *end && (*end != chSeparator) && (*end != _T('\n'));
                  end++)
                    ;

                if ((*end == _T('\0')) && (token == end))
                    token = NULL;

                *end = _T('\0');

                nSubItem++;
            }
            nItem++;
        }

        AutoSizeColumns(GetAutoSizeStyle());
        File.Close();
    }

    CATCH (CFileException, e)
    {
        AfxMessageBox(_T("Unable to load grid data"));
        return FALSE;
    }

    ****/
    return TRUE;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl overrideables

#ifndef GRIDCONTROL_NO_DRAGDROP
// This is no longer needed since I've changed to OLE drag and drop - but it's
// still cool code. :)
CImageList* CGridCtrl::CreateDragImage(POINT_u *pHotSpot)
{
    CDC* pDC = GetDC();
    if (!pDC)
        return NULL;

    RECT rect;
    CCellID cell = GetFocusCell();
    if (!GetCellRect(cell.row, cell.col, rect))
        return NULL;

    // Translate coordinate system
    rect.BottomRight() = POINT_u(RectWidth(rect), RectHeight(rect));
    rect.TopLeft()     = POINT_u(0, 0);
    *pHotSpot = rect.BottomRight();

    // Create a new imagelist (the caller of this function has responsibility
    // for deleting this list)
    CImageList* pList = new CImageList;
    if (!pList || !pList->Create(RectWidth(rect), RectHeight(rect), ILC_MASK, 1, 1))
    {
        if (pList)
            delete pList;
        return NULL;
    }

    // Create mem DC and bitmap
    CDC MemDC;
    CBitmap bm;
    MemDC.CreateCompatibleDC(pDC);
    bm.CreateCompatibleBitmap(pDC, RectWidth(rect), RectHeight(rect));
    CBitmap* pOldBitmap = MemDC.SelectObject(&bm);
    MemDC.SetWindowOrg(0, 0);

    // Draw cell onto bitmap in memDC
    CGridCellBase* pCell = GetCell(cell.row, cell.col);
    if (pCell)
        pCell->Draw(&MemDC, cell.row, cell.col, rect, FALSE);

    // Clean up
    MemDC.SelectObject(pOldBitmap);
    ReleaseDC(pDC);

    // Add the bitmap we just drew to the image list.
    pList->Add(&bm, GetDefaultCell(FALSE, FALSE)->GetBackClr());
    bm.DeleteObject();

    return pList;
}
#endif

void CGridCtrl::OnFixedRowClick(CCellID& cell)
{
    if (!IsValid(cell))
        return;

    if (GetHeaderSort())
    {
        ////CWaitCursor waiter;
        if (cell.col == GetSortColumn())
            SortItems(cell.col, !GetSortAscending());
        else
            SortItems(cell.col, TRUE);
        ////SEAN SEAN SEAN Invalidate();
    }

    if (GetFixedRowSelection())
    {
        if (cell.col < GetFixedColumnCount())
        {
            _MouseMode = MOUSE_SELECT_ALL;
            OnSelecting(cell);
        }
        else
        {
            _MouseMode = MOUSE_SELECT_COL;
            OnSelecting(cell);
        }
    }
}

void CGridCtrl::OnFixedColumnClick(CCellID& cell)
{
    if (!IsValid(cell))
        return;

//    if (_bListMode && (GetItemState(cell.row, _nFixedCols) & GVNI_SELECTED))
//    {
//        OnEditCell(cell.row, cell.col, VK_LBUTTON);
//        return;
//    }

    if (GetFixedColumnSelection())
    {
        if (cell.row < GetFixedRowCount())
        {
            _MouseMode = MOUSE_SELECT_ALL;
            OnSelecting(cell);
        }
        else
        {
            _MouseMode = MOUSE_SELECT_ROW;
            OnSelecting(cell);
        }
    }
}

// Gets the extent of the text pointed to by str (no CDC needed)
// By default this uses the selected font (which is a bigger font)
SIZE_u CGridCtrl::GetTextExtent(int nRow, int nCol, LPCTSTR str)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return SIZE_u(0, 0);
    else
        return pCell->GetTextExtent(str);
}

// virtual
void CGridCtrl::OnEditCell(int nRow, int nCol, POINT_u point, UINT nChar)
{
#ifndef GRIDCONTROL_NO_TITLETIPS
    _TitleTip.Hide();  // hide any titletips
#endif

    // Can we do it?
    CCellID cell(nRow, nCol);
    if (!IsValid(cell) || !IsCellEditable(nRow, nCol))
        return;

    // Can we see what we are doing?
    EnsureVisible(nRow, nCol);
    if (!IsCellVisible(nRow, nCol))
        return;

    // Where, exactly, are we gonna do it??
    RECT rect;
    if (!GetCellRect(cell, &rect))
        return;

    // Check we can edit...
    if (SendMessageToParent(nRow, nCol, GVN_BEGINLABELEDIT) >= 0)
    {
        // Let's do it...
        CGridCellBase* pCell = GetCell(nRow, nCol);
        if (pCell)
            pCell->Edit(nRow, nCol, rect, point, IDC_INPLACE_CONTROL, nChar);
    }
}

// virtual
void CGridCtrl::EndEditing()
{
    CCellID cell = GetFocusCell();
    if (!IsValid(cell)) return;
    CGridCellBase *pCell = GetCell(cell.row, cell.col);
    if (pCell)
        pCell->EndEdit();
}

// virtual
void CGridCtrl::OnEndEditCell(int nRow, int nCol, tstring str)
{
    tstring strCurrentText = GetItemText(nRow, nCol);
    if (strCurrentText != str)
    {
        SetItemText(nRow, nCol, str.c_str());
        if (ValidateEdit(nRow, nCol, str.c_str()) &&
            SendMessageToParent(nRow, nCol, GVN_ENDLABELEDIT) >= 0)
        {
            SetModified(TRUE, nRow, nCol);
            RedrawCell(nRow, nCol);
        }
        else
        {
            SetItemText(nRow, nCol, strCurrentText.c_str());
        }
    }

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (pCell)
        pCell->OnEndEdit();
}

// If this returns FALSE then the editing isn't allowed
// virtual
BOOL CGridCtrl::ValidateEdit(int nRow, int nCol, LPCTSTR str)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return TRUE;

    return pCell->ValidateEdit(str);
}

// virtual
tstring CGridCtrl::GetItemText(int nRow, int nCol) const
{
    if (nRow < 0 || nRow >= _nRows || nCol < 0 || nCol >= _nCols)
        return _T("");

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return _T("");

    return pCell->GetText();
}

}
