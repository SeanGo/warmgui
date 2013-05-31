#ifndef __warmgui_grid_control_h__
#define __warmgui_grid_control_h__



/////////////////////////////////////////////////////////////////////////////
// GridCtrl.h : header file
//
// MFC Grid Control - main header
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
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
// Defines - these determine the features (and the final size) of the final code
///////////////////////////////////////////////////////////////////////////////////

//#define GRIDCONTROL_NO_TITLETIPS   // Do not use titletips for cells with large data
//#define GRIDCONTROL_NO_DRAGDROP    // Do not use OLE drag and drop
//#define GRIDCONTROL_NO_CLIPBOARD   // Do not use clipboard routines


// Use this as the classname when inserting this control as a custom control
// in the MSVC++ dialog editor
#define GRIDCTRL_CLASSNAME    _T("MFCGridCtrl")  // Window class name
#define IDC_INPLACE_CONTROL   8                  // ID of inplace edit controls

#define GRIDCONTROL_NO_TITLETIPS
#define GRIDCONTROL_NO_DRAGDROP
#define GRIDCONTROL_NO_CLIPBOARD


///////////////////////////////////////////////////////////////////////////////////
// Conditional includes
///////////////////////////////////////////////////////////////////////////////////

#ifndef GRIDCONTROL_NO_TITLETIPS
#   include "TitleTip.h"
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP
#   include "GridDropTarget.h"
#   undef GRIDCONTROL_NO_CLIPBOARD     // Force clipboard functions on
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
#   include <afxole.h>
#endif


///////////////////////////////////////////////////////////////////////////////////
// Helper functions
///////////////////////////////////////////////////////////////////////////////////

// Handy functions
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

// Backwards compatibility for pre 2.20 grid versions
#define DDX_GridControl(pDX, nIDC, rControl)  DDX_Control(pDX, nIDC, rControl)     



///////////////////////////////////////////////////////////////////////////////////
// Defines
///////////////////////////////////////////////////////////////////////////////////

// Grid line/scrollbar selection
#define GVL_NONE                0L      // Neither
#define GVL_HORZ                1L      // Horizontal line or scrollbar
#define GVL_VERT                2L      // Vertical line or scrollbar
#define GVL_BOTH                3L      // Both

// Autosizing option
#define GVS_DEFAULT             0
#define GVS_HEADER              1       // Size using column fixed cells data only
#define GVS_DATA                2       // Size using column non-fixed cells data only
#define GVS_BOTH                3       // Size using column fixed and non-fixed

// Cell Searching options
#define GVNI_FOCUSED            0x0001
#define GVNI_SELECTED           0x0002
#define GVNI_DROPHILITED        0x0004
#define GVNI_READONLY           0x0008
#define GVNI_FIXED              0x0010
#define GVNI_MODIFIED           0x0020

#define GVNI_ABOVE              LVNI_ABOVE
#define GVNI_BELOW              LVNI_BELOW
#define GVNI_TOLEFT             LVNI_TOLEFT
#define GVNI_TORIGHT            LVNI_TORIGHT
#define GVNI_ALL                (LVNI_BELOW|LVNI_TORIGHT|LVNI_TOLEFT)
#define GVNI_AREA               (LVNI_BELOW|LVNI_TORIGHT)

// Hit test values (not yet implemented)
#define GVHT_DATA               0x0000
#define GVHT_TOPLEFT            0x0001
#define GVHT_COLHDR             0x0002
#define GVHT_ROWHDR             0x0004
#define GVHT_COLSIZER           0x0008
#define GVHT_ROWSIZER           0x0010
#define GVHT_LEFT               0x0020
#define GVHT_RIGHT              0x0040
#define GVHT_ABOVE              0x0080
#define GVHT_BELOW              0x0100

// Messages sent to the grid's parent (More will be added in future)
#define GVN_BEGINDRAG           LVN_BEGINDRAG        // LVN_FIRST-9
#define GVN_BEGINLABELEDIT      LVN_BEGINLABELEDIT   // LVN_FIRST-5
#define GVN_BEGINRDRAG          LVN_BEGINRDRAG
#define GVN_COLUMNCLICK         LVN_COLUMNCLICK
#define GVN_DELETEITEM          LVN_DELETEITEM
#define GVN_ENDLABELEDIT        LVN_ENDLABELEDIT     // LVN_FIRST-6
#define GVN_SELCHANGING         LVN_ITEMCHANGING
#define GVN_SELCHANGED          LVN_ITEMCHANGED
#define GVN_GETDISPINFO         LVN_GETDISPINFO 
#define GVN_ODCACHEHINT         LVN_ODCACHEHINT 


