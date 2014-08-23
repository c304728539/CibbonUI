#include "MHandler.h"

using namespace cibbonui;




int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPTSTR,
	_In_ int)
{
	CUIMainWindow* pMainWindow = new CUIMainWindow(new Handler,L"CibbonWindow");
	Canvas* pCanvas = new Canvas();
	Win8Button* pButton = new Win8Button(L"Win8Button");
	FlatButton* pFButton = new FlatButton(L"FlatButton");
	pButton->OnClick([=]()
	{
		auto hClipBoard = ::OpenClipboard(pMainWindow->GetWindowHandle());
		if (!hClipBoard) return;
		std::wstring Formation;
		auto FormatCounts = ::EnumClipboardFormats(0);
		wchar_t* wa(nullptr);
		while (FormatCounts)
		{
			switch (FormatCounts)
			{
			case CF_BITMAP:
			{
				Formation = L"CF_BITMAP";
				break;
			}
			case CF_DIB:
			{
				Formation = L"CF_DIB";
				break;
			}
			case CF_UNICODETEXT:
			{
				Formation = L"CF_UNICODETEXT";
				break;
			}
			case CF_ENHMETAFILE:
			{
				Formation = L"CF_ENHMETAFILE";
				break;
			}
			default:
				break;
			}
			//auto Formation = std::wstring(std::to_wstring(FormatCounts));
			//if (Form)
			::MessageBox(0, Formation.data(), L"Test", 0);
			FormatCounts = ::EnumClipboardFormats(FormatCounts);
		}
		auto Data = ::GetClipboardData(CF_UNICODETEXT);
		if (Data)
		{
			wa = (decltype(wa))GlobalLock(Data);
			if (wa)
			{
				Formation = wa;
				GlobalUnlock(Data);
			}
		}
		::MessageBox(0, Formation.data(), L"Test", 0);
		::CloseClipboard();
	});
	pCanvas->Add(pButton, cuiRect(120, 20, 200, 50));
	pCanvas->Add(pFButton, cuiRect(20, 20, 100, 50));
	pMainWindow->Add(pCanvas);
	pMainWindow->Run();
	delete pMainWindow;
	return 0;

}