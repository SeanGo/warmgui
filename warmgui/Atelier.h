#ifndef __warmgui_atelier_h_include__
#define __warmgui_atelier_h_include__

namespace WARMGUI {

class ICanvas;
class IDataContainer;

class WARMGUI_API IAtelier : public IGlyphNode
{
public:
	                       IAtelier(void);
	virtual               ~IAtelier(void);

	virtual HRESULT        Draw(bool redraw = false);     ///draw the graph
	virtual HRESULT        DrawBkg(bool redraw = false);  ///draw background   
    virtual HRESULT        Renew();                       ///renew by increasement
    void                   StartDrawing() { CriticalLock::Scoped scope(_lockDrawing); _drawing = true; }
    void                   StopDrawing()  { CriticalLock::Scoped scope(_lockDrawing); _drawing = false; }
    bool                   IsBusy()       { CriticalLock::Scoped scope(_lockDrawing); return _drawing = false; }
    void                   redraw_window(){ ::InvalidateRect(_hwnd, 0, TRUE); }

protected:
    HRESULT                RealDraw(bool redraw = false);
	HRESULT                DrawScreenImage();

public:                    
    inline void            SetLayout(ILayout* layout) {_layout = layout;}

	virtual HRESULT        InitAtelier(HWND hwnd, CWarmguiConfig* config);

	inline HRESULT         CreateRenderTarget(HWND hwnd);
	inline void            SetArtistToCanvas();
	inline void            SetRect(RECT& rect);

	inline void            Insert(ICanvas* const canvas);
	inline bool            Remove(const ICanvas* canvas);
	inline bool            Remove(const char * name);
	inline ICanvas*        FindCanvas(const char* name) const;
    inline IGlyph*         FindGlyph(const char* name) const;
	inline void            Clear();

	inline void            SetClearRenderTarget(bool clear) { _clear_render_target = clear; }

	inline ICanvas*        GetSelectedCanvas();

    inline AppBitmap*      GetAppBitmap() {return &_appbmp;}
	bool                   GetBitmapMaterial();
	inline HRESULT         CopyBitmapFormRenderTarget();

	virtual uint32_t       IsChanged()    {CriticalLock::Scoped scope(_lockChange); return _changed_type;}
	void                   ClearChanged() {CriticalLock::Scoped scope(_lockChange); _changed_type = GLYPH_CHANGED_TYPE_NONE;}
	virtual void           Changed(GLYPH_CHANGED_TYPE   changed_type) {CriticalLock::Scoped scope(_lockChange); _changed_type = (GLYPH_CHANGED_TYPE)((int) _changed_type | (int)changed_type);}
	virtual void           Dechanged(GLYPH_CHANGED_TYPE changed_type) {CriticalLock::Scoped scope(_lockChange); _changed_type = (GLYPH_CHANGED_TYPE)((int) _changed_type & ~((int)changed_type));}
	bool                   HadChanged(GLYPH_CHANGED_TYPE   changed_type) {CriticalLock::Scoped scope(_lockChange); return (0 != ((int) _changed_type & (int)changed_type));}

    void                   SetHwnd(HWND hwnd) { _hwnd = hwnd; }
    void                   SetConfigFile(CWarmguiConfig* config, const char* str_conf = 0)
                           { _config = config; if (str_conf) strcpy_s(_str_conf, MAX_PATH, str_conf);}
	HWND                   GetHwnd()   {return _hwnd;}
	ID2D1HwndRenderTarget* GetHwndRT() {return _pHwndRT;}

	virtual void           WindowSizing(bool sizing = true) {_sizing = sizing;}
	bool                   IsWindowSizing() { return _sizing; }


	//Message Functions
	inline virtual int     OnMouseMove(int x, int y);
    inline virtual int     OnLButtonUp(int x, int y);
    inline virtual int     OnRButtonUp(int x, int y);


    virtual bool           GetAllCanvas() { return true; }
    inline  void           GetWinrectInMaterial(RECT& canvas_rect, RECT_f* bmprect);
    const char*            GetConfigString() { return _str_conf; }

private:
    virtual void           setClass() { SetMyClass("IAtelier"); }  //set class name, by IObject

protected:
	inline  bool           GetMyBackgroundRect();
protected:
	CCanvasArray         _canvasses;    ///contained by this atelier
	HWND                      _hwnd;    ///the handle of window
	ID2D1HwndRenderTarget* _pHwndRT;    ///HwndRT of the window

	bool       _clear_render_target;    ///need clear HwndRT
	ILayout*                _layout;    ///the layout of this atelier to deside position of cavasses

	CanvasIter      _iterCanvasSlct;    ///selected canvas
	CriticalLock        _lockChange;    ///the lock for change state of glyph
	CriticalLock       _lockDrawing;    ///the lock for change state of glyph
    bool                   _drawing;

	AppBitmap               _appbmp;    ///bitmap of materies and background of all
	bool                    _sizing;    ///the windows is re-sizing
    CWarmguiConfig*         _config;
    RECT       _winrect_in_material;    ///the window-rect in material-bitmap
    char        _str_conf[MAX_PATH];    ///config string of atelier

    char     _calculators[MAX_PATH];
    char     _dispatchers[MAX_PATH];

    ICanvas*       _selected_canvas;
};


/////////////////////////////////////////////////////////////////////////
// class one-atelier
class WARMGUI_API COneAtelier : public IAtelier{
public:
    COneAtelier() : IAtelier() { setClass(); }
	virtual HRESULT InitAtelier(HWND hwnd, CWarmguiConfig* config);
	virtual void SetLayout();
	friend class COneLayout;
private:
	COneLayout* _layoutOne;


private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("COneAtelier"); }
};


class WARMGUI_API IAtelierCreator
{
public:
    IAtelierCreator(void) : _config(0) {}
    virtual ~IAtelierCreator(void){}


    void SetConfigure(CWarmguiConfig* config) { _config = config; }
    virtual IAtelier* CreateAtelier(const char* config_pos) = 0;

protected:
    CWarmguiConfig* _config;
};


}	//namespace WARMGUI

#endif //__warmgui_atelier_h_include__