namespace WARMGUI {

///////////////////////////////////////////////////////////////////////////////////
// Structures
///////////////////////////////////////////////////////////////////////////////////

// This structure sent to Grid's parent in a WM_NOTIFY message
typedef struct tagNM_GRIDVIEW {
    NMHDR hdr;
    int   iRow;
    int   iColumn;
} NM_GRIDVIEW;

// This is sent to the Grid from child in-place edit controls
typedef struct tagGV_DISPINFO {
    NMHDR   hdr;
    GV_ITEM item;
} GV_DISPINFO;

// This is sent to the Grid from child in-place edit controls
typedef struct tagGV_CACHEHINT {
    NMHDR      hdr;
    CCellRange range;
} GV_CACHEHINT;


// For virtual mode callback
typedef BOOL (CALLBACK* GRIDCALLBACK)(GV_DISPINFO *, LPARAM);
class WARMGUI_API IGuiControl : public IDataGraph
{
public:
                               IGuiControl(const char* name) : IDataGraph(name), _config(0) {}
    virtual                   ~IGuiControl(){}
    // Generated message map functions
    //{{AFX_MSG(CGridCtrl)
    virtual void               OnPaint() {}
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype) { return GLYPH_CHANGED_TYPE_NONE; }
    virtual void            OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {}
    virtual void            OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {}
    virtual void            OnSize(UINT nType, int cx, int cy) {}
    virtual void            OnLButtonUp(UINT nFlags, POINT_u point) {}
    virtual void            OnLButtonDown(UINT nFlags, POINT_u point) {}
    virtual void            OnMouseMove(UINT nFlags, POINT_u point) {}
    virtual void            OnTimer(UINT_PTR nIDEvent) {}
    virtual UINT            OnGetDlgCode() {return (0);}
    virtual void            OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {}
	virtual void            OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {}
    virtual void            OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {}
    virtual void            OnLButtonDblClk(UINT nFlags, POINT_u point) {}
    virtual BOOL            OnEraseBkgnd() {return TRUE;}
    virtual void            OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {}
    virtual void            OnUpdateEditSelectAll() {}
    //}}AFX_MSG             
    virtual BOOL            OnSetCursor(UINT nHitTest, UINT message) {return TRUE;}

    virtual void            OnRButtonDown(UINT nFlags, POINT_u point) {}
    virtual void            OnRButtonUp(UINT nFlags, POINT_u point) {}    // EFW - Added
    virtual void            OnSysColorChange() {}

    virtual void            OnCaptureChanged() {}

#                           ifndef GRIDCONTROL_NO_CLIPBOARD
    virtual void            OnUpdateEditCopy(CCmdUI* pCmdUI) {}
    virtual void            OnUpdateEditCut(CCmdUI* pCmdUI) {}
    virtual void            OnUpdateEditPaste(CCmdUI* pCmdUI) {}
#                           endif

    virtual LRESULT         OnSetFont(WPARAM hFont, LPARAM lParam) {return (0);}
    virtual LRESULT         OnGetFont(WPARAM hFont, LPARAM lParam) {return (0);}
    virtual LRESULT         OnImeChar(WPARAM wCharCode, LPARAM lParam) {return (0);}
    virtual void            OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult) {}

    enum                    eMouseModes {
                                MOUSE_NOTHING, MOUSE_SELECT_ALL, MOUSE_SELECT_COL, MOUSE_SELECT_ROW,
                                MOUSE_SELECT_CELLS, MOUSE_SCROLLING_CELLS,
                                MOUSE_OVER_ROW_DIVIDE, MOUSE_SIZING_ROW,
                                MOUSE_OVER_COL_DIVIDE, MOUSE_SIZING_COL,
                                MOUSE_PREPARE_EDIT,
#                               ifndef GRIDCONTROL_NO_DRAGDROP
                                MOUSE_PREPARE_DRAG, MOUSE_DRAGGING
#                               endif
                            };
    enum                    ZOOM_FIX_TYPE {
                                ZOOM_FIX_NONE   = 0,
                                ZOOM_FIX_WIDTH  = 1,
                                ZOOM_FIX_HEIDHT = 2,
                                ZOOM_FIX_BOTH   = ZOOM_FIX_WIDTH | ZOOM_FIX_HEIDHT,
                            };
    inline void             SetConfig(CWarmguiConfig* config) {_config = config;}
    inline void             SetConfigStr(const char* canvas_config) {strcpy_s(_strconf, MAX_PATH, canvas_config);}

protected:
    char                    _strconf[MAX_PATH];
    CWarmguiConfig*         _config;
};

