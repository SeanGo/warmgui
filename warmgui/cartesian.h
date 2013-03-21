#ifndef __warmgui_reference_frame_h_include__
#define __warmgui_reference_frame_h_include__


namespace WARMGUI {


/**
	@brief interface reference-frame is math coordinate system, cartesian system, polar coordinates.
	       for now. we consider the 2d system only
*/
class WARMGUI_API CCartesian
{
public:
	inline CCartesian();
	inline ~CCartesian();

	/// set the rectagle on the screen
	inline void SetRectSize(RECT& rect);
	/// Set the world rect by LIMIT_2D struce
	inline void SetWorldRect(LIMIT_2D& limit);
	/// Set the world rect by all params
	inline void SetWorldRect(float minx, float maxx, float miny, float maxy, float x0, float y0);

	/// Get the world-rect
	inline LIMIT_2D& GetWorldRect() { return _limit; }
	/// Get the transform matrix
	inline MATRIX_2D* GetTransform() { return &_transform; }
	/// if the point (x, y) lies in the frame, return true
	inline bool Intersect(int x, int y);

	/// transform point(x, y) in reference frame to screen point
	inline POINT Transform(float x, float y);
	/// invert transform point on screen to point in reference frame
	inline POINT_f InvertTransform(int x, int y);

	///zoom in or out on the screen
	inline void Zoom(float minx, float maxx, float miny, float maxy);
	///undo zoom
	inline void UndoZoom() ;
	///reset the zoom to initial size
	inline void ResetZoom();

    inline float GetXScale() { return _transform._11; }
    inline float GetYScale() { return _transform._22; }

    inline void ChangeYlimit(float miny, float maxy, float y0);
    inline void ChangeXlimit(float minx, float maxx, float x0);

	RECT& GetRect() { return _rect; }

protected:
	inline virtual void SetScale();

	LIMIT_2D  _limit;				///the limit of world-rect
	LIMIT_2D  _limit_zoom_back;	    ///backup of _limit_frot
	LIMIT_2D  _limit_zoom_front;	///the front world on screen
	MATRIX_2D _transform;			///the matrix of transform
	RECT      _rect;	///the rectagle on screen
};


}

#endif //__warmgui_reference_frame_h_include__