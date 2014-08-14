#include "CUIWindows.h"

using namespace D2D1;
namespace cibbonui
{
	std::shared_ptr<Controller> Controller::pController = nullptr;

	Controller::Controller()
		:Subject()
	{

	}

	std::shared_ptr<Controller> Controller::GetpController()
	{
		if (!Controller::pController)
			pController.reset(new Controller());
		return pController;
	}

	inline HINSTANCE getInstance()
	{
		return GetModuleHandle(nullptr);
	}


	void Subject::RegisterObserver(Observer* o)
	{
		auto it = std::find(Observerlist.cbegin(),
			Observerlist.cend(),
			o);
		if (it == Observerlist.end()) Observerlist.push_back(o);
	}

	void Subject::RemoveObserver(Observer* o)
	{
		auto it = std::find(Observerlist.cbegin(),
			Observerlist.cend(),
			o);
		if (it != Observerlist.end()) Observerlist.erase(it);
	}

	Context::Context(CUIWindowBase* p)
		:pWindow(p)
	{

	}

	void Leaf::Add(Component* p)
	{
		return;
	}

	void Leaf::Remove(Component* p)
	{
		return;
	}

	const Component* Leaf::GetComposite() const
	{
		return nullptr;
	}

	//cuiRect Component::GetEventPosition() const
	//{
	//	return cuiRect();
	//}

	/*void Component::SetRoot(Composite* p)
	{
		pRoot = p;
	}*/

	void Composite::Add(Component* p)
	{
		auto it = std::find(Leaves.cbegin(),
			Leaves.cend(),
			p);
		if (it == Leaves.end()) Leaves.push_back(p);
	}


	void Composite::Remove(Component* p)
	{
		auto it = std::find(Leaves.cbegin(),
			Leaves.cend(),
			p);
		if (it != Leaves.end()) Leaves.erase(it);
	}
	const Component* Composite::GetComposite() const
	{
		return this;
	}
	void Canvas::Draw(Context* p)
	{
		for (auto pl : Leaves)
		{
			p->Position = pl->GetPosition();
			pl->Draw(p);
		}
	}

	void Canvas::Add(Component* p, const cuiRect& rc)
	{
		//auto temp = rc;
		p->SetPosition(GetMoveDown(rc,CUIMainWindow::CaptionHeight));
		this->Composite::Add(p);
	}
	void Canvas::SetPosition(const cuiRect& cr)
	{
		Position = cr;
	}

	void Canvas::ReSize(const cuiRect& cr)
	{
		/*for (auto pl : Leaves)
		{
			pl->SetPosition(pl->GetPosition)
		}*/
	}

	void Canvas::SetRoot(CUIWindowBase* p)
	{
		for (auto pl : Leaves)
		{
			pl->SetRoot(p);
		}
	}

