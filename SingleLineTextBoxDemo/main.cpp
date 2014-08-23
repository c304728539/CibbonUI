#include "..\\CibbonUI\\CUIWindows.h"
#include <tchar.h>

using namespace cibbonui;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPTSTR,
	_In_ int)
{
	CUIMainWindow* pMainWindow = new CUIMainWindow(L"CibbonWindow");
	Canvas* pCanvas = new Canvas();
	SingleLineTextBox* pTextBox = new SingleLineTextBox();
	Win8Button* pButton = new Win8Button(L"Win8Button");
	
	pCanvas->Add(pTextBox, cuiRect(150, 20, 300, 60));
	pCanvas->Add(pButton, cuiRect(20, 20, 120, 40));
	pMainWindow->Add(pCanvas);
	pMainWindow->Run();
	delete pMainWindow;
	return 0;

}