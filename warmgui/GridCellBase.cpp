// GridCellBase.cpp : implementation file
//
// MFC Grid Control - Main grid cell base class
//
// Provides the implementation for the base cell type of the
// grid control. No data is stored (except for state) but default
// implementations of drawing, printingetc provided. MUST be derived
// from to be used.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2001. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.22+
//
// History:
// Ken Bertelson - 12 Apr 2000 - Split CGridCell into CGridCell and CGridCellBase
// C Maunder     - 19 May 2000 - Fixed sort arrow drawing (Ivan Ilinov)
// C Maunder     - 29 Aug 2000 - operator= checks for NULL font before setting (Martin Richter)
// C Maunder     - 15 Oct 2000 - GetTextExtent fixed (Martin Richter)
// C Maunder     -  1 Jan 2001 - Added ValidateEdit
//
// NOTES: Each grid cell should take care of it's own drawing, though the Draw()
//        method takes an "erase background" paramter that is called if the grid
//        decides to draw the entire grid background in on hit. Certain ambient
//        properties such as the default font to use, and hints on how to draw
//        fixed cells should be fetched from the parent grid. The grid trusts the
//        cells will behave in a certain way, and the cells trust the grid will
//        supply accurate information.
//        
/////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "warmgui_summer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef ASSERT
#define ASSERT(s)
#endif //ASSERT

