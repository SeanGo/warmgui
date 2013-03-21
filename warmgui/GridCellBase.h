/////////////////////////////////////////////////////////////////////////////
// GridCellBase.h : header file
//
// MFC Grid Control - Grid cell base class header file
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
//////////////////////////////////////////////////////////////////////

#if !defined (__grid_cell_base_h__)
#define __grid_cell_base_h__

class CGridCtrl;

// Cell states
#define GVIS_FOCUSED            0x0001
#define GVIS_SELECTED           0x0002
#define GVIS_DROPHILITED        0x0004
#define GVIS_READONLY           0x0008
#define GVIS_FIXED              0x0010
#define GVIS_FIXEDROW           0x0020
#define GVIS_FIXEDCOL           0x0040
#define GVIS_MODIFIED           0x0080

// Cell data mask
#define GVIF_TEXT               LVIF_TEXT
#define GVIF_IMAGE              LVIF_IMAGE
#define GVIF_PARAM              LVIF_PARAM
#define GVIF_STATE              LVIF_STATE
#define GVIF_BKCLR              (GVIF_STATE<<1)
#define GVIF_FGCLR              (GVIF_STATE<<2)
#define GVIF_FORMAT             (GVIF_STATE<<3)
#define GVIF_FONT               (GVIF_STATE<<4)
#define GVIF_MARGIN             (GVIF_STATE<<5)
#define GVIF_ALL                (GVIF_TEXT|GVIF_IMAGE|GVIF_PARAM|GVIF_STATE|GVIF_BKCLR|GVIF_FGCLR| \
                                 GVIF_FORMAT|GVIF_FONT|GVIF_MARGIN)

// Used for Get/SetItem calls.
typedef struct _GV_ITEM {
    int      row,col;     // Row and Column of item
    UINT     mask;        // Mask for use in getting/setting cell data
    UINT     nState;      // cell state (focus/hilighted etc)
    DWORD    nFormat;     // Format of cell
    int      iImage;      // index of the list view item’s icon
    COLORREF crBkClr;     // Background colour (or CLR_DEFAULT)
    COLORREF crFgClr;     // Forground colour (or CLR_DEFAULT)
    LPARAM   lParam;      // 32-bit value to associate with item
    LOGFONT  lfFont;      // Cell font
    UINT     nMargin;     // Internal cell margin
    LPCTSTR  strText;     // Text in cell
} GV_ITEM;

// Each cell contains one of these. Fields "row" and "column" are not stored since we
// will usually have acces to them in other ways, and they are an extra 8 bytes per
// cell that is probably unnecessary.

class CGridCellBase
{
    friend class CGridCtrl;

// Construction/Destruction
public:
    CGridCellBase();
    virtual ~CGridCellBase();

// Attributes
public:
    // can't do pure virtual because of DECLARE_DYNCREATE requirement
    //  use ASSERT() to require that programmer overrides all that should
    //  be pure virtuals

    virtual void SetText(LPCTSTR /* szText */)              {    }
    virtual void SetImage(int /* nImage */)                 {    }
    virtual void SetData(LPARAM /* lParam */)               {    }
    virtual void SetState(DWORD nState)                     { m_nState = nState; }
    virtual void SetFormat(DWORD /* nFormat */)             {    }
    virtual void SetTextClr(COLORREF /* clr */)             {    }
    virtual void SetBackClr(COLORREF /* clr */)             {    }
    virtual void SetFont(const LOGFONT* /* plf */)          {    }
    virtual void SetMargin( UINT /* nMargin */)             {    }
    virtual void SetGrid(CGridCtrl* /* pGrid */)            {    }
    virtual void SetCoords( int /* nRow */, int /* nCol */) {    }

    virtual LPCTSTR  GetText() const                        { return NULL;  }
    virtual LPCTSTR  GetTipText() const                     { return GetText();             } // may override TitleTip return
    virtual int      GetImage() const                       { return -1;    }
    virtual LPARAM   GetData() const                        { return 0;     }
    virtual DWORD    GetState() const                       { return m_nState;              }
    virtual DWORD    GetFormat() const                      { return 0;     }
    virtual COLORREF GetTextClr() const                     { return 0;     }
    virtual COLORREF GetBackClr() const                     { return 0;     }
    virtual LOGFONT* GetFont() const                        { return NULL;  }
    virtual LOGFONT* GetFontObject() const                  { return NULL;  }
    virtual CGridCtrl* GetGrid() const                      { return NULL;  }
    virtual HWND*    GetEditWnd() const                     { return NULL;  }
    virtual UINT     GetMargin() const                      { return 0;     }

    virtual CGridCellBase* GetDefaultCell();

    virtual BOOL IsDefaultFont() const                      { return FALSE; }
    virtual BOOL IsEditing() const                          { return FALSE; }
    virtual BOOL IsFocused()  const                         { return (m_nState & GVIS_FOCUSED);  }
    virtual BOOL IsFixed()    const                         { return (m_nState & GVIS_FIXED);    }
    virtual BOOL IsFixedCol() const                         { return (m_nState & GVIS_FIXEDCOL); }
    virtual BOOL IsFixedRow() const                         { return (m_nState & GVIS_FIXEDROW); }
    virtual BOOL IsSelected() const                         { return (m_nState & GVIS_SELECTED); }
    virtual BOOL IsReadOnly() const                         { return (m_nState & GVIS_READONLY); }
    virtual BOOL IsModified() const                         { return (m_nState & GVIS_MODIFIED); }
    virtual BOOL IsDropHighlighted() const                  { return (m_nState & GVIS_DROPHILITED); }

// Operators
public:
    virtual void operator=(const CGridCellBase& cell);

// Operations
public:
    virtual void Reset();

    //virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
    virtual BOOL Draw();
    virtual BOOL GetTextRect( LPRECT pRect);    // i/o:  i=dims of cell rect; o=dims of text rect
    virtual BOOL GetTipTextRect( LPRECT pRect) { return GetTextRect( pRect); }  // may override for btns, etc.
    //virtual CSize GetTextExtent(LPCTSTR str, CDC* pDC = NULL);
    virtual SIZE_u GetTextExtent(LPCTSTR str);
    //virtual CSize GetCellExtent(CDC* pDC);
    virtual SIZE_u GetCellExtent();

    // Editing
    virtual BOOL Edit( int /* nRow */, int /* nCol */, RECT& /* rect */, POINT& /* point */, 
                       UINT /* nID */, UINT /* nChar */) { return FALSE;}
	virtual BOOL ValidateEdit(LPCTSTR str);
    virtual void EndEdit() {}

    // EFW - Added to print cells properly
    //virtual BOOL PrintCell(CDC* pDC, int nRow, int nCol, CRect rect);
    virtual BOOL PrintCell(int nRow, int nCol, RECT& rect);

    // add additional protected grid members required of cells
    LRESULT SendMessageToParent(int nRow, int nCol, int nMessage);

protected:
    virtual void OnEndEdit();
    virtual void OnMouseEnter();
    virtual void OnMouseOver();
    virtual void OnMouseLeave();
    virtual void OnClick(POINT PointCellRelative);
    virtual void OnClickDown(POINT PointCellRelative);
    virtual void OnRClick(POINT PointCellRelative);
    virtual void OnDblClick(POINT PointCellRelative);
    virtual BOOL OnSetCursor();

protected:
    DWORD    m_nState;      // Cell state (selected/focus etc)
};


#endif // __grid_cell_base_h__
