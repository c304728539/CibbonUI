#include "..\\CibbonUI\\cibboncore.h"
#include <string>

using namespace cibbonui;
using namespace D2D1;
using namespace std;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE ,
		_In_ LPTSTR  ,
		_In_ int)
{
	//_CrtSetBreakAlloc(268);
	SkinManager::setstyle(daystyle);
	cuistdwindow MainWindow(hInstance, L"CibbonWindow");
	cuibutton<SizeBoxPattern> closebutton(MainWindow.gethwnd(), D2D1::RectF(MainWindow.getPosition().right-closebuttonwidth, 0, MainWindow.getPosition().right, Captionheight), L"\162");
	closebutton.Onclick([](cuievent* pe)->void{
	::PostQuitMessage(0);
	});
	cuibutton<SizeBoxPattern> minibutton(MainWindow.gethwnd(), D2D1::RectF(MainWindow.getPosition().right - 2 * closebuttonwidth, 0, MainWindow.getPosition().right - closebuttonwidth, Captionheight), L"\60");
	minibutton.Onclick([&](cuievent* pe)->void{
	  ::CloseWindow(MainWindow.gethwnd());
	});
	MainWindow.registerobserver(&closebutton);
	MainWindow.registerobserver(&minibutton);
	glowwindow gw(&MainWindow);
	MainWindow.run();
	
	return 0;
}