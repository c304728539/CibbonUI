#include "..\\CibbonUI\\cibbonwindows.h"


using namespace cibbonui;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE ,
		_In_ LPTSTR  ,
		_In_ int)
	{
		cuistdwindow MainWindow(hInstance, L"haha");
		ButtonPattern Pattern(MainWindow.gethwnd());
		cuibutton Buttontest(&Pattern, D2D1::RectF(120, 120, 220, 150), L"ÍË³ö");
		Buttontest.Onclick([]()->void{::MessageBox(0,L"haha",L"Demo",0);
		//::PostQuitMessage(0); 
		});
		MainWindow.registerobserver(&Buttontest);
		auto hWnd = MainWindow.gethwnd();
		MainWindow.addevents(WM_LBUTTONDOWN, [hWnd](WINPAR)->bool{
			::SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0); 
			return already;
		});
		MainWindow.run();
		return 0;
	}