class WARMGUI_API CGridCtrl : public IGuiControl
{
public:
                            //CGridCtrl();
                            CGridCtrl(const char* name, int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
    virtual                 ~CGridCtrl(void);

    virtual HRESULT         Init();
    virtual HRESULT         DrawGraph(bool redraw = false);
    virtual void            SetRect(RECT& rect);

    friend class            CGridCell;
    friend class            CGridCellBase;

// Construction
public:

    BOOL                    Create(const RECT& rect, UINT nID, DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);

///////////////////////////////////////////////////////////////////////////////////
// Attributes
///////////////////////////////////////////////////////////////////////////////////
public:
    int                     GetRowCount() const                    { return _nRows; }
    int                     GetColumnCount() const                 { return _nCols; }
    int                     GetFixedRowCount() const               { return _nFixedRows; }
    int                     GetFixedColumnCount() const            { return _nFixedCols; }
    BOOL                    SetRowCount(int nRows = 10);
    BOOL                    SetColumnCount(int nCols = 10);
    BOOL                    SetFixedRowCount(int nFixedRows = 1);
    BOOL                    SetFixedColumnCount(int nFixedCols = 1);

    int                     GetRowHeight(int nRow) const;
    BOOL                    SetRowHeight(int row, int height);
    int                     GetColumnWidth(int nCol) const;
    BOOL                    SetColumnWidth(int col, int width);

    BOOL                    GetCellOrigin(int nRow, int nCol, POINT_u* p);
    BOOL                    GetCellOrigin(const CCellID& cell, POINT_u* p);
    BOOL                    GetCellRect(int nRow, int nCol, LPRECT pRect);
    BOOL                    GetCellRect(const CCellID& cell, LPRECT pRect);

    BOOL                    GetTextRect(const CCellID& cell, LPRECT pRect);
    BOOL                    GetTextRect(int nRow, int nCol, LPRECT pRect);

    CCellID                 GetCellFromPt(POINT_u point, BOOL bAllowFixedCellCheck = TRUE);

    int                     GetFixedRowHeight() const;
    int                     GetFixedColumnWidth() const;
    long                    GetVirtualWidth() const;
    long                    GetVirtualHeight() const;

    SIZE_u                  GetTextExtent(int nRow, int nCol, LPCTSTR str);
    // EFW - Get extent of current text in cell
    inline                  SIZE_u GetCellTextExtent(int nRow, int nCol)
                            { return GetTextExtent(nRow, nCol, GetItemText(nRow,nCol).c_str()); }

    void                    SetGridBkColor(COLORALPHA clr)         { _uiSetting._crGridBkColour = clr;           }
    COLORALPHA              GetGridBkColor() const               { return _uiSetting._crGridBkColour;          }
    void                    SetGridLineColor(COLORALPHA clr)       { _uiSetting._crGridLineColour = clr;         }
    COLORALPHA              GetGridLineColor() const             { return _uiSetting._crGridLineColour;        }

	void	                SetTitleTipBackClr(COLORALPHA clr = DEFAULT_COLOR_ALPHA) { _uiSetting._crTTipBackClr = clr;  }
	COLORALPHA              GetTitleTipBackClr()				            { return _uiSetting._crTTipBackClr; }
	void	                SetTitleTipTextClr(COLORALPHA clr = DEFAULT_COLOR_ALPHA) { _uiSetting._crTTipTextClr = clr;  }
	COLORALPHA              GetTitleTipTextClr()				            { return _uiSetting._crTTipTextClr; }

    // ***************************************************************************** //
    // These have been deprecated. Use GetDefaultCell and then set the colors
    void                    SetTextColor(COLORALPHA clr)      { _cellDefault.SetTextClr(clr);        }
    COLORALPHA              GetTextColor()                  { return _cellDefault.GetTextClr();    }
    void                    SetTextBkColor(COLORALPHA clr)    { _cellDefault.SetBackClr(clr);        }
    COLORALPHA              GetTextBkColor()                { return _cellDefault.GetBackClr();    }
    void                    SetFixedTextColor(COLORALPHA clr) { _cellFixedRowDef.SetTextClr(clr); 
                                                              _cellFixedColDef.SetTextClr(clr); 
                                                              _cellFixedRowColDef.SetTextClr(clr); }
    COLORALPHA              GetFixedTextColor() const       { return _cellFixedRowDef.GetTextClr(); }
    void                    SetFixedBkColor(COLORALPHA clr)   { _cellFixedRowDef.SetBackClr(clr); 
                                                              _cellFixedColDef.SetBackClr(clr); 
                                                              _cellFixedRowColDef.SetBackClr(clr); }
    COLORALPHA              GetFixedBkColor() const         { return _cellFixedRowDef.GetBackClr(); }
    void                    SetGridColor(COLORALPHA clr)      { SetGridLineColor(clr);                }
    COLORALPHA              GetGridColor()                  { return GetGridLineColor();            }
    void                    SetBkColor(COLORALPHA clr)        { SetGridBkColor(clr);                  }
    COLORALPHA              GetBkColor()                    { return GetGridBkColor();              }

    void                    SetDefCellMargin( int nMargin)  { _cellDefault.SetMargin(nMargin); 
                                                              _cellFixedRowDef.SetMargin(nMargin); 
                                                              _cellFixedColDef.SetMargin(nMargin); 
                                                              _cellFixedRowColDef.SetMargin(nMargin); }
    int                     GetDefCellMargin() const        { return _cellDefault.GetMargin();     }

    int                     GetDefCellHeight() const        { return _cellDefault.GetHeight();     }
    void                    SetDefCellHeight(int nHeight)   { _cellDefault.SetHeight(nHeight); 
                                                              _cellFixedRowDef.SetHeight(nHeight); 
                                                              _cellFixedColDef.SetHeight(nHeight); 
                                                              _cellFixedRowColDef.SetHeight(nHeight); }
    int                     GetDefCellWidth() const         { return _cellDefault.GetWidth();     }
    void                    SetDefCellWidth(int nWidth)     { _cellDefault.SetWidth(nWidth); 
                                                              _cellFixedRowDef.SetWidth(nWidth); 
                                                              _cellFixedColDef.SetWidth(nWidth); 
                                                              _cellFixedRowColDef.SetWidth(nWidth); }

    // ***************************************************************************** //

    int                     GetSelectedCount() const          { return (int)_SelectedCellMap.size(); }

    CCellID                 SetFocusCell(CCellID cell);
    CCellID                 SetFocusCell(int nRow, int nCol);
    CCellID                 GetFocusCell() const              { return _idCurrentCell;           }


    void                    SetVirtualMode(BOOL bVirtual);
    BOOL                    GetVirtualMode() const            { return _bVirtualMode;            }
    void                    SetCallbackFunc(GRIDCALLBACK pCallback, LPARAM lParam)           { _pfnCallback = pCallback; _lParam = lParam; }
    GRIDCALLBACK            GetCallbackFunc()                 { return _pfnCallback;             }


    //void                  SetImageList(CImageList* pList)   { _pImageList = pList;             }
    //CImageList*           GetImageList() const              { return _pImageList;              }

    void                    SetGridLines(int nWhichLines = GVL_BOTH);
    int                     GetGridLines() const                     { return _nGridLines;              }
    void                    SetEditable(BOOL bEditable = TRUE)       { _bEditable = bEditable;          }
    BOOL                    IsEditable() const                       { return _bEditable;               }
    void                    SetListMode(BOOL bEnableListMode = TRUE);
    BOOL                    GetListMode() const                      { return _bListMode;               }
    void                    SetSingleRowSelection(BOOL bSing = TRUE) { _bSingleRowSelection = bSing;    }
    BOOL                    GetSingleRowSelection()                  { return _bSingleRowSelection & _bListMode; }
    void                    SetSingleColSelection(BOOL bSing = TRUE) { _bSingleColSelection = bSing;    }
    BOOL                    GetSingleColSelection()                  { return _bSingleColSelection;     }
    void                    EnableSelection(BOOL bEnable = TRUE)     { ResetSelectedRange(); _bEnableSelection = bEnable; ResetSelectedRange(); }
    BOOL                    IsSelectable() const                     { return _bEnableSelection;        }
    void                    SetFixedColumnSelection(BOOL bSelect)    { _bFixedColumnSelection = bSelect;}
    BOOL                    GetFixedColumnSelection()                { return _bFixedColumnSelection;   }
    void                    SetFixedRowSelection(BOOL bSelect)       { _bFixedRowSelection = bSelect;   }
    BOOL                    GetFixedRowSelection()                   { return _bFixedRowSelection;      }
    void                    EnableDragAndDrop(BOOL bAllow = TRUE)    { _bAllowDragAndDrop = bAllow;     }
    BOOL                    GetDragAndDrop() const                   { return _bAllowDragAndDrop;       }
    void                    SetRowResize(BOOL bResize = TRUE)        { _bAllowRowResize = bResize;      }
    BOOL                    GetRowResize() const                     { return _bAllowRowResize;         }
    void                    SetColumnResize(BOOL bResize = TRUE)     { _bAllowColumnResize = bResize;   }
    BOOL                    GetColumnResize() const                  { return _bAllowColumnResize;      }
    void                    SetHeaderSort(BOOL bSortOnClick = TRUE)  { _bSortOnClick = bSortOnClick;    }
    BOOL                    GetHeaderSort() const                    { return _bSortOnClick;            }
    void                    SetHandleTabKey(BOOL bHandleTab = TRUE)  { _bHandleTabKey = bHandleTab;     }
    BOOL                    GetHandleTabKey() const                  { return _bHandleTabKey;           }
    void                    SetDoubleBuffering(BOOL bBuffer = TRUE)  { _bDoubleBuffer = bBuffer;        }
    BOOL                    GetDoubleBuffering() const               { return _bDoubleBuffer;           }
    void                    EnableTitleTips(BOOL bEnable = TRUE)     { _bTitleTips = bEnable;           }
    BOOL                    GetTitleTips()                           { return _bTitleTips;              }
    void                    SetSortColumn(int nCol);
    int                     GetSortColumn() const                    { return _nSortColumn;             }
    void                    SetSortAscending(BOOL bAscending)        { _bAscending = bAscending;        }
    BOOL                    GetSortAscending() const                 { return _bAscending;              }
    void                    SetTrackFocusCell(BOOL bTrack)           { _bTrackFocusCell = bTrack;       }
    BOOL                    GetTrackFocusCell()                      { return _bTrackFocusCell;         }
    void                    SetFrameFocusCell(BOOL bFrame)           { _bFrameFocus = bFrame;           }
    BOOL                    GetFrameFocusCell()                      { return _bFrameFocus;             }
    void                    SetAutoSizeStyle(int nStyle = GVS_BOTH)  { _nAutoSizeColumnStyle = nStyle;  }
    int                     GetAutoSizeStyle()                       { return _nAutoSizeColumnStyle; }

    void                    EnableHiddenColUnhide(BOOL bEnable = TRUE){ _bHiddenColUnhide = bEnable;    }
    BOOL                    GetHiddenColUnhide()                     { return _bHiddenColUnhide;        }
    void                    EnableHiddenRowUnhide(BOOL bEnable = TRUE){ _bHiddenRowUnhide = bEnable;    }
    BOOL                    GetHiddenRowUnhide()                     { return _bHiddenRowUnhide;        }

    void                    EnableColumnHide(BOOL bEnable = TRUE)    { _bAllowColHide = bEnable;        }
    BOOL                    GetColumnHide()                          { return _bAllowColHide;           }
    void                    EnableRowHide(BOOL bEnable = TRUE)       { _bAllowRowHide = bEnable;        }
    BOOL                    GetRowHide() { return _bAllowRowHide;           }

    UI_FORMAT_SETTING&      GetUiSetting() { return _uiSetting; }

///////////////////////////////////////////////////////////////////////////////////
// default Grid cells. Use these for setting default values such as colors and fonts
///////////////////////////////////////////////////////////////////////////////////
public:
    CGridCellBase*          GetDefaultCell(BOOL bFixedRow, BOOL bFixedCol) const;

///////////////////////////////////////////////////////////////////////////////////
// Grid cell Attributes
///////////////////////////////////////////////////////////////////////////////////
public:
    CGridCellBase*          GetCell(int nRow, int nCol) const;   // Get the actual cell!

    void                    SetModified(BOOL bModified = TRUE, int nRow = -1, int nCol = -1);
    BOOL                    GetModified(int nRow = -1, int nCol = -1);
    BOOL                    IsCellFixed(int nRow, int nCol);

    BOOL                    SetItem(const GV_ITEM* pItem);
    BOOL                    GetItem(GV_ITEM* pItem);
    BOOL                    SetItemText(int nRow, int nCol, LPCTSTR str);
    // The following was virtual. If you want to override, use 
    //  CGridCellBase-derived class's GetText() to accomplish same thing
    tstring                 GetItemText(int nRow, int nCol) const;

    BOOL                    SetItemTextFmt(int nRow, int nCol, LPCTSTR szFmt, ...);
    BOOL                    SetItemTextFmtID(int nRow, int nCol, UINT nID, ...);

    BOOL                    SetItemData(int nRow, int nCol, LPARAM lParam);
    LPARAM                  GetItemData(int nRow, int nCol) const;
    BOOL                    SetItemImage(int nRow, int nCol, int iImage);
    int                     GetItemImage(int nRow, int nCol) const;
    BOOL                    SetItemState(int nRow, int nCol, UINT state);
    UINT                    GetItemState(int nRow, int nCol) const;
    BOOL                    SetItemFormat(int nRow, int nCol, UINT nFormat);
    UINT                    GetItemFormat(int nRow, int nCol) const;
    BOOL                    SetItemBkColour(int nRow, int nCol, COLORALPHA cr = DEFAULT_COLOR_ALPHA);
    COLORALPHA              GetItemBkColour(int nRow, int nCol) const;
    BOOL                    SetItemFgColour(int nRow, int nCol, COLORALPHA cr = DEFAULT_COLOR_ALPHA);
    COLORALPHA              GetItemFgColour(int nRow, int nCol) const;
    BOOL                    SetItemFont(int nRow, int nCol, const IDWriteTextFormat* lf);
    const IDWriteTextFormat*          GetItemFont(int nRow, int nCol);

    BOOL                    IsItemEditing(int nRow, int nCol);

    BOOL                    SetCellType(int nRow, int nCol);
    BOOL                    SetDefaultCellType();

///////////////////////////////////////////////////////////////////////////////////
// Operations
///////////////////////////////////////////////////////////////////////////////////
public:
    int                     InsertColumn(LPCTSTR strHeading, UINT nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE, int nColumn = -1);
    int                     InsertRow(LPCTSTR strHeading, int nRow = -1);
    BOOL                    DeleteColumn(int nColumn);
    BOOL                    DeleteRow(int nRow);
    BOOL                    DeleteNonFixedRows();
    BOOL                    DeleteAllItems();

	void                    ClearCells(CCellRange Selection);

    BOOL                    AutoSizeRow(int nRow, BOOL bResetScroll = TRUE);
    BOOL                    AutoSizeColumn(int nCol, UINT nAutoSizeStyle = GVS_DEFAULT, BOOL bResetScroll = TRUE);
    void                    AutoSizeRows();
    void                    AutoSizeColumns(UINT nAutoSizeStyle = GVS_DEFAULT);
    void                    AutoSize(UINT nAutoSizeStyle = GVS_DEFAULT);
    void                    ExpandColumnsToFit(BOOL bExpandFixed = TRUE);
    void                    ExpandLastColumn();
    void                    ExpandRowsToFit(BOOL bExpandFixed = TRUE);
    void                    ExpandToFit(BOOL bExpandFixed = TRUE);

    void                    Refresh();
    void                    AutoFill();   // Fill grid with blank cells

    void                    EnsureVisible(CCellID &cell)       { EnsureVisible(cell.row, cell.col); }
    void                    EnsureVisible(int nRow, int nCol);
    BOOL                    IsCellVisible(int nRow, int nCol);
    BOOL                    IsCellVisible(CCellID cell);
    BOOL                    IsCellEditable(int nRow, int nCol) const;
    BOOL                    IsCellEditable(CCellID &cell) const;
    BOOL                    IsCellSelected(int nRow, int nCol) const;
    BOOL                    IsCellSelected(CCellID &cell) const;

    // SetRedraw stops/starts redraws on things like changing the # rows/columns
    // and autosizing, but not for user-intervention such as resizes
    void                    SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars = FALSE);
    BOOL                    RedrawCell(int nRow, int nCol);     //, CDC* pDC = NULL);
    BOOL                    RedrawCell(const CCellID& cell);//, CDC* pDC = NULL);
    BOOL                    RedrawRow(int row);
    BOOL                    RedrawColumn(int col);

