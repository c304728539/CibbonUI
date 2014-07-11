#include "stdafx.h"
#include "cibbonwindows.h"

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
	cuirendermanager::cuirendermanager(HWND hWnd) :m_hWnd(hWnd), ifbegin(false), beginnum(0)
	{
		CreateDeviceIndependentResources();
		CreateDeviceResources();
	}


	cuirendermanager::~cuirendermanager()
	{

		Free(&pD2DFactory);
		Free(&pRT);
		for_each(brushmap.begin(), brushmap.end(), [](pair<int, ID2D1SolidColorBrush*> pr)->void{pr.second->Release(); });
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
	inline void cuirendermanager::clearall(cint Color)
	{
		return pRT->Clear(ColorF(Color));
	}

	inline void cuirendermanager::begindraw()
	{

		if (InterlockedIncrement(&beginnum)== 1)
			pRT->BeginDraw();
	}

	void cuirendermanager::enddraw()
	{
		if (!InterlockedDecrement(&beginnum))
		{
			HRESULT hr = pRT->EndDraw();
		    //if (FAILED(hr)) throw "haha";
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
			DWRITE_FONT_WEIGHT_THIN,
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

	inline void cuirendermanager::drawline(const CPointf& ltop, const CPointf& rbottom, float linewidth, cint color)
	{
		return pRT->DrawLine(ltop, rbottom, getBrush(color), linewidth);
	}

	inline void cuirendermanager::FillRect(const D2D1_RECT_F& rect, cint color)
	{
		return pRT->FillRectangle(rect, getBrush(color));
	}

	void cuirendermanager::drawtext(wstring text, cint fontsize, const CRect& _rect, DWRITE_TEXT_ALIGNMENT Alig, cint Color)
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
	inline void PatternManagerBase::drawtexthelper(cibboncontrolbase* pControl, DWRITE_TEXT_ALIGNMENT Alig, cint Color)
	{
		return pRendermanager->drawtext(pControl->getwindowtext().c_str(),sqrt(pControl->getPosition().bottom - pControl->getPosition().top), pControl->getPosition(), Alig, Color);
	}

	ButtonPattern::ButtonPattern(HWND hWnd) : PatternManagerBase(hWnd)
	{

	}
	void ButtonPattern::drawdown(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->enddraw();
	}
	void ButtonPattern::drawusual(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->FillRect(pControl->getPosition(), ColorF::White);
		drawtexthelper(pControl, Alignmentcenter, ColorF::Black);
		//pRendermanager->drawtext(pControl->getwindowtext().c_str(), 20, pControl->getPosition(), Alignmentleft, ColorF::Black);
		pRendermanager->enddraw();
	}
	void ButtonPattern::drawfocus(cibboncontrolbase* pControl)
	{

	}
	void ButtonPattern::drawmove(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->FillRect(pControl->getPosition(), ColorF::SkyBlue);
		drawtexthelper(pControl, Alignmentcenter, ColorF::Black);
		pRendermanager->enddraw();
	}
	inline void ButtonPattern::drawup(cibboncontrolbase* pControl)
	{
		return drawusual(pControl);
	}

	void ButtonPattern::initdraw(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->clearall();
		pRendermanager->FillRect(pControl->getPosition(), ColorF::White);
		drawtexthelper(pControl, Alignmentcenter, ColorF::Black);
		pRendermanager->enddraw();
	}

	/******************************************************************
	*                                                                 *
	*                                                                 *
	*                  Control                                        *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/


	cuiwindowbase::cuiwindowbase()
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

	cuiwindowbase::~cuiwindowbase()
	{
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
		//initevents();
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
			//map<UINT, cuiwindowbase::winfunc>::iterator it;
			auto it = pwindow->windowmessage.find(Message);
			if (it != (pwindow->windowmessage).end())
			{
				bool ifall = false;
				for (auto x : it->second)
					ifall = x(hWnd,Message,wParam,lParam);
				if (ifall) return 0;
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
		addevents(WM_CLOSE, [](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT{
			::PostQuitMessage(0);
			return notyet;
		}
		);
		addevents(WM_PAINT, [this](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT
		{
			cuievent bevent;
			bevent.eventname = cuieventenum::controlinit;
			notifyobservers(&bevent);
			return notyet;
		});
		addevents(WM_SIZE, [](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT
		{
			LONG_PTR Style = ::GetWindowLongPtr(hWnd, GWL_STYLE);
			Style = Style &~WS_CAPTION &~WS_SYSMENU &~WS_SIZEBOX;
			::SetWindowLongPtr(hWnd, GWL_STYLE, Style);
			return notyet;
		}
		);
		addhelper(WM_LBUTTONDOWN, lbuttondown);
		addhelper(WM_MOUSEMOVE, mousemove);
		addhelper(WM_LBUTTONUP, lbuttonup);
		addhelper(WM_RBUTTONDOWN, rbuttondown);
		addhelper(WM_RBUTTONUP, rbuttonup);
		addhelper(WM_LBUTTONDBLCLK, lbuttondoubleclick);
	}

	inline void cuistdwindow::addhelper(UINT Message, cuieventenum cuienum)
	{
		addevents(Message, [=](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT{
			cuievent bevent;
			bevent.eventposition = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
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
		rPosition = { Position.left, Position.top, Position.right, Position.bottom };
	}

	void cibboncontrolbase::HandleNotify(cuievent* pceb)
	{
		if (pceb->eventname == controlinit) goto notify;
		if (!PtInRect(&rPosition, pceb->eventposition))
		{
			if (!iffocus && pceb->eventname != mousemove) return;
			if (!ifin)
				return;
			else if (pceb->eventname == mousemove)
			{
				pceb->eventname = mousemoveout;
				ifin = false;
			}
		}
		else
		{
			if (ifin && pceb->eventname == mousemove) return;
			if (!ifin && pceb->eventname == mousemove)
			{
				pceb->eventname = mousemovein;
				ifin = true;
			}
		}
	notify:
		auto it = EventHandler.find(pceb->eventname);
		if (it != EventHandler.end())
		{
			for (auto vit : it->second)
				vit();
		}
	}


	cuibutton::cuibutton(PatternManagerBase* _pPatternManager, const CRect& _Position, const std::wstring& _text, bool Enable)
		:cibboncontrolbase(_pPatternManager, _Position, _text, Enable)
	{
		initevents();
	}
	//绘制事件
	void cuibutton::initevents()
	{
		addevents(controlinit, [this]()->void{
			static_cast<ButtonPattern*>(pPatternManager)->initdraw(this);
		});
		addevents(lbuttondown, [this]()->void{
			static_cast<ButtonPattern*>(pPatternManager)->drawdown(this);
		});
		addevents(lbuttonup, [this]()->void{
			static_cast<ButtonPattern*>(pPatternManager)->drawup(this);
		});
		addevents(mousemovein, [this]()->void{
			static_cast<ButtonPattern*>(pPatternManager)->drawmove(this);
		});
		auto Func = static_cast<unsigned int(__stdcall*)(void*)>([](void* p)-> unsigned int{
			auto x = static_cast<cuibutton*>(p);
			static_cast<ButtonPattern*>(x->getPatternManager())->drawusual(x);
			return 0;
		});
		addevents(mousemoveout, [this, Func]()->void{
			_beginthreadex(0, 0,
				Func,
				this, 0, 0);  });//添加动画
	}
	void cuibutton::Onclick(const std::function<void()>& func)
	{
		return addevents(lbuttondown, func);
	}

}

