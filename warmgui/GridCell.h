#ifndef __warmgui_grid_cell_h__
#define __warmgui_grid_cell_h__


namespace WARMGUI {
class CGridCell :  public CGridCellBase
{
public:
// Construction/Destruction
    CGridCell(void);
    virtual ~CGridCell(void);

    friend class CGridCtrl;


// Attributes
public:
    void operator=(const CGridCell& cell);

    virtual void  SetText(LPCTSTR szText)        { m_strText = szText;  }                       
    virtual void  SetImage(int nImage)           { m_nImage = nImage;   }                        
    virtual void  SetData(LPARAM lParam)         { m_lParam = lParam;   }      
    virtual void  SetGrid(CGridCtrl* pGrid)      { m_pGrid = pGrid;     }                          
    // virtual void SetState(const DWORD nState);  -  use base class version   
    virtual void  SetFormat(DWORD nFormat)       { m_nFormat = nFormat; }                      
    virtual void  SetTextClr(COLORALPHA clr)     { m_crFgClr = clr;     }                          
    virtual void  SetBackClr(COLORALPHA clr)     { m_crBkClr = clr;     }                          
    virtual void  SetFont(IDWriteTextFormat* plf);
    virtual void  SetMargin(UINT nMargin)        { m_nMargin = nMargin; }
    //virtual CWnd* GetEditWnd() const             { return m_pEditWnd;   }
    virtual void  SetCoords( int /* nRow */, int /* nCol */) {}  // don't need to know the row and
                                                                // column for base implementation

    virtual LPCTSTR     GetText() const             { return (m_strText.empty())? _T("") : m_strText.c_str(); }
    virtual int         GetImage() const            { return m_nImage;  }
    virtual LPARAM      GetData() const             { return m_lParam;  }
    virtual CGridCtrl*  GetGrid() const             { return m_pGrid;   }
    // virtual DWORD    GetState() const - use base class
    virtual DWORD       GetFormat() const;
    virtual COLORALPHA  GetTextClr() const          { return m_crFgClr; } // TODO: change to use default cell
    virtual COLORALPHA  GetBackClr() const          { return m_crBkClr; }
    virtual IDWriteTextFormat*    GetFont() const;
    virtual UINT        GetMargin() const;

    virtual BOOL        IsEditing() const           { return m_bEditing; }
    virtual BOOL        IsDefaultFont() const       { return (m_pTextFont == NULL); }
    virtual void        Reset();

// editing cells
public:
    virtual BOOL Edit(int nRow, int nCol, RECT rect, POINT_u point, UINT nID, UINT nChar);
    virtual void EndEdit();
protected:
    virtual void OnEndEdit();

protected:
    tstring    m_strText;      // Cell text (or binary data if you wish...)
    LPARAM     m_lParam;       // 32-bit value to associate with item
    int        m_nImage;       // Index of the list view item’s icon
    DWORD      m_nFormat;
    COLORALPHA m_crFgClr;
    COLORALPHA m_crBkClr;
    IDWriteTextFormat*   m_pTextFont;
    UINT       m_nMargin;

    BOOL       m_bEditing;     // Cell being edited?

    CGridCtrl* m_pGrid;        // Parent grid control
    //CWnd*      m_pEditWnd;
};


// This class is for storing grid default values. It's a little heavy weight, so
// don't use it in bulk 
class CGridDefaultCell : public CGridCell
{
// Construction/Destruction
public:
    CGridDefaultCell();
    virtual ~CGridDefaultCell();

public:
    virtual DWORD GetStyle() const                      { return m_dwStyle;      }
    virtual void  SetStyle(DWORD dwStyle)               { m_dwStyle = dwStyle;   }
    virtual int   GetWidth() const                      { return m_Size.width;      }
    virtual int   GetHeight() const                     { return m_Size.height;      }
    virtual void  SetWidth(int nWidth)                  { m_Size.width = nWidth;    }
    virtual void  SetHeight(int nHeight)                { m_Size.height = nHeight;   }

    // Disable these properties
    virtual void  SetData(LPARAM /*lParam*/)             {          }      
    virtual void  SetState(DWORD /*nState*/)             {          }
    virtual DWORD GetState() const                       { return CGridCell::GetState()|GVIS_READONLY; }
    virtual void  SetCoords( int /*row*/, int /*col*/)   {          }
    virtual void  SetFont(IDWriteTextFormat* /*plf*/);
    virtual IDWriteTextFormat* GetFont() const;   

protected:
    SIZE_u m_Size;       // Default Size
    DWORD  m_dwStyle;    // Cell Style - unused
};


} //namespace WARMGUI

#endif //__warmgui_grid_cell_h__