	WndClass::WndClass(const std::wstring& _name,
		bool shadow,
		WNDPROC WndProc,
		HINSTANCE hInst)
		:ClassName(_name)
	{
		WndClassEx.cbSize = sizeof(WndClassEx);
		WndClassEx.style = CS_HREDRAW | CS_VREDRAW | (shadow ? CS_DROPSHADOW : 0);
		WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
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
		Width(400),
		Height(300),
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
			GetSystemMetrics(SM_CXSCREEN) / 2,
			GetSystemMetrics(SM_CYSCREEN) / 2,
			Width,
			Height,
			hParentWindow,
			NULL,
			getInstance(),
			lpParam);
	}

	MessageHandler::MessageHandler(CUIWindowBase* pWindow)
		:pOwnerWindow(pWindow),
		pContext(new Context(pWindow)),
		pController(Controller::GetpController())
	{

	}

	MessageHandler::~MessageHandler()
	{
		delete pContext;
		pContext = nullptr;
	}
	MainWindowMessageHandler::MainWindowMessageHandler(CUIWindowBase* p)
		: MessageHandler(p),
		pContainer(nullptr),
		Tracked(true)
	{

	}
    
	void MainWindowMessageHandler::SetContainer(Container* p)
	{
		pContainer = p;
	}
	
	bool MainWindowMessageHandler::MessageHandlerWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre)
	{
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

		case WM_CREATE:
		{
			/*hRenderThread = (HANDLE)_beginthreadex(
				0,
				0,
				RenderThreadFunc,
				pContext,
				0,
				0);*/
		}
		case WM_NCHITTEST:
			if ((lre = HitTestNCA(hWnd, wParam, lParam)) != HTNOWHERE)
				return Already;
			else break;
		case WM_CLOSE:
			::PostQuitMessage(0);
			break;
		case WM_SIZE:
		{
			
			pOwnerWindow->ReSize(LOWORD(lParam), HIWORD(lParam));
			break;
		}
		case WM_PAINT:
		{
			RECT rect;
			GetWindowRect(hWnd, &rect);
			pOwnerWindow->GetpRenderManager()->Resize(rect.right - rect.left , rect.bottom - rect.top);
			pContext->Position = pContext->pWindow->GetClientRect();
			pOwnerWindow->Draw(pContext);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			MouseEventArgs mea;
			mea.EventPosition = cuiPoint((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
			//mea.Sender = pContainer;
			for (auto p : pController->Observerlist)
			{
				p->LeftButtonDown(mea);
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			MouseEventArgs mea;
			mea.EventPosition = cuiPoint((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
			//mea.Sender = pContainer;
			for (auto p : pController->Observerlist)
			{
				p->MouseMove(mea);
			}
			if (Tracked)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE /*| TME_HOVER*/;
				tme.hwndTrack = hWnd;
				//tme.dwHoverTime = Tooltiptime;
				::_TrackMouseEvent(&tme);
				Tracked = false;
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			MouseEventArgs mea;
			mea.EventPosition = cuiPoint((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
			//mea.Sender = pContainer;
			for (auto p : pController->Observerlist)
			{
				p->LeftButtonUp(mea);
			}
			break;
		}
		case WM_MOUSELEAVE:
		{
			Tracked = true;
			MouseEventArgs mea;
			mea.EventPosition = cuiPoint(-1, -1);
			//mea.Sender = pContainer;
			for (auto p : pController->Observerlist)
			{
				p->MouseMove(mea);
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

		if (PtMouse.y >= WindowRect.top && PtMouse.y < WindowRect.top - rcFrame.top)
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
		pRenderManager(nullptr)
	{

	}

	CUIWindowBase::~CUIWindowBase()
	{
		delete pWndClass;
		delete pWindowCreator;
	}

	void CUIWindowBase::Initialize()
	{
		//Set things Here
		SetThings();
		WindowHandle = pWindowCreator->Create();
		pRenderManager = RenderManager::GetpRenderManager(this);
		if (!WindowHandle) throw  std::exception("Sorry");
	}

	HWND CUIWindowBase::GetWindowHandle() const
	{
		return WindowHandle;
	}

	void CUIWindowBase::ReDraw() const
	{
		::SendMessage(WindowHandle, WM_PAINT, 0, 0);
	}
	std::shared_ptr<RenderManager> CUIWindowBase::GetpRenderManager() const
	{
		return pRenderManager;
	}

	/*Template Methord*/
	void CUIWindowBase::Run()
	{
		CreateInitors();
		Initialize();
		RunMessageLoop();
	}

	CUIMainWindow::CUIMainWindow(const std::wstring& Title)
		:CUIWindowBase(Title),
		pMainWindowMessageHandler(new MainWindowMessageHandler(this)),
		pCaption(new Caption(this)),
		HasFirstSized(false)
	{
		pCaption->SetRoot(this);
	}


	void CUIMainWindow::CreateInitors()
	{
		pWndClass = new WndClass(L"CUIMainWindow", true, WndProc, getInstance());
		pWindowCreator = new WindowCreator(pWndClass->getClassName(), WindowTitle);
		//pMainWindowMessageHandler = new MainWindowMessageHandler(this);
	}

	void CUIMainWindow::SetThings()
	{
		pWindowCreator->SetlpParam(this);
	}

	void CUIMainWindow::RunMessageLoop()
	{
		//pContainer->RegisterObserver()
		::ShowWindow(WindowHandle, SW_SHOWNORMAL);
		::UpdateWindow(WindowHandle);
		MSG msg{ 0 };
		while (::GetMessage(&msg, NULL, 0, 0))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	void CUIMainWindow::SetContainer(Container* p)
	{
		pMainWindowMessageHandler->SetContainer(p);
	}

	bool CUIMainWindow::HandleMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre)
	{
		return pMainWindowMessageHandler->MessageHandlerWndProc(hWnd, Message, wParam, lParam, lre);
	}

	CUIMainWindow::~CUIMainWindow()
	{
		delete pContainer;
		delete pMainWindowMessageHandler;
		delete pCaption;
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

	void CUIMainWindow::Add(Container* p)
	{
		pContainer = p;
		pMainWindowMessageHandler->SetContainer(p);
		p->SetRoot(this);
	}

	void CUIMainWindow::Draw(Context* p)
	{
		pRenderManager->BeginRender();
		pRenderManager->Clear(0xffffff);
		pCaption->Draw(p);
		pContainer->Draw(p);
		
		pRenderManager->EndRender();
	}

	void CUIMainWindow::ReSize(USHORT x, USHORT y)
	{
		pContainer->ReSize(cuiRect(0, 0, x, y));
		pCaption->ReSize(cuiRect(0, 0, x, y));
		pRenderManager->Resize(x, y);

		if (!HasFirstSized)
		{
			RECT rect;
			::GetWindowRect(WindowHandle, &rect);
			::SetWindowPos(WindowHandle, NULL, rect.left, rect.top,
				rect.right - rect.left, rect.bottom - rect.top, SWP_FRAMECHANGED);
			HasFirstSized = true;
			ReDraw();
		}
		
		
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

	unsigned int __stdcall RenderThreadFunc(void* p)
	{
		MSG msg;
		auto pContext = static_cast<Context*>(p);
		while (::GetMessage(&msg,NULL,0,0))
		{
			switch (msg.message)
			{
			case WM_PAINT:
			{
				pContext->Position = pContext->pWindow->GetClientRect();
				dynamic_cast<CUIMainWindow*>(pContext->pWindow)->Draw(pContext);
			}
			default:
				break;
			}
		}
		
		return 0;
		
	}


	/*Render APIs*/

	std::unique_ptr<FactoriesCreator> FactoriesCreator::pFactoriesCreator(nullptr);

	FactoriesCreator::FactoriesCreator()
		:pD2DFactory(nullptr),
		pDwFactory(nullptr),
		pNormalTextFormat(nullptr)
	{
		if (!pDwFactory)
			HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(pDwFactory),
			reinterpret_cast<IUnknown **>(&pDwFactory));
		if (!pD2DFactory)
			HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
		if (!pNormalTextFormat)
		{
			pDwFactory->CreateTextFormat(
				L"Microsoft YaHei",
				nullptr,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				(FLOAT)NormalFontSize,
				L"",
				&pNormalTextFormat);
			pNormalTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			pNormalTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
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
		return pD2DFactory;
	}

	IDWriteFactory* FactoriesCreator::GetpDwFactory() const
	{
		return pDwFactory;
	}

	IDWriteTextFormat* FactoriesCreator::GetpNormalTextFormat()
	{
		return pNormalTextFormat;
	}

	std::map<CUIWindowBase*, std::shared_ptr<RenderManager>> RenderManager::RenderManagerMap;

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

	std::shared_ptr<RenderManager> RenderManager::GetpRenderManager(CUIWindowBase* p)
	{
		auto it = RenderManager::RenderManagerMap.find(p);
		if (it == RenderManager::RenderManagerMap.end())
		{
			RenderManager::RenderManagerMap[p] = std::shared_ptr<RenderManager>(new RenderManager(p));
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

	void RenderManager::BeginRender()
	{
		//if (pRT)
		pRT->BeginDraw();
	}

	void RenderManager::EndRender()
	{
		//if (pRT)
		pRT->EndDraw();
	}

	void RenderManager::Clear(cint ClearColor)
	{
		pRT->Clear(D2D1::ColorF(ClearColor));
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

	void RenderManager::RenderText(const std::wstring& Content, const std::wstring& FontName, cint FontSize, const cuiRect& Area, DwriteTextAlignment dta, DwriteParagraphAlignment dpa,cint TextColor)
	{
		auto pTextFormat = FactoriesCreator::GetFactoriesCreator()->GetpTextFormat(FontSize, FontName, dta, dpa);
		pRT->DrawTextW(
			Content.c_str(),
			Content.size(),
			pTextFormat,
			Area,
			GetBrush(TextColor),
			D2D1_DRAW_TEXT_OPTIONS_CLIP
			);
		pTextFormat->Release();
	}

	void RenderManager::RenderText(const std::wstring& Content, const cuiRect& Area, cint TextColor, IDWriteTextFormat* pFormat)
	{
		pRT->DrawTextW(
			Content.c_str(),
			Content.size(),
			pFormat,
			Area,
			GetBrush(TextColor),
			D2D1_DRAW_TEXT_OPTIONS_CLIP
			);
	}


	IDWriteTextFormat* FactoriesCreator::GetpTextFormat(cint FontSize, const std::wstring& FontName, DwriteTextAlignment dta, DwriteParagraphAlignment dpa)
	{
		IDWriteTextFormat* pFormat = nullptr;
		pDwFactory->CreateTextFormat(
			FontName.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			(FLOAT)FontSize,
			L"",
			&pFormat);
		pFormat->SetTextAlignment(dta);
		pFormat->SetParagraphAlignment(dpa);
		return pFormat;
	}

	void RenderManager::Resize(cint x, cint y)
	{
		pRT->Resize(D2D1::SizeU(x, y));
	}


	CUIControl::CUIControl(EventHandler* p)
		:pOwnerWindow(nullptr),
		Observer(),
		Composite(),
		pEventHandler(p),
		pController(Controller::GetpController())
	{
		pController->RegisterObserver(this);
	}
	
	void CUIControl::Update()
	{
		pOwnerWindow->ReDraw();
	}
	/*  Delegate  */
	void CUIControl::LeftButtonDown(MouseEventArgs mea)
	{
		pEventHandler->LeftButtonDown(mea);
	}

	void CUIControl::LeftButtonUp(MouseEventArgs mea)
	{
		pEventHandler->LeftButtonUp(mea);
	}

	void CUIControl::RightButtonDown(MouseEventArgs mea)
	{
		pEventHandler->RightButtonDown(mea);
	}

	void CUIControl::RightButtonUp(MouseEventArgs mea)
	{
		pEventHandler->RightButtonUp(mea);
	}

	void CUIControl::MouseMove(MouseEventArgs mea)
	{
		pEventHandler->MouseMove(mea);
	}
	CUIControl::~CUIControl()
	{
		delete pEventHandler;
		pController->RemoveObserver(this);
	}
	void CUIControl::SetpOwnerWindow(CUIWindowBase* p)
	{
		pOwnerWindow = p;
	}

	ButtonEventHandler::ButtonEventHandler(Button* _pButton)
		:pButton(_pButton)
		//Position(_Position)
	{

	}

	void ButtonEventHandler::OnClick(std::function<void()> _foo)
	{
		func = _foo;
	}

	void ButtonEventHandler::DownClick(std::function<void()> _foo)
	{
		Downfunc = _foo;
	}
	void ButtonEventHandler::SetPosition(const cuiRect& pos)
	{
		pButton->SetPosition(pos);
	}

	void ButtonEventHandler::LeftButtonDown(MouseEventArgs mea)
	{
		if (PtinRect(mea.EventPosition, pButton->GetPosition()))
		{
			pButton->SetState(Click);
			pButton->Update();
			if (Downfunc) Downfunc();
		}
		
	}

	void ButtonEventHandler::LeftButtonUp(MouseEventArgs mea)
	{
		if (pButton->GetState() == Statesenum::Click)
		{
			pButton->SetState(MoveOn);
			pButton->Update();
			if (func) func();
		}
	}

	void ButtonEventHandler::MouseMove(MouseEventArgs mea)
	{
		Statesenum TempState;
		PtinRect(mea.EventPosition, pButton->GetPosition()) ? TempState = MoveOn : TempState = Normal;
		if (TempState != pButton->GetState())
		{
			pButton->SetState(TempState);
			pButton->Update();
		}
		
	}

	void ButtonEventHandler::RightButtonDown(MouseEventArgs)
	{
		return;
	}

	void ButtonEventHandler::RightButtonUp(MouseEventArgs)
	{
		return;
	}

	cuiRect ButtonEventHandler::GetPosition() const
	{
		return pButton->GetPosition();
	}

	Button::Button(ButtonBorderFactory* _pBorderFactory,
		ButtonInsideFactory* _pInsidefactory,
		ButtonContentFactory* _pContentFactory, const std::wstring& _ButtonContent)
		:ButtonContent(_ButtonContent),
		CUIControl(new ButtonEventHandler(this)),
		ButtonState(Normal),
		pBorderFactory(_pBorderFactory),
		pInsideFactory(_pInsidefactory),
		pContentFactory(_pContentFactory)
	{

	}

	void Button::OnClick(std::function<void()> foo)
	{
		static_cast<ButtonEventHandler*>(pEventHandler)->OnClick(foo);
	}
	void Button::DownClick(std::function<void()> foo)
	{
		static_cast<ButtonEventHandler*>(pEventHandler)->DownClick(foo);
	}

	void Button::SetFont(const std::wstring& sw, cint fontsize,DwriteTextAlignment dta, DwriteParagraphAlignment dpa)
	{
		pContentFactory->SetFont(sw, fontsize);
	}
	Button::~Button()
	{
		delete pBorderFactory;
		delete pInsideFactory;
		delete pContentFactory;
	}

	void CUIControl::SetPosition(const cuiRect& cr)
	{
		Position = cr;
	}

	Statesenum Button::GetState()
	{
		return ButtonState;
	}

	void Button::SetState(Statesenum _state)
	{
		ButtonState = _state;
	}

	void Win8Button::Update()
	{
		Leaves.clear();
		switch (ButtonState)
		{
		case cibbonui::Click:
		{
			
			Add(pBorderFactory->GetClickBorder());
			Add(pInsideFactory->GetClickInside());
			Add(pContentFactory->GetNormalContent());
		}
			break;
		case cibbonui::MoveOn:
		{

			Add(pBorderFactory->GetMoveOnBorder());
			Add(pInsideFactory->GetMoveOnInside());
			Add(pContentFactory->GetNormalContent());
		}
			break;
		case cibbonui::Normal:
		{

			Add(pBorderFactory->GetNormalBorder());
			Add(pInsideFactory->GetNormalInside());
			Add(pContentFactory->GetNormalContent());
		}
			break;
		case cibbonui::Disabled:
			break;
		case MoveOut:
			break;
		default:
			break;
		}
		this->CUIControl::Update();
	}

	void Button::Draw(Context* p)
	{
		p->pParent = this;
		for (auto pl : Leaves)
		{
			pl->Draw(p);
		}
	}

	cuiRect CUIControl::GetPosition() const
	{
		return Position;
	}


	Win8Button::Win8Button(const std::wstring& _Content)
		:Button(new Win8ButtonBorderFactory,
		new Win8ButtonInsideFactory,
		new Win8ButtonContentFactory,
		_Content)
	{
		Add(pBorderFactory->GetNormalBorder());
		Add(pInsideFactory->GetNormalInside());
		Add(pContentFactory->GetNormalContent());
	}

	FlatButton::FlatButton(const std::wstring& _Content)
		:Button(nullptr,
		new FlatButtonInsideFactory,
		new FlatButtonContentFactory,
		_Content)
	{
		//Add(pBorderFactory->GetNormalBorder());
		//Add(pInsideFactory->GetNormalInside());
		Add(pContentFactory->GetNormalContent());
	}

	void FlatButton::Update()
	{
		Leaves.clear();
		switch (ButtonState)
		{
		case cibbonui::Click:
		{

			//Add(pBorderFactory->GetClickBorder());
			Add(pInsideFactory->GetClickInside());
			Add(pContentFactory->GetClickContent());
		}
			break;
		case cibbonui::MoveOn:
		{

			//Add(pBorderFactory->GetMoveOnBorder());
			Add(pInsideFactory->GetMoveOnInside());
			Add(pContentFactory->GetMoveOnContent());
		}
			break;
		case cibbonui::Normal:
		{

			//Add(pBorderFactory->GetNormalBorder());
			//Add(pInsideFactory->GetNormalInside());
			Add(pContentFactory->GetNormalContent());
		}
			break;
		case cibbonui::Disabled:
			break;
		case MoveOut:
			break;
		default:
			break;
		}
		this->CUIControl::Update();
	}

	CapButton::CapButton(const std::wstring& ws)
		:Button(
		nullptr,
		nullptr,
		nullptr,
		ws),
		pc(new WindowContent)
	{
		Leaves.push_back(pc);
	}

	void CapButton::SetFont(const std::wstring& sw, cint _fontsize, DwriteTextAlignment dta , DwriteParagraphAlignment dpa )
	{
		pc->SetFont(sw, _fontsize,dta,dpa);
	}
	void CapButton::Update()
	{
		this->CUIControl::Update();
	}
	/*------ButtonComposites-----*/
	/*Factory or Class itself ?*/

	Border::Border()
		:BorderColor(),
		BorderWidth(1.f )
	{

	}


	void Border::Draw(Context* p)
	{
		p->pWindow->GetpRenderManager()->DrawBorder(p->Position, BorderColor, BorderWidth);
	}

	void Border::SetBorderColor(cint _c)
	{
		BorderColor = _c;
	}

	void Border::SetBorderWidth(float width)
	{
		BorderWidth = width;
	}

	LinearInside::LinearInside()
		:LinearInsideColor1(0),
		LinearInsideColor2(0)
	{

	}

	void LinearInside::Draw(Context* p)
	{
		p->pWindow->GetpRenderManager()->LinearFillRect(p->Position, LinearInsideColor1, LinearInsideColor2);
	}

	void LinearInside::SetInsideColor(cint _color, cint _color2)
	{
		LinearInsideColor1 = _color;
		LinearInsideColor2 = _color2;
	}

	PureInside::PureInside()
		:InsideColor()
	{

	}

	void PureInside::SetInsideColor(cint _color, cint _color2)
	{
		InsideColor = _color;
	}

	void PureInside::Draw(Context* p)
	{
		p->pWindow->GetpRenderManager()->NormalFillRect(p->Position, InsideColor);
	}

	Content::Content()
		:ContentColor()
	{

	}

	void PureContent::Draw(Context* p)
	{
		p->pWindow->GetpRenderManager()->RenderText(dynamic_cast<CUIControl*>(p->pParent)->GetControlText(), p->Position,0);
	}

	PureContent::PureContent()
		:Content()
	{

	}
		
	SpecialContent::SpecialContent()
		:Content(),
		pFormat(FactoriesCreator::GetFactoriesCreator()->GetpNormalTextFormat())
	{

	}

	void SpecialContent::Draw(Context* p)
	{
		p->pWindow->GetpRenderManager()->RenderText(dynamic_cast<CUIControl*>(p->pParent)->GetControlText(), p->Position, 0, pFormat);
	}

	void SpecialContent::SetFont(const std::wstring& _fontname, cint fontsize ,DwriteTextAlignment dta, DwriteParagraphAlignment dpa)
	{
		if (pFormat != FactoriesCreator::GetFactoriesCreator()->GetpNormalTextFormat())
		 Free(pFormat);
		pFormat = FactoriesCreator::GetFactoriesCreator()->GetpTextFormat(fontsize, _fontname,dta,dpa);
	}

	SpecialContent::~SpecialContent()
	{
		if (pFormat != FactoriesCreator::GetFactoriesCreator()->GetpNormalTextFormat())
			Free(pFormat);
	}
	//ScrollBar::Scr

	void WindowContent::Draw(Context* p)
	{
		p->pWindow->GetpRenderManager()->RenderText(p->pWindow->GetControlText(), p->Position, 0, pFormat);
	}

/******************************************************************
	*                                                                 *
	*                                                                 *
	*                   ButtonBorderFactory                           *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/

	ButtonBorderFactory::ButtonBorderFactory(Border* p)
		:pBorder(p)
	{

	}

	Win8ButtonBorderFactory::Win8ButtonBorderFactory()
		:ButtonBorderFactory(new Border())
	{
		pBorder->SetBorderWidth(Win8ButtonBorderWidth);
	}
	


	ButtonBorderFactory::~ButtonBorderFactory()
	{
		delete pBorder;
	}

	Border* Win8ButtonBorderFactory::GetNormalBorder()
	{
		pBorder->SetBorderColor(0xacacac);
		return pBorder;
	}
	Border* Win8ButtonBorderFactory::GetMoveOnBorder()
	{
		pBorder->SetBorderColor(0x7eb4ea);
		return pBorder;
	}
	
	Border* Win8ButtonBorderFactory::GetClickBorder()
	{
		
	    pBorder->SetBorderColor(0x569de5);
		return pBorder;
	}
	/******************************************************************
	*                                                                 *
	*                                                                 *
	*                   ButtonInsideFactory                           *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/
	ButtonInsideFactory::ButtonInsideFactory(Inside* p)
		:pInside(p)
	{

	}

	ButtonInsideFactory::~ButtonInsideFactory()
	{
		delete pInside;
	}


	Win8ButtonInsideFactory::Win8ButtonInsideFactory()
		:ButtonInsideFactory(new LinearInside())
	{

	}

	Inside* Win8ButtonInsideFactory::GetNormalInside()
	{
		pInside->SetInsideColor(0xf0f0f0, 0xe5e5e5);
		return pInside;
	}
	Inside* Win8ButtonInsideFactory::GetMoveOnInside()
	{
		pInside->SetInsideColor(0xebf4fc, 0xdcecfc);
		return pInside;
	}

	Inside* Win8ButtonInsideFactory::GetClickInside()
	{
		pInside->SetInsideColor(0xdaecfc, 0xc4e0fc);;
		return pInside;
	}

	FlatButtonInsideFactory::FlatButtonInsideFactory()
		:ButtonInsideFactory(new PureInside())
	{

	}

	Inside* FlatButtonInsideFactory::GetNormalInside()
	{
		return nullptr;
	}
	Inside* FlatButtonInsideFactory::GetMoveOnInside()
	{
		
		pInside->SetInsideColor(0xd5e1f2,0);
		return pInside;
	}

	Inside* FlatButtonInsideFactory::GetClickInside()
	{
		pInside->SetInsideColor(0xa3bde3,0);
		return pInside;
	}
	/******************************************************************
	*                                                                 *
	*                                                                 *
	*                   ButtonContentFactory                          *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/
	ButtonContentFactory::ButtonContentFactory(Content* p)
		:pContent(p)
	{

	}

	Win8ButtonContentFactory::Win8ButtonContentFactory()
		:ButtonContentFactory(new PureContent())
	{

	}

	ButtonContentFactory::~ButtonContentFactory()
	{
		delete pContent;
	}		

	Content* Win8ButtonContentFactory::GetNormalContent()
	{
		return pContent;
	}
	Content* Win8ButtonContentFactory::GetMoveOnContent()
	{
		return nullptr;
	}

	Content* Win8ButtonContentFactory::GetClickContent()
	{
		return nullptr;
	}
	FlatButtonContentFactory::FlatButtonContentFactory()
		:ButtonContentFactory(new SpecialContent())
	{

	}
	Content* FlatButtonContentFactory::GetNormalContent()
	{
		return pContent;
	}
	Content* FlatButtonContentFactory::GetMoveOnContent()
	{
		return pContent;
	}

	Content* FlatButtonContentFactory::GetClickContent()
	{
		return pContent;
	}

	SizeBox::SizeBox(CUIWindowBase* p)
		:Container(),
		pMini(new FlatButton(L"\60")),
		pClose(new FlatButton(L"\162")),
		pMax(new FlatButton(L"\61"))
	{
		pMini->SetFont(L"Webdings");
		pClose->SetFont(L"Webdings");
		pMax->SetFont(L"Webdings");
		pMini->OnClick(
			[=]()
		{
			::CloseWindow(p->GetWindowHandle());
		});
		pClose->OnClick([=]()
		{
			::PostQuitMessage(0);
		});
		pMax->OnClick([=]()
		{
			if (pMax->GetControlText() == L"\61")
				pMax->SetControlText(L"\62");
			else
			{
				pMax->SetControlText(L"\61");
			}
			//Sth...
		});
		Add(pMax);
		Add(pMini);
		Add(pClose);
	}
	void SizeBox::Draw(Context* p)
	{
		auto temp = p->Position;
		for (auto pl : Leaves)
		{
			p->Position = pl->GetPosition();
			pl->Draw(p);
		}
		p->Position = temp;
	}

	Caption::Caption(CUIWindowBase* p)
		:pSizeBox(new SizeBox(p)),
		pCap(new CapButton())
	{
		Add(pSizeBox);
		pCap->SetFont(L"Segoe WP Semilight",15,TextAlignmentLeft);
		pCap->DownClick(
			[=]()
		{
			::SendMessage(pCap->GetpOwnerWindow()->GetWindowHandle(), WM_NCLBUTTONDOWN, HTCAPTION, 0);
		});
		Add(pCap);
	}

	void Caption::Draw(Context* p)
	{
		auto temp = p->Position;
		for (auto pl : Leaves)
		{
			p->Position = pl->GetPosition();
			pl->Draw(p);
		}
		p->Position = temp;
	}
	void SizeBox::SetRoot(CUIWindowBase* p)
	{
		pMini->SetRoot(p);
		pClose->SetRoot(p);
		pMax->SetRoot(p);
	}

	void SizeBox::ReSize(const cuiRect& Position)
	{
		pMini->SetPosition(cuiRect(Position.right - 29 * 3, Position.top + 3, Position.right - 29 * 2, 23));
		pClose->SetPosition(cuiRect(Position.right - 29, Position.top + 3, Position.right, 23));
		pMax->SetPosition(cuiRect(Position.right - 29 * 2, Position.top + 3, Position.right - 29, 23));
	}

	void Caption::ReSize(const cuiRect& po)
	{
		pSizeBox->ReSize(po);
		pCap->SetPosition(cuiRect(po.left+10, po.top, po.right - 29*3, po.top + 36));
	}

	void Caption::SetRoot(CUIWindowBase* p)
	{
		pSizeBox->SetRoot(p);
		pCap->SetRoot(p);
	}
}