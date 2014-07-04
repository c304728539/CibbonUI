#include "stdafx.h"

#include "cibbonwindows.h"
using namespace std;

namespace cibbonui
{
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
		pmanager(new controllermanager()),
		windowmessage()
	{
	}

	void cuiwindowbase::run()
	{
		init();
		runmessageloop();
	}

	cuiwindowbase::~cuiwindowbase()
	{
	}

	void cuiwindowbase::runmessageloop()
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
		initevents();
		if (!m_hWnd) std::abort();
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);
	}

	LRESULT CALLBACK cuiwindowbase::WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (Message == WM_CREATE)
		{
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			cuiwindowbase* pwindow = reinterpret_cast<cuiwindowbase*>(pcs);
			::SetWindowLongPtr(
				hWnd,
				GWLP_USERDATA,
				PtrToUlong(pwindow));
		}
		else
		{
			cuiwindowbase* pwindow = reinterpret_cast<cuiwindowbase*>
				(static_cast<LONG_PTR>(
				::GetWindowLongPtr(
				hWnd,
				GWLP_USERDATA)));
			if (pwindow)
			{
				//auto it = pwindow->windowmessage.find(Message);
				if (((pwindow->windowmessage).find(Message)) != (pwindow->windowmessage).end())
				{
					::MessageBox(0, L"haha", 0, 0);
					return pwindow->windowmessage[Message](hWnd, Message, wParam, lParam);
				}
					
				pwindow->pmanager->handlemessage(hWnd, Message, wParam, lParam);
			}
		}
		return ::DefWindowProc(hWnd, Message, wParam, lParam);
	}


	void cuiwindowbase::addevents(UINT Message, winfunc Func)
	{
		windowmessage[Message] = Func;
	}

	cuistdwindow::cuistdwindow(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle, cint _width, cint _height, cstyle _style)
		:cuiwindowbase(_hInst, _title, _windowstyle, _width, _height, _style),
		closefunc([](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT{
		::PostQuitMessage(0);
		return ::DefWindowProc(hWnd, Message, wParam, lParam);
	})
	{
		//initevents();
		run();
	}

	cuistdwindow::cuistdwindow():cuiwindowbase(){}
	cuistdwindow::~cuistdwindow(){}

	void cuistdwindow::initevents()
	{
		addevents(WM_CLOSE, closefunc);
	}

	controllermanager::controllermanager(){}

	controllermanager::~controllermanager(){}

	LRESULT controllermanager::handlemessage(
		HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		//this->notifyobservers();
		//这里应该只处理控件事件
		/*if (Message == WM_DESTROY)
		{
			PostQuitMessage(0);
		}*/
		return ::DefWindowProc(hWnd, Message, wParam, lParam);
	}

	void controllermanager::registerobserver(observer* obs)
	{
		observers.push_back(obs);
	}

	void controllermanager::removeobserver()
	{
		observers.pop_back();
	}

	void controllermanager::notifyobservers(cuievent* )
	{
		for (auto it : observers)
		{
			it->update();
		}
	}
}

