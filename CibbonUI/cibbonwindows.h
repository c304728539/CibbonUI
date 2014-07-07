
#pragma once
#include "cuirendermanager.h"

namespace cibbonui{

	class cibbonevent;
	class controllermanager;
	class cibboncontrolbase;
	class observer;
	class observer
	{
	public:
		observer();
		~observer();
		virtual void HandleNotify(cuieventbase*) = 0;//Core

	private:

	};
	class subject
	{
		//提供接口
	public:
		virtual void registerobserver(observer* po)
		{
			Observers.push_back(po);
		}
		virtual void removeobserver()
		{
			if (!Observers.empty()) Observers.pop_back();
		}
		virtual void notifyobservers(cuieventbase* pce)
		{
			for_each(Observers.begin(), Observers.end(), [pce](observer* po)->void { po->HandleNotify(pce); });
		}
		virtual ~subject() = default;
	protected:
		std::vector<observer*> Observers;
	};

	

	class cuiwindowbase : public subject
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
		
		virtual void init();
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
		//std::shared_ptr<controllermanager> pmanager;
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
		//cibbonevent() :pevents(new eventcontainer)
		//{};
		//void doevent();
		~cibbonevent(){};
		//cibbonevent& operator+=(std::function<void(void*)>);
	private:
		//std::shared_ptr<eventcontainer> pevents;//每个事件的队列
	};

	//class controllermanager : public subject
	//{
	//public:
	//	controllermanager();
	//	~controllermanager();
	//	LRESULT handlemessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);//负责分发消息，真正意义上的观察者
	//	void registerobserver(observer*) override;
	//	void removeobserver() override;
	//	void notifyobservers(cuieventbase* pcuievent) override;
	//private:
	//	std::vector<observer*> observers;
	//	

	//};

	class cibboncontrolbase :public observer,public subject
	{
	public:
		cibboncontrolbase();
		cibboncontrolbase(PatternManagerBase* pPatternManager, const CRect& _Position, const std::wstring& _text, bool Enable = true);
		virtual ~cibboncontrolbase() = default;
		//void update(cuieventbase*) override;
		/*void registerobserver(observer*) override ;
		void removeobserver() override;
		void notifyobservers(cuieventbase*) override;*/
		void HandleNotify(cuieventbase*) override;
		virtual void initevents() = 0;
		void setwindowtext(const std::wstring& text)
		{
			windowtext = text;
		}
		const std::wstring& getwindowtext() const
		{
			return windowtext;
		}
		void setPosition(const CRect& _pos)
		{
			Position = _pos;
		}
		const CRect& getPosition() const
		{
			return Position;
		}
	private:
		bool iffocus;
		bool ifenabled;
		CRect Position;
		std::wstring windowtext;
		std::map <cint,std::vector<std::function<void(cuieventbase*)>>> EventHandler;
	protected:
		
		std::shared_ptr<PatternManagerBase> pPatternManager;
		void addevents(cuieventenum cee, const std::function<void(cuieventbase*)>& func)
		{
			EventHandler[cee].push_back(func);
		}
	};

	class cuibutton:public cibboncontrolbase//这是一个简单的button
	{
	public:
		//friend class ButtonPattern;
		cuibutton(HWND hWnd, PatternManagerBase* pPatternManager, const CRect& _Position, const std::wstring& _text, bool Enable = true);
		~cuibutton();
	private:
		void initevents() override;
	};

	/*class mininumbutton : public cuibutton
	{

	};*/

}
