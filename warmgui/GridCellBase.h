#if !defined( __warmgui_grid_cell_base_h__ )
#define __warmgui_grid_cell_base_h__


// The code contained in this file is based on the original
// WorldCom Grid control written by Joe Willcoxson,
//      mailto:chinajoe@aol.com
//      http://users.aol.com/chinajoe


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
    COLORALPHA   crBkClr;     // Background colour (or CLR_DEFAULT)
    COLORALPHA   crFgClr;     // Forground colour (or CLR_DEFAULT)
    LPARAM   lParam;      // 32-bit value to associate with item
    IDWriteTextFormat*  lfFont;      // Cell font
    UINT         nMargin;     // Internal cell margin
    std::wstring strText;     // Text in cell

    _GV_ITEM()
        : lfFont(0)
    { }
} GV_ITEM;


// Each cell contains one of these. Fields "row" and "column" are not stored since we
// will usually have acces to them in other ways, and they are an extra 8 bytes per
// cell that is probably unnecessary.

#ifndef WARMGUI_API
#   ifdef WARMGUI_EXPORTS
#       define WARMGUI_API __declspec(dllexport)
#   else
//#     define WARMGUI_API __declspec(dllimport)
#       define WARMGUI_API
#   endif //WARMGUI_EXPORTS
#endif //WARMGUI_API

namespace WARMGUI {

class CGridCtrl;

typedef struct UI_FORMAT_SETTING {
    COLORALPHA              _crGridBkColour, _crGridLineColour;
    COLORALPHA              _cr3DFace, _crShadow;
    COLORALPHA              _crTTipBackClr, _crTTipTextClr;                       // Titletip colours - FNA
    COLORALPHA              _crHighLightBkg, _crHighLightTxt;

    COLORALPHA              _crDefaultText, _crDefaultBkgText;
    COLORALPHA              _crFixColText, _crFixColBkgText;
    COLORALPHA              _crFixRowText, _crFixRowBkgText;
    COLORALPHA              _crFixRowColText, _crFixRowColBkgText;

    IDWriteTextFormat*      _pDefaultFont;
    IDWriteTextFormat*      _pFixedColFont;
    IDWriteTextFormat*      _pFixedRowFont;
    IDWriteTextFormat*      _pFixedRowColFont;

    UI_FORMAT_SETTING()
        : _pDefaultFont(0)
        , _pFixedColFont(0)
        , _pFixedRowFont(0)
        , _pFixedRowColFont(0)
    {
    }
    ~UI_FORMAT_SETTING()
    {
        SafeRelease(&_pDefaultFont);
        SafeRelease(&_pFixedRowFont);
        SafeRelease(&_pFixedColFont);
        SafeRelease(&_pFixedRowColFont);
    }
}UI_FORMAT_SETTING;

class CGridCellBase
{
public:
// Construction/Destruction
    CGridCellBase(void);
    virtual ~CGridCellBase(void);

    friend class CGridCtrl;

// Attributes
public:
    // can't do pure virtual because of DECLARE_DYNCREATE requirement
    //  use ASSERT() to require that programmer overrides all that should
    //  be pure virtuals

    virtual void SetText(LPCTSTR /* szText */)              {     }
    virtual void SetImage(int /* nImage */)                 {     }
    virtual void SetData(LPARAM /* lParam */)               {     }
    virtual void SetState(DWORD nState)                     { m_nState = nState; }
    virtual void SetFormat(DWORD /* nFormat */)             {     }
    virtual void SetTextClr(COLORALPHA   /* clr */)             {     }
    virtual void SetBackClr(COLORALPHA   /* clr */)             {     }
    virtual void SetFont(const IDWriteTextFormat* /* plf */)          {     }
    virtual void SetMargin( UINT /* nMargin */)             {     }
    virtual void SetGrid(CGridCtrl* /* pGrid */)        {     }
    virtual void SetCoords( int /* nRow */, int /* nCol */) {     }

