#include "..\\CibbonUI\\cibboncore.h"
#include <string>

using namespace cibbonui;
using namespace std;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE ,
		_In_ LPTSTR  ,
		_In_ int)
{
	SkinManager::setstyle(nightstyle);
	cuistdwindow MainWindow(hInstance, L"CibbonWindow");
	MainWindow.run();
	return 0;
}