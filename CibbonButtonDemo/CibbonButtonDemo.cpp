#include "..\\CibbonUI\\cibbonwindows.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPTSTR    lpCmdLine,
		_In_ int       nCmdShow)
	{
		UNREFERENCED_PARAMETER(hPrevInstance);
		UNREFERENCED_PARAMETER(lpCmdLine);
		cibbonui::cuistdwindow MainWindow(hInstance, L"hah");
		cibbonui::ButtonPattern Pattern(MainWindow.gethwnd());
		cibbonui::cuibutton Buttontest(&Pattern, D2D1::RectF(20, 20, 70, 70), L"Button Demo");
		Buttontest.Onclick([]()->void{::MessageBox(0,L"haha",L"Demo",0); });
		MainWindow.registerobserver(&Buttontest);
		auto hWnd = MainWindow.gethwnd();
		MainWindow.addevents(WM_LBUTTONDOWN, [hWnd](HWND, UINT, WPARAM, LPARAM)->LRESULT{
			::SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0); 
			return cibbonui::already;
		});
		MainWindow.run();
		//CaptionTest test(hInstance);
		return 0;
	}