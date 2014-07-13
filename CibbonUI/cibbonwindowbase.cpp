#include "stdafx.h"
#include "cibbonwindowbase.h"

using namespace std;
using namespace D2D1;
namespace cibbonui
{
	/******************************************************************
	*                                                                 *
	*                                                                 *
	*                   RenderManager                                 *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/
	shared_ptr<cuirendermanager> cuirendermanager::pManager(nullptr);
	template<class Interface>
	inline void Free(Interface **ppInterfaceToRelease)
	{
		if (*ppInterfaceToRelease != nullptr)
		{
			(*ppInterfaceToRelease)->Release();

			(*ppInterfaceToRelease) = nullptr;
		}
	}
	cuirendermanager::cuirendermanager(HWND hWnd) :m_hWnd(hWnd), ifbegin(false), beginnum(0), falsepRT(nullptr)
	{
		CreateDeviceIndependentResources();
		CreateDeviceResources();
	}


	cuirendermanager::~cuirendermanager()
	{
		Free(&pD2DFactory);
		Free(&pRT);
		for_each(brushmap.begin(), brushmap.end(), [](pair<int, ID2D1SolidColorBrush*> pr)->void{Free(&pr.second); });
		//_CrtDumpMemoryLeaks();
	}

	std::shared_ptr<cuirendermanager> cuirendermanager::getManager(HWND hWnd)
	{
		if (!pManager)
			pManager.reset(new cuirendermanager(hWnd));
		return pManager;
	}

	void cuirendermanager::CreateDeviceIndependentResources()
	{
		HRESULT hr = E_FAIL;
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pD2DFactory);
		if (FAILED(hr)) std::abort();
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(pDwFactory),
			reinterpret_cast<IUnknown **>(&pDwFactory));
		if (FAILED(hr)) std::abort();
		HDC screen = GetDC(0);
		DPIX = GetDeviceCaps(screen, LOGPIXELSX) / 96.0f;
		DPIY = GetDeviceCaps(screen, LOGPIXELSY) / 96.0f;
		ReleaseDC(0, screen);
	}

	void cuirendermanager::CreateDeviceResources()
	{
		HRESULT hr;
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		windowrect = D2D1::RectF(static_cast<float>(rect.left), static_cast<float>(rect.top), static_cast<float>(rect.right), static_cast<float>(rect.bottom));
		hr = pD2DFactory->CreateHwndRenderTarget(
			RenderTargetProperties(/*D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED)*/),
			HwndRenderTargetProperties(m_hWnd, SizeU(static_cast<UINT>(rect.right - rect.left),
			static_cast<UINT>(rect.bottom - rect.top))),
			&pRT
			);
		if (FAILED(hr)) std::abort();

	}

	void cuirendermanager::resize(cint x, cint y)
	{
		pRT->Resize(SizeU(x, y));
	}

	 void cuirendermanager::clearall(cint Color)
	{
		return pRT->Clear(ColorF(Color));
	}

	 void cuirendermanager::begindraw()
	{
		 pRT->BeginDraw();
	}

	void cuirendermanager::enddraw()
	{
		pRT->EndDraw();
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

	IDWriteTextFormat* cuirendermanager::getFormat(float fontsize, wstring fontname)
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


	inline void cuirendermanager::drawrect(const CRect& rect, float linewidth, cint color)
	{
		return pRT->DrawRectangle(rect, getBrush(color), linewidth);
	}

	 void cuirendermanager::drawline(const CPointf& ltop, const CPointf& rbottom, float linewidth, cint color)
	{
		return pRT->DrawLine(ltop, rbottom, getBrush(color), linewidth);
	}

	void cuirendermanager::FillRect(const D2D1_RECT_F& rect, cint color)
	{
		return pRT->FillRectangle(rect, getBrush(color));
	}

	void cuirendermanager::drawtext(wstring text, float fontsize, const CRect& _rect, DWRITE_TEXT_ALIGNMENT Alig, cint Color)
	{
		auto pFormat = getFormat(fontsize, L"Microsoft YaHei");
		pFormat->SetTextAlignment(Alig);
		pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pRT->DrawTextW(
			text.c_str(),
			text.size(),
			pFormat,
			_rect,
			getBrush(Color)
			);
		pFormat->Release();
	}



	PatternManagerBase::PatternManagerBase(HWND hWnd) :pRendermanager(cuirendermanager::getManager(hWnd))
	{

	}
	 void PatternManagerBase::drawtexthelper(cibboncontrolbase* pControl, DWRITE_TEXT_ALIGNMENT Alig, cint Color)
	{
		return pRendermanager->drawtext(pControl->getwindowtext().c_str(),2* sqrt(pControl->getPosition().bottom - pControl->getPosition().top), pControl->getPosition(), Alig, Color);
	}

	

	/******************************************************************
	*                                                                 *
	*                                                                 *
	*                  Windows & ControlBase                          *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/


	cuiwindowbase::cuiwindowbase()
	{
	}
	cuiwindowbase::~cuiwindowbase()
	{
		
	}

	cuiwindowbase::cuiwindowbase(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle, cint _width, cint _height, cstyle _style)
		:
		hInst(_hInst),
		title(_title),
		windowstyle(_windowstyle),
		width(_width),
		height(_height),
		realstyle(_style),
		windowmessage(),
		subject()
	{

	}

	void cuiwindowbase::run()
	{
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);
		runmessageloop();
	}


	inline void cuiwindowbase::runmessageloop()
	{
		MSG msg{ 0 };
		while (::GetMessage(&msg, NULL, 0, 0))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	void cuiwindowbase::init()
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = cuiwindowbase::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = hInst;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = 0;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = L"cuiwindow";
		RegisterClassEx(&wcex);
		m_hWnd = CreateWindow(
			L"cuiwindow",
			title.c_str(),
			windowstyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			width,
			height,
			0,
			0,
			hInst,
			this
			);
		if (!m_hWnd) std::abort();
		
		
	}

	LRESULT CALLBACK cuiwindowbase::WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		cuiwindowbase* pwindow = nullptr;
		if (Message == WM_CREATE)
		{
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			pwindow = reinterpret_cast<cuiwindowbase*>(pcs->lpCreateParams);
			::SetWindowLongPtr(
				hWnd,
				GWLP_USERDATA,
				PtrToUlong(pwindow));

		}
		else
		{
			pwindow = reinterpret_cast<cuiwindowbase*>
				(static_cast<LONG_PTR>(
				::GetWindowLongPtr(
				hWnd,
				GWLP_USERDATA)));
		}
		if (pwindow)
		{
			auto it = pwindow->windowmessage.find(Message);
			if (it != (pwindow->windowmessage).end())
			{
				bool ifall = false;
				for (auto x : it->second)
				{
					ifall = x(hWnd, Message, wParam, lParam);
					if (ifall) return 0;
				}
			}
		}
		return ::DefWindowProc(hWnd, Message, wParam, lParam);
	}


	inline void cuiwindowbase::addevents(UINT Message, winfunc Func)
	{
		windowmessage[Message].push_back(Func);
	}

	cuistdwindow::cuistdwindow(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle, cint _width, cint _height, cstyle _style)
		:cuiwindowbase(_hInst, _title, _windowstyle, _width, _height, _style)
	{
		initevents();
		init();
	}

	cuistdwindow::cuistdwindow() :cuiwindowbase(){}
	cuistdwindow::~cuistdwindow(){}

	void cuistdwindow::initevents()
	{
		addevents(WM_CLOSE, [](WINPAR)->bool{
			::PostQuitMessage(0);
			return notyet;
		}
		);
		addevents(WM_PAINT, [this](WINPAR)->bool
		{
			PAINTSTRUCT ps;
			auto hdc = BeginPaint(hWnd, &ps);
			auto x = cuirendermanager::getManager(hWnd);
			x->begindraw();
			x->clearall(defaultbackgroundcolor);
			x->drawtext(title, 3 * sqrt( Captionheight), RectF(20, 5, getPosition().right, Captionheight),Alignmentleft);
			x->enddraw();
			cuievent bevent;
			bevent.eventname = cuieventenum::controlinit;
			notifyobservers(&bevent);
			EndPaint(hWnd, &ps);
			return notyet;
		});
		addevents(WM_SIZE, [](WINPAR)->LRESULT
		{
			cuirendermanager::getManager(hWnd)->resize( LOWORD(lParam), HIWORD(lParam));
			return already;
		}
		);
		
		addhelper(WM_MOUSEMOVE, mousemove,true);
		addevents(WM_LBUTTONDOWN, [this](WINPAR)->bool{
			if (GET_Y_LPARAM(lParam) < Captionheight && GET_X_LPARAM(lParam) < getPosition().right - 2 * closebuttonwidth)
			{
				::SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
				return already;
			}
			else
				SetCapture(hWnd);/**/
			return notyet;
		});
		addhelper(WM_LBUTTONDOWN, lbuttondown,true);
		
		addevents(WM_LBUTTONUP, [=](WINPAR)->bool{
			ReleaseCapture();
			cuievent bevent;
			bevent.eventposition = { GET_X_LPARAM(lParam)  , GET_Y_LPARAM(lParam) };
			bevent.eventname = lbuttonup;
			notifyobservers(&bevent);
			return already;
		});
		addhelper(WM_RBUTTONDOWN, rbuttondown,false);
		addhelper(WM_RBUTTONUP, rbuttonup,true);
		addhelper(WM_LBUTTONDBLCLK, lbuttondoubleclick,true);
		addhelper(WM_PAINT, controlinit,false);
		
		
	}

	inline void cuistdwindow::addhelper(UINT Message, cuieventenum cuienum,bool bif)
	{
		addevents(Message, [=](WINPAR)->bool{
			cuievent bevent;
			bevent.eventposition = { GET_X_LPARAM(lParam) , GET_Y_LPARAM(lParam) };
			bevent.eventname = cuienum;
			notifyobservers(&bevent);
			return notyet;
		});
	}

	cibboncontrolbase::cibboncontrolbase(PatternManagerBase* _pPatternManager, const CRect& _Position, const std::wstring& _text, bool Enable)
		:observer(),
		subject(),
		iffocus(false),
		Position(_Position),
		windowtext(_text),
		ifenabled(Enable),
		pPatternManager(_pPatternManager),
		ifin(false)
	{
		rPosition = { static_cast<LONG>(Position.left), static_cast<LONG>(Position.top), static_cast<LONG>(Position.right), static_cast<LONG>(Position.bottom) };
	}

	cibboncontrolbase::~cibboncontrolbase()
	{
		delete pPatternManager;
	}


	template<typename P,typename R>
	inline bool ifinrect(P Point, R Rect)
	{
		return Point.x > Rect.left && Point.x < Rect.right && Point.y > Rect.top && Point.y < Rect.bottom;
	}

	void cibboncontrolbase::HandleNotify(cuievent* pceb)
	{
		if (pceb->eventname == controlinit) goto notify;
		bool x(ifinrect( pceb->eventposition,rPosition));
		if (pceb->eventname == mousemove)
		{
			if ((!x && !ifin) || (x && ifin)) return;
			pceb->eventname = x ? mousemovein : mousemoveout;
			ifin = x;
		}
		else
		{
			if (!x) return;
		}
		/*else if (!iffocus&&pceb->eventname!=lbuttondown)
			return;*/
	notify:
		auto it = EventHandler.find(pceb->eventname);
		if (it != EventHandler.end())
		{
			for (auto vit : it->second)
				vit();
		}
	}


	

}

