#include "..\\CibbonUI\\cibboncore.h"
#include <string>

using namespace cibbonui;
using namespace std;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPTSTR,
	_In_ int)
{
	SkinManager::setstyle(nightstyle);
	cuistdwindow MainWindow(hInstance, L"CibbonWindow");
	cuiFixedTab x(&MainWindow, L"Tab1");
	cuiFixedTab x2(&MainWindow, L"Tab2");
	cuiFixedTabs<TabPattern> fts(&MainWindow, cuirect(20, 20, 540, 320), L"haha");
	//该坐标为白色区域坐标
	fts.AddTab({ &x,&x2 });
	MainWindow.registerobserver(&fts);
	MainWindow.run();
	return 0;
}