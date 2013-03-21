#ifndef __STOCK_GLYPH_H_INCLUDE__
#define __STOCK_GLYPH_H_INCLUDE__


namespace WARMGUI {

static const float AXIS_LINE_COLOR_R =   0;
static const float AXIS_LINE_COLOR_G =   0;
static const float AXIS_LINE_COLOR_B = 1.0;
static const float AXIS_LINE_ALPHA   = 1.0;

/*
static const float SIDE_BAR_WIDTH  = 50.0;
static const float SIDE_BAR_HEIGHT = 20.0;
static const float STROKE_WIDTH    =  .4f;
*/
static const float SPLIT_PIECES    = 19.0;


class WARMGUI_API CCoordFrame : public IGlyph
{
public:
	CCoordFrame();
	CCoordFrame(const TCHAR * name, bool bHasBuffer = false);
	~CCoordFrame();

	virtual HRESULT DrawGraph(bool redraw/* = false*/) = 0;
	virtual bool Intersect(POINT /*point*/) { return false; }
	inline void SetWorldRect(LIMIT_2D* limit);
protected:
    ///get from aCCartesian instance
	LIMIT_2D* _limit;
};



////////////////////////////////////////////////////////////////////////////////////////
/// class CCoordGrid 
class WARMGUI_API CCoordGrid : public CCoordFrame
{
public:
	CCoordGrid(const TCHAR * name);

	virtual HRESULT DrawGraph(bool redraw/* = false*/);

	inline void SetTextFormat(	const TCHAR * fontFamilyName,
								float fontSize,
								DWRITE_FONT_WEIGHT   fontWeight  = DWRITE_FONT_WEIGHT_NORMAL,
								DWRITE_FONT_STYLE    fontStyle   = DWRITE_FONT_STYLE_NORMAL,
								DWRITE_FONT_STRETCH  fontStretch = DWRITE_FONT_STRETCH_NORMAL,
								const WCHAR * localeName = L"en-us");
	inline void SetLineColor(COLORREF color, float a/* = 1.0f */);

	inline void SetSideBarWidth(RULER_WIDTH& ruler_width) {_ruler_width = ruler_width;}
    inline void SetRect(RECT& rect);
    virtual HRESULT PreDraw();

private:
	D2D1_COLOR_F _lineclr;
	float _barWidth;
	RULER_WIDTH _ruler_width;
    RECT _rect_calc;

private:
	void DrawSideBar();
	void DrawGrid();
	void DrawRuler();
	void DrawRulerText(TCHAR * wszText, size_t len, float Y, float y);
};




}

#endif //__STOCK_GLYPH_H_INCLUDE__