    BOOL                    Save(LPCTSTR filename, TCHAR chSeparator = _T(','));
    BOOL                    Load(LPCTSTR filename, TCHAR chSeparator = _T(','));

///////////////////////////////////////////////////////////////////////////////////
// Cell Ranges
///////////////////////////////////////////////////////////////////////////////////
 public:
    CCellRange              GetCellRange() const;
    CCellRange              GetSelectedCellRange() const;
    void                    SetSelectedRange(const CCellRange& Range, BOOL bForceRepaint = FALSE, BOOL bSelectCells = TRUE);
    void                    SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol, BOOL bForceRepaint = FALSE, BOOL bSelectCells = TRUE);
    BOOL                    IsValid(int nRow, int nCol) const;
    BOOL                    IsValid(const CCellID& cell) const;
    BOOL                    IsValid(const CCellRange& range) const;

///////////////////////////////////////////////////////////////////////////////////
// Clipboard, drag and drop, and cut n' paste operations
///////////////////////////////////////////////////////////////////////////////////
#ifndef GRIDCONTROL_NO_CLIPBOARD
    virtual void            CutSelectedText();
    virtual COleDataSource* CopyTextFromGrid();
    virtual BOOL            PasteTextToGrid(CCellID cell, COleDataObject* pDataObject);
