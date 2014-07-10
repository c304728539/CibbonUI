#include "..\\CibbonUI\\cibbonwindows.h"
#include "CaptionTest.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPTSTR    lpCmdLine,
		_In_ int       nCmdShow)
	{
		UNREFERENCED_PARAMETER(hPrevInstance);
		UNREFERENCED_PARAMETER(lpCmdLine);
		/*cibbonui::cuistdwindow haha(hInstance, L"haha");
		cibbonui::ButtonPattern pattern(haha.gethwnd());
		cibbonui::cuibutton buttontest(haha.gethwnd(), &pattern, D2D1::RectF(20, 20, 70, 70), L"wakaka");
		haha.registerobserver(&buttontest);
		haha.run();*/
		CaptionTest test(hInstance);
		return 0;
	}