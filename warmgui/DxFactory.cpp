#include "StdAfx.h"
#include "utility.h"
#include "ChineseCodeLib.h"
#include "warmgui.h"
#include "warmgui_type.h"
#include "DxFactorys.h"

using namespace WARMGUI;

CDxFactorys::CDxFactorys()
//	: m_hwnd(0)
	: m_pD2DFactory(0)
	, m_pImgFactory(0)
	, m_pDWriteFactory(0)
//	, m_pHwndRenderTarget(0)
	, m_dpiScaleX(0)
	, m_dpiScaleY(0)
	, m_itsme(0)
{
}

CDxFactorys::~CDxFactorys()
{
	ReleaseResource();
}

void CDxFactorys::ReleaseResource()
{
	SafeRelease(&m_pImgFactory);
    SafeRelease(&m_pD2DFactory);
	SafeRelease(&m_pDWriteFactory);
}

CDxFactorys * CDxFactorys::GetInstance()
{
	static CDxFactorys factorys;
	if (!factorys.m_itsme) {
		factorys.InitDxFactory();
		factorys.m_itsme = &factorys;
	}
	return &factorys;
}

HRESULT CDxFactorys::InitDxFactory()
{
	HRESULT hr = S_OK;
    // Create a Direct2D factory.

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_pD2DFactory);

	if (SUCCEEDED(hr))
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID*)&m_pImgFactory);

	//if (SUCCEEDED(hr))
//		hr = CreateHwndRenderTarget();

    // Create a shared DirectWrite factory.
    if (SUCCEEDED(hr))
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_pDWriteFactory));

    HDC screen = GetDC(0);
    m_dpiScaleX = GetDeviceCaps(screen, LOGPIXELSX) / 96.0f;
    m_dpiScaleY = GetDeviceCaps(screen, LOGPIXELSY) / 96.0f;
    ReleaseDC(0, screen);

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	_size_screen.width  = static_cast<float>(cx);
	_size_screen.height = static_cast<float>(cy);

	return hr;
}

/*
HRESULT CDxFactorys::CreateHwndRenderTarget(HWND hwnd, ID2D1HwndRenderTarget ** m_pHwndRenderTarget)
{
	D2D1_SIZE_U size = D2D1::SizeU(0, 0);

	// Create a Direct2D render target.
	return m_pD2DFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(hwnd, size),
				m_pHwndRenderTarget);
}

HRESULT CDxFactorys::CreateBitmapRenderTarget(ID2D1BitmapRenderTarget ** pBitmapRenderTarget)
{
	HRESULT hr = m_pHwndRenderTarget->CreateCompatibleRenderTarget(
		m_pHwndRenderTarget->GetSize(),
        pBitmapRenderTarget);

	return hr;
}

*/

HRESULT CDxFactorys::CreateRenderTarget(HWND hwnd,
										ID2D1HwndRenderTarget ** pHwndRenderTarget)
{
	D2D1_SIZE_U size = D2D1::SizeU(0, 0);

	// Create a Direct2D render target.
	return m_pD2DFactory->CreateHwndRenderTarget(
					D2D1::RenderTargetProperties(),
					D2D1::HwndRenderTargetProperties(hwnd, size),
					pHwndRenderTarget);
}

HRESULT CDxFactorys::CreateRenderTarget(HWND hwnd,
										ID2D1HwndRenderTarget ** pHwndRenderTarget,
										ID2D1BitmapRenderTarget ** pBitmapRenderTarget)
{
	D2D1_SIZE_U size = D2D1::SizeU(0, 0);

	// Create a Direct2D render target.
	HRESULT hr = m_pD2DFactory->CreateHwndRenderTarget(
					D2D1::RenderTargetProperties(),
					D2D1::HwndRenderTargetProperties(hwnd, size),
					pHwndRenderTarget);

	if (SUCCEEDED(hr))
		hr = (*pHwndRenderTarget)->CreateCompatibleRenderTarget(
				(*pHwndRenderTarget)->GetSize(),
				pBitmapRenderTarget);
	else {
		SafeRelease(pHwndRenderTarget);
		*pHwndRenderTarget = 0;
	}

	return hr;
}


//ID2D1HwndRenderTarget * CDxFactorys::GetHwndRenderTarget() { return m_pHwndRenderTarget; }
ID2D1Factory          * CDxFactorys::GetD2DFactory() { return m_pD2DFactory;}
IWICImagingFactory    * CDxFactorys::GetImgFactory() { return m_pImgFactory; }
IDWriteFactory        * CDxFactorys::GetDWriteFactory() { return m_pDWriteFactory; }


/////////////////////////////////////////////////////////////////////////////////
// 

