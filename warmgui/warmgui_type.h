#ifndef __warmgui_type_define_h_include__
#define __warmgui_type_define_h_include__

#define msgfun virtual
#define DXWM_CONTRL_NOTIFICATION  100
#define EDIT_FINISHED_INPUT 101
#define EDIT_TABLE_INPUTED  102
#define BGR(b,g,r) ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))
#define MAX_WARMGUI_NAME_LEN 40



/******************************************************************
*                                                                 *
*  Macros                                                         *
*                                                                 *
******************************************************************/

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



#define EXPORT_STL_SET( dllmacro, settype ) \
  template class dllmacro std::allocator< settype >; \
  template struct dllmacro std::less< settype >; \
  template class dllmacro std::allocator< \
    std::_Tree_nod<std::_Tset_traits<settype,std::less<settype>, \
    std::allocator<settype>,false> > >; \
  template class dllmacro std::allocator<  \
    std::_Tree_ptr<std::_Tset_traits<settype,std::less<settype>, \
    std::allocator<settype>,false> > >; \
  template class dllmacro std::_Tree_ptr< \
    std::_Tset_traits<settype,std::less<settype>, \
    std::allocator<settype>,false> >; \
  template class dllmacro std::_Tree_nod< \
    std::_Tset_traits<settype,std::less<settype>, \
    std::allocator<settype>,false> >; \
  template class dllmacro std::_Tree_val< \
    std::_Tset_traits<settype,std::less<settype>, \
    std::allocator<settype>,false> >; \
  template class dllmacro std::set< settype, std::less< settype >, \
    std::allocator< settype > >;


#define EXPORT_STL_MAP( dllmacro, mapkey, mapvalue ) \
  template struct dllmacro std::pair< mapkey,mapvalue >; \
  template class dllmacro std::allocator< \
    std::pair<const mapkey,mapvalue> >; \
  template struct dllmacro std::less< mapkey >; \
  template class dllmacro std::allocator< \
    std::_Tree_ptr<std::_Tmap_traits<mapkey,mapvalue,std::less<mapkey>, \
    std::allocator<std::pair<const mapkey,mapvalue> >,false> > >; \
  template class dllmacro std::allocator< \
    std::_Tree_nod<std::_Tmap_traits<mapkey,mapvalue,std::less<mapkey>, \
    std::allocator<std::pair<const mapkey,mapvalue> >,false> > >; \
  template class dllmacro std::_Tree_nod< \
    std::_Tmap_traits<mapkey,mapvalue,std::less<mapkey>, \
    std::allocator<std::pair<const mapkey,mapvalue> >,false> >; \
  template class dllmacro std::_Tree_ptr< \
    std::_Tmap_traits<mapkey,mapvalue,std::less<mapkey>, \
    std::allocator<std::pair<const mapkey,mapvalue> >,false> >; \
  template class dllmacro std::_Tree_val< \
    std::_Tmap_traits<mapkey,mapvalue,std::less<mapkey>, \
	std::allocator<std::pair<const mapkey,mapvalue> >,false> >; \
  template class dllmacro std::map< \
    mapkey, mapvalue, std::less< mapkey >, \
    std::allocator<std::pair<const mapkey,mapvalue> > >;



WARMGUI_API void WINAPI _MyTraceW( LPCWSTR strMsg, ... );
WARMGUI_API void WINAPI _MyTraceA( LPCSTR strMsg, ... );
#ifdef UNICODE
#define MYTRACE _MyTraceW
#else
#define MYTRACE _MyTraceA
#endif




#ifndef GET_X_LPARAM
#	define GET_X_LPARAM(lp)	((int)(short)LOWORD(lp))
#endif //GET_X_LPARAM

#ifndef GET_Y_LPARAM
#	define GET_Y_LPARAM(lp)	((int)(short)HIWORD(lp))
#endif //GET_Y_LPARAM

#define IsInRect(rect, x, y) (((x) >= (rect).left) && ((x) <= (rect).right) && ((y) >= (rect).top) && ((y )<= (rect).bottom))
#define RectWidth(rect) ((rect).right  - (rect).left)
#define RectHeight(rect) ((rect).bottom - (rect).top)
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


enum WM_COMMAND_MESSAGE {
	WM_MYCOMMAND_FIRST          =    WM_USER + 588,
    WM_COMMAND_DRAW_GRAPH       ,
    WM_COMMAND_FILL_GRAPH       ,
    WM_COMMAND_SOLID_BRUSH      ,
    WM_COMMAND_STORKE_STYLE     ,
    WM_COMMAND_TEXT_FORMAT      ,
    WM_COMMAND_EXIT             ,
    WM_COMMAND_OPEN_MARKET_DATA ,
    WM_COMMAND_ZOOM             ,
    WM_COMMAND_UNDO             ,
    WM_COMMAND_REDO             ,
    WM_COMMAND_SELECTED_GRAPH   ,
    WM_COMMAND_MOUSE_OVER       ,
	WM_CANVAS_THREAD_BEGIN      ,
	WM_CANVAS_THREAD_DONE       ,
};

enum WM_DRAW_MESSAGE {
	WM_GLYPHMSG_FIRST            = WM_USER + 999,
	WM_THREAD_DRAW_START,
	WM_THREAD_DRAW_STOP,
	WM_THREAD_DRAW_SUSPEND,
};


typedef struct DATABLOCKINFO {
	void * data;
	size_t data_block_size;
	int offset_x;
	int offset_y;
	size_t num;
} DATABLOCKINFO;

enum POINT_GLYPH_TYPE {
	POINT_GLYPH_TYPE_CROSS,
	POINT_GLYPH_TYPE_STAR,
	POINT_GLYPH_TYPE_X,
	POINT_GLYPH_TYPE_PLUS
};

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

