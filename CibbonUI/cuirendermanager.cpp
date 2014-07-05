#include "stdafx.h"
#include "cuirendermanager.h"
#include "uibase.h"

namespace cibbonui{
	using namespace std;
	using namespace D2D1;

	shared_ptr<cuirendermanager> cuirendermanager::pManager = nullptr;
	template<class Interface>
	inline void Free(Interface **ppInterfaceToRelease)
	{
		if (*ppInterfaceToRelease != nullptr)
		{
			(*ppInterfaceToRelease)->Release();

			(*ppInterfaceToRelease) = nullptr;
		}
	}
	cuirendermanager::cuirendermanager(HWND hWnd) :m_hWnd(hWnd), ifbegin(false)
	{
		CreateDeviceIndependentResources();
		CreateDeviceResources();
	}


	cuirendermanager::~cuirendermanager()
	{
		Free(&pD2DFactory);
		Free(&pRT);
		for_each(brushmap.begin(), brushmap.end(), [](std::map<int, ID2D1SolidColorBrush*>::iterator it)->void{it->second->Release(); });
	}

	std::shared_ptr<cuirendermanager> cuirendermanager::getManager(HWND hWnd)
	{
		if (!pManager)
			pManager = make_shared<cuirendermanager>(hWnd);
		return pManager;
	}

	void cuirendermanager::CreateDeviceIndependentResources()
	{
		HRESULT hr = E_FAIL;
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
		if (FAILED(hr)) std::abort();
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(pDwFactory),
			reinterpret_cast<IUnknown **>(&pDwFactory));
		if (FAILED(hr)) std::abort();
	}

	void cuirendermanager::CreateDeviceResources()
	{
		HRESULT hr;
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		windowrect = D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom);
		hr = pD2DFactory->CreateHwndRenderTarget(
			RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED)),
			HwndRenderTargetProperties(m_hWnd, SizeU(static_cast<UINT>(rect.right - rect.left),
			static_cast<UINT>(rect.bottom - rect.top))),
			&pRT
			);
		if (FAILED(hr)) std::abort();
		
	}

	void cuirendermanager::begindraw()
	{
		if (!ifbegin)
		{
			ifbegin = true;
			pRT->BeginDraw();
		}
	}

	void cuirendermanager::enddraw()
	{
		if (ifbegin)
		{
			HRESULT hr = pRT->EndDraw();
			ifbegin = false;
			if (FAILED(hr)) std::abort();
		}
	}

	ID2D1SolidColorBrush* cuirendermanager::getBrush(cint color)
	{
		auto it = brushmap.find(color);
		if (it != brushmap.end())
		{
			return it->second;
		}
		ID2D1SolidColorBrush* pBrush = nullptr;
		HRESULT hr = pRT->CreateSolidColorBrush(
			ColorF(color), &pBrush);
		if (FAILED(hr)) std::abort();
		brushmap[color] = pBrush;
		return pBrush;
	}
	
	IDWriteTextFormat* cuirendermanager::getFormat(cint fontsize, wstring fontname)
	{
		IDWriteTextFormat* pFormat = nullptr;
		HRESULT hr = pDwFactory->CreateTextFormat(
			fontname.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontsize,
			L"",
			&pFormat);
		if (FAILED(hr)) std::abort();
		return pFormat;
	}


	void cuirendermanager::drawrect(const CPointf& ltop, const CPointf& rbottom, float linewidth, cint color)
	{
		pRT->DrawRectangle(RectF(ltop.x, ltop.y, rbottom.x, rbottom.y),getBrush(color),linewidth);
	}

	void cuirendermanager::drawline(const CPointf& ltop, const CPointf& rbottom, float linewidth, cint color)
	{
		pRT->DrawLine(ltop, rbottom, getBrush(color), linewidth);
	}

	void cuirendermanager::FillRect(const D2D1_RECT_F& rect, cint color)
	{
		pRT->FillRectangle(rect, getBrush(color));
	}

	void cuirendermanager::drawtext(wstring text, cint fontsize, const CRect& _rect, decltype(Alignmentcenter) Alig , cint Color)
	{
		auto pFormat = getFormat(fontsize);
		pFormat->SetTextAlignment(Alig);
		pRT->DrawTextW(
			text.c_str(),
			text.size(),
			pFormat,
			_rect,
			getBrush(Color)
			);
		pFormat->Release();
	}



	LayoutManagerBase::LayoutManagerBase(HWND hWnd) :pRendermanager(cuirendermanager::getManager(hWnd))
	{

	}
}

