#ifndef __warmgui_glyph_h_include__
#define __warmgui_glyph_h_include__

namespace WARMGUI {

class WARMGUI_API AppBitmap {
public:
    AppBitmap()
        :   _material(0)
        ,     _screen(0)
        , _background(0)
        //,    _content(0)
    {}

    ~AppBitmap()
    {
        ReleaseScreenBitmap();
        //SafeRelease(&_material  );
    }

    void SetSize(ID2D1HwndRenderTarget* pHwndRT, RECT& rect) {
        ReleaseScreenBitmap();
        
        pHwndRT->CreateBitmap(
            D2D1::SizeU(RectWidth(rect), RectHeight(rect)),
            D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
            &_screen);
    }

private:
    void ReleaseScreenBitmap()
    {
        SafeRelease(&_screen);
        //SafeRelease(&_background);
        //SafeRelease(&_content   );
    }

public:
    WGBitmap*   _material;  ///material of all application
    WGBitmap*     _screen;  ///bitmap of the all screen
    WGBitmap* _background;  ///bitmap of background
};

///IGlyphNode Interface
///@brief general glyph interface, to implement draw, locate etc.
///       and define the method to draw, animation and effect.
class WARMGUI_API IGlyphNode : public IObject
{
public:
    IGlyphNode(void);
    IGlyphNode(const char* name);
    virtual ~IGlyphNode(void);

    enum EDGE_POSITION_TYPE {
        EDGE_POSITION_TYPE_NONE,            ///inside or outside
        EDGE_POSITION_TYPE_LEFT,
        EDGE_POSITION_TYPE_LEFT_TOP,
        EDGE_POSITION_TYPE_TOP,
        EDGE_POSITION_TYPE_RIGHT_TOP,
        EDGE_POSITION_TYPE_RIGHT,
        EDGE_POSITION_TYPE_RIGHT_BOTTOM,
        EDGE_POSITION_TYPE_BOTTOM,
        EDGE_POSITION_TYPE_LEFT_BOTTOM,
    };

public:
    virtual void        SetRect(RECT& rect) {_rect = rect;}   ///set rectangle of glyph on screen
    RECT&               GetRect() { return _rect; }           ///get the rectangle

    const char*         GetName() {return _name;}
    void                SetName(const char* name) { if (name) strcpy_s(_name, MAX_WARMGUI_NAME_LEN, name); }

    uint32_t            GetId() {return _myid;}
    void                SetId(uint32_t id) {_myid = id;}
    inline bool         IsMe(UINT id) { return id == _myid; }
    inline bool         IsMe(const char* name) { return (!(strcmp(_name, name)));}
    
    virtual HRESULT     Draw(bool redraw = false)  = 0; ///draw the graph
    virtual HRESULT     DrawBkg(bool redraw = false)  = 0;  ///draw background
    virtual HRESULT     Renew() = 0;                        ///renew by increasement
    virtual HRESULT     Init() { return S_OK; }
    inline virtual bool Intersect(int x, int y);            //for IRectPosition
    inline virtual EDGE_POSITION_TYPE IsEdge(int x, int y); ///the point(x,y) of screen is edge of graph

    GLYPH_STATE_TYPE    GetGlyphState() {return (GLYPH_STATE_TYPE)_glyph_state;}
    void                SetGlyphState(GLYPH_STATE_TYPE glyph_state) {glyph_state = (GLYPH_STATE_TYPE)_glyph_state;}

    virtual void        SetCommonArtist(eArtist* artist) {_common_artist = artist;}
    eArtist*            GetCommonArtist() { return _common_artist; }

    void                SetVisible(bool bVisible = true) { _visible = bVisible; }
    bool                IsVisible() {return _visible;}

    virtual int         OnMouseMove    (int x, int y);  //Message Functions
    virtual int         OnLButtonUp    (int x, int y);
    virtual int         OnRButtonUp    (int x, int y);

    virtual void        OnSize         (UINT nType, int cx, int cy);
    virtual int         OnSetfocus     (WPARAM wParam, LPARAM lParam);
    virtual int         OnKillfocus    (WPARAM wParam, LPARAM lParam);
    virtual int         OnEnable       (WPARAM wParam, LPARAM lParam);
    virtual void        OnPaint        ();
    virtual int         OnClose        (WPARAM wParam, LPARAM lParam);
    virtual int         OnErasebkgnd   (WPARAM wParam, LPARAM lParam);

