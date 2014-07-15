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
	cuistdwindow MainWindow(hInstance, L"haha");
	cuibutton<closebuttonPattern> closebutton(MainWindow.gethwnd(), D2D1::RectF(MainWindow.getPosition().right-closebuttonwidth, 0, MainWindow.getPosition().right, Captionheight), L"ÍË³ö");
	closebutton.Onclick([](cuievent* pe)->void{//::MessageBox(0, L"haha", L"Demo", 0);
	::PostQuitMessage(0);
	});
	cuibutton<minimizebuttonPattern> minibutton(MainWindow.gethwnd(), D2D1::RectF(MainWindow.getPosition().right - 2 * closebuttonwidth, 0, MainWindow.getPosition().right - closebuttonwidth, Captionheight), L"ÍË³ö");
	minibutton.Onclick([&](cuievent* pe)->void{//::MessageBox(0, L"haha", L"Demo", 0);
	//::PostQuitMessage(0);
		cuiTooltip::setpOwner(&MainWindow);
		cuiTooltip::getTooltip()->show(pe->eventposition, L"haha");
	//::CloseWindow(MainWindow.gethwnd());
	});
	MainWindow.registerobserver(&closebutton);
	MainWindow.registerobserver(&minibutton);
	glowwindow gw(&MainWindow);
	MainWindow.run();
	
	return 0;
}