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
	template<class Interface>
	inline void Free(Interface* & ppInterfaceToRelease)
	{
		if (ppInterfaceToRelease != nullptr)
		{
			ppInterfaceToRelease->Release();
			ppInterfaceToRelease = nullptr;
		}
	}

	unique_ptr<ID2D1Factory, std::function<void(ID2D1Factory*)>> FactoryManager::pD2DFactory(nullptr, [](ID2D1Factory* p)->void{Free(p); });
	unique_ptr<IDWriteFactory, std::function<void(IDWriteFactory*)>> FactoryManager::pDwFactory(nullptr, [](IDWriteFactory* p)->void{Free(p); });
	std::map<HWND, std::shared_ptr<cuirendermanager>> cuirendermanager::RenderManagers;
	cuiwindowbase* cuiTooltip::pOwner(nullptr);
	unique_ptr<cuiTooltip> cuiTooltip::pTooltip(nullptr);
	inline const unique_ptr<ID2D1Factory, std::function<void(ID2D1Factory*)>>& FactoryManager::getpD2DFactory()
	{
		if (!FactoryManager::pD2DFactory)
		{
			ID2D1Factory* pFactory = nullptr;
			HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
			FactoryManager::pD2DFactory.reset(pFactory);
		}
		return FactoryManager::pD2DFactory;
	}

	inline const unique_ptr<IDWriteFactory, std::function<void(IDWriteFactory*)>>& FactoryManager::getpDwFactory()
	{
		if (!FactoryManager::pDwFactory)
		{
			IDWriteFactory* pDwriteFactory = nullptr;
			HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(pDwriteFactory),
				reinterpret_cast<IUnknown **>(&pDwriteFactory));
			FactoryManager::pDwFactory.reset(pDwriteFactory);
		}
		return FactoryManager::pDwFactory;
	}

	cuirendermanager::cuirendermanager(HWND hWnd) :m_hWnd(hWnd), ifbegin(false), beginnum(0), pRT(nullptr)
	{
		CreateDeviceResources();
	}


	cuirendermanager::~cuirendermanager()
	{
		Free(pRT);
		for_each(brushmap.begin(), brushmap.end(), [](pair<int, ID2D1SolidColorBrush*> pr)->void{Free(pr.second); });
		//_CrtDumpMemoryLeaks();
	}

	std::shared_ptr<cuirendermanager> cuirendermanager::getManager(HWND hWnd)
	{
		if (RenderManagers.find(hWnd) == RenderManagers.end())
			RenderManagers[hWnd].reset(new cuirendermanager(hWnd));
		return RenderManagers[hWnd];
	}

	void cuirendermanager::CreateDeviceResources()
	{
		HRESULT hr;
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		windowrect = D2D1::RectF(static_cast<float>(rect.left), static_cast<float>(rect.top), static_cast<float>(rect.right), static_cast<float>(rect.bottom));
		hr = FactoryManager::getpD2DFactory()->CreateHwndRenderTarget(
			RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED)),
			HwndRenderTargetProperties(m_hWnd, SizeU(static_cast<UINT>(rect.right - rect.left),
			static_cast<UINT>(rect.bottom - rect.top))),
			&pRT
			);
		if (FAILED(hr)) std::abort();
		HDC screen = GetDC(0);
		DPIX = GetDeviceCaps(screen, LOGPIXELSX) / 96.0f;
		DPIY = GetDeviceCaps(screen, LOGPIXELSY) / 96.0f;
		ReleaseDC(0, screen);

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
		HRESULT hr = FactoryManager::getpDwFactory()->CreateTextFormat(
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

	cuiwindowbase::cuiwindowbase(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle, cdword dwExStyle , cint _width, cint _height, cstyle _style)
		:
		hInst(_hInst),
		title(_title),
		windowstyle(_windowstyle),
		width(_width),
		height(_height),
		realstyle(_style),
		windowmessage(),
		subject(),
		extendstyle(dwExStyle)
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

	cuistdwindow::cuistdwindow(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle, cdword dwExStyle, cint _width, cint _height, cstyle _style)
		:cuiwindowbase(_hInst, _title, _windowstyle, dwExStyle, _width, _height, _style)
		, iftrack(true)
	{
		initevents();
		init();
		cuiTooltip::setpOwner(this);
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
			return notyet;
		}
		);
		addevents(WM_MOUSEMOVE, [this](WINPAR)->bool{
			if (iftrack)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE | TME_HOVER;
				tme.hwndTrack = hWnd;
				tme.dwHoverTime = Tooltiptime;
				::_TrackMouseEvent(&tme);
				iftrack = false;
			}
			return notyet;
		});
		addevents(WM_MOUSELEAVE, [this](WINPAR)->bool{
			iftrack = true;
			return notyet;
		});
		addhelper(WM_MOUSEHOVER, mousehover, true);
		addhelper(WM_MOUSELEAVE, mousemoveout, true);
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
		//addhelper(WM_PAINT, controlinit,false);
		
		
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

	glowwindow::glowwindow(cuiwindowbase* _pOwner, cint size)
		:pOwner(_pOwner), m_size(size), ifupdate(false), status(shadowenabled), m_WndSize(0)
	{
		init();
		initevents();
	}

	void glowwindow::init()
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DefWindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = pOwner->hInst;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = 0;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = L"cuiglowwindow";
		RegisterClassEx(&wcex);
		thishwnd = CreateWindowEx(
			WS_EX_LAYERED | WS_EX_TRANSPARENT,
			L"cuiglowwindow",
			NULL,
			WS_POPUPWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT, 
			pOwner->m_hWnd,
			0,
			pOwner->hInst,
			NULL
			);
		if (!thishwnd) std::abort();
	}

	void glowwindow::initevents()
	{
		pOwner->addevents(WM_MOVE,
			[this](WINPAR)->bool{
			if (status & shadowvisible)
			{
				RECT x;
				GetWindowRect(pOwner->m_hWnd, &x);
				::SetWindowPos(this->thishwnd, 0, x.left + glowoffset - m_size
					, x.top + glowoffset - m_size, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
			}
			return notyet;
			
		});


		pOwner->addevents(WM_SIZE, [this](WINPAR)->bool
		{
			if ( status & shadowenabled)
			{
				if (SIZE_MAXIMIZED == wParam || SIZE_MINIMIZED == wParam)
				{
					ShowWindow(thishwnd, SW_HIDE);
					status &= ~shadowvisible;
				}
				else
				{
					LONG_PTR Parentstyle = ::GetWindowLongPtr(pOwner->m_hWnd, GWL_STYLE);
					if (Parentstyle & WS_VISIBLE)
					{
						status |= parentvisible;
						if (!(status & shadowvisible))
						{
							status |= shadowvisible;
							this->update();
							ShowWindow(thishwnd, SW_SHOWNA);
							this->ifupdate = true;
						}
						else if (LOWORD(lParam) > LOWORD(this->m_WndSize) || HIWORD(lParam) > HIWORD(this->m_WndSize))
							this->ifupdate = true;
						else
						{
							this->update();
						}
					}
				}
			}
			this->m_WndSize = lParam;
			return notyet;
		});

		pOwner->addevents(WM_PAINT, [this](WINPAR)->bool{
			if (this->ifupdate)
			{
				this->update();
				this->ifupdate = false;
			}
			return notyet;
		});
		//std::swap(pOwner->windowmessage[WM_PAINT][0], pOwner->windowmessage[WM_PAINT][1]);
		//std::swap(pOwner->windowmessage[WM_SIZE][0], pOwner->windowmessage[WM_SIZE][1]);
		//pOwner->windowmessage[WM_PAINT].insert(pOwner->windowmessage[WM_PAINT].begin(), pOwner->windowmessage[WM_PAINT].back());
		//pOwner->windowmessage[WM_PAINT].pop_back();
		pOwner->addevents(WM_EXITSIZEMOVE, [this](WINPAR)->bool
		{
			if (this->status & shadowvisible)
				this->update();
			return notyet;
		});
		pOwner->addevents(WM_SHOWWINDOW, [this](WINPAR)->bool
		{
			if (this->status & shadowenabled)
			{
				auto x = ::DefWindowProc(hWnd, Message, wParam, lParam);
				if (!wParam)
				{
					::ShowWindow(this->thishwnd, SW_HIDE);
					this->status &= ~(shadowvisible | parentvisible);
				}
				else
				{
					this->ifupdate = true;
					this->show();
				}
				return already;
			}
			return notyet;
		});

		pOwner->addevents(WM_DESTROY, [this](WINPAR)->bool{
			DestroyWindow(this->thishwnd);
			return notyet;
		});

		pOwner->addevents(WM_PAINT, [this](WINPAR)->bool
		{
			this->show();
			LONG_PTR Parentstyle = ::GetWindowLongPtr(pOwner->m_hWnd, GWL_STYLE);
			if (Parentstyle & WS_VISIBLE)
			{
				status |= parentvisible;
				if (!(status & shadowvisible))
				{
					status |= shadowvisible;
				}
			}
			return notyet;
		});
	}

	void glowwindow::update()
	{
		RECT WndRect;
		GetWindowRect(pOwner->m_hWnd, &WndRect);
		int nShadWndWid = WndRect.right - WndRect.left + m_size * 2;
		int nShadWndHei = WndRect.bottom - WndRect.top + m_size * 2;

		// Create the alpha blending bitmap
		BITMAPINFO bmi;        // bitmap header

		ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = nShadWndWid;
		bmi.bmiHeader.biHeight = nShadWndHei;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = nShadWndWid * nShadWndHei * 4;

		BYTE *pvBits;          // pointer to DIB section
		HBITMAP hbitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void **)&pvBits, NULL, 0);

		ZeroMemory(pvBits, bmi.bmiHeader.biSizeImage);
		MakeShadow((UINT32 *)pvBits, pOwner->m_hWnd, &WndRect);

		HDC hMemDC = CreateCompatibleDC(NULL);
		HBITMAP hOriBmp = (HBITMAP)SelectObject(hMemDC, hbitmap);

		POINT ptDst = { WndRect.left + glowoffset - m_size, WndRect.top + glowoffset - m_size };
		POINT ptSrc = { 0, 0 };
		SIZE WndSize = { nShadWndWid, nShadWndHei };
		BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

		MoveWindow(thishwnd, ptDst.x, ptDst.y, nShadWndWid, nShadWndHei, FALSE);

		BOOL bRet = UpdateLayeredWindow(thishwnd, NULL, &ptDst, &WndSize, hMemDC,
			&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);

		_ASSERT(bRet); // something was wrong....

		// Delete used resources
		SelectObject(hMemDC, hOriBmp);
		DeleteObject(hbitmap);
		DeleteDC(hMemDC);
	}
	void glowwindow::MakeShadow(UINT32 *pShadBits, HWND hParent, RECT *rcParent)
	{
		// The shadow algorithm:
		// Get the region of parent window,
		// Apply morphologic erosion to shrink it into the size (ShadowWndSize - Sharpness)
		// Apply modified (with blur effect) morphologic dilation to make the blurred border
		// The algorithm is optimized by assuming parent window is just "one piece" and without "wholes" on it

		// Get the region of parent window,
		// Create a full rectangle region in case of the window region is not defined
		HRGN hParentRgn = CreateRectRgn(0, 0, rcParent->right - rcParent->left, rcParent->bottom - rcParent->top);
		//GetWindowRgn(hParent, hParentRgn);

		// Determine the Start and end point of each horizontal scan line
		SIZE szParent = { rcParent->right - rcParent->left, rcParent->bottom - rcParent->top };
		SIZE szShadow = { szParent.cx + 2 * m_size, szParent.cy + 2 * m_size };
		// Extra 2 lines (set to be empty) in ptAnchors are used in dilation
		int nAnchors = max(szParent.cy, szShadow.cy);	// # of anchor points pares
		int(*ptAnchors)[2] = new int[nAnchors + 2][2];
		int(*ptAnchorsOri)[2] = new int[szParent.cy][2];	// anchor points, will not modify during erosion
		ptAnchors[0][0] = szParent.cx;
		ptAnchors[0][1] = 0;
		ptAnchors[nAnchors + 1][0] = szParent.cx;
		ptAnchors[nAnchors + 1][1] = 0;
		if (m_size > 0)
		{
			// Put the parent window anchors at the center
			for (int i = 0; i < m_size; i++)
			{
				ptAnchors[i + 1][0] = szParent.cx;
				ptAnchors[i + 1][1] = 0;
				ptAnchors[szShadow.cy - i][0] = szParent.cx;
				ptAnchors[szShadow.cy - i][1] = 0;
			}
			ptAnchors += m_size;
		}
		for (int i = 0; i < szParent.cy; i++)
		{
			// find start point
			int j;
			for (j = 0; j < szParent.cx; j++)
			{
				if (PtInRegion(hParentRgn, j, i))
				{
					ptAnchors[i + 1][0] = j + m_size;
					ptAnchorsOri[i][0] = j;
					break;
				}
			}

			if (j >= szParent.cx)	// Start point not found
			{
				ptAnchors[i + 1][0] = szParent.cx;
				ptAnchorsOri[i][1] = 0;
				ptAnchors[i + 1][0] = szParent.cx;
				ptAnchorsOri[i][1] = 0;
			}
			else
			{
				// find end point
				for (j = szParent.cx - 1; j >= ptAnchors[i + 1][0]; j--)
				{
					if (PtInRegion(hParentRgn, j, i))
					{
						ptAnchors[i + 1][1] = j + 1 + m_size;
						ptAnchorsOri[i][1] = j + 1;
						break;
					}
				}
			}
			// 		if(0 != ptAnchorsOri[i][1])
			// 			_RPTF2(_CRT_WARN, "%d %d\n", ptAnchorsOri[i][0], ptAnchorsOri[i][1]);
		}

		if (m_size > 0)
			ptAnchors -= m_size;	// Restore pos of ptAnchors for erosion
		int(*ptAnchorsTmp)[2] = new int[nAnchors + 2][2];	// Store the result of erosion
		// First and last line should be empty
		ptAnchorsTmp[0][0] = szParent.cx;
		ptAnchorsTmp[0][1] = 0;
		ptAnchorsTmp[nAnchors + 1][0] = szParent.cx;
		ptAnchorsTmp[nAnchors + 1][1] = 0;
		int nEroTimes = 0;
		// morphologic erosion
		for (int i = 0; i < shadowsharpness - m_size; i++)
		{
			nEroTimes++;
			//ptAnchorsTmp[1][0] = szParent.cx;
			//ptAnchorsTmp[1][1] = 0;
			//ptAnchorsTmp[szParent.cy + 1][0] = szParent.cx;
			//ptAnchorsTmp[szParent.cy + 1][1] = 0;
			for (int j = 1; j < nAnchors + 1; j++)
			{
				ptAnchorsTmp[j][0] = max(ptAnchors[j - 1][0], max(ptAnchors[j][0], ptAnchors[j + 1][0])) + 1;
				ptAnchorsTmp[j][1] = min(ptAnchors[j - 1][1], min(ptAnchors[j][1], ptAnchors[j + 1][1])) - 1;
			}
			// Exchange ptAnchors and ptAnchorsTmp;
			int(*ptAnchorsXange)[2] = ptAnchorsTmp;
			ptAnchorsTmp = ptAnchors;
			ptAnchors = ptAnchorsXange;
		}

		// morphologic dilation
		ptAnchors += (m_size < 0 ? -m_size : 0) + 1;	// now coordinates in ptAnchors are same as in shadow window
		// Generate the kernel
		int nKernelSize = m_size > shadowsharpness ? m_size : shadowsharpness;
		int nCenterSize = m_size > shadowsharpness ? (m_size - shadowsharpness) : 0;
		UINT32 *pKernel = new UINT32[(2 * nKernelSize + 1) * (2 * nKernelSize + 1)];
		UINT32 *pKernelIter = pKernel;
		for (int i = 0; i <= 2 * nKernelSize; i++)
		{
			for (int j = 0; j <= 2 * nKernelSize; j++)
			{
				double dLength = sqrt((i - nKernelSize) * (i - nKernelSize) + (j - nKernelSize) * (double)(j - nKernelSize));
				if (dLength < nCenterSize)
					*pKernelIter = shadowdarkness << 24 | PreMultiply(ColorF::Black, shadowdarkness);
				else if (dLength <= nKernelSize)
				{
					UINT32 nFactor = ((UINT32)((1 - (dLength - nCenterSize) / (shadowsharpness + 1)) * shadowdarkness));
					*pKernelIter = nFactor << 24 | PreMultiply(ColorF::Black, nFactor);
				}
				else
					*pKernelIter = 0;
				//TRACE("%d ", *pKernelIter >> 24);
				pKernelIter++;
			}
			//TRACE("\n");
		}
		// Generate blurred border
		for (int i = nKernelSize; i < szShadow.cy - nKernelSize; i++)
		{
			int j;
			if (ptAnchors[i][0] < ptAnchors[i][1])
			{

				// Start of line
				for (j = ptAnchors[i][0];
					j < min(max(ptAnchors[i - 1][0], ptAnchors[i + 1][0]) + 1, ptAnchors[i][1]);
					j++)
				{
					for (int k = 0; k <= 2 * nKernelSize; k++)
					{
						UINT32 *pPixel = pShadBits +
							(szShadow.cy - i - 1 + nKernelSize - k) * szShadow.cx + j - nKernelSize;
						UINT32 *pKernelPixel = pKernel + k * (2 * nKernelSize + 1);
						for (int l = 0; l <= 2 * nKernelSize; l++)
						{
							if (*pPixel < *pKernelPixel)
								*pPixel = *pKernelPixel;
							pPixel++;
							pKernelPixel++;
						}
					}
				}	// for() start of line

				// End of line
				for (j = max(j, min(ptAnchors[i - 1][1], ptAnchors[i + 1][1]) - 1);
					j < ptAnchors[i][1];
					j++)
				{
					for (int k = 0; k <= 2 * nKernelSize; k++)
					{
						UINT32 *pPixel = pShadBits +
							(szShadow.cy - i - 1 + nKernelSize - k) * szShadow.cx + j - nKernelSize;
						UINT32 *pKernelPixel = pKernel + k * (2 * nKernelSize + 1);
						for (int l = 0; l <= 2 * nKernelSize; l++)
						{
							if (*pPixel < *pKernelPixel)
								*pPixel = *pKernelPixel;
							pPixel++;
							pKernelPixel++;
						}
					}
				}	// for() end of line

			}
		}	// for() Generate blurred border

		// Erase unwanted parts and complement missing
		UINT32 clCenter = shadowdarkness << 24 | PreMultiply(ColorF::Black, shadowdarkness);
		for (int i = min(nKernelSize, max(m_size - glowoffset, 0));
			i < max(szShadow.cy - nKernelSize, min(szParent.cy + m_size - glowoffset, szParent.cy + 2 * m_size));
			i++)
		{
			UINT32 *pLine = pShadBits + (szShadow.cy - i - 1) * szShadow.cx;
			if (i - m_size + glowoffset < 0 || i - m_size + glowoffset >= szParent.cy)	// Line is not covered by parent window
			{
				for (int j = ptAnchors[i][0]; j < ptAnchors[i][1]; j++)
				{
					*(pLine + j) = clCenter;
				}
			}
			else
			{
				for (int j = ptAnchors[i][0];
					j < min(ptAnchorsOri[i - m_size + glowoffset][0] + m_size - glowoffset, ptAnchors[i][1]);
					j++)
					*(pLine + j) = clCenter;
				for (int j = max(ptAnchorsOri[i - m_size + glowoffset][0] + m_size - glowoffset, 0);
					j < min(ptAnchorsOri[i - m_size + glowoffset][1] + m_size - glowoffset, szShadow.cx);
					j++)
					*(pLine + j) = 0;
				for (int j = max(ptAnchorsOri[i - m_size + glowoffset][1] + m_size - glowoffset, ptAnchors[i][0]);
					j < ptAnchors[i][1];
					j++)
					*(pLine + j) = clCenter;
			}
		}

		// Delete used resources
		delete[](ptAnchors - (m_size < 0 ? -m_size : 0) - 1);
		delete[] ptAnchorsTmp;
		delete[] ptAnchorsOri;
		delete[] pKernel;
		DeleteObject(hParentRgn);
	}

	void glowwindow::show()
	{
		this->status &= shadowenabled;
		if (status & shadowenabled)
		{
			LONG_PTR ParentStyle = ::GetWindowLongPtr(pOwner->m_hWnd, GWL_STYLE);
			if (WS_VISIBLE & ParentStyle)
			{
				status |= shadowvisible;
				if (!((WS_MAXIMIZE | WS_MINIMIZE) & ParentStyle))	// Parent visible but does not need shadow
					status |= shadowvisible;
			}
		}
		if (status & shadowvisible)
		{
			ShowWindow(thishwnd, SW_SHOWNA);
			update();
		}
		else
		{
			ShowWindow(thishwnd, SW_SHOWNA);
		}
	}

	cuiTooltip::cuiTooltip() : m_hWnd()
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = cuiTooltip::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = cuiTooltip::pOwner->hInst;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = 0;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = L"cuitooltip";
		RegisterClassEx(&wcex);
		m_hWnd = CreateWindowEx(
			0,
			L"cuitooltip",
			NULL,
			WS_POPUPWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			cuiTooltip::pOwner->m_hWnd,
			0,
			cuiTooltip::pOwner->hInst,
			NULL
			);
		if (!m_hWnd) std::abort();
		ShowWindow(m_hWnd, SW_HIDE);
		UpdateWindow(m_hWnd);
	}


	void cuiTooltip::setpOwner(cuiwindowbase* p)
	{
		cuiTooltip::pOwner = p;
	}

	const unique_ptr<cuiTooltip>& cuiTooltip::getTooltip()
	{
		if (!pTooltip)
		{
			cuiTooltip::pTooltip.reset(new cuiTooltip);
		}
		return cuiTooltip::pTooltip;
	}

	void cuiTooltip::show(CPointf point, wstring text)
	{
		RECT rect;
		GetWindowRect(cuiTooltip::pOwner->m_hWnd, &rect);
		SetWindowPos(m_hWnd, 0, point.x + rect.left, point.y + rect.top, 70, 30, SWP_NOACTIVATE);
		
		ShowWindow(m_hWnd, SW_SHOWNA);
	}

	void cuiTooltip::hide()
	{
		ShowWindow(m_hWnd, SW_HIDE);
	}

	LRESULT CALLBACK cuiTooltip::WndProc(WINPAR)
	{
		switch (Message)
		{
		case WM_MOUSEACTIVATE:
			//::ShowWindow(hWnd, SW_HIDE);
			return WA_INACTIVE;
		case WM_SETFOCUS:
			return 0;
		case WM_PAINT:
		{
			auto x = cuirendermanager::getManager(hWnd);
			x->begindraw();
			x->clearall(defaultbackgroundcolor);
			x->drawtext(L"×îÐ¡»¯", 12, RectF(0, 0, 70, 30));
			x->enddraw();
		}
		default:
			break;
		}
		return ::DefWindowProc(hWnd, Message, wParam, lParam);
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
		addevents(mousehover, [](cuievent* pe)->void
		{
			cuiTooltip::getTooltip()->show(pe->eventposition, L"haha");
		});
		addevents(mousemoveout, [](cuievent* pe)->void{
			cuiTooltip::getTooltip()->hide();
		});
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
				vit(pceb);
		}
	}


	

}

