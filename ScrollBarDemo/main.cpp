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
	Win8Button* pButton = new Win8Button(L"Win8Button");
	FlatButton* pFButton = new FlatButton(L"FlatButton");
	pButton->OnClick([=]()
	{
		::MessageBox(0, L"Hello!", L"Demo", 0);
	});
	pCanvas->Add(pButton, cuiRect(120, 20, 200, 50));
	pCanvas->Add(pFButton, cuiRect(20, 20, 100, 50));
	pMainWindow->Add(pCanvas);
	pMainWindow->Run();
	delete pMainWindow;
	return 0;

}