#include "stdafx.h"
#include "cibbonwindows.h"
#include "cuirendermanager.h"
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
		windowmessage(),
		subject()
	{
		//initevents();
		//init();
	}

	void cuiwindowbase::run()
	{
		
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
		//initevents();
		if (!m_hWnd) std::abort();
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);
	}

	LRESULT CALLBACK cuiwindowbase::WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (Message == WM_CREATE)
		{
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			cuiwindowbase* pwindow = reinterpret_cast<cuiwindowbase*>(pcs->lpCreateParams);
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
				map<UINT, cuiwindowbase::winfunc>::iterator it;
				if ((it = pwindow->windowmessage.find(Message)) != (pwindow->windowmessage).end())
				{
					return it->second(hWnd, Message, wParam, lParam);
				}
			}
		}
		return ::DefWindowProc(hWnd, Message, wParam, lParam);
	}


	void cuiwindowbase::addevents(UINT Message, winfunc Func)
	{
		windowmessage[Message] = Func;
	}

	cuistdwindow::cuistdwindow(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle, cint _width, cint _height, cstyle _style)
		:cuiwindowbase(_hInst, _title, _windowstyle, _width, _height, _style)
	{
		
		//run();
		initevents();
		init();
	}

	cuistdwindow::cuistdwindow():cuiwindowbase(){}
	cuistdwindow::~cuistdwindow(){}

	void cuistdwindow::initevents()
	{
		addevents(WM_CLOSE, [](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT{
			::PostQuitMessage(0);
			return ::DefWindowProc(hWnd, Message, wParam, lParam);
		}
		);
		addevents(WM_SIZE, [](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT
		{
			LONG_PTR Style = ::GetWindowLongPtr(hWnd, GWL_STYLE);
			Style = Style &~WS_CAPTION &~WS_SYSMENU &~WS_SIZEBOX;
			::SetWindowLongPtr(hWnd, GWL_STYLE, Style);
			return::DefWindowProc(hWnd, Message, wParam, lParam);
		}
		);
		addevents(WM_LBUTTONDOWN, [this](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT{
			cuibuttonevent bevent;
			bevent.eventposition = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			bevent.eventname = cuieventenum::lbuttondown;
			notifyobservers(&bevent);
			return::DefWindowProc(hWnd, Message, wParam, lParam);
		});
		addevents(WM_MOUSEMOVE, [this](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT{
			cuibuttonevent bevent;
			bevent.eventposition = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			bevent.eventname = cuieventenum::mousemove;
			notifyobservers(&bevent);
			return::DefWindowProc(hWnd, Message, wParam, lParam);
		});
		addevents(WM_LBUTTONUP, [this](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT{
			cuibuttonevent bevent;
			bevent.eventposition = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			bevent.eventname = cuieventenum::lbuttonup;
			notifyobservers(&bevent);
			return::DefWindowProc(hWnd, Message, wParam, lParam);
		});
		addevents(WM_RBUTTONDOWN, [this](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT{
			cuibuttonevent bevent;
			bevent.eventposition = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			bevent.eventname = cuieventenum::lbuttondown;
			notifyobservers(&bevent);
			return::DefWindowProc(hWnd, Message, wParam, lParam);
		});
		addevents(WM_RBUTTONUP, [this](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT{
			cuibuttonevent bevent;
			bevent.eventposition = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			bevent.eventname = cuieventenum::rbuttonup;
			notifyobservers(&bevent);
			return::DefWindowProc(hWnd, Message, wParam, lParam);
		});
		addevents(WM_LBUTTONDBLCLK, [this](HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT{
			cuibuttonevent bevent;
			bevent.eventposition = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			bevent.eventname = cuieventenum::lbuttondoubleclick;
			notifyobservers(&bevent);
			return::DefWindowProc(hWnd, Message, wParam, lParam);
		});
	}


	//controllermanager::controllermanager(){}

	//controllermanager::~controllermanager(){}

	//LRESULT controllermanager::handlemessage(
	//	HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	//{
	//	//this->notifyobservers();
	//	//这里应该只处理控件事件
	//	/*if (Message == WM_DESTROY)
	//	{
	//		PostQuitMessage(0);
	//	}*/
	//	return ::DefWindowProc(hWnd, Message, wParam, lParam);
	//}

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
		pPatternManager->setrect(this->getPosition());
	}

	void cibboncontrolbase::HandleNotify(cuieventbase* pceb)
	{
		if (!PtInRect(&rPosition, static_cast<cuibuttonevent*>(pceb)->eventposition))
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
			if (ifin) return;
			if (!ifin && pceb->eventname == mousemove)
			{
				pceb->eventname = mousemovein;
				ifin = true;
			}
		}
		auto it = EventHandler.find(pceb->eventname);
		if ( it != EventHandler.end())
		{
			for (auto vit : it->second)
				vit(pceb);
		}
	}

	cuibutton::cuibutton(HWND hWnd, PatternManagerBase* _pPatternManager, const CRect& _Position, const std::wstring& _text, bool Enable)
		:cibboncontrolbase(new ButtonPattern(hWnd),_Position, _text, Enable)
	{
		initevents();
	}
	//绘制事件整装待发！
	void cuibutton::initevents()
	{
		auto Func = static_cast<unsigned int(__stdcall*)(void*)>([](void* p)-> unsigned int{static_cast<ButtonPattern*>(p)->drawdown(); return 0; });
		addevents(lbuttondown, [this, Func](cuieventbase* pceb)->void{
			_beginthreadex(0, 0, 
				Func,
				pPatternManager.get(), 0, 0); });

			Func = static_cast<unsigned int(__stdcall*)(void*)>([](void* p)-> unsigned int{static_cast<ButtonPattern*>(p)->drawup(); return 0; });
			addevents(lbuttonup, [this, Func](cuieventbase* pceb)->void{
			_beginthreadex(0, 0, 
				Func,
				pPatternManager.get(), 0, 0);  });
			Func = static_cast<unsigned int(__stdcall*)(void*)>([](void* p)-> unsigned int{static_cast<ButtonPattern*>(p)->drawdown(); return 0; });
	        addevents(mousemovein, [this, Func](cuieventbase* pceb)->void{
		      _beginthreadex(0, 0,
			Func,
			pPatternManager.get(), 0, 0);  });
			Func = static_cast<unsigned int(__stdcall*)(void*)>([](void* p)-> unsigned int{static_cast<ButtonPattern*>(p)->drawusual(); return 0; });
			addevents(mousemoveout, [this, Func](cuieventbase* pceb)->void{
				  _beginthreadex(0, 0,
					  Func,
					  pPatternManager.get(), 0, 0);  });
	}

}