typedef struct SIZE_u {
	long width;
	long height;
} SIZE_u;


typedef struct LIMIT_2D {
	float minx;
	float maxx;
	float miny;
	float maxy;
	float   x0;
	float   y0;
} LIMIT_2D;

typedef struct FPOINT {
    float x, y;
    FPOINT() { x = y = 0.0f; }
    FPOINT(float fx, float fy) {x = fx, y = fy;};
    //FPOINT& operator= (FPOINT& pnt) {x = pnt.x, y = pnt.y; return *this;}
    FPOINT& operator= (FPOINT pnt) {x = pnt.x, y = pnt.y; return *this;}
} FPOINT;

#ifdef _WINDOWS
	typedef D2D1_POINT_2F     POINT_f;
	typedef D2D1_ELLIPSE      ELLIPSE_f;
	typedef D2D1_RECT_F	      RECT_f;
	typedef D2D1_MATRIX_3X2_F MATRIX_2D;
	typedef ID2D1Bitmap       WGBitmap;
	typedef D2D1_SIZE_F       SIZE_f;
	typedef POINT             POINT_u;
#else
	typedef struct POINT_u {
		long x;
		long y;
	} POINT_u;

	typedef struct POINT_f {
		float x;
		float y;
	} POINT_f;

	typedef struct SIZE_f {
		float width;
		float height;
	} SIZE_f;


	typedef struct ELLIPSE_f {
	  POINT_f point;
	  float   radiusX;
	  float   radiusY;
	} ELLIPSE_f;

	typedef struct MATRIX_2D {
		float _11;
		float _12;
		float _21;
		float _22;
		float _31;
		float _32;
	} MATRIX_2D;

	typedef struct RECT_f {
		float left, top, right, bottom;
		RECT_f(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b) {}
	} RECT_f;

	typedef MATRIX_2D MATRIX_2D;
	typedef WGBitmap    WGBitmap;

	typedef struct SIZE_f {
		float width ;
		float height;
	} SIZE_f;
#endif

typedef RECT  MARGIN;
typedef RECT  RULER_WIDTH;
typedef unsigned int uint32_t;

extern WARMGUI_API RECT  NULL_RECT;
extern WARMGUI_API POINT_f NULL_POINT_f;

enum ARTIST_TYPE {
	ARTIST_TYPE_BACK_BUFFER ,
	ARTIST_TYPE_FRONT_BUFFER,
};

enum RESIZE_DRAW_METHOD {
	RESIZE_DRAW_METHOD_SCALE ,
	RESIZE_DRAW_METHOD_REDRAW,
};

enum REDRAW_METHOD {
	REDRAW_METHOD_REDRAW,
	REDRAW_METHOD_COPY_FROM_BACK,
};


enum DRAW_THREAD_TYPE {
	DRAW_THREAD_SINGLE,
	DRAW_THREAD_DOUBLE,
	DRAW_THREAD_MULTI ,
};

enum DRAW_THREAD_STAGE {
	DRAW_THREAD_STAGE_RUNNING  ,
	DRAW_THREAD_STAGE_STOPED   ,
	DRAW_THREAD_STAGE_SUSPENDED,
};

enum DATA_BREADTH_TYPE {
    DATA_BREADTH_TYPE_PERCENT,
    DATA_BREADTH_TYPE_VALUE,
};


enum GLYPH_CHANGED_TYPE {
	 GLYPH_CHANGED_TYPE_NONE           = 0x000,  ///the glyph did not changed
	 GLYPH_CHANGED_TYPE_ATELIER_RESIZE = 0x001,  ///the size of atelier was changed
	 GLYPH_CHANGED_TYPE_CANVAS_RESIZE  = 0x002,  ///the size of canvas  was changed
	 GLYPH_CHANGED_TYPE_GLYPH_RESIZE   = 0x004,  ///the size of glyph   was changed
	 GLYPH_CHANGED_TYPE_ATELIER_BKG    = 0x008,  ///the background of atelier was changed
	 GLYPH_CHANGED_TYPE_CANVAS_BKG     = 0x010,  ///the background of canvas  was changed
	 GLYPH_CHANGED_TYPE_GLYPH_BKG      = 0x020,  ///the background of glyph   was changed
	 GLYPH_CHANGED_TYPE_COORDFRAME     = 0x040,  ///the coordinate-frame of glyph was changed, N/A for atelier and canvas
	 GLYPH_CHANGED_TYPE_CHANGED        = 0x080,  ///the glyph was changed, N/A for atelier and canvas
     GLYPH_CHANGED_TYPE_INCREASED      = 0x100,  ///glyph was increased
};

typedef struct ValueIncrease {
	float      _init_y0_S_mag;
	float      _init_y0_L_mag;
	DATA_BREADTH_TYPE _breadth_type;
} ValueIncrease;

typedef struct RtChartSettings {
    int       _predict_len;
    int         _space_len;
    int       _down_intval;
    LIMIT_2D        _limit;
    ValueIncrease      _vi;
    float    _stroke_width;
    RULER_WIDTH _rule_size;
} RtChartSettings;


typedef void* dataptr;

namespace WARMGUI {

extern int gettopt(int argc, TCHAR *argv[], TCHAR *opstring);
extern TCHAR* toptarg;
extern inline bool pt_in_rect(RECT& rect, int x, int y);
extern inline bool pt_in_rect(RECT& rect, POINT_u pt);
extern inline bool pt_in_rect(RECT& rect, POINT pt);
} //namespace WARMGUI

#ifndef SafeDelete
#   define SafeDelete(p) if ((p)) {delete (p); (p) = 0;}
#endif //SafeDelete


#endif //__warmgui_type_define_h_include__
