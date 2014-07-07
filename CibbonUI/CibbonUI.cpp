// CibbonUI.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "cibbonwindows.h"



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	cibbonui::cuistdwindow haha(hInstance, L"haha");

	//std::wstring leak(std::to_wstring(_CrtDumpMemoryLeaks()));
	//MessageBox(0, leak.c_str(), L"MemoryLeaks", 0);

	return 0;
}

