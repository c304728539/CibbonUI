#include "CUIWindows.h"

using namespace D2D1;
namespace cibbonui
{

	inline HINSTANCE getInstance()
	{
		return GetModuleHandle(nullptr);
	}


	void Subject::RegisterObserver(Observer* o)
	{
		auto it = std::find(Observerlist.cbegin(),
			Observerlist.cend(),
			o);
		if (it != Observerlist.end()) Observerlist.push_back(o);
	}

	void Subject::RemoveObserver(Observer* o)
	{
		auto it = std::find(Observerlist.cbegin(),
			Observerlist.cend(),
			o);
		if (it != Observerlist.end()) Observerlist.erase(it);
	}


	WndClass::WndClass(const std::wstring& _name,
		bool shadow,
		WNDPROC WndProc,
		HINSTANCE hInst)
		:ClassName(_name)
	{
		WndClassEx.cbSize = sizeof(WndClassEx);
		WndClassEx.style = CS_HREDRAW | CS_VREDRAW | (shadow ? CS_DROPSHADOW : 0);
		WndClassEx.hCursor = NULL;
		WndClassEx.hIcon = NULL;
		WndClassEx.hbrBackground = NULL;
		WndClassEx.hInstance = hInst;
		WndClassEx.lpfnWndProc = WndProc;
		WndClassEx.lpszClassName = ClassName.c_str();
		WndClassEx.cbWndExtra = sizeof(LONG_PTR);
		WndClassEx.lpszMenuName = NULL;
		WndClassEx.cbClsExtra = NULL;
		WndClassEx.hIconSm = 0;
		RegisterWindow();
	}

	WndClass::WndClass() :atom()
	{

	}

	std::wstring WndClass::getClassName() const
	{
		return ClassName;
	}

	void WndClass::RegisterWindow()
	{
		atom = RegisterClassEx(&WndClassEx);
	}

	WindowCreator::WindowCreator(const std::wstring& ClassName, const std::wstring& Title)
		:WindowTitle(Title),
		Width(CW_USEDEFAULT),
		Height(CW_USEDEFAULT),
		lpParam(nullptr),
		ExStyle(0),
		hParentWindow(0),
		WindowClassName(ClassName),
		WindowStyle(WS_OVERLAPPEDWINDOW)
	{

	}

	void WindowCreator::SetExStyle(DWORD _ExStyle)
	{
		ExStyle = _ExStyle;
	}

	void WindowCreator::SetWidth(cint _width)
	{
		Width = _width;
	}

	void WindowCreator::SetHeight(cint _Height)
	{
		Height = _Height;
	}

	void WindowCreator::SetParentWindow(HWND hParent)
	{
		hParentWindow = hParent;
	}

	void WindowCreator::SetlpParam(LPVOID p)
	{
		lpParam = p;
	}

	HWND WindowCreator::Create()
	{
		return CreateWindowEx(
			ExStyle,
			WindowClassName.c_str(),
			WindowTitle.c_str(),
			WindowStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			Width,
			Height,
			hParentWindow,
			NULL,
			getInstance(),
			lpParam);
	}

	MessageHandler::MessageHandler(CUIWindowBase* pWindow)
		:pOwnerWindow(pWindow)
	{

	}

	MainWindowMessageHandler::MainWindowMessageHandler(CUIWindowBase* p)
		: MessageHandler(p)
	{

	}
 