#                           endif

#                           ifndef GRIDCONTROL_NO_DRAGDROP
 public:                    
    virtual void            OnBeginDrag();
    virtual DROPEFFECT      OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, POINT_u point);
    virtual DROPEFFECT      OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, POINT_u point);
    virtual void            OnDragLeave();
    virtual BOOL            OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, POINT_u point);
#                           endif

#                           ifndef GRIDCONTROL_NO_CLIPBOARD
    virtual void            OnEditCut();
    virtual void            OnEditCopy();
    virtual void            OnEditPaste();
#                           endif
    virtual void            OnEditSelectAll();

///////////////////////////////////////////////////////////////////////////////////
// Misc.
///////////////////////////////////////////////////////////////////////////////////
public:
    CCellID                 GetNextItem(CCellID& cell, int nFlags) const;

	BOOL                    SortItems(int nCol, BOOL bAscending, LPARAM data = 0);
    BOOL                    SortTextItems(int nCol, BOOL bAscending, LPARAM data = 0);
    BOOL                    SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data = 0);

	void                    SetCompareFunction(PFNLVCOMPARE pfnCompare);

	// in-built sort functions
	static int CALLBACK     pfnCellTextCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK     pfnCellNumericCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

///////////////////////////////////////////////////////////////////////////////////
// Printing
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
// Printing overrides for derived classes
///////////////////////////////////////////////////////////////////////////////////

