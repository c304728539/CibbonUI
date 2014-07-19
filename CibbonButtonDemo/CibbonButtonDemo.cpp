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
	SkinManager::setstyle(nightstyle);
	cuistdwindow MainWindow(hInstance, L"CibbonWindow");
	cuibutton<SizeBoxPattern> closebutton(MainWindow.gethwnd(), cuirect(MainWindow.getclientPosition().right - closebuttonwidth, 0.f, MainWindow.getclientPosition().right, Captionheight), L"\162");
	closebutton.Onclick([](cuievent* pe)->void{
	::PostQuitMessage(0);
	});
	cuibutton<SizeBoxPattern> minibutton(MainWindow.gethwnd(), cuirect(MainWindow.getclientPosition().right - 2 * closebuttonwidth, 0.f, MainWindow.getclientPosition().right - closebuttonwidth, Captionheight), L"\60");
	minibutton.Onclick([&](cuievent* pe)->void{
	  ::CloseWindow(MainWindow.gethwnd());
	});
	MainWindow.registerobserver(&closebutton);
	MainWindow.registerobserver(&minibutton);
	glowwindow gw(&MainWindow);
	closebutton.RegisterTooltip(L"关闭窗口");
	minibutton.RegisterTooltip(L"最小化");
	MainWindow.run();
	
	return 0;
}