
#pragma once
#include "..\\CibbonUI\\CUIWindows.h"
#include <tchar.h>

class Handler : public cibbonui::MainWindowMessageHandler
{
public:
	Handler(cibbonui::CUIWindowBase* p)
		:MainWindowMessageHandler(p)
	{

	}
	Handler()
		:MainWindowMessageHandler()
	{

	}
	bool MessageHandlerWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre) override;
};

bool Handler::MessageHandlerWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre)
{
	switch (Message)
	{
	default:
		break;
	}
	return MainWindowMessageHandler::MessageHandlerWndProc(hWnd, Message, wParam,  lParam, lre);
}