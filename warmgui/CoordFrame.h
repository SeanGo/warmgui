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



////////////////////////////////////////////////////////////////////////////////////////
/// class CCoordFrame 
class WARMGUI_API CCoordFrame : public IDataGraph
{
public:
	                        CCoordFrame(const char* name);
                           ~CCoordFrame();
	virtual HRESULT         DrawGraph(bool redraw/* = false*/);
    inline  virtual HRESULT Init();
	inline  void            SetLineColor(COLORREF color, float a/* = 1.0f */);
	inline  void            SetSideBarWidth(RULER_WIDTH& ruler_width) {_ruler_width = ruler_width;}
    inline  void            SetRect(RECT& rect);
    virtual HRESULT         PreDraw();
    inline  virtual GLYPH_CHANGED_TYPE  NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype);
    //inline  virtual GLYPH_CHANGED_TYPE  NewData(int index, float x, float y);
    //GLYPH_CHANGED_TYPE      renew_world(WORLD_RECT* pwr, bool bfirstdata = false);
    GLYPH_CHANGED_TYPE      renew_world(float x, float y, bool redraw_cood = false, bool bfirstdata = false);
    RULER_WIDTH&            getRulerWidth() { return _ruler_width; }
private:
    //set class name, by IObject
    virtual void           setClass() { SetMyClass("CCoordFrame"); }
	void                   DrawSideBar();
	void                   DrawGrid();
	void                   DrawRuler();
	void                   DrawRulerText(TCHAR * wszText, size_t len, float Y, float y);

    void                   redraw();
        
private:
	COLORALPHA            _clr_line;
	float                 _barWidth;
	RULER_WIDTH        _ruler_width;
    RECT                 _rect_calc;
    FONT                      _font;
    IDWriteTextFormat* _pTextFormat;
    IDWriteTextLayout* _pTextLayout;
};




}

#endif //__STOCK_GLYPH_H_INCLUDE__
