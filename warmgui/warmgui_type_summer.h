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


enum GLYPH_OWN_ARTIST_TYPE {
    GLYPH_OWN_ARTIST_TYPE_OUTSIDE,
    GLYPH_OWN_ARTIST_TYPE_MYSELF,
};

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


#endif //__warmgui_type_summer_h__