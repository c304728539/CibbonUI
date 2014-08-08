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
			p->Position = pl->GetEventPosition();
			pl->Draw(p);
		}
	}

	void Canvas::Add(Component* p, const cuiRect& rc)
	{
		p->SetPosition(rc);
		RegisterObserver(dynamic_cast<Observer*>(p));
		this->Composite::Add(p);
	}
	void Canvas::SetPosition(const cuiRect& cr)
	{
		Position = cr;
	}

	void Canvas::LeftButtonDown(MouseEventArgs mea)
	{
		for (auto p : Observerlist)
		{
			p->LeftButtonDown(mea);
		}
	}
	void Canvas::LeftButtonUp(MouseEventArgs mea)
	{
		for (auto p : Observerlist)
		{
			p->LeftButtonUp(mea);
		}
	}

	void Canvas::RightButtonDown(MouseEventArgs mea)
	{
		for (auto p : Observerlist)
		{
			p->RightButtonDown(mea);
		}
	}

	void Canvas::RightButtonUp(MouseEventArgs mea)
	{
		for (auto p : Observerlist)
		{
			p->RightButtonUp(mea);
		}
	}
	void Canvas::MouseMove(MouseEventArgs mea)
	{
		for (auto p : Observerlist)
		{
			p->MouseMove(mea);
		}
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
		:pOwnerWindow(pWindow),
		pContext(new Context(pWindow))
	{

	}

	MessageHandler::~MessageHandler()
	{
		delete pContext;
		pContext = nullptr;
	}
	MainWindowMessageHandler::MainWindowMessageHandler(CUIWindowBase* p)
		: MessageHandler(p),
		pContainer(nullptr)
	{

	}
    
	void MainWindowMessageHandler::SetContainer(Container* p)
	{
		pContainer = p;
	}
	
	bool MainWindowMessageHandler::MessageHandlerWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre)
	{
		static bool HasFirstSized = false;
		/*if (Message == WM_NCHITTEST)
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
		}*/


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
			pOwnerWindow->GetpRenderManager()->Resize(LOWORD(lParam), HIWORD(lParam));
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
		case WM_PAINT:
		{
			pContext->Position = pOwnerWindow->GetClientRect();
			dynamic_cast<CUIMainWindow*>(pOwnerWindow)->Draw(pContext);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			MouseEventArgs mea;
			mea.EventPosition = cuiPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			mea.Sender = pContainer;
			pContainer->LeftButtonDown(mea);
			break;
		}
		case WM_MOUSEMOVE:
		{
			MouseEventArgs mea;
			mea.EventPosition = cuiPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			mea.Sender = pContainer;
			pContainer->MouseMove(mea);
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
		pMainWindowMessageHandler(new MainWindowMessageHandler(this))
	{

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
		pContainer->Draw(p);
		pRenderManager->EndRender();
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
		{
			pDwFactory->CreateTextFormat(
				L"Microsoft YaHei",
				nullptr,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				NormalFontSize,
				L"",
				&pNormalTextFormat);
			pNormalTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			pNormalTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
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
		pRT->BeginDraw();
	}

	void RenderManager::EndRender()
	{
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
		auto pTextFormat = GetpTextFormat(FontSize, FontName, dta, dpa);
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

	IDWriteTextFormat* RenderManager::GetpTextFormat(cint FontSize, const std::wstring& FontName, DwriteTextAlignment dta, DwriteParagraphAlignment dpa)
	{
		IDWriteTextFormat* pFormat = nullptr;
		pDwFactory->CreateTextFormat(
			FontName.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			FontSize,
			L"",
			&pFormat);
		return pFormat;
	}

	void RenderManager::Resize(cint x, cint y)
	{
		pRT->Resize(D2D1::SizeU(x, y));
	}


	CUIControl::CUIControl()
		:pOwnerWindow(nullptr),
		Observer(),
		Composite()
	{

	}
	
	void CUIControl::Update()
	{
		pOwnerWindow->ReDraw();
	}


	void CUIControl::SetpOwnerWindow(CUIWindowBase* p)
	{
		pOwnerWindow = p;
	}

	ButtonEventHandler::ButtonEventHandler(Win8Button* _pButton)
		:pButton(_pButton)
		//Position(_Position)
	{

	}

	void ButtonEventHandler::SetPosition(const cuiRect& pos)
	{
		Position = pos;
	}

	void ButtonEventHandler::LeftButtonDown(MouseEventArgs mea)
	{
		if (PtinRect(mea.EventPosition, Position))
		{
			pButton->SetState(Click);
			pButton->Update();
		}
	}

	void ButtonEventHandler::LeftButtonUp(MouseEventArgs mea)
	{
		if (pButton->GetState() == Statesenum::Click)
		{
			pButton->SetState(Normal);
			pButton->Update();
		}
	}

	void ButtonEventHandler::MouseMove(MouseEventArgs mea)
	{
		Statesenum TempState;
		PtinRect(mea.EventPosition, Position) ? TempState = MoveOn : TempState = Normal;
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
		return Position;
	}

	Win8Button::Win8Button(const std::wstring& _ButtonContent)
		:pEventHandler(new ButtonEventHandler(this)),
		pNormalButtonFactory(new NormalButtonCompositeFactory),
		pMoveOnButtonFactory(new MoveOnButtonCompositeFactory),
		pClickButtonFactory(new ClickButtonCompositeFactory),
		pNowButtonFactory(pNormalButtonFactory),
		ButtonContent(_ButtonContent),
		CUIControl(),
		ButtonState(Normal)
	{

	}

	/*  Delegate  */
	void Win8Button::LeftButtonDown(MouseEventArgs mea)
	{
		pEventHandler->LeftButtonDown(mea);
	}

	void Win8Button::LeftButtonUp(MouseEventArgs mea)
	{
		pEventHandler->LeftButtonUp(mea);
	}

	void Win8Button::RightButtonDown(MouseEventArgs mea)
	{
		pEventHandler->RightButtonDown(mea);
	}

	void Win8Button::RightButtonUp(MouseEventArgs mea)
	{
		pEventHandler->RightButtonUp(mea);
	}

	void Win8Button::MouseMove(MouseEventArgs mea)
	{
		pEventHandler->MouseMove(mea);
	}

	Win8Button::~Win8Button()
	{
		delete pEventHandler;
	}

	void Win8Button::SetPosition(const cuiRect& cr)
	{
		pEventHandler->SetPosition(cr);
	}

	Statesenum Win8Button::GetState()
	{
		return ButtonState;
	}

	void Win8Button::SetState(Statesenum _state)
	{
		ButtonState = _state;
	}

	void Win8Button::Update()
	{
		switch (ButtonState)
		{
		case cibbonui::Click:
			pNowButtonFactory = pClickButtonFactory;
			break;
		case cibbonui::MoveOn:
			pNowButtonFactory = pMoveOnButtonFactory;
			break;
		case cibbonui::Normal:
			pNowButtonFactory = pNormalButtonFactory;
			break;
		case cibbonui::Disabled:
			break;
		default:
			break;
		}
		this->CUIControl::Update();
	}

	void Win8Button::Draw(Context* p)
	{
		pNowButtonFactory->CreateBorder()->Draw(p);
		pNowButtonFactory->CreateInside()->Draw(p);
		pNowButtonFactory->CreateContent(ButtonContent)->Draw(p);
	}

	cuiRect Win8Button::GetEventPosition() const
	{
		return pEventHandler->GetPosition();
	}
	/*------ButtonComposites-----*/
	/*Factory or Class itself ?*/

	Border::Border(cint _BorderColor, float _BorderWidth)
		:BorderColor( _BorderColor ),
		BorderWidth( _BorderWidth )
	{

	}


	void Border::Draw(Context* p)
	{
		p->pWindow->GetpRenderManager()->DrawBorder(p->Position.getMargin(1.f,1.f), BorderColor, BorderWidth);
	}

	Inside::Inside(cint color1, cint color2)
		:InsideColor1(color1),
		InsideColor2(color2)
	{

	}

	void Inside::Draw(Context* p)
	{
		p->pWindow->GetpRenderManager()->LinearFillRect(p->Position, InsideColor1, InsideColor2);
	}


	Content::Content(const std::wstring& _text)
		:ContentString(_text)
	{

	}

	void Content::Draw(Context* p)
	{
		p->pWindow->GetpRenderManager()->RenderText(ContentString, p->Position,0);
	}

	const std::wstring& Content::GetContent() const
	{
		return ContentString;
	}

	/* ButtonFactories */

	ButtonCompositeFactory::ButtonCompositeFactory()
		:pBorder(nullptr),
		pInside(nullptr),
		pContent(nullptr)
	{

	}

	ButtonCompositeFactory::~ButtonCompositeFactory()
	{
		delete pBorder;
		delete pInside;
		delete pContent;
		pBorder = nullptr;
		pInside = nullptr;
		pContent = nullptr;
	}

	Border* NormalButtonCompositeFactory::CreateBorder()
	{
		if (!pBorder)
			pBorder = new Border(0xacacac);
		return pBorder;
	}


	Inside* NormalButtonCompositeFactory::CreateInside()
	{
		if (!pInside)
			pInside = new Inside(0xf0f0f0, 0xe5e5e5);
		return pInside;
	}

	Content* NormalButtonCompositeFactory::CreateContent(const std::wstring& _text)
	{
		if (!pContent || pContent->GetContent() != _text)
		{
			delete pContent;
			pContent = new Content(_text);
		}
		return pContent;
	}


	Border* MoveOnButtonCompositeFactory::CreateBorder()
	{
		if (!pBorder)
			pBorder = new Border(0x7eb4ea);
		return pBorder;
	}


	Inside* MoveOnButtonCompositeFactory::CreateInside()
	{
		if (!pInside)
			pInside = new Inside(0xebf4fc, 0xdcecfc);
		return pInside;
	}

	Content* MoveOnButtonCompositeFactory::CreateContent(const std::wstring& _text)
	{
		if (!pContent || pContent->GetContent() != _text)
		{
			delete pContent;
			pContent = new Content(_text);
		}
		return pContent;
	}

	Border* ClickButtonCompositeFactory::CreateBorder()
	{
		if (!pBorder)
			pBorder = new Border(0x7eb4ea);
		return pBorder;
	}


	Inside* ClickButtonCompositeFactory::CreateInside()
	{
		if (!pInside)
			pInside = new Inside(0xebf4fc, 0xdcecfc);
		return pInside;
	}

	Content* ClickButtonCompositeFactory::CreateContent(const std::wstring& _text)
	{
		if (!pContent || pContent->GetContent() != _text)
		{
			delete pContent;
			pContent = new Content(_text);
		}
		return pContent;
	}
}