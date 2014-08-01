#pragma once
#include "CUIBase.h"
#include <string>
#include <vector>

namespace cibbonui
{
	class Subject;
	struct EventArgs
	{
		Subject* Sender;
	};

	//There will be many EventArgs like MouseEventArgs...

	//It's Here..

	struct MouseEventArgs : public EventArgs
	{
		enum{
			LeftButtonDown,
			RightButtonDown,
			LeftButtonUp,
			RightButtonUp
		};
		cuiPoint EventPosition;
	};
	//Observer Pattern
	
	class Observer
	{
	public:
		virtual void LeftButtonDown(MouseEventArgs mea);
		virtual void LeftButtonUp(MouseEventArgs mea);
		virtual void RightButtonDown(MouseEventArgs mea);
		virtual void RightButtonUp(MouseEventArgs mea);
		virtual void Draw();
		virtual ~Observer();

	private:

	};

	class Subject
	{
	public:
		void RegisterObserver(Observer* o);
		void RemoveObserver(Observer* o);
		//void NotifyObservers(EventArgs* pe);
	protected:
		std::vector<Observer*> Observerlist;
	};

	class CUIWindowBase;
	//Strategy Pattern 
	//this class is used to register a window class
	class WndClass
	{
	public:
		WndClass(
			const std::wstring& _name,
			bool shadow,
			WNDPROC WndProc,
			HINSTANCE hInst
			);
		WndClass();
		std::wstring getClassName() const;
		void RegisterWindow();
	private:
		ATOM atom;
		WNDCLASSEX WndClassEx;
		std::wstring ClassName;
	};

	class WindowCreator
	{
	public:
		WindowCreator(const std::wstring& ClassName,const std::wstring& Title);
		void SetExStyle(DWORD _ExStyle);
		void SetWidth(cint _width);
		void SetHeight(cint _Height);
		void SetParentWindow(HWND hParent);
		void SetlpParam(LPVOID p);
		HWND Create();
	private:
		std::wstring WindowTitle;
		DWORD ExStyle;
		DWORD WindowStyle;
		cint Width;
		cint Height;
		HWND hParentWindow;
		LPVOID lpParam;
		std::wstring WindowClassName;
	};

	//MessageHandler is a Subject
	//A Mediator
	class MessageHandler : public Subject
	{
	public:
		const bool Already = true;
		const bool NotYet = false;
		virtual bool MessageHandlerWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam,LRESULT& lre) = 0;
		MessageHandler(CUIWindowBase* pWindow);
		virtual ~MessageHandler() = default;
	protected:
		CUIWindowBase* pOwnerWindow;
	};

	class MainWindowMessageHandler : public MessageHandler
	{
	public:
		MainWindowMessageHandler(CUIWindowBase* pOwnerWindow);
		bool MessageHandlerWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre) override;
	   
	private:
		LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam);
	
	};


	LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	class CUIWindowBase
	{
	public:
		 
		CUIWindowBase(const std::wstring& Title);
		virtual ~CUIWindowBase();
		void Initialize();
		virtual void Run() = 0;
		virtual void SetThings() = 0;
		bool HandleMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre);
		cuiRect GetWindowRect();
		/*virtual void RegisterWindow(std::wstring _name,
			bool shadow,
			WNDPROC WndProc,
			HINSTANCE hInst) = 0;
		virtual void CreateInit(const std::wstring& ClassName, std::wstring Title) = 0;*/
	protected:

		WndClass* pWndClass;
		WindowCreator* pWindowCreator;
		MessageHandler* pHandler;
		std::wstring WindowTitle;
		HWND WindowHandle;

		virtual void CreateInitors() = 0;
		
	};

	class CUIMainWindow :public CUIWindowBase
	{
	public:

		CUIMainWindow(const std::wstring& Title);
		void Run() override;
		void RunMessageLoop();
		
		void SetThings() override;
	protected:

		virtual void CreateInitors() override;
	};


}