	bool MainWindowMessageHandler::MessageHandlerWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre)
	{
		static bool HasFirstSized = false;
		if (Message == WM_NCHITTEST)
		{
			if (::DwmDefWindowProc(hWnd, Message, wParam, lParam, &lre))
			{
				return Already;
			}
			else
			{
				lre = HitTestNCA(hWnd, wParam, lParam);
			}
			return Already;
		}


		if ((Message == WM_NCCALCSIZE) && (wParam == TRUE))
		{
			NCCALCSIZE_PARAMS *pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
			pncsp->rgrc[0].left = pncsp->rgrc[0].left + 0;        
			pncsp->rgrc[0].top = pncsp->rgrc[0].top + 0;         
			pncsp->rgrc[0].right = pncsp->rgrc[0].right - 0;         
			pncsp->rgrc[0].bottom = pncsp->rgrc[0].bottom - 0;
			lre = 0;
			return Already;// No need to pass 
		}
		switch (Message)
		{
		case WM_CLOSE:
			::PostQuitMessage(0);
			break;
		case WM_SIZE:
		{
			if (!HasFirstSized)
			{
				RECT rect;
				GetWindowRect(hWnd, &rect);
				::SetWindowPos(hWnd, NULL, rect.left, rect.top,
					rect.right - rect.left, rect.bottom - rect.top, SWP_FRAMECHANGED);
				HasFirstSized = true;
			}
			
			break;
		}
		default:
			break;
		}
		return NotYet;
	}

	LRESULT MainWindowMessageHandler::HitTestNCA(HWND , WPARAM , LPARAM lParam)
	{
		cuiPoint PtMouse(static_cast<float>(GET_X_LPARAM(lParam)), static_cast<float>(GET_Y_LPARAM(lParam)));
		cuiRect WindowRect = pOwnerWindow->GetWindowRect();
		RECT rcFrame = { 0 };
		AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);
		USHORT uRow = 1;
		USHORT uCol = 1;
		bool fOnResizeBorder = false;

		if (PtMouse.y >= WindowRect.top && PtMouse.y < WindowRect.top - rcFrame.top )
		{
			fOnResizeBorder = (PtMouse.y < (WindowRect.top + 4 - rcFrame.top));
			uRow = 0;
		}
		else if (PtMouse.y < WindowRect.bottom  && PtMouse.y >= WindowRect.bottom - rcFrame.bottom)    
		{ 
			uRow = 2; 
		}
		// Determine if the point is at the left or right of the window.     
		if (PtMouse.x >= WindowRect.left && PtMouse.x < WindowRect.left - rcFrame.left ) 
		{         
			uCol = 0; // left side     
		}     
		else if (PtMouse.x < WindowRect.right && PtMouse.x >= WindowRect.right - rcFrame.right)     
		{         
			uCol = 2; // right side     
		}  
		// Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)     
		LRESULT hitTests[3][3] =      
		{         
			{ HTTOPLEFT, fOnResizeBorder ? HTTOP : HTCAPTION, HTTOPRIGHT },
			{ HTLEFT, HTNOWHERE, HTRIGHT },         
			{ HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },     
		};  
		return hitTests[uRow][uCol]; 
	}
	
	CUIWindowBase::CUIWindowBase(const std::wstring& Title)
		:WindowTitle(Title),
		pWndClass(nullptr),
		pWindowCreator(nullptr),
		WindowHandle(0),
		pHandler(nullptr)
	{

	}

	CUIWindowBase::~CUIWindowBase()
	{
		delete pWndClass;
		delete pWindowCreator;
		delete pHandler;
	}

	void CUIWindowBase::Initialize()
	{
		//Set things Here
		SetThings();
		WindowHandle = pWindowCreator->Create();
		if (!WindowHandle) throw  std::exception("Sorry");
	}

	HWND CUIWindowBase::GetWindowHandle() const
	{
		return WindowHandle;
	}

	CUIMainWindow::CUIMainWindow(const std::wstring& Title)
		:CUIWindowBase(Title)
	{

	}


	void CUIMainWindow::CreateInitors()
	{
		pWndClass = new WndClass(L"CUIMainWindow", true, WndProc, getInstance());
		pWindowCreator = new WindowCreator(pWndClass->getClassName(), WindowTitle);
		pHandler = new MainWindowMessageHandler(this);
	}

	void CUIMainWindow::Run()
	{
		CreateInitors();
		Initialize();
		ShowWindow(WindowHandle, SW_SHOWNORMAL);
		UpdateWindow(WindowHandle);
		RunMessageLoop();
	}

	void CUIMainWindow::SetThings()
	{
		pWindowCreator->SetlpParam(this);
	}

	void CUIMainWindow::RunMessageLoop()
	{
		MSG msg{ 0 };
		while (::GetMessage(&msg, NULL, 0, 0))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	bool CUIWindowBase::HandleMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre)
	{
		return pHandler->MessageHandlerWndProc(hWnd, Message, wParam, lParam, lre);
	}

	cuiRect CUIWindowBase::GetWindowRect() const
	{
		RECT rc;
		::GetWindowRect(WindowHandle,&rc);
		return cuiRect(rc);
	}

	cuiRect CUIWindowBase::GetClientRect() const
	{
		RECT rc;
		::GetClientRect(WindowHandle, &rc);
		return cuiRect(rc);
	}


	LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		CUIMainWindow* pMainWindow = nullptr;
		if (Message == WM_NCCREATE)
		{
			LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			pMainWindow = reinterpret_cast<CUIMainWindow*>(pcs->lpCreateParams);
			::SetWindowLongPtr(hWnd,
				GWLP_USERDATA,
				PtrToLong(pMainWindow));
		}
		else
		{
			pMainWindow = reinterpret_cast<CUIMainWindow*>(
				static_cast<LONG_PTR>(::GetWindowLongPtr(
				hWnd,
				GWLP_USERDATA)));
		}
		if (pMainWindow)
		{
			LRESULT lre = 0;
			if (pMainWindow->HandleMessage(hWnd, Message, wParam, lParam, lre))
			{
				return lre;
			}
		}
		return ::DefWindowProc(hWnd, Message, wParam, lParam);
		
	}

	/*Render APIs*/

	std::unique_ptr<FactoriesCreator> FactoriesCreator::pFactoriesCreator(nullptr);

	FactoriesCreator::FactoriesCreator()
		:pD2DFactory(nullptr),
		pDwFactory(nullptr),
		pNormalTextFormat(nullptr)
	{

	}

	FactoriesCreator::~FactoriesCreator()
	{
		Free(pD2DFactory);
		Free(pDwFactory);
		Free(pNormalTextFormat);
	}

	const std::unique_ptr<FactoriesCreator>& FactoriesCreator::GetFactoriesCreator()
	{
		if (!pFactoriesCreator) pFactoriesCreator.reset(new FactoriesCreator());
		return pFactoriesCreator;
	}

	ID2D1Factory* FactoriesCreator::GetpD2DFactory() const
	{
		if (!pD2DFactory) 
			HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
		return pD2DFactory;
	}

	IDWriteFactory* FactoriesCreator::GetpDwFactory() const
	{
		if (!pDwFactory) 
			HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(pDwFactory),
			reinterpret_cast<IUnknown **>(&pDwFactory));
		return pDwFactory;
	}

	IDWriteTextFormat* FactoriesCreator::GetpNormalTextFormat()
	{
		if (!pNormalTextFormat)
			pDwFactory->CreateTextFormat(
			L"Microsoft YaHei",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			NormalFontSize,
			L"",
			&pNormalTextFormat);
		return pNormalTextFormat;
	}

	std::map<CUIWindowBase*, std::unique_ptr<RenderManager>> RenderManager::RenderManagerMap;

	RenderManager::RenderManager(CUIWindowBase* p)
		:pD2DFactory(FactoriesCreator::GetFactoriesCreator()->GetpD2DFactory()),
		pDwFactory(FactoriesCreator::GetFactoriesCreator()->GetpDwFactory()),
		pNormalTextFormat(FactoriesCreator::GetFactoriesCreator()->GetpNormalTextFormat()),
		pRT(nullptr)
	{
		pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED)),
			HwndRenderTargetProperties(p->GetWindowHandle(), SizeU(static_cast<UINT>(p->GetClientRect().GetWidth()),
			static_cast<UINT>(p->GetClientRect().GetHeight()))),
			&pRT);
		pRT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		pRT->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
	}

	const std::unique_ptr<RenderManager>& RenderManager::GetpRenderManager(CUIWindowBase* p)
	{
		auto it = RenderManager::RenderManagerMap.find(p);
		if (it == RenderManager::RenderManagerMap.end())
		{
			RenderManager::RenderManagerMap[p] = std::unique_ptr<RenderManager>(new RenderManager(p));
		}
		return RenderManager::RenderManagerMap[p];
	}

	RenderManager::~RenderManager()
	{
		Free(pRT);
		for (auto& p : SolidBrushMap)
		{
			Free(p.second);
		}
		for (auto&p : LinearBrushMap)
		{
			Free(p.second);
		}
	}

	void RenderManager::DrawBorder(const cuiRect& rect, cint BorderColor, float LineWidth)
	{
		pRT->DrawRectangle(rect, GetBrush(BorderColor), LineWidth);
	}

	void RenderManager::NormalFillRect(const cuiRect& rect, cint FillColor)
	{
		pRT->FillRectangle(rect, GetBrush(FillColor));
	}

	void RenderManager::LinearFillRect(const cuiRect& rect, cint FillColor1, cint FillColor2)
	{
		pRT->FillRectangle(rect, GetBrush(FillColor1, FillColor2,rect));
	}

	ID2D1SolidColorBrush* RenderManager::GetBrush(cint BrushColor)
	{
		auto it = RenderManager::SolidBrushMap.find(BrushColor);
		if (it == RenderManager::SolidBrushMap.end())
		{
			pRT->CreateSolidColorBrush(
				ColorF(BrushColor), &RenderManager::SolidBrushMap[BrushColor]);
		}
		return RenderManager::SolidBrushMap[BrushColor];
	}

	ID2D1LinearGradientBrush* RenderManager::GetBrush(cint BrushColor1, cint BrushColor2, cuiRect _rect)
	{
		auto ColorPair = std::make_pair(BrushColor1, BrushColor2);
		auto it = RenderManager::LinearBrushMap.find(ColorPair);
		if (it == RenderManager::LinearBrushMap.end())
		{
			ID2D1GradientStopCollection* pStop(nullptr);
			D2D1_GRADIENT_STOP Stop[] = {
				{ 0.f, ColorF(BrushColor1) },
				{ 1.f, ColorF(BrushColor1) }
			};
			pRT->CreateGradientStopCollection(
				Stop,
				ARRAYSIZE(Stop),
				D2D1_GAMMA_2_2,
				D2D1_EXTEND_MODE_CLAMP,
				&pStop);
			pRT->CreateLinearGradientBrush(LinearGradientBrushProperties(
				Point2F(_rect.left + _rect.GetWidth() / 2, _rect.top),
				Point2F(_rect.left + _rect.GetHeight() / 2, _rect.bottom)),
				pStop, &RenderManager::LinearBrushMap[ColorPair]);
			pStop->Release();
		}
		return RenderManager::LinearBrushMap[ColorPair];
	}

	void RenderManager::RenderText(const std::wstring& Content, const cuiRect& Area,cint TextColor)
	{
		pRT->DrawTextW(
			Content.c_str(),
			Content.size(),
			pNormalTextFormat,
			Area,
			GetBrush(TextColor),
			D2D1_DRAW_TEXT_OPTIONS_CLIP
			);
	}

}