#include "CUIWindows.h"
#include <algorithm>

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


	WndClass::WndClass(std::wstring _name,
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
		RegisterWindow();
	}

	std::wstring WndClass::getClassName() const
	{
		return ClassName;
	}

	void WndClass::RegisterWindow()
	{
		atom = RegisterClassEx(&WndClassEx);
	}

	WindowCreator::WindowCreator(const std::wstring& ClassName, std::wstring Title)
		:WindowTitle(Title),
		Width(CW_USEDEFAULT),
		Height(CW_USEDEFAULT),
		lpParam(this),
		ExStyle(0),
		hParentWindow(0),
		WindowClassName(ClassName)
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
 
	CUIWindowBase::CUIWindowBase(const std::wstring& Title)
		:WindowTitle(Title),
		pWndClass(nullptr),
		pWindowCreator(nullptr),
		pHandler(nullptr),
		WindowHandle(0)
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
		WindowHandle = pWindowCreator->Create();
	}

	CUIMainWindow::CUIMainWindow(const std::wstring& Title)
		:CUIWindowBase(Title)
	{

	}


	void CUIMainWindow::CreateInitors()
	{
		pWndClass = new WndClass(L"CUIMainWindow", true, CUIMainWindow::WndProc, getInstance());
		pWindowCreator = new WindowCreator(pWndClass->getClassName(), WindowTitle);
	}

	void CUIMainWindow::Run()
	{
		Initialize();
		ShowWindow(WindowHandle, SW_SHOWNORMAL);
		UpdateWindow(WindowHandle);
		RunMessageLoop();
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

}