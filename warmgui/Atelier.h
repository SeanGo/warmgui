#ifndef __warmgui_atelier_h_include__
#define __warmgui_atelier_h_include__

namespace WARMGUI {

class ICanvas;
class IDataContainer;

class WARMGUI_API CAtelier : public IGlyphNode
{
public:
	CAtelier(void);
	virtual ~CAtelier(void);

	///draw the graph
	virtual HRESULT Draw(bool redraw = false);
	///draw background
	virtual HRESULT DrawBkg(bool redraw = false);
    ///renew by increasement
    virtual HRESULT Renew();

protected:
    HRESULT RealDraw(bool redraw = false);
	HRESULT DrawScreenImage();

public:
	virtual void    SetLayout() = 0;
	virtual HRESULT InitAtelier(HWND hwnd, CWarmguiConfig* config);

	inline HRESULT  CreateRenderTarget(HWND hwnd);
	inline void     SetArtistToCanvas();
	inline void     SetRect(RECT& rect);

	inline void     Insert(ICanvas* const canvas);
	inline bool     Remove(const ICanvas* canvas);
	inline bool     Remove(const TCHAR * name);
	inline ICanvas* FindCanvas(const char* name) const;
	inline ICanvas* FindCanvas(const TCHAR * name) const;
	inline IGlyph*  FindGlyph (const TCHAR * name) const;
    inline IGlyph*  FindGlyph(const char* name) const;
	inline void     Clear();

	inline void            SetClearRenderTarget(bool clear) { _clear_render_target = clear; }

	inline ICanvas*        GetSelectedCanvas();

	bool                   GetBitmapMaterial();
	inline HRESULT         CopyBitmapFormRenderTarget();

	virtual uint32_t       IsChanged()    {CriticalLock::Scoped scope(_lockChange); return _changed_type;}
	void                   ClearChanged() {CriticalLock::Scoped scope(_lockChange); _changed_type = GLYPH_CHANGED_TYPE_NONE;}
	virtual void           Changed(GLYPH_CHANGED_TYPE   changed_type) {CriticalLock::Scoped scope(_lockChange); _changed_type = (GLYPH_CHANGED_TYPE)((int) _changed_type | (int)changed_type);}
	virtual void           Dechanged(GLYPH_CHANGED_TYPE changed_type) {CriticalLock::Scoped scope(_lockChange); _changed_type = (GLYPH_CHANGED_TYPE)((int) _changed_type & ~((int)changed_type));}

	HWND                   GetHwnd()   {return _hwnd;}
	ID2D1HwndRenderTarget* GetHwndRT() {return _pHwndRT;}

	inline void  GetMyBackgroundRect(RECT& myrect, RECT_f* bmprect);
	virtual void WindowSizing(bool sizing = true) {_sizing = sizing;}
	bool         IsWindowSizing() { return _sizing; }


	//Message Functions
	inline virtual int OnMouseMove(int x, int y);
    inline virtual int OnLButtonUp(int x, int y);
    inline virtual int OnRButtonUp(int x, int y);

protected:
	CCanvasArray         _canvasses;    ///contained by this atelier
	HWND                      _hwnd;    ///the handle of window
	ID2D1HwndRenderTarget* _pHwndRT;    ///HwndRT of the window

	bool       _clear_render_target;    ///need clear HwndRT
	ILayout*                _layout;    ///the layout of this atelier to deside position of cavasses

	CanvasIter _iterCanvasSlct;         ///selected canvas
	CriticalLock   _lockChange;         ///the lock for change state of glyph

	AppBitmap _appbmp;                  ///bitmap of materies and background of all
	bool      _sizing;                  ///the windows is re-sizing

protected:
	inline void give_some(ICanvas* const canvas);
};


/////////////////////////////////////////////////////////////////////////
// class one-atelier
class WARMGUI_API COneAtelier : public CAtelier{
public:
	virtual HRESULT InitAtelier(HWND hwnd, CWarmguiConfig* config);
	virtual void SetLayout();
	friend class COneLayout;
private:
	COneLayout* _layoutOne;
};




}	//namespace WARMGUI

#endif //__warmgui_atelier_h_include__
