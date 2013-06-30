#ifndef __warmgui_type_summer_h__
#define __warmgui_type_summer_h__

template<class Interface>
inline void
SafeRelease(
    Interface **ppInterfaceToRelease
    )
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


#define EXPORT_STL_VECTOR( dllmacro, vectype ) \
  template class dllmacro std::allocator< vectype >; \
  template class dllmacro std::vector<vectype, \
    std::allocator< vectype > >;

#define EXPORT_STRUCT_VECTOR( dllmacro, vectype ) \
  template struct dllmacro std::allocator< vectype >; \
  template struct dllmacro std::vector<vectype, \
    std::allocator< vectype > >;


enum GLYPH_OWN_WORLD_TYPE {
    GLYPH_OWN_WORLD_TYPE_OUTSIDE,
    GLYPH_OWN_WORLD_TYPE_MYSELF,
};

typedef D2D1_POINT_2F     POINT_f;
typedef D2D1_ELLIPSE      ELLIPSE_f;
typedef D2D1_RECT_F	      RECT_f;
typedef D2D1_MATRIX_3X2_F MATRIX_2D;
typedef ID2D1Bitmap       WGBitmap;
typedef D2D1_SIZE_F       SIZE_f;
typedef D2D1_COLOR_F      COLORALPHA;

/*
typedef struct POINTF {
    float x;
    float y;
} POINTF;
*/

typedef RECT              MARGIN;
typedef RECT         RULER_WIDTH;
typedef unsigned int    uint32_t;



typedef struct FONT {
    TCHAR                       fontName[64];
    float                       fontSize;
	DWRITE_FONT_WEIGHT          fontWeight /*= DWRITE_FONT_WEIGHT_NORMAL */;
	DWRITE_FONT_STYLE           fontStyle  /*= DWRITE_FONT_STYLE_NORMAL  */;
	DWRITE_FONT_STRETCH         fontStretch/*= DWRITE_FONT_STRETCH_NORMAL*/;
	DWRITE_TEXT_ALIGNMENT       textAlignment /*= DWRITE_TEXT_ALIGNMENT_CENTER*/;
	DWRITE_PARAGRAPH_ALIGNMENT  paragraphAlignment /*= DWRITE_PARAGRAPH_ALIGNMENT_CENTER*/;
    TCHAR                       localeName[64];

    FONT()
        : fontWeight (DWRITE_FONT_WEIGHT_NORMAL)
        , fontStyle  (DWRITE_FONT_STYLE_NORMAL )
        , fontStretch(DWRITE_FONT_STRETCH_NORMAL)
        , textAlignment(DWRITE_TEXT_ALIGNMENT_CENTER)
        , paragraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER)
        , fontSize(16)
    {
        _tcscpy_s(localeName, 64, L"en-us");
        *fontName = L'\0';
    }

} FONT;


enum GLYPH_STATE_TYPE {
	 GLYPH_STATE_NORMAL,
	 GLYPH_STATE_OVER,
	 GLYPH_STATE_SELECTED,
	 GLYPH_STATUS_MOVING
};

enum TOOLBAR_POSITION {
	TOOLBAR_POSITION_TOP,
	TOOLBAR_POSITION_LEFT,
	TOOLBAR_POSITION_RIGHT,
	TOOLBAR_POSITION_BOTTOM,
};

enum DATA_BREADTH_TYPE {
    DATA_BREADTH_TYPE_PERCENT,
    DATA_BREADTH_TYPE_VALUE,
};


EXPORT_STL_VECTOR(WARMGUI_API, std::wstring)
typedef std::vector<std::wstring> WStringArray;
typedef WStringArray::iterator WStrIter;
typedef WStringArray::const_iterator WStrConstIter;

EXPORT_STL_VECTOR(WARMGUI_API, HWND)
typedef std::vector<HWND> HwndArray;
typedef HwndArray::iterator HwndIter;
typedef HwndArray::const_iterator HwndConstIter;

EXPORT_STL_VECTOR(WARMGUI_API, std::string)
typedef std::vector<std::string> StringArray;
typedef StringArray::iterator StrIter;
typedef StringArray::const_iterator StrConstIter;

typedef struct ValueIncrease {
	float           _min_decres_mag;
	float           _max_incres_mag;
	DATA_BREADTH_TYPE _breadth_type;
    float               _init_width;    //the width of world
    float         _hold_right_space;
    bool               _b_fix_width;

    ValueIncrease()
        : _min_decres_mag(0)
        , _max_incres_mag(0)
        , _breadth_type(DATA_BREADTH_TYPE_VALUE)
        , _b_fix_width(false)
        , _init_width(0)
        , _hold_right_space(0)
    { }
} ValueIncrease;