// Implementation
protected:
    BOOL                    Initialise();
    void                    SetupDefaultCells();

    LRESULT                 SendMessageToParent(int nRow, int nCol, int nMessage) const;
    LRESULT                 SendDisplayRequestToParent(GV_DISPINFO* pDisplayInfo) const;
    LRESULT                 SendCacheHintToParent(const CCellRange& range) const;

    BOOL                    InvalidateCellRect(const int row, const int col);
    BOOL                    InvalidateCellRect(const CCellID& cell);
    BOOL                    InvalidateCellRect(const CCellRange& cellRange);
    void                    EraseBkgnd();

    BOOL                    GetCellRangeRect(const CCellRange& cellRange, LPRECT lpRect);

    BOOL                    SetCell(int nRow, int nCol, CGridCellBase* pCell);

    int                     SetMouseMode(int nMode) { int nOldMode = _MouseMode; _MouseMode = nMode; return nOldMode; }
    int                     GetMouseMode() const    { return _MouseMode; }

    BOOL                    MouseOverRowResizeArea(POINT_u& point);
    BOOL                    MouseOverColumnResizeArea(POINT_u& point);

    CCellID                 GetTopleftNonFixedCell(BOOL bForceRecalculation = FALSE);
    CCellRange              GetUnobstructedNonFixedCellRange(BOOL bForceRecalculation = FALSE);
    CCellRange              GetVisibleNonFixedCellRange(LPRECT pRect = NULL, BOOL bForceRecalculation = FALSE);

    ////BOOL                IsVisibleVScroll() { return ( (_nBarState & GVL_VERT) > 0); } 
    ////BOOL                IsVisibleHScroll() { return ( (_nBarState & GVL_HORZ) > 0); }
    void                    ResetSelectedRange();
    ////void                ResetScrollBars();
    ////void                EnableScrollBars(int nBar, BOOL bEnable = TRUE);
    ////int                 GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
    ////BOOL                SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);

    BOOL                    SortTextItems(int nCol, BOOL bAscending, int low, int high);
    BOOL                    SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high);

    POINT_u                 GetPointClicked(int nRow, int nCol, const POINT_u& point);

	void                    ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText);


#                           ifndef GRIDCONTROL_NO_DRAGDROP
// Drag n' drop
    virtual CImageList*     CreateDragImage(POINT_u *pHotSpot);    // no longer necessary
#                           endif

    // Mouse Clicks         
    virtual void            OnFixedColumnClick(CCellID& cell);
    virtual void            OnFixedRowClick(CCellID& cell);

    // Editing              
    virtual void            OnEditCell(int nRow, int nCol, POINT_u point, UINT nChar);
    virtual void            OnEndEditCell(int nRow, int nCol, tstring str);
	virtual BOOL            ValidateEdit(int nRow, int nCol, LPCTSTR str);
    virtual void            EndEditing();

    // Drawing              
    //virtual void          OnDraw();

    // CGridCellBase Creation and Cleanup
    virtual CGridCellBase*  CreateCell(int nRow, int nCol);
    virtual void            DestroyCell(int nRow, int nCol);

