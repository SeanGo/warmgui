#ifndef __warmgui_reference_frame_h_include__
#define __warmgui_reference_frame_h_include__


typedef int WORLD_CHANGED_TYPE;
const int WORLD_CHANGED_TYPE_NONE  = 0x00;
const int WORLD_CHANGED_TYPE_MAX_X = 0x01;
const int WORLD_CHANGED_TYPE_MIN_X = 0x02;
const int WORLD_CHANGED_TYPE_MAX_Y = 0x04;
const int WORLD_CHANGED_TYPE_MIN_Y = 0x08;

namespace WARMGUI {


/**
	@brief interface reference-frame is math coordinate system, cartesian system, polar coordinates.
	       for now. we consider the 2d system only
*/
class WARMGUI_API CWorld
{
public:
	inline CWorld();
	inline ~CWorld();

	/// set the rectagle on the screen
	inline void         SetRectSize(RECT& rect);
	/// Set the world rect by WORLD_RECT struce
	inline void         SetWorldRect(WORLD_RECT& limit);
	/// Set the world rect by all params
	inline void         SetWorldRect(double miny, double minpos, double maxy, double maxpos, double x0, double y0, double xn, double yn);
	/// Set the world rect by all params
	inline void         SetWorldRect(float miny, float minpos, float maxy, float maxpos, float x0, float y0, float xn, float yn);
	/// Get the world-rect
	inline WORLD_RECT&  GetWorldRect() { return _real_world; }
	/// Get the transform matrix
	inline MATRIX_2D*   GetTransform() { return &_transform; }
	/// if the point (x, y) lies in the frame, return true
	inline bool         Intersect(int x, int y);

	/// transform point(x, y) in reference frame to screen point
	inline POINT        Transform(float x, float y);
	/// invert transform point on screen to point in reference frame
	inline POINT_f      InvertTransform(int x, int y);

	///zoom in or out on the screen
	inline void         Zoom(float minx, float maxx, float miny, float maxy);
	///undo zoom
	inline void         UndoZoom() ;
	///reset the zoom to initial size
	inline void         ResetZoom();

    inline float        GetXScale() { return _transform._11; }
    inline float        GetYScale() { return _transform._22; }

    inline void         ChangeYlimit(float miny, float maxy, float y0);
    inline void         ChangeXlimit(float minx, float maxx, float x0);

	inline RECT&        GetRect() { return _rect; }

    bool                setConfig(CWarmguiConfig* config, const char* config_str);
    void                setName(const char* name) { strcpy_s(_name, MAX_PATH, name); }
    void                RenewByRealWorld();

    inline void         RenewYLimit(float x, float y);
    inline void         fresh_y_limit(float x, float y);

    inline WORLD_CHANGED_TYPE fresh_limit(float x, float y);
    inline WORLD_CHANGED_TYPE get_world_change() { return _world_changed; }
    inline void         set_world_change(WORLD_CHANGED_TYPE type = WORLD_CHANGED_TYPE_NONE) { _world_changed = type; }

    inline float        get_x_left();
protected:
	inline virtual void SetScale();

	MATRIX_2D          _transform;   ///the matrix of transform
	RECT               _rect;   ///the rectagle on screen

    inline  void        real_world_to_screen();
    WORLD_CHANGED_TYPE  _world_changed;
public:
    ///get from aCCartesian instance
    WORLD_RECT         _real_world; ///the init-world
    WORLD_RECT         _zero_world; ///the init-world
	WORLD_RECT         _bak_world; ///the world of background
    ValueIncrease      _vi;

    CriticalLock       _lock_change;

    bool               _fist_data;

protected:
    CWarmguiConfig*   _config;
    char   _strconf[MAX_PATH];
    char      _name[MAX_PATH];
    float     _x_left;
};


}

#endif //__warmgui_reference_frame_h_include__