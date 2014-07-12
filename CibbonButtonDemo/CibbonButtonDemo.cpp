#include "..\\CibbonUI\\cibboncore.h"


using namespace cibbonui;
using namespace D2D1;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE ,
		_In_ LPTSTR  ,
		_In_ int)
{
	//_CrtSetBreakAlloc(268);
	cuistdwindow MainWindow(hInstance, L"haha");
	cuibutton<closebuttonPattern> Buttontest(MainWindow.gethwnd(), D2D1::RectF(MainWindow.getPosition().right - 20, 0, MainWindow.getPosition().right, 20), L"ÍË³ö");
	Buttontest.Onclick([]()->void{::MessageBox(0, L"haha", L"Demo", 0);
	//::PostQuitMessage(0);
	});
	MainWindow.registerobserver(&Buttontest);
	MainWindow.addevents(WM_LBUTTONDOWN, [](WINPAR)->bool{
		::SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		return already;
	});
	MainWindow.run();
	
	return 0;
}