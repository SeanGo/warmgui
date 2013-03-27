#ifndef __warmgui_canvas_h_include__
#define __warmgui_canvas_h_include__


namespace WARMGUI {



class WARMGUI_API ICanvas : public IGlyphNode
{
public:
	ICanvas(void);
	ICanvas(const TCHAR * name, bool bHasBkgBlind = true);
	virtual ~ICanvas(void);

	///set children's rect
	virtual void SetGlyphRect() = 0;
	///initial canvas
	virtual HRESULT Init();
	///draw the graph
	virtual HRESULT Draw(bool redraw = false);
	///draw background
	virtual HRESULT DrawBkg(bool redraw = false);
	///draw the new part of graph
	virtual HRESULT Renew();

protected:
    HRESULT _DrawBkg(bool redraw = false);

public:
	///the point(x,y) of screen is inside of graph
	virtual bool Intersect(int x, int y);
	///the point(x,y) of screen is edge of graph
	virtual EDGE_POSITION_TYPE IsEdge(int x, int y);

    inline int             getChildNumber(const GlyphTreeIter iter);
    inline IGlyph*         getChild(const GlyphTreeIter iter, int index);
	inline virtual IGlyph* InsertAfter(IGlyph* const g);
	inline IGlyph*         AppendChild(const GlyphTreeIter iter, IGlyph* const g);
	inline IGlyph*         PrependChild(const GlyphTreeIter iter, IGlyph* const g);
	inline IGlyph*         InsertNext(const GlyphTreeIter iter, IGlyph* const g);
	inline IGlyph*         InsertPrev(const GlyphTreeIter iter, IGlyph* const g);

	inline GlyphTreeIter   Find(IGlyph* const g) const;
	inline GlyphTreeIter   Find(const UINT gid) const;
	inline IGlyph*         Find(const TCHAR * name) const;
	inline bool            Remove(const IGlyph* g);
	inline bool            Remove(const UINT gid);
	inline void            Clear();

	virtual void           SetRect(RECT& rect);

	void SetMargin(MARGIN& margin)
	{
		_margin = margin;
		_offset.x = static_cast<float>(_rect.left + _margin.left), _offset.y = static_cast<float>(_rect.top + _margin.top);
	}

	
	inline IGlyph* GetSelectedGlyph();
	inline virtual void GiveGlyphSomething(IGlyph* const g);

	void SetBitmap(AppBitmap* appbmp) {_appbmp = appbmp;}
	///must call this function before all
	inline void SetAtelier(CAtelier *atelier);

	virtual GLYPH_CHANGED_TYPE IsChanged() {CriticalLock::Scoped scope(_lockChange); return _changed_type;}
	void ClearChanged() {CriticalLock::Scoped scope(_lockChange); _changed_type = GLYPH_CHANGED_TYPE_NONE;}
	virtual void Changed(GLYPH_CHANGED_TYPE   changed_type) {CriticalLock::Scoped scope(_lockChange); _changed_type = (GLYPH_CHANGED_TYPE)((int) _changed_type | (int)changed_type);}
	virtual void Dechanged(GLYPH_CHANGED_TYPE changed_type) {CriticalLock::Scoped scope(_lockChange); _changed_type = (GLYPH_CHANGED_TYPE)((int) _changed_type & ~((int)changed_type));}

protected:
	MARGIN    _margin;
	POINT_f   _offset;              //offset point to the originla window
	uint32_t  _conter;              ///for automatic 
	CGlyphTree              _gt;
	GlyphTreeIter _iterSelected;    ///selected glyph

	AppBitmap*       _appbmp;
	CAtelier*       _atelier;
	bool     _bHasBackground;		///if have background, then add a blind rectangle

	bool          _bUsingBkg;	    ///if true, using the background image stored by _bmp_bkg of atelier

	CriticalLock _lockChange;

protected:
	void set_background();
};



EXPORT_STL_VECTOR(WARMGUI_API, ICanvas*)

typedef vector <ICanvas*> WARMGUI_API CCanvasArray;
typedef CCanvasArray::iterator CanvasIter;
typedef CCanvasArray::const_iterator ConstCanvasIter;



class WARMGUI_API CBkgCanvas : public ICanvas
{
public:
	CBkgCanvas(void);
	CBkgCanvas(const TCHAR * name);
	~CBkgCanvas(void);

	virtual HRESULT Init();
	virtual void SetGlyphRect();

protected:
	CSharedImage* _imgBkg;
};

} //namespace WARMGUI
#endif //__warmgui_canvas_h_include__