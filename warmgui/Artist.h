#ifndef __warmgui_artist_h_include__
#define __warmgui_artist_h_include__

namespace WARMGUI {
class WARMGUI_API eArtist
{
public:
	eArtist(void);
	~eArtist(void);

	void SetHwndRenderTarget(ID2D1HwndRenderTarget* pHwndRT, HWND hwnd);
    HWND GetHwnd() { return _hwnd; }
    ID2D1RenderTarget* GetUsingRT() { return _pPaintRT; }

	inline HRESULT SetStrokeStyle(const D2D1_STROKE_STYLE_PROPERTIES& strokeStyleProperties);
	inline HRESULT SetStrokeStyle(
						D2D1_CAP_STYLE startCap = D2D1_CAP_STYLE_ROUND,
						D2D1_CAP_STYLE  endCap = D2D1_CAP_STYLE_ROUND,
						D2D1_CAP_STYLE  dashCap  = D2D1_CAP_STYLE_ROUND,
						D2D1_LINE_JOIN  lineJoin = D2D1_LINE_JOIN_ROUND,
						FLOAT           miterLimit = 10.0f,
						D2D1_DASH_STYLE dashStyle  = D2D1_DASH_STYLE_SOLID,
						FLOAT           dashOffset = 0.0f);
	inline HRESULT SetStrokeStyle(int argc, TCHAR * argv[]);

    inline HRESULT SetSolidColorBrush(const COLORALPHA   &color, GLYPH_STATE_TYPE scbt = GLYPH_STATE_NORMAL);
	inline HRESULT SetSolidColorBrush(int argc, TCHAR * argv[]);
    inline void    SetTextFormat(IDWriteTextFormat* pTextFormat);
    inline void    SetTextLayout(IDWriteTextLayout* pTextLayout);

    /*
	inline HRESULT SetTextFormat(const TCHAR * fontFamilyName,
								 FLOAT  fontSize,
								 DWRITE_FONT_WEIGHT  fontWeight  = DWRITE_FONT_WEIGHT_NORMAL,
								 DWRITE_FONT_STYLE   fontStyle   = DWRITE_FONT_STYLE_NORMAL,
								 DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL,
								 DWRITE_TEXT_ALIGNMENT textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER,
								 DWRITE_PARAGRAPH_ALIGNMENT  paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
								 const WCHAR * localeName = L"en-us");
	inline HRESULT SetTextFormat(int argc, TCHAR * argv[]);
    */

	inline void DrawText(const TCHAR * text, D2D1_RECT_F& rect);
	inline void DrawText(const TCHAR * text, RECT& rect);
	inline void DrawLine(POINT_f p1, POINT_f p2, float fStrokeWidth = 1.0f);
	inline void DrawLine(float x1, float y1, float x2, float y2, float fStrokeWidth = 1.0f);
	inline void DrawBitmap(WGBitmap* pImage,
							RECT& rectDest,
							RECT& rectSrc,
							float opacity = 1.0f,
							D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	inline void DrawBitmap(WGBitmap* pImage,
							RECT& rectDest,
							RECT_f& rectSrc,
							float opacity = 1.0f,
							D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	inline void DrawBitmap(	WGBitmap* pImage,
							D2D1_RECT_F& rectDest,
							D2D1_RECT_F& rectSrc,
							float opacity = 1.0f,
							D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	///Draw the bitmap from default bitmap render target
	inline void DrawBmpRTBitmap(
							WGBitmap* pImage,
							RECT& rectDest,
							RECT& rectSrc,
							float opacity = 1.0f,
							D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	inline void DrawRectangle(RECT_f& rect_f, float fStrokeWidth = 1.0f);
	inline void DrawRectangle(float x1, float y1, float x2, float y2, float fStrokeWidth = 1.0f);
	inline void DrawRectangle(RECT& rect, float fStrokeWidth = 1.0f);
	inline void FillRectangle(RECT_f& rect_f, float fStrokeWidth = 1.0f);
    inline void FillRectangle(RECT& rect, float fStrokeWidth = 1.0f);
	inline void FillRectangle(float x1, float y1, float x2, float y2, float fStrokeWidth = 1.0f);
	inline void DrawEllipse(ELLIPSE_f& ellipse, float fStrokeWidth = 1.0f);
	inline void FillEllipse(ELLIPSE_f& ellipse, float fStrokeWidth = 1.0f);
	inline void DrawGeometry(ID2D1Geometry *geometry, ID2D1Brush *brush, FLOAT strokeWidth = 1.0f,ID2D1StrokeStyle *strokeStyle = 0);

	inline HRESULT Flush();
    inline void Clear();


	inline void BeginBmpDraw(bool clear = false);
	inline HRESULT EndBmpDraw();
	inline void BeginDraw(bool clear = false);
	inline HRESULT EndDraw();
	inline HRESULT ResizeRenderTarget(int cx, int cy);

	inline void UsingHwndRT();
	inline void UsingBmpRT();

	inline void SetTransform(MATRIX_2D* trans);
	inline void SetTranslationTrasform(RECT& rect);
	inline void GetTransformMatrix(MATRIX_2D* trans);
	inline void GetHwndRtTransform(MATRIX_2D* trans);

	inline ID2D1BitmapRenderTarget* GetBmpRT(){ return _pDefaultBmpRT; }
	inline ID2D1HwndRenderTarget* GetHwndRT() { return _pHwndRT ;}
	inline ID2D1SolidColorBrush* GetSCBrush() { return _pscBrush;}
	inline ID2D1StrokeStyle* GetStrokeStyle() { return _pStroke ;}

	inline HRESULT CreateBitmap(WGBitmap** pBitmap, RECT& rect);
	inline HRESULT CopyFromRenderTarget(WGBitmap* pBitmap, POINT pointDest, RECT rectSrc);

	inline ID2D1Bitmap* GetDefaultBmp() { return _pDefaultBmp; }


	inline HRESULT PushLayer(RECT& rect);
	inline HRESULT PushLayer(float left, float top, float right, float bottom);
	inline void PopLayer();

private:
	ID2D1HwndRenderTarget*         _pHwndRT;
	ID2D1BitmapRenderTarget* _pDefaultBmpRT;
    ID2D1RenderTarget*           _pBackupRT;
	ID2D1Bitmap*               _pDefaultBmp;
	ID2D1SolidColorBrush*         _pscBrush;
	ID2D1StrokeStyle*              _pStroke;
	ID2D1BitmapBrush*            _pbmpBrush;
	ID2D1LinearGradientBrush*     _plgBrush;
	ID2D1RadialGradientBrush*     _prgBrush;
	IDWriteTextFormat*         _pTextFormat;
	IDWriteTextLayout*         _pTextLayout;
	ID2D1Layer *                     _layer;
	ID2D1Layer *               _bmprt_layer;

	///indecate it's a inependet hwnd-render-target
	ID2D1RenderTarget* _pPaintRT;
	HWND _hwnd;
public:
	CriticalLock _lock_artist;

};

} //namespace WARMGUI

#endif //__warmgui_artist_h_include__