// Attributes               
protected:                  
    // General attributes   
    UI_FORMAT_SETTING       _uiSetting;
    BOOL                    _bVirtualMode;
    LPARAM                  _lParam;              // lParam for callback
    GRIDCALLBACK            _pfnCallback;         // The callback function

    int                     _nGridLines;
    BOOL                    _bEditable;
    BOOL                    _bModified;
    BOOL                    _bAllowDragAndDrop;
    BOOL                    _bListMode;
    BOOL                    _bSingleRowSelection;
    BOOL                    _bSingleColSelection;
    BOOL                    _bAllowDraw;
    BOOL                    _bEnableSelection;
    BOOL                    _bFixedRowSelection, _bFixedColumnSelection;
    BOOL                    _bSortOnClick;
    BOOL                    _bHandleTabKey;
    BOOL                    _bDoubleBuffer;
    BOOL                    _bTitleTips;
    int                     _nBarState;
    BOOL                    _bWysiwygPrinting;
    BOOL                    _bHiddenColUnhide, _bHiddenRowUnhide;
    BOOL                    _bAllowColHide, _bAllowRowHide;
    BOOL                    _bAutoSizeSkipColHdr;
    BOOL                    _bTrackFocusCell;
    BOOL                    _bFrameFocus;
    UINT                    _nAutoSizeColumnStyle;
    ZOOM_FIX_TYPE           _zoom_fix;
    // Cell size details    
    int                     _nRows, _nFixedRows, _nCols, _nFixedCols;
    CIntArray               _arRowHeights, _arColWidths;
    int                     _nVScrollMax, _nHScrollMax;

    // Fonts and images     
    CGridDefaultCell        _cellDefault;  // "default" cell. Contains default colours, font etc.
    CGridDefaultCell        _cellFixedColDef, _cellFixedRowDef, _cellFixedRowColDef;
    //CImageList*           _pImageList;

    // Cell data            
    CGridRowArray           _RowData;

    // Mouse operations such as cell selection
    int                     _MouseMode;
    BOOL                    _bLMouseButtonDown, _bRMouseButtonDown;
    POINT_u                 _LeftClickDownPoint, _LastMousePoint;
    CCellID                 _LeftClickDownCell, _SelectionStartCell;
    CCellID                 _idCurrentCell, _idTopLeftCell;
    int                     _nTimerID;
    int                     _nTimerInterval;
    int                     _nResizeCaptureRange;
    BOOL                    _bAllowRowResize, _bAllowColumnResize;
    int                     _nRowsPerWheelNotch;
    CCellIDMap              _SelectedCellMap;
    CCellIDMap              _PrevSelectedCellMap;

#                           ifndef GRIDCONTROL_NO_TITLETIPS
    CTitleTip               _TitleTip;             // Title tips for cells
#                           endif

    // Drag and drop        
    CCellID                 _LastDragOverCell;
#                           ifndef GRIDCONTROL_NO_DRAGDROP
    CGridDropTarget         _DropTarget;       // OLE Drop target for the grid
#                           endif

    // Printing information 
    SIZE_u                  _CharSize;
    int                     _nPageHeight;
    SIZE_u                  _LogicalPageSize;      // Page size in gridctrl units.
    SIZE_u                  _PaperSize;            // Page size in device units.
    // additional properties to support Wysiwyg printing
    int                     _nPageWidth;
    int                     _nPrintColumn;
    int                     _nCurrPrintRow;
    int                     _nNumPages;
    int                     _nPageMultiplier;

    // sorting              
    int                     _bAscending;
    int                     _nSortColumn;
	PFNLVCOMPARE            _pfnCompare;

    MARGIN                  _margin;
    //COLORALPHA              _clr_dark ;

    // EFW - Added to support shaded/unshaded printout.  If true, colored
    // cells will print as-is.  If false, all text prints as black on white.
    BOOL                    _bShadedPrintOut;

    // EFW - Added support for user-definable margins.  Top and bottom are in 
    // lines.  Left, right, and gap are in characters (avg width is used).
    int                     _nHeaderHeight, _nFooterHeight, _nLeftMargin,
                            _nRightMargin , _nTopMargin, _nBottomMargin , _nGap;

protected:                  

    // Generated message map functions
    //{{AFX_MSG(CGridCtrl)
    virtual void            OnPaint();
    virtual void            OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    virtual void            OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    virtual void            OnSize(UINT nType, int cx, int cy);
    virtual void            OnLButtonUp(UINT nFlags, POINT_u point);
    virtual void            OnLButtonDown(UINT nFlags, POINT_u point);
    virtual void            OnMouseMove(UINT nFlags, POINT_u point);
    virtual void            OnTimer(UINT_PTR nIDEvent);
    virtual UINT            OnGetDlgCode();
    virtual void            OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void            OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual void            OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual void            OnLButtonDblClk(UINT nFlags, POINT_u point);
    virtual BOOL            OnEraseBkgnd();
    virtual void            OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual void            OnUpdateEditSelectAll();
    //}}AFX_MSG             
    virtual BOOL            OnSetCursor(UINT nHitTest, UINT message);

    virtual void            OnRButtonDown(UINT nFlags, POINT_u point);
    virtual void            OnRButtonUp(UINT nFlags, POINT_u point);    // EFW - Added
    virtual void            OnSysColorChange();

    virtual void            OnCaptureChanged();

#                           ifndef GRIDCONTROL_NO_CLIPBOARD
    virtual void            OnUpdateEditCopy(CCmdUI* pCmdUI);
    virtual void            OnUpdateEditCut(CCmdUI* pCmdUI);
    virtual void            OnUpdateEditPaste(CCmdUI* pCmdUI);