namespace WARMGUI {

/////////////////////////////////////////////////////////////////////////////
// GridCellBase

CGridCellBase::CGridCellBase(void)
{
    Reset();
}

CGridCellBase::~CGridCellBase(void)
{
}

/////////////////////////////////////////////////////////////////////////////
// GridCellBase Operations

void CGridCellBase::Reset()
{
    m_nState  = 0;
}

void CGridCellBase::operator=(const CGridCellBase& cell)
{
    SetGrid(cell.GetGrid());    // do first in case of dependencies

    SetText(cell.GetText());
    SetImage(cell.GetImage());
    SetData(cell.GetData());
    SetState(cell.GetState());
    SetFormat(cell.GetFormat());
    SetTextClr(cell.GetTextClr());
    SetBackClr(cell.GetBackClr());
    SetFont(cell.IsDefaultFont()? NULL : cell.GetFont());
    SetMargin(cell.GetMargin());
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Attributes

// Returns a pointer to a cell that holds default values for this particular type of cell
CGridCellBase* CGridCellBase::GetDefaultCell() const
{
    if (GetGrid())
        return GetGrid()->GetDefaultCell(IsFixedRow(), IsFixedCol());
    return NULL;
}

void CGridCellBase::SetArtistFont(CGridCellBase* cell)
{
    if (cell->GetFont()) {
        GetGrid()->_artist->SetTextFormat(cell->GetFont());
    }else if (cell->IsFixedCol()) {
        GetGrid()->_artist->SetTextFormat(GetGrid()->_uiSetting._pFixedColFont);
    } else if (cell->IsFixedRow()) {
        GetGrid()->_artist->SetTextFormat(GetGrid()->_uiSetting._pFixedRowFont);
    } else if (cell->IsFixed()) {
        GetGrid()->_artist->SetTextFormat(GetGrid()->_uiSetting._pFixedRowColFont);
    } else {
        GetGrid()->_artist->SetTextFormat(GetGrid()->_uiSetting._pDefaultFont);
    }
}

void CGridCellBase::SetArtistTextClr(CGridCellBase* cell, COLORALPHA& TextClr, COLORALPHA& TextBkClr)
{
    TextBkClr = cell->GetTextClr(), TextClr = cell->GetBackClr();
    if (TextClr.a < 0) {
        if (cell->IsFixedCol()) {
            TextClr = GetGrid()->_uiSetting._crFixColText, TextBkClr = GetGrid()->_uiSetting._crFixColBkgText;
        } else if (cell->IsFixedRow()) {
            TextClr = GetGrid()->_uiSetting._crFixRowText, TextBkClr = GetGrid()->_uiSetting._crFixRowBkgText;
        } else if (cell->IsFixed()) {
            TextClr = GetGrid()->_uiSetting._crFixRowColText, TextBkClr = GetGrid()->_uiSetting._crFixRowColBkgText;
        } else {
            TextClr = GetGrid()->_uiSetting._crDefaultText, TextBkClr = GetGrid()->_uiSetting._crDefaultBkgText;
        }
    }
}



/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Operations

// EFW - Various changes to make it draw cells better when using alternate
// color schemes.  Also removed printing references as that's now done
// by PrintCell() and fixed the sort marker so that it doesn't draw out
// of bounds.
BOOL CGridCellBase::Draw(int nRow, int nCol, RECT rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
    // Note - all through this function we totally brutalise 'rect'. Do not
    // depend on it's value being that which was passed in.

    CGridCtrl* pGrid = GetGrid();
    ASSERT(pGrid);

    CGridCellBase* cell = pGrid->GetCell(nRow, nCol);
    ASSERT(cell);

    if (!pGrid)
        return FALSE;

    if( RectWidth(rect) <= 0 || RectHeight(rect) <= 0)  // prevents imagelist item from drawing even
        return FALSE;                             //  though cell is hidden

    //MYTRACE(L"Drawing %scell %d, %d\n", IsFixed()? _T("Fixed ") : _T(""), nRow, nCol);

    ////SEAN SEAN SEAN
    //// Set alpha = 0


    // Get the default cell implementation for this kind of cell. We use it if this cell
    // has anything marked as "default"
    //CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
    //if (!pDefaultCell)
    //    return FALSE;

    // Set up text and background colours
    COLORALPHA TextClr, TextBkClr;
    SetArtistTextClr(cell, TextClr, TextBkClr);

    
    // Draw cell background and highlighting (if necessary)
    if ( IsFocused() || IsDropHighlighted() )
    {
        // Always draw even in list mode so that we can tell where the
        // cursor is at.  Use the highlight colors though.
        if(GetState() & GVIS_SELECTED)
        {
            TextBkClr = pGrid->_uiSetting._crHighLightBkg, TextClr = pGrid->_uiSetting._crHighLightTxt;
            bEraseBkgnd = TRUE;
        }

        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBkgnd)
        {
            pGrid->_artist->SetSolidColorBrush(TextBkClr);
            pGrid->_artist->FillRectangle(rect);
        }

        // Don't adjust frame rect if no grid lines so that the
        // whole cell is enclosed.
        if(pGrid->GetGridLines() != GVL_NONE)
        {
            rect.right--;
            rect.bottom--;
        }

        if (pGrid->GetFrameFocusCell())
        {
            // Use same color as text to outline the cell so that it shows
            // up if the background is black.
            pGrid->_artist->SetSolidColorBrush(TextClr);
            pGrid->_artist->FillRectangle(rect);
        }
        ////SEAN SEAN SEAN
        ////Set Text color
        pGrid->_artist->SetSolidColorBrush(TextClr);

        // Adjust rect after frame draw if no grid lines
        if(pGrid->GetGridLines() == GVL_NONE)
        {
            rect.right--;
            rect.bottom--;
        }

        DeflateRect(rect, 1, 1);
    }
    else if ((GetState() & GVIS_SELECTED))
    {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        pGrid->_artist->SetSolidColorBrush(pGrid->_uiSetting._crHighLightBkg);
        pGrid->_artist->FillRectangle(rect);
        rect.right--; rect.bottom--;
        ////SEAN SEAN SEAN
        ////Set Text Color
        pGrid->_artist->SetSolidColorBrush(pGrid->_uiSetting._crHighLightTxt);
    }
    else
    {
        if (bEraseBkgnd)
        {
            rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
            ////SEAN SEAN SEAN
            ////Fill Rect
            pGrid->_artist->SetSolidColorBrush(TextBkClr);
            pGrid->_artist->FillRectangle(rect);

            rect.right--; rect.bottom--;
        }
        ////SEAN SEAN SEAN
        ////Set Text Color
        pGrid->_artist->SetSolidColorBrush(TextClr);
    }

    // Draw lines only when wanted
    if (IsFixed() && pGrid->GetGridLines() != GVL_NONE)
    {
        CCellID FocusCell = pGrid->GetFocusCell();

        // As above, always show current location even in list mode so
        // that we know where the cursor is at.
        BOOL bHiliteFixed = pGrid->GetTrackFocusCell() && pGrid->IsValid(FocusCell) &&
                            (FocusCell.row == nRow || FocusCell.col == nCol);

        // If this fixed cell is on the same row/col as the focus cell,
        // highlight it.
        if (bHiliteFixed)
        {
            rect.right++; rect.bottom++;
            ////SEAN SEAN SEAN
            ////Draw edge
            pGrid->_artist->DrawRectangle(rect);
            DeflateRect(rect, 1,1);
        }
        else
        {
            ////SEAN SEAN SEAN
            ////Draw 3d rectangle line
            pGrid->_artist->SetSolidColorBrush(pGrid->_uiSetting._cr3DFace);
            pGrid->_artist->DrawLine(rect.right, rect.top, rect.left, rect.top);
            pGrid->_artist->DrawLine(rect.left, rect.top, rect.left, rect.bottom);
            pGrid->_artist->SetSolidColorBrush(pGrid->_uiSetting._crShadow);
            pGrid->_artist->DrawLine(rect.right, rect.top, rect.right, rect.bottom);
            pGrid->_artist->DrawLine(rect.right, rect.bottom, rect.left, rect.bottom);

            DeflateRect(rect, 1,1);
        }
    }

    // Draw Text and image
    ////SEAN SEAN SEAN
    ////Set Font
    /****if (!pDC->m_bPrinting)
    {
        IDWriteTextFormat* pFont = GetFontObject();
        if (pFont) {
            ////pDC->SelectObject(pFont);
        }
    }****/

    SetArtistFont(cell);
    DeflateRect(rect, GetMargin(), 0);

    ////SEAN SEAN SEAN
    ////Draw Image in image list
    /*
    if (pGrid->GetImageList() && GetImage() >= 0)
    {
        IMAGEINFO Info;
        if (pGrid->GetImageList()->GetImageInfo(GetImage(), &Info))
        {
            //  would like to use a clipping region but seems to have issue
            //  working with CMemDC directly.  Instead, don't display image
            //  if any part of it cut-off
            //
            // CRgn rgn;
            // rgn.CreateRectRgnIndirect(rect);
            // pDC->SelectClipRgn(&rgn);
            // rgn.DeleteObject();

            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            int nImageHeight = Info.rcImage.bottom-Info.rcImage.top+1;

            if( nImageWidth + rect.left <= rect.right + (int)(2*GetMargin())
                && nImageHeight + rect.top <= rect.bottom + (int)(2*GetMargin())  )
            {
                pGrid->GetImageList()->Draw(pDC, GetImage(), rect.TopLeft(), ILD_NORMAL);
            }

            //rect.left += nImageWidth+GetMargin();
        }
    }
    */
    // Draw sort arrow
    if (pGrid->GetSortColumn() == nCol && nRow == 0)
    {
        ////SEAN SEAN SEAN
        SIZE_u size;    //// = pDC->GetTextExtent(_T("M"));
        int nOffset = 2;

        // Base the size of the triangle on the smaller of the column
        // height or text height with a slight offset top and bottom.
        // Otherwise, it can get drawn outside the bounds of the cell.
        size.height -= (nOffset * 2);

        if (size.height >= RectHeight(rect))
            size.height = RectHeight(rect) - (nOffset * 2);

        size.width = size.height;      // Make the dimensions square

        // Kludge for vertical text
        BOOL bVertical = FALSE;    //(GetFont()->lfEscapement == 900);

        // Only draw if it'll fit!
        if (size.width + rect.left < rect.right + (int)(2*GetMargin()))
        {
            int nTriangleBase = rect.bottom - nOffset - size.height;    // Triangle bottom right
            //int nTriangleBase = (rect.top + rect.bottom - size.height)/2; // Triangle middle right
            //int nTriangleBase = rect.top + nOffset;                 // Triangle top right

            //int nTriangleLeft = rect.right - size.width;                 // Triangle RHS
            //int nTriangleLeft = (rect.right + rect.left - size.width)/2; // Triangle middle
            //int nTriangleLeft = rect.left;                            // Triangle LHS

            int nTriangleLeft;
            if (bVertical)
                nTriangleLeft = (rect.right + rect.left - size.width)/2; // Triangle middle
            else
                nTriangleLeft = rect.right - size.width;               // Triangle RHS

            ////SEAN SEAN SEAN
            ////Draw 3d triangle point upwards/downward
            if (pGrid->GetSortAscending())
            {
                // Draw triangle pointing upwards
                pGrid->_artist->SetSolidColorBrush(pGrid->_uiSetting._cr3DFace);
                pGrid->_artist->DrawLine( nTriangleLeft + 1, nTriangleBase + size.height + 1, nTriangleLeft + (size.width / 2) + 1, nTriangleBase + 1 );
                pGrid->_artist->DrawLine( nTriangleLeft + (size.width / 2) + 1, nTriangleBase + 1, nTriangleLeft + size.width + 1, nTriangleBase + size.height + 1);
                pGrid->_artist->DrawLine( nTriangleLeft + size.width + 1, nTriangleBase + size.height + 1, nTriangleLeft + 1, nTriangleBase + size.height + 1);

                pGrid->_artist->SetSolidColorBrush(pGrid->_uiSetting._crShadow);
                pGrid->_artist->DrawLine( nTriangleLeft, nTriangleBase + size.height, nTriangleLeft + (size.width / 2), nTriangleBase );
                pGrid->_artist->DrawLine( nTriangleLeft + (size.width / 2), nTriangleBase, nTriangleLeft + size.width, nTriangleBase + size.height );
                pGrid->_artist->DrawLine( nTriangleLeft + size.width, nTriangleBase + size.height, nTriangleLeft, nTriangleBase + size.height );
            }
            else
            {
                // Draw triangle pointing downwards
                pGrid->_artist->SetSolidColorBrush(pGrid->_uiSetting._cr3DFace);
                pGrid->_artist->DrawLine( nTriangleLeft + 1, nTriangleBase + 1, nTriangleLeft + (size.width / 2) + 1, nTriangleBase + size.height + 1 );
                pGrid->_artist->DrawLine( nTriangleLeft + (size.width / 2) + 1, nTriangleBase + size.height + 1, nTriangleLeft + size.width + 1, nTriangleBase + 1 );
                pGrid->_artist->DrawLine( nTriangleLeft + size.width + 1, nTriangleBase + 1, nTriangleLeft + 1, nTriangleBase + 1 );
    
                pGrid->_artist->SetSolidColorBrush(pGrid->_uiSetting._crShadow);
                pGrid->_artist->DrawLine( nTriangleLeft, nTriangleBase, nTriangleLeft + (size.width / 2), nTriangleBase + size.height );
                pGrid->_artist->DrawLine( nTriangleLeft + (size.width / 2), nTriangleBase + size.height, nTriangleLeft + size.width, nTriangleBase );
                pGrid->_artist->DrawLine( nTriangleLeft + size.width, nTriangleBase, nTriangleLeft, nTriangleBase );
            }
            if (!bVertical)
                rect.right -= size.height;
        }
    }

    // We want to see '&' characters so use DT_NOPREFIX
    GetTextRect(&rect);
    ////SEAN SEAN SEAN
    ////Draw Text
    ////DrawText(pDC->m_hDC, GetText(), -1, rect, GetFormat() | DT_NOPREFIX);

    if (GetText()) {
        pGrid->_artist->SetSolidColorBrush(TextClr);
        pGrid->_artist->DrawTextW(GetText(), rect);
    }

    ////SEAN SEAN SEAN
    ////get every thing back
    ////pDC->RestoreDC(nSavedDC);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Mouse and Cursor events

// Not yet implemented
void CGridCellBase::OnMouseEnter()
{
    MYTRACE(L"Mouse entered cell\n");
}

void CGridCellBase::OnMouseOver()
{
    //MYTRACE(L"Mouse over cell\n");
}

// Not Yet Implemented
void CGridCellBase::OnMouseLeave()
{
    MYTRACE(L"Mouse left cell\n");
}

void CGridCellBase::OnClick(POINT_u PointCellRelative)
{
    MYTRACE(L"Mouse Left btn up in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CGridCellBase::OnClickDown( POINT_u PointCellRelative)
{
    MYTRACE(L"Mouse Left btn down in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CGridCellBase::OnRClick( POINT_u PointCellRelative)
{
    MYTRACE(L"Mouse right-clicked in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CGridCellBase::OnDblClick( POINT_u PointCellRelative)
{
    MYTRACE(L"Mouse double-clicked in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

// Return TRUE if you set the cursor
BOOL CGridCellBase::OnSetCursor()
{
    ////SEAN SEAN SEAN
    ////Set cursor
    ////SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase editing

void CGridCellBase::OnEndEdit() 
{
	
}

BOOL CGridCellBase::ValidateEdit(LPCTSTR str)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Sizing

BOOL CGridCellBase::GetTextRect( LPRECT pRect)  // i/o:  i=dims of cell rect; o=dims of text rect
{
    if (GetImage() >= 0)
    {
        IMAGEINFO Info;

        CGridCtrl* pGrid = GetGrid();
        ////SEAN SEAN SEAN
        ////Set image
        ////CImageList* pImageList = pGrid->GetImageList();
        /****
        if (pImageList && pImageList->GetImageInfo( GetImage(), &Info))
        {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            pRect->left += nImageWidth + GetMargin();
        }****/
    }

    return TRUE;
}

// By default this uses the selected font (which is a bigger font)
SIZE_u CGridCellBase::GetTextExtent(LPCTSTR szText)////, CDC* pDC /*= NULL*/)
{
    CGridCtrl* pGrid = GetGrid();
    ASSERT(pGrid);

    if (!szText) 
    {
        CGridDefaultCell* pDefCell = (CGridDefaultCell*) GetDefaultCell();
        if (pDefCell)
            return SIZE_u(pDefCell->GetWidth(), pDefCell->GetHeight());
        else
            return SIZE_u(0, 0);
    }

    SIZE_u size;
    /**** SEAN SEAN SEAN
    Draw Text and get the size of text
    int nFormat = GetFormat();

    // If the cell is a multiline cell, then use the width of the cell
    // to get the height
    if ((nFormat & DT_WORDBREAK) && !(nFormat & DT_SINGLELINE))
    {
        tstring str = szText;
        int nMaxWidth = 0;
        while (TRUE)
        {
            size_t nPos = str.find(_T('\n'));
            tstring TempStr = (nPos == tstring::npos)? str : str.Left(nPos);
            int nTempWidth = pDC->GetTextExtent(TempStr).cx;
            if (nTempWidth > nMaxWidth)
                nMaxWidth = nTempWidth;

            if (nPos < 0)
                break;
            str = str.Mid(nPos + 1);    // Bug fix by Thomas Steinborn
        }
        
        RECT rect;
        rect.left = rect.top = rect.bottom = 0, rect.right = nMaxWidth + 1;
        pDC->DrawText(szText, -1, rect, nFormat | DT_CALCRECT);
        size = rect.Size();
    }
    else
        size = pDC->GetTextExtent(szText, _tcslen(szText));

    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);
    size.width += (tm.tmOverhang);

    if (pOldFont)
        pDC->SelectObject(pOldFont);
    
    size += SIZE_u(4*GetMargin(), 2*GetMargin());

    // Kludge for vertical text
    LOGFONT *pLF = GetFont();
    if (pLF->lfEscapement == 900 || pLF->lfEscapement == -900)
    {
        int nTemp = size.width;
        size.width = size.height;
        size.height = nTemp;
        size += SIZE_u(0, 4*GetMargin());
    }
    
    if (bReleaseDC)
        pGrid->ReleaseDC(pDC);
    ****/
    return size;
}

SIZE_u CGridCellBase::GetCellExtent()////CDC* pDC)
{
    SIZE_u size = GetTextExtent(GetText());////, pDC);
    /*
    SIZE_u ImageSize(0,0);

    int nImage = GetImage();
    if (nImage >= 0) 
    {
        CGridCtrl* pGrid = GetGrid();
        ASSERT(pGrid);

        if (pGrid->GetImageList()) 
        {
            IMAGEINFO Info;
            if (pGrid->GetImageList()->GetImageInfo(nImage, &Info))
                ImageSize = SIZE_u(Info.rcImage.right-Info.rcImage.left+1, 
                                  Info.rcImage.bottom-Info.rcImage.top+1);
        }
    }
    
    return SIZE_u(size.width + ImageSize.cx, max(size.height, ImageSize.cy));*/
    return SIZE_u(size.width, size.height);
}

/*****************************************************************************
Callable by derived classes, only
*****************************************************************************/
LRESULT CGridCellBase::SendMessageToParent(int nRow, int nCol, int nMessage)
{
    CGridCtrl* pGrid = GetGrid();
    if( pGrid)
        return pGrid->SendMessageToParent(nRow, nCol, nMessage);
    else
        return 0;
}

} //namespace WARMGUI