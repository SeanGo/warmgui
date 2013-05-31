// GridCell.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for the "default" cell type of the
// grid control. Adds in cell editing.
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
// For use with CGridCtrl v2.20+
//
// History:
// Eric Woodruff - 20 Feb 2000 - Added PrintCell() plus other minor changes
// Ken Bertelson - 12 Apr 2000 - Split CGridCell into CGridCell and CGridCellBase
// <kenbertelson@hotmail.com>
// C Maunder     - 17 Jun 2000 - Font handling optimsed, Added CGridDefaultCell
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "warmgui.h"



namespace WARMGUI {


/////////////////////////////////////////////////////////////////////////////
// GridCell

CGridCell::CGridCell(void)
    : m_pTextFont(0)
{
    Reset();
}

CGridCell::~CGridCell(void)
{
}

/////////////////////////////////////////////////////////////////////////////
// GridCell Attributes

void CGridCell::operator=(const CGridCell& cell)
{
    CGridCellBase::operator=(  cell);
}

void CGridCell::Reset()
{
    CGridCellBase::Reset();

    m_strText.clear();
    m_nImage   = -1;
    m_pGrid    = NULL;
    m_bEditing = FALSE;
    ////m_pEditWnd = NULL;       //CWnd

    m_nFormat = (DWORD)-1;       // Use default from CGridDefaultCell
    m_crBkClr = DEFAULT_COLOR_ALPHA;     // Background colour (or DEFAULT_COLOR_ALPHA)
    m_crFgClr = DEFAULT_COLOR_ALPHA;     // Forground colour (or DEFAULT_COLOR_ALPHA)
    m_nMargin = (UINT)-1;        // Use default from CGridDefaultCell

    m_pTextFont = NULL;            // Cell font
}

void CGridCell::SetFont(IDWriteTextFormat* plf)
{
    m_pTextFont = plf;
}

IDWriteTextFormat* CGridCell::GetFont() const
{
    if (m_pTextFont == NULL)
    {
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return NULL;

        return pDefaultCell->GetFont();
    }

    return m_pTextFont; 
}

DWORD CGridCell::GetFormat() const
{
    if (m_nFormat == (DWORD)-1)
    {
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return 0;

        return pDefaultCell->GetFormat();
    }

    return m_nFormat;
}

UINT CGridCell::GetMargin() const           
{
    if (m_nMargin == (UINT)-1)
    {
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return 0;

        return pDefaultCell->GetMargin();
    }

    return m_nMargin; 
}

/////////////////////////////////////////////////////////////////////////////
// GridCell Operations

BOOL CGridCell::Edit(int nRow, int nCol, RECT rect, POINT_u /* point */, UINT nID, UINT nChar)
{
    if ( m_bEditing )
	{      
        ////if (m_pEditWnd)
		////    m_pEditWnd->SendMessage ( WM_CHAR, nChar );    
    }  
	else  
	{   
		DWORD dwStyle = ES_LEFT;
		if (GetFormat() & DT_RIGHT) 
			dwStyle = ES_RIGHT;
		else if (GetFormat() & DT_CENTER) 
			dwStyle = ES_CENTER;
		
		m_bEditing = TRUE;
		
		// InPlaceEdit auto-deletes itself
		CGridCtrl* pGrid = GetGrid();
        ////SET A EDIT WINDOW
		////m_pEditWnd = new CInPlaceEdit(pGrid, rect, dwStyle, nID, nRow, nCol, GetText(), nChar);
    }
    return TRUE;
}

void CGridCell::EndEdit()
{
    ////if (m_pEditWnd)
    ////    ((CInPlaceEdit*)m_pEditWnd)->EndEdit();
}

void CGridCell::OnEndEdit()
{
    m_bEditing = FALSE;
    ////m_pEditWnd = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CGridDefaultCell

CGridDefaultCell::CGridDefaultCell() 
{
    m_nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX | DT_END_ELLIPSIS;

    m_crFgClr = DEFAULT_COLOR_ALPHA;
    m_crBkClr = DEFAULT_COLOR_ALPHA;
    m_Size.width = 30, m_Size.height = 10;
    m_dwStyle = 0;

    if (m_pGrid)
        SetFont(m_pGrid->GetUiSetting()._pDefaultFont);
}

CGridDefaultCell::~CGridDefaultCell()
{
}

void CGridDefaultCell::SetFont(IDWriteTextFormat* plf)
{
    if (!plf) return;


    CGridCell::SetFont(plf);

    // Get the font size and hence the default cell size
    HDC hdc = ::GetDC(NULL);
    if (hdc && m_pTextFont)
    {
        LOGFONT lf;
        memset(&lf, 0, sizeof(LOGFONT));
        lf.lfHeight = (long)m_pTextFont->GetFontSize();
        TCHAR fontName[32];
        if (SUCCEEDED(m_pTextFont->GetFontFamilyName(fontName, 32))) { 
            _tcscpy_s(lf.lfFaceName, 32, fontName);

            HFONT hMyFont = CreateFontIndirect(&lf);
            HFONT oldFont = (HFONT)::SelectObject(hdc, hMyFont);

            SIZE size;
            GetTextExtentPoint32(hdc, _T(" "), 1, &size);
            SetMargin(size.cx);
            GetTextExtentPoint32(hdc, _T(" XXXXXXXXXXXX "), 14, &size);
            m_Size.width = size.cx, m_Size.height = (size.cy * 3) / 2;

            SelectObject(hdc, oldFont);
            DeleteObject(hMyFont);
        } else {
            SetMargin(3);
            m_Size.width = 40, m_Size.height = 16;
        }
        ReleaseDC(NULL, hdc);
    }
    else
    {
        SetMargin(3);
        m_Size.width = 40, m_Size.height = 16;
    }
}

IDWriteTextFormat* CGridDefaultCell::GetFont() const
{
    return m_pTextFont;
}


} //namespace WARMGUI