#                           endif

    virtual LRESULT         OnSetFont(WPARAM hFont, LPARAM lParam);
    virtual LRESULT         OnGetFont(WPARAM hFont, LPARAM lParam);
    virtual LRESULT         OnImeChar(WPARAM wCharCode, LPARAM lParam);
    virtual void            OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult);

    enum                    eMouseModes {
                                MOUSE_NOTHING, MOUSE_SELECT_ALL, MOUSE_SELECT_COL, MOUSE_SELECT_ROW,
                                MOUSE_SELECT_CELLS, MOUSE_SCROLLING_CELLS,
                                MOUSE_OVER_ROW_DIVIDE, MOUSE_SIZING_ROW,
                                MOUSE_OVER_COL_DIVIDE, MOUSE_SIZING_COL,
                                MOUSE_PREPARE_EDIT,
#                               ifndef GRIDCONTROL_NO_DRAGDROP
                                MOUSE_PREPARE_DRAG, MOUSE_DRAGGING
#                               endif
                            };

    void                    SelectAllCells();
    void                    SelectColumns(CCellID currentCell, BOOL bForceRedraw=FALSE, BOOL bSelectCells=TRUE);
    void                    SelectRows(CCellID currentCell, BOOL bForceRedraw=FALSE, BOOL bSelectCells=TRUE);
    void                    SelectCells(CCellID currentCell, BOOL bForceRedraw=FALSE, BOOL bSelectCells=TRUE);
    void                    OnSelecting(const CCellID& currentCell);


private:
    //set class name, by IObject
    virtual void            setClass() { SetMyClass("CGridCtrl"); }

};

// Returns the default cell implementation for the given grid region
inline CGridCellBase* CGridCtrl::GetDefaultCell(BOOL bFixedRow, BOOL bFixedCol) const
{ 
    if (bFixedRow && bFixedCol) return (CGridCellBase*) &_cellFixedRowColDef;
    if (bFixedRow)              return (CGridCellBase*) &_cellFixedRowDef;
    if (bFixedCol)              return (CGridCellBase*) &_cellFixedColDef;
    return (CGridCellBase*) &_cellDefault;
}

inline CGridCellBase* CGridCtrl::GetCell(int nRow, int nCol) const
{
    if (nRow < 0 || nRow >= _nRows || nCol < 0 || nCol >= _nCols) 
        return NULL;

    if (GetVirtualMode())
    {
        CGridCellBase* pCell = GetDefaultCell(nRow < _nFixedRows, nCol < _nFixedCols);
        static GV_DISPINFO gvdi;
        gvdi.item.row     = nRow;
        gvdi.item.col     = nCol;
        gvdi.item.mask    = 0xFFFFFFFF;
        gvdi.item.nState  = 0;
        gvdi.item.nFormat = pCell->GetFormat();
        gvdi.item.iImage  = pCell->GetImage();
        gvdi.item.crBkClr = pCell->GetBackClr();
        gvdi.item.crFgClr = pCell->GetTextClr();
        gvdi.item.lParam  = pCell->GetData();
        memcpy(&gvdi.item.lfFont, pCell->GetFont(), sizeof(LOGFONT));
        gvdi.item.nMargin = pCell->GetMargin();
        gvdi.item.strText.clear();

        // Fix the state bits
        if (IsCellSelected(nRow, nCol))   gvdi.item.nState |= GVIS_SELECTED;
        if (nRow < GetFixedRowCount())    gvdi.item.nState |= (GVIS_FIXED | GVIS_FIXEDROW);
        if (nCol < GetFixedColumnCount()) gvdi.item.nState |= (GVIS_FIXED | GVIS_FIXEDCOL);
        if (GetFocusCell() == CCellID(nRow, nCol)) gvdi.item.nState |= GVIS_FOCUSED;
        
        if (_pfnCallback)
            _pfnCallback(&gvdi, _lParam);
        else
            SendDisplayRequestToParent(&gvdi);

        static CGridCell cell;

        cell.SetState(gvdi.item.nState);
        cell.SetFormat(gvdi.item.nFormat);
        cell.SetImage(gvdi.item.iImage);
        cell.SetBackClr(gvdi.item.crBkClr);
        cell.SetTextClr(gvdi.item.crFgClr);
        cell.SetData(gvdi.item.lParam);
        cell.SetFont(gvdi.item.lfFont);
        cell.SetMargin(gvdi.item.nMargin);
        cell.SetText(gvdi.item.strText.c_str());
        cell.SetGrid((CGridCtrl*)this);

        return (CGridCellBase*) &cell;
    }

    GridRow* pRow = _RowData[nRow];
    if (!pRow) return NULL;
    return (*pRow)[nCol];
}

inline BOOL CGridCtrl::SetCell(int nRow, int nCol, CGridCellBase* pCell)
{
    if (GetVirtualMode())
        return FALSE;

    if (nRow < 0 || nRow >= _nRows || nCol < 0 || nCol >= _nCols) 
        return FALSE;

    GridRow* pRow = _RowData[nRow];
    if (!pRow) return FALSE;

    pCell->SetCoords( nRow, nCol); 
    //pRow->SetAt(nCol, pCell);
    (*pRow)[nCol] = pCell;

    return TRUE;
}






} //namespace WARMGUI
#endif //__warmgui_grid_control_h__
