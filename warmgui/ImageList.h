#ifndef __warmgui_image_list_h_include__
#define __warmgui_image_list_h_include__


namespace WARMGUI {

class WARMGUI_API CImageList : public IGlyph
{
public:
    CImageList();
    CImageList(IGlyph::GLYPH_TYPE glyph_type, WGBitmap* pBmp, GLYPH_STRETCH_METHOD sm = GLYPH_STRETCH_METHOD_RECT);
    ~CImageList();

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
    virtual void setClass() { SetMyClass("CImageList"); }
};

} //namespace WARMGUI


#endif //__warmgui_image_list_h_include__