    virtual LPCTSTR  GetText() const                        {  return NULL;  }
    virtual LPCTSTR  GetTipText() const                     { return GetText();             } // may override TitleTip return
    virtual int      GetImage() const                       {  return -1;    }
    virtual LPARAM   GetData() const                        {  return 0;     }
    virtual DWORD    GetState() const                       { return m_nState;              }
    virtual DWORD    GetFormat() const                      {  return 0;     }
    virtual COLORALPHA   GetTextClr() const                 {  return DEFAULT_COLOR_ALPHA;     }
    virtual COLORALPHA   GetBackClr() const                 {  return DEFAULT_COLOR_ALPHA;     }
    virtual IDWriteTextFormat* GetFont() const                        {  return NULL;  }
    virtual IDWriteTextFormat* GetFontObject() const                  {  return NULL;  }
    virtual CGridCtrl* GetGrid() const                  {  return NULL;  }
    //virtual CWnd*    GetEditWnd() const                   {  return NULL;  }
    virtual UINT     GetMargin() const                      {  return 0;     }

    virtual CGridCellBase* GetDefaultCell() const;

    virtual BOOL IsDefaultFont() const                      {  return FALSE; }
    virtual BOOL IsEditing() const                          {  return FALSE; }
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

    virtual BOOL Draw(int nRow, int nCol, RECT rect, BOOL bEraseBkgnd = TRUE);
    virtual BOOL GetTextRect( LPRECT pRect);    // i/o:  i=dims of cell rect; o=dims of text rect
    virtual BOOL GetTipTextRect( LPRECT pRect) { return GetTextRect( pRect); }  // may override for btns, etc.
    virtual SIZE_u GetTextExtent(LPCTSTR str);   //, CDC* pDC = NULL);
    virtual SIZE_u GetCellExtent();

    // Editing
    virtual BOOL Edit( int /* nRow */, int /* nCol */, RECT /* rect */, POINT_u /* point */, 
                       UINT /* nID */, UINT /* nChar */) {  return FALSE;}
	virtual BOOL ValidateEdit(LPCTSTR str);
    virtual void EndEdit() {}

    // EFW - Added to print cells properly
    //virtual BOOL PrintCell(CDC* pDC, int nRow, int nCol, RECT rect);

    // add additional protected grid members required of cells
    LRESULT SendMessageToParent(int nRow, int nCol, int nMessage);

protected:
    virtual void OnEndEdit();
    virtual void OnMouseEnter();
    virtual void OnMouseOver();
    virtual void OnMouseLeave();
    virtual void OnClick( POINT_u PointCellRelative);
    virtual void OnClickDown( POINT_u PointCellRelative);
    virtual void OnRClick( POINT_u PointCellRelative);
    virtual void OnDblClick( POINT_u PointCellRelative);
    virtual BOOL OnSetCursor();
    void         SetArtistFont(CGridCellBase* cell);
    void         SetArtistTextClr(CGridCellBase* cell, COLORALPHA& TextClr, COLORALPHA& TextBkClr);
protected:
    DWORD    m_nState;      // Cell state (selected/focus etc)

};

// storage typedef for each row in the grid
typedef std::vector<CGridCellBase*> GridRow;


//EXPORT_STL_VECTOR(WARMGUI_API, CGridCellBase*)
typedef std::vector<CGridCellBase*> GridRow;
typedef GridRow::iterator GridRowIter;
typedef GridRow::const_iterator GridRowConstIter;

class CGridRow : public GridRow
{
public:
     CGridRow() {}
    ~CGridRow() {}

    void InsertAt(int pos, CGridCellBase* gcb) {
        GridRowIter iter = begin();
        iter += pos;
        if (iter != end())
            insert(iter, gcb);
    }

    void RemoveAt(int pos) {
        GridRowIter iter = begin();
        iter += pos;
        if (iter != end())
            erase(iter);
    }
};

//EXPORT_STL_VECTOR(WARMGUI_API, CGridRow*)
typedef std::vector<CGridRow*> GridRowArray;
typedef GridRowArray::iterator GridRowArrayIter;
typedef GridRowArray::const_iterator GridRowArrayConstIter;


class CGridRowArray : public GridRowArray
{
public:
     CGridRowArray() {}
    ~CGridRowArray() {}

    void InsertAt(int pos, CGridRow* cba) {
        GridRowArrayIter iter = begin();
        iter += pos;
        if (iter != end())
            insert(iter, cba);
    }

    void RemoveAt(int pos) {
        GridRowArrayIter iter = begin();
        iter += pos;
        if (iter != end())
            erase(iter);
    }
};


} //namespace WARMGUI

#endif //__warmgui_grid_cell_base_h__