typedef struct RtChartSettings {
    int       _predict_len;
    int         _space_len;
    int       _down_intval;
    WORLD_RECT      _limit;
    ValueIncrease      _vi;
    float    _stroke_width;
    RULER_WIDTH _rule_size;
} RtChartSettings;

typedef struct TripleTuple {
    float f1;
    float f2;
    float f3;
    TripleTuple() : f1(0), f2(0), f3(0) {}
}TripleTuple;

#define msgfun virtual

#define ERROR_GLYPH_TREE -100

WARMGUI_API void WINAPI _MyTraceW( LPCWSTR strMsg, ... );
WARMGUI_API void WINAPI _MyTraceA( LPCSTR strMsg, ... );

#ifdef UNICODE
#define MYTRACE _MyTraceW
#else
#define MYTRACE _MyTraceA
#endif

#define IsInRect(rect, x, y) (((x) >= (rect).left) && ((x) <= (rect).right) && ((y) >= (rect).top) && ((y )<= (rect).bottom))
#define RectWidth(rect) ((rect).right  - (rect).left)
#define RectHeight(rect) ((rect).bottom - (rect).top)
#define DeflateRect(rect, l,r) ((rect).left += (l), (rect).right -= (r))
#define fRectWidth(rect) (static_cast<float>((rect).right  - (rect).left))
#define fRectHeight(rect) (static_cast<float>((rect).bottom - (rect).top))

#define RectEqulas(rect1, rect2) (!(memcmp(&(rect1), &(rect2), sizeof(RECT))))

#define Rect2Frect(rect) \
D2D1::RectF(static_cast<float>((rect).left),\
            static_cast<float>((rect).top),\
            static_cast<float>((rect).right),\
            static_cast<float>((rect).bottom))

#define Rect2Urect(rect) \
D2D1::RectU((rect).left,\
            (rect).top,\
            (rect).right,\
            (rect).bottom)

typedef int GLYPH_CHANGED_TYPE;

const int GLYPH_CHANGED_NONE           = 0x000;  ///the glyph did not changed
const int GLYPH_CHANGED_ATELIER_RESIZE = 0x001;  ///the size of atelier was changed
const int GLYPH_CHANGED_CANVAS_RESIZE  = 0x002;  ///the size of canvas  was changed
const int GLYPH_CHANGED_ATELIER_BKG    = 0x004;  ///the background of atelier was changed
const int GLYPH_CHANGED_CANVAS_BKG     = 0x008;  ///the background of canvas  was changed
const int GLYPH_CHANGED_GLYPH_BKG      = 0x010;  ///the background of glyph   was changed
const int GLYPH_CHANGED_GLYPH          = 0x020;  ///the glyph was changed, N/A for atelier and canvas
const int GLYPH_CHANGED_CANVAS         = 0x040;


#define BGR(b,g,r) ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))

const RECT zero_rect = {0, 0, 0, 0};

typedef void* dataptr;


typedef struct DATA_POINTS {
    POINTF*  _points;
    size_t _buf_size;
    size_t    _count;

    DATA_POINTS() : _points(0), _buf_size(0), _count(0) {}
    ~DATA_POINTS() {
        SafeDelete(_points);
    }

    void set_size(size_t bufsize) {
        SafeDelete(_points);
        _count = 0, _buf_size = bufsize;

        _points = new POINTF[_buf_size];
    }

    void reset() {
        _count = 0 ;

        if (_points)
            memset(_points, 0, sizeof(POINTF) * _buf_size);
        else
            _buf_size = 0;
    }

    bool add_data(float x, float y) {
        if (_count < _buf_size) {
            _points[_count].x = x, _points[_count++].y = y;
        } else {
            memmove(_points, _points + 1, (_count - 1) * sizeof(POINTF));
            _points[_count - 1].x = x, _points[_count - 1].y = y;
        }
        return true;
    }
} DATA_POINTS;

typedef struct DOUBLE_DATA_POINTER {
    double* x;
    double* y;
    size_t  count;
    DOUBLE_DATA_POINTER() : x(0), y(0), count(0) {}
    ~DOUBLE_DATA_POINTER() { reset(); }
    void reset() { SafeDelete(x); SafeDelete(y); count = 0; }
    void init(size_t size) { reset() ; x = new double[size]; y = new double[size]; }
} DOUBLE_DATA_POINTER;


#define WM_FIRST_USER_COMMAND WM_USER + 1
#define WM_WINDOWS_REDRAW WM_FIRST_USER_COMMAND

#endif //__warmgui_type_summer_h__