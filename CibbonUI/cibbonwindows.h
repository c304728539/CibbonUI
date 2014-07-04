
#pragma once
#include "uibase.h"

namespace cibbonui{

	class cibbonevent;
	class controllermanager;
	class cibboncontrolbase;
	class observer;

	class subject
	{
		//提供接口
	public:
		virtual void registerobserver(observer*) = 0;
		virtual void removeobserver() = 0;
		virtual void notifyobservers(cuievent*) = 0;
	};

	class observer
	{
	public:
		observer();
		~observer();
		virtual void update() = 0;

	private:

	};

	class cuiwindowbase
	{
		
	public:
		using winfunc = std::function<LRESULT CALLBACK(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)>;
		cuiwindowbase();
		cuiwindowbase(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle = WS_OVERLAPPEDWINDOW, cint _width = 640, cint _height = 480, cstyle _style = cstyle::daystyle);
		virtual ~cuiwindowbase();
		void run();

	protected:
		virtual void initevents() = 0;
		//具体实现应该形如addevents(WM_SIZE,_Func)
		/*virtual std::map<UINT, winfunc>* get()
		{
			return &windowmessage;
		}*/
		void addevents(UINT Message, winfunc Func);
		
		void init();
		void runmessageloop();
	private:
		
		static LRESULT CALLBACK WndProc
			(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		std::map <UINT, winfunc> windowmessage;
		HINSTANCE hInst;
		HWND m_hWnd;
		std::wstring title;
		cdword windowstyle;
		cint width;
		cint height;
		cstyle realstyle;
		std::shared_ptr<controllermanager> pmanager;
	};

	class  cuistdwindow : public cuiwindowbase
	{
		//using cuiwindowbase::winfunc;
	public:
		 cuistdwindow();
		 cuistdwindow(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle = WS_OVERLAPPEDWINDOW , cint _width = 640, cint _height = 480, cstyle _style = cstyle::daystyle);
		~cuistdwindow();
	protected:
		void initevents() override;
	private:

	};


	class cibbonevent
	{
		using eventcontainer = std::map<cint, std::vector<std::function<void(void*)>>>;
	public:
		cibbonevent() :pevents(new eventcontainer)
		{};
		void doevent();
		~cibbonevent(){};
		cibbonevent& operator+=(std::function<void(void*)>);
	private:
		std::shared_ptr<eventcontainer> pevents;//每个事件的队列
	};

	class controllermanager : public subject
	{
	public:
		controllermanager();
		~controllermanager();
		LRESULT handlemessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);//负责分发消息，真正意义上的观察者
		void registerobserver(observer*) override;
		void removeobserver() override;
		void notifyobservers(cuievent* pcuievent) override;
	private:
		std::vector<observer*> observers;
		

	};

	class cibboncontrolbase :public observer
	{
	public:
		cibboncontrolbase();
		~cibboncontrolbase();
		void update() override;
	private:

	};

}
