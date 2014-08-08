#include "CUIWindows.h"
#include <tchar.h>

using namespace cibbonui;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPTSTR,
	_In_ int)
{
	CUIMainWindow* pMainWindow = new CUIMainWindow(L"Test");
	Canvas* pCanvas = new Canvas();
	Win8Button* pButton = new Win8Button(L"Button");
	pCanvas->Add(pButton,cuiRect(20,20,100,100));
	pMainWindow->Add(pCanvas);
	pMainWindow->Run();
	delete pMainWindow;
	return 0;
}