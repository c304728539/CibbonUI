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
		virtual ~Observer();

	private:

	};

	class Subject
	{
	public:
		void RegisterObserver(Observer* o);
		void RemoveObserver(Observer* o);
		void NotifyObservers(EventArgs* pe);
	private:
		std::vector<Observer*> Observerlist;
	};


	//Strategy Pattern 
	//this class is used to register a window class
	class WndClass
	{
	public:
		WndClass(
			std::wstring _name,
			bool shadow,
			WNDPROC WndProc,
			HINSTANCE hInst
			);
	private:
		ATOM atom;
		WNDCLASSEX WndClassEx;
	};

	//MessageHandler is a Subject

	class CUIWindowBase
	{
	public:
	private:
		WndClass* pWndClass;
	};




}