namespace WARMGUI {
#ifdef _UNICODE
WARMGUI_API HRESULT GetBitmapFromFile(const char* szImgPath, ID2D1HwndRenderTarget* pRenderTarget, WGBitmap** pBitmap)
{
	TCHAR filepath[MAX_PATH];
	CChineseCodeLib::Gb2312ToUnicode(filepath, MAX_PATH, szImgPath);
	return GetBitmapFromFile(filepath, pRenderTarget, pBitmap);
}
#endif

WARMGUI_API HRESULT GetBitmapFromFile(const TCHAR* szImgPath, ID2D1HwndRenderTarget* pRenderTarget, WGBitmap** pBitmap)
{
	ID2D1BitmapRenderTarget* pBmpRT = 0;
	HRESULT hr = S_OK;
	//TCHAR filepath[MAX_PATH];
	//CChineseCodeLib::Gb2312ToUnicode(filepath, MAX_PATH, szImgPath);
	if (SUCCEEDED(pRenderTarget->CreateCompatibleRenderTarget(&pBmpRT)))
		hr = GetBitmapFromFile(szImgPath, pBmpRT, pBitmap);
	SafeRelease(&pBmpRT);
	return hr;
}

HRESULT GetBitmapFromFile(const TCHAR* szImgPath, ID2D1BitmapRenderTarget* pRenderTarget, WGBitmap** pBitmap)
{
	if (!pRenderTarget || !*szImgPath)
		return -1;

    HRESULT hr = S_OK;

	IWICBitmapDecoder * pDecoder = 0;

    hr = CDxFactorys::GetInstance()->GetImgFactory()->CreateDecoderFromFilename(
				szImgPath,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&pDecoder);

	IWICBitmapFrameDecode * pSource = 0;
	if (SUCCEEDED(hr))
        hr = pDecoder->GetFrame(0, &pSource);
    
	IWICFormatConverter * pConverter = 0;
    // Convert the image format to 32bppPBGRA
    // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
	if (SUCCEEDED(hr))
		hr = CDxFactorys::GetInstance()->GetImgFactory()->CreateFormatConverter(&pConverter);
	
	if (SUCCEEDED(hr))
        hr = pConverter->Initialize(pSource,
									GUID_WICPixelFormat32bppPBGRA,
									WICBitmapDitherTypeNone,
									NULL,
									0.f,
									WICBitmapPaletteTypeMedianCut);

	SafeRelease(pBitmap);
	if (SUCCEEDED(hr))
        // Create a Direct2D bitmap from the WIC bitmap.
        hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter,
														NULL,
														pBitmap);


	SafeRelease(&pConverter);
    SafeRelease(&pSource);
    SafeRelease(&pDecoder);

	return hr;
}

HRESULT ResizeRenderTarget(ID2D1HwndRenderTarget * pHwndRT, const int cx, const int cy)
{
	if (!pHwndRT)
		return (-1);

	D2D1_SIZE_U size = D2D1::SizeU(cx, cy);
	return pHwndRT->Resize(size);
}

HRESULT GetSharedBitmap(ID2D1RenderTarget * pRT, const WGBitmap * pSrcBmp, WGBitmap ** const pDestBmp)
{
	return pRT->CreateSharedBitmap(
					__uuidof(WGBitmap),
					(WGBitmap *)pSrcBmp,
					(0),
					pDestBmp);
}

HRESULT CopyFromRenderTarget(const ID2D1HwndRenderTarget* pHwndRT, const WGBitmap* pbmp, POINT& pnt, RECT& rect)
{
	if (!pHwndRT || !pbmp) return (-1);
	D2D1_POINT_2U destPoint = D2D1::Point2U(pnt.x, pnt.y);
	D2D1_RECT_U srcRect = D2D1::RectU(0, 0, RectWidth(rect), RectHeight(rect));
	ID2D1BitmapRenderTarget* pbmprt = 0;
	MATRIX_2D M;
	pHwndRT->GetTransform(&M);
	((ID2D1HwndRenderTarget*)pHwndRT)->SetTransform(D2D1::Matrix3x2F::Identity());
	HRESULT hr = ((ID2D1HwndRenderTarget*)pHwndRT)->CreateCompatibleRenderTarget(
		pHwndRT->GetSize(),
		&pbmprt);
	if (SUCCEEDED(hr))
		hr =((WGBitmap*)pbmp)->CopyFromRenderTarget(&destPoint, pbmprt, &srcRect);
	SafeRelease(&pbmprt);
	((ID2D1HwndRenderTarget*)pHwndRT)->SetTransform(M);
	return hr;
}


HRESULT CopyFromRenderTarget(const ID2D1BitmapRenderTarget* pBmpRT, const WGBitmap* pbmp, POINT& pnt, RECT& rect)
{
	if (!pBmpRT || !pbmp) return (-1);
	D2D1_POINT_2U destPoint = D2D1::Point2U(pnt.x, pnt.y);
	D2D1_RECT_U srcRect = D2D1::RectU(0, 0, RectWidth(rect), RectHeight(rect));
	MATRIX_2D M;
	pBmpRT->GetTransform(&M);
	((ID2D1BitmapRenderTarget*)pBmpRT)->SetTransform(D2D1::Matrix3x2F::Identity());
	HRESULT hr =((WGBitmap*)pbmp)->CopyFromRenderTarget(&destPoint, (ID2D1BitmapRenderTarget*)pBmpRT, &srcRect);
	((ID2D1BitmapRenderTarget*)pBmpRT)->SetTransform(M);
	return hr;
}

}