    virtual GLYPH_CHANGED_TYPE unselected()
                               {
                                   _glyph_state = (GLYPH_CHANGED_TYPE)((int)_glyph_state & ~((int)GLYPH_STATE_SELECTED));
#                                  ifdef _DEBUG
                                   TCHAR name[MAX_PATH];
                                   CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
                                   MYTRACE(L"unselected %s\n", name);
#                                  endif

                                   _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CANVAS_BKG);
                                   return GLYPH_CHANGED_TYPE_CANVAS_BKG;
                               }
    virtual GLYPH_CHANGED_TYPE selected()
                               {
                                   _glyph_state = (int)_glyph_state | (int)GLYPH_STATE_SELECTED;
#                                  ifdef _DEBUG
                                   TCHAR name[MAX_PATH];
                                   CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
                                   MYTRACE(L"selected %s\n", name);
#                                  endif

                                   _changed_type = (GLYPH_CHANGED_TYPE)((int)_changed_type | (int)GLYPH_CHANGED_TYPE_CANVAS_BKG);
                                   return GLYPH_CHANGED_TYPE_CANVAS_BKG;
                               }

public:
    char  _name[MAX_WARMGUI_NAME_LEN];  ///graph name
protected:
    RECT                        _rect;  ///every glyph has an rectagle region to draw, redraw or animate
    uint32_t                    _myid;  ///graph unique id

    int                  _glyph_state;   ///indicate the glyph state, selected, moving, etc.
    GLYPH_CHANGED_TYPE  _changed_type;   ///the glyph has changed, so need redraw it
    eArtist*           _common_artist;
    bool                     _visible;      ///is visible or not

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("IGlyphNode"); }
};


class ICanvas;
class IAtelier;

typedef std::vector<MATRIX_2D>      Operations;
typedef Operations::iterator            OpIter;
typedef Operations::const_iterator ConstOpIter;

class IGlyph;
typedef tree<IGlyph*> WARMGUI_API CGlyphTree;
typedef CGlyphTree::iterator GlyphTreeIter;


////////////////////////////////////////////////////////////////////////////////////////////////
///class IGlyph
class WARMGUI_API IGlyph : public IGlyphNode
{
public:
    IGlyph(void);
    IGlyph(const char* name, bool own_artist = false);
    virtual ~IGlyph(void);

    //DrawGraph Action
    ///this type indecates how to draw a glyph, canvas ...
    enum DRAW_METHOD_TYPE {
        DRAW_METHOD_TYPE_DRAW      = 0x0,    ///(re-)draw
        DRAW_METHOD_TYPE_DRAW_BMP  = 0x1,    ///draw the bitmap, if do not exist then create a bitmap, 
                                             ///keep glyph in the bitmap after (re-)draw.
        DRAW_METHOD_TYPE_THREAD    = 0x2,    ///draw by thread
        DRAW_METHOD_TYPE_ANIMATION = 0x4,    ///draw animation
    };

    enum GLYPH_TYPE {
        GLYPH_TYPE_GLYPH, ///as a graph
        GLYPH_TYPE_BKG,   ///as a backgound graph
    };


    enum GLYPH_STRETCH_METHOD {
        GLYPH_STRETCH_METHOD_RECT,  ///stretch to the configed-rect
        GLYPH_STRETCH_METHOD_OSIZE, ///original size
    };


    virtual HRESULT            Draw(bool redraw = false);
    virtual HRESULT            DrawBkg(bool redraw = false);
    virtual HRESULT            Renew();
    virtual HRESULT            DrawGraph(bool redraw = false) = 0;
    virtual HRESULT            PreDraw();
    virtual HRESULT            RenewGraph();
    virtual HRESULT            Update() { return S_OK; }

    //transform
    inline void                SetTransform(const MATRIX_2D* mytrans);
    //inline void                GetbackTransform();
    inline void                InvertTransform(int x, int y, POINT_f* ptOut);
    inline void                Transform(float x, float y, POINT* ptOut);

    //operation
    inline void                Translation();
    inline void                Roate();
    inline void                Skew();
    inline void                Mirror();

    virtual int                OnCut  (WPARAM wParam, LPARAM lParam);
    virtual int                OnCopy (WPARAM wParam, LPARAM lParam);
    virtual int                OnPaste(WPARAM wParam, LPARAM lParam);
    virtual int                OnClear(WPARAM wParam, LPARAM lParam);
    virtual int                OnUndo (WPARAM wParam, LPARAM lParam);

    inline void                SetFill(bool bFill) { _bFill = bFill; }
    void                       SetCanvas (ICanvas   *canvas) {_canvas  =  canvas;}
    void                       SetAtelier(IAtelier *atelier) {_atelier = atelier;}
    virtual void               SetParam(uint32_t id, eArtist* artist, ICanvas *canvas, IAtelier *atelier)
                               { _myid = id, _artist = _common_artist = artist, _canvas = canvas, _atelier = atelier, SetRenderTarget();}

