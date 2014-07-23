#include "..\\CibbonUI\\cibboncore.h"

using namespace cibbonui;
using namespace std;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPTSTR,
	_In_ int)
{
	SkinManager::setstyle(daystyle);
	cuistdwindow MainWindow(hInstance, L"CibbonWindow");
	cuiFixedTab x(&MainWindow, L"Tab1");
	cuiFixedTab x2(&MainWindow, L"Tab2");
	cuiFixedTab x3(&MainWindow, L"This is Tab3");
	cuiFixedTabs<TabPattern> fts(&MainWindow, cuirect(20, 20, 600, 400), L"Demo");
	fts.AddTab({ &x, &x2, &x3 });
	MainWindow.registerobserver(&fts);
	MainWindow.run();
	return 0;
}