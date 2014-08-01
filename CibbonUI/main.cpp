#include "CUIWindows.h"
#include <iostream>
#include <Windows.h>
#include <tchar.h>

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPTSTR,
	_In_ int)
{
	cibbonui::CUIMainWindow MainWindow(L"test");
	MainWindow.Run();
	return 0;
}