    ///set the rectangle on screen, but some kind of glyph retrive rect from calculated by himself
    inline virtual void        SetRect(RECT& rect);
    //virtual void             DrawBitmap() = 0;

    void                       SetDrawMethod(DRAW_METHOD_TYPE draw_method) {_draw_method = draw_method;}
    DRAW_METHOD_TYPE           GetDrawMethod() {return _draw_method;}

    GLYPH_TYPE                 GetGlyphType() {return _glyph_type;}
    void                       SetGlyphTYpe(GLYPH_TYPE glyph_type) {_glyph_type = glyph_type;}


    bool                       IsResponseInput() { return _bResponseInput;}
    void                       SetResponseInput(bool bResponse) { _bResponseInput = bResponse;}

    virtual GLYPH_CHANGED_TYPE IsChanged() { return _changed_type; }
    virtual void               Changed(GLYPH_CHANGED_TYPE changed_type)  { _changed_type = (GLYPH_CHANGED_TYPE) ((int)_changed_type | (int)changed_type); }

    inline void                SetRenderTarget();

    void                       SetGlyphTreeIter(GlyphTreeIter iter) { _iter = iter;}
    GlyphTreeIter              GetGlyphTreeIter(void) { return _iter; }


protected:
    bool                   _bFill;

    ICanvas*              _canvas;
    IAtelier*            _atelier;

    DRAW_METHOD_TYPE _draw_method;  ///draw method
    GLYPH_TYPE        _glyph_type;  ///glyph type
    bool          _bResponseInput;  ///does the glyph response to mouse or keyboard, key etc.

    eArtist*        _myown_artist;
    eArtist*              _artist;

    CriticalLock*     _lockArtist;

    Operations*              _ops;
    GlyphTreeIter           _iter;   ///a iter pointer to myself
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("IGlyph"); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// class CImage
class WARMGUI_API CImage : public IGlyph {
public:
    CImage();
    CImage(const char* name, const char* imgpath);
    CImage(const char* name, const TCHAR * imgpath);
    CImage(const char* name);
    virtual ~CImage();

    HRESULT         DrawGraph_wired();
    virtual HRESULT DrawGraph(bool redraw = false);
    inline  HRESULT DrawImage(RECT& rectSrc, RECT& rectDest, float opacity = 1.0f);
    inline  HRESULT DrawImage(RECT_f& rectSrc, RECT_f& rectDest, float opacity = 1.0f);
    inline  virtual bool Intersect(int x, int y) { return pt_in_rect(_rect, x, y); }
    inline  virtual EDGE_POSITION_TYPE IsEdge(int x, int y) {return EDGE_POSITION_TYPE_NONE;}
    inline  HRESULT SetImage(const TCHAR* imgpath = 0);
    inline  void    SetOpacity(float opacity);
    inline  void    SetSrcRect(RECT* rect);
    inline  void    SetSrcRectFromScaledRect(RECT* scaledRect, RECT* targetRect);

    inline HRESULT  GetSharedImage(WGBitmap** pDestBmp) const;
    inline HRESULT  SetSharedImage(const WGBitmap* pSrcBmp) const;

protected:
    WGBitmap*        _pImage;          ///Dont call SafeRelease to _pImage!!!
    TCHAR _imgpath[MAX_PATH];
    float           _opacity;

    RECT            _srcRect;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CImage"); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// class CImage
class WARMGUI_API CSharedImage : public IGlyph {
public:
    CSharedImage();
    CSharedImage(IGlyph::GLYPH_TYPE glyph_type, WGBitmap* pBmp, GLYPH_STRETCH_METHOD sm = GLYPH_STRETCH_METHOD_RECT);
    ~CSharedImage();

    inline virtual HRESULT DrawGraph(bool redraw = false);
    inline virtual void    SetRect(RECT& rect);
    inline void            SetSharedImage(WGBitmap* pBmp) {_pBmp = pBmp;}
    inline HRESULT         DrawImage(RECT& rectSrc, RECT& rectDest, float opacity = 1.0f);
    inline void            SetSharedImage(RECT& rectSrc, float opacity = 1.0f);

protected:
    WGBitmap*          _pBmp;            ///Dont call SafeRelease to _pImage!!!
    TCHAR _imgpath[MAX_PATH];
    float           _opacity;
    GLYPH_STRETCH_METHOD _sm;   ///stretch method
    D2D1_RECT_F     _srcRect;
    D2D1_RECT_F     _dstRect;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CSharedImage"); }
};


/////////////////////////////////////////////////////////////////////////////////////////////////
// class CBlind
class WARMGUI_API CBlind : public IGlyph
{
public:
    CBlind(const char* name)
        : _bkgclr(0)
        , _alpha(0.3f)
    {
        _snprintf_s(_name, MAX_WARMGUI_NAME_LEN, _TRUNCATE, "Blind-%s", name);
        _glyph_type = IGlyph::GLYPH_TYPE_BKG;
        setClass();
    }

