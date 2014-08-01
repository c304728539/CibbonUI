#include "CUIWindows.h"

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

		if (PtMouse.y >= WindowRect.top && PtMouse.y < WindowRect.top )
		{
			fOnResizeBorder = (PtMouse.y < (WindowRect.top + 4 - rcFrame.top));
			uRow = 0;
		}
		else if (PtMouse.y < WindowRect.bottom  && PtMouse.y >= WindowRect.bottom - 7)    
		{ 
			uRow = 2; 
		}
		// Determine if the point is at the left or right of the window.     
		if (PtMouse.x >= WindowRect.left && PtMouse.x < WindowRect.left+7 ) 
		{         
			uCol = 0; // left side     
		}     
		else if (PtMouse.x < WindowRect.right && PtMouse.x >= WindowRect.right - 7)     
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

	cuiRect CUIWindowBase::GetWindowRect()
	{
		RECT rc;
		::GetWindowRect(WindowHandle,&rc);
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

}