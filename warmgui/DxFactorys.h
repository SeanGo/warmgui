#pragma once

namespace WARMGUI {
class WARMGUI_API CDxFactorys {
private:
	CDxFactorys();
	~CDxFactorys();

public:
	//HRESULT CreateBitmapRenderTarget();
	HRESULT CreateRenderTarget(HWND hwnd,
								ID2D1HwndRenderTarget ** pHwndRenderTarget,
								ID2D1BitmapRenderTarget ** pBitmapRenderTarget);
	HRESULT CreateRenderTarget(HWND hwnd,
								ID2D1HwndRenderTarget ** pHwndRenderTarget);
	//IWICImagingFactory * CreateImageFactory();

	//ID2D1HwndRenderTarget * GetHwndRenderTarget();
	ID2D1Factory          * GetD2DFactory();
	IWICImagingFactory    * GetImgFactory();
	IDWriteFactory        * GetDWriteFactory();

	HRESULT InitDxFactory();
	void ReleaseResource();

	static CDxFactorys * GetInstance();
public:
	float   m_dpiScaleX;
	float   m_dpiScaleY;
	SIZE_f _size_screen;

protected:
	//HWND m_hwnd;
	//ID2D1HwndRenderTarget * m_pHwndRenderTarget;
	ID2D1Factory          * m_pD2DFactory;
	IWICImagingFactory    * m_pImgFactory;
	IDWriteFactory        * m_pDWriteFactory;
	CDxFactorys * m_itsme;

};

#ifdef _UNICODE
WARMGUI_API HRESULT GetBitmapFromFile(const char* szImgPath, ID2D1HwndRenderTarget* pRenderTarget, WGBitmap** pBitmap);
#endif

WARMGUI_API HRESULT GetBitmapFromFile(const TCHAR* szImgPath, ID2D1HwndRenderTarget* pRenderTarget, WGBitmap** pBitmap);
WARMGUI_API HRESULT GetBitmapFromFile(const TCHAR* szImgPath, ID2D1BitmapRenderTarget* pRenderTarget, WGBitmap** pBitmap);
WARMGUI_API HRESULT ResizeRenderTarget(ID2D1HwndRenderTarget* pHwndRT, const int cx, const int cy);
WARMGUI_API HRESULT GetSharedBitmap(ID2D1RenderTarget* pRT, const WGBitmap* pSrcBmp, WGBitmap** pDestBmp);
WARMGUI_API HRESULT CopyFromRenderTarget(const ID2D1HwndRenderTarget* pRT, const WGBitmap* pbmp, POINT& pnt, RECT& rect);
WARMGUI_API HRESULT CopyFromRenderTarget(const ID2D1BitmapRenderTarget* pRT, const WGBitmap* pbmp, POINT& pnt, RECT& rect);
}