    CBlind(const char* name, COLORREF bkgclr, float alpha = 0.3f, bool isbkg = true)
        : _bkgclr(bkgclr)
        , _alpha(alpha)
    {
        _snprintf_s(_name, MAX_WARMGUI_NAME_LEN, _TRUNCATE, "Blind-%s", name);
        _glyph_type = (isbkg) ? IGlyph::GLYPH_TYPE_BKG : IGlyph::GLYPH_TYPE_GLYPH;
        setClass();
    }

    virtual HRESULT DrawGraph(bool redraw = false);
    void            SetBrush(COLORREF bkgclr, float alpha = 0.5f) {_bkgclr = bkgclr, _alpha = alpha;}

private:
    float     _alpha;
    COLORREF _bkgclr;


private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CBlind"); }
};


class IDataContainer;
////////////////////////////////////////////////////////////////////////////////////////////////////
// class IDataGraph
class WARMGUI_API IDataGraph : public IGlyph
{
public:
                               IDataGraph ( const char* name,
                                            bool world_own_type = false,
                                            bool data_own_type  = false,
                                            bool own_artist = false)
                                   : IGlyph(name, own_artist)
                                   , _pdata(0)
                                   , _data_own_type(data_own_type)
                                   , _world_own_type(world_own_type)
                               {
                                   setClass(); *_strconf = 0;
                                   if (world_own_type)
                                       _referframe = new CWorld();
                                   else
                                       _referframe = 0;
                               }
    virtual                   ~IDataGraph ()
                               { if (_data_own_type == true) SafeDelete(_pdata);
                                 if (_world_own_type == true) SafeDelete(this->_referframe); }

    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype) = 0;
    virtual bool               RenewData()    { return true; }
    virtual bool               process_data() { return true; }

    /*void                       setConfig(CWarmguiConfig* config) { _config = config; }
    void                       setConfigStr(const char* config_str)
                               {if (*_name)
                                    _snprintf_s(_strconf, MAX_PATH, _TRUNCATE, "%s.%s", config_str, _name);
                               else
                                    strcpy_s(_strconf, MAX_PATH, config_str);}*/
    void                       setConfig(CWarmguiConfig* config, const char* config_str)
                               {
                                   _config = config;
                                   strcpy_s(_strconf, MAX_PATH, config_str);
                                    /*
                                    try {
                                        //_my_disp;
                                        char temp[MAX_PATH];
                                        _snprintf_s(temp, MAX_PATH, "%s.containers", this->_strconf);
                                        strcpy_s(_my_cont, MAX_PATH, _config->getString(temp).c_str());
                                    } catch(...) {
                                    }
                                    */
                               }
    virtual HRESULT     Init() {
        if (_world_own_type && _referframe) {
            char key[MAX_PATH];
            _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.world", _strconf);
            if (_referframe->setConfig(_config, _strconf))
                return S_OK;
            else
                return (-1);
        }
        return S_OK;
    }
    void                       setWorld(const CWorld* cart) {_referframe = (CWorld*)cart;}
    CWorld*                    getWorld() { return _referframe; }
    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr dop) { return GLYPH_CHANGED_TYPE_NONE;}
    virtual GLYPH_CHANGED_TYPE NewData(dataptr pdata, DataObject::MARKET_DATA_TYPE datatype) { return GLYPH_CHANGED_TYPE_NONE;}
    const   char*              getContainerName() {return _my_cont;}
    const   char*              getName() {return _name;}

protected:
    CWarmguiConfig*   _config;
    char   _strconf[MAX_PATH];
    char _data_name[MAX_PATH];    //the data name from data-container
    dataptr            _pdata;
    size_t       _n_data_size;
    bool       _data_own_type;
    bool      _world_own_type;
    CWorld*       _referframe;    ///the pointer to reference frame system
    char   _my_cont[MAX_PATH];
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("IDataGraph"); }
};

typedef std::vector<IDataGraph*>       DataGraphArray    ;
typedef DataGraphArray::iterator       DataGraphIter     ;
typedef DataGraphArray::const_iterator DataGraphConstIter;


}   //namespace WARMGUI

#endif //__warmgui_glyph_h_include__
