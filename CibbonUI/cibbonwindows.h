
#pragma once
#include "cuirendermanager.h"

namespace cibbonui{

	class cibboncontrolbase;
	
class observer
	{
	public:
		observer() = default;
		virtual ~observer() = default;
		virtual void HandleNotify(cuievent*) = 0;//Core
    };
	class subject
	{
	public:
		virtual void registerobserver(observer* po)
		{
			Observers.push_back(po);
		}
		virtual void removeobserver()
		{
			if (!Observers.empty()) Observers.pop_back();
		}
		virtual void notifyobservers(cuievent* pce)
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
		HWND gethwnd() const
		{
			return m_hWnd;
		}
		CRect getPosition() const
		{
			RECT rect;
			GetClientRect(m_hWnd,&rect);
			return D2D1::RectF( rect.left, rect.top, rect.right, rect.bottom );
		}
		void addevents(UINT Message, winfunc Func);
	protected:
		virtual void initevents() = 0;
		//具体实现应该形如addevents(WM_SIZE,_Func)
		virtual void init();
		void runmessageloop();
	private:
		
		static LRESULT CALLBACK WndProc
			(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		std::map <UINT, std::vector<winfunc>> windowmessage;
		HINSTANCE hInst;
		HWND m_hWnd;
		std::wstring title;
		cdword windowstyle;
		cint width;
		cint height;
		cstyle realstyle;
		
	};

	class  cuistdwindow : public cuiwindowbase
	{
	public:
		 cuistdwindow();
		 cuistdwindow(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle = WS_OVERLAPPEDWINDOW , cint _width = 640, cint _height = 480, cstyle _style = cstyle::daystyle);
		~cuistdwindow();
	protected:
		void initevents() override;
		void addhelper(UINT Message,cuieventenum cuienum);
	private:
		
	};


	class cibboncontrolbase :public observer,public subject
	{
	public:
		cibboncontrolbase();
		cibboncontrolbase(PatternManagerBase* pPatternManager, const CRect& _Position, const std::wstring& _text, bool Enable = true);
		virtual ~cibboncontrolbase() = default;
		void HandleNotify(cuievent*) override;
		
		//virtual void InitialEvents() = 0;
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
		PatternManagerBase* getPatternManager() const
		{
			return pPatternManager;
		}
	private:
		bool iffocus;
		bool ifenabled;
		
		CRect Position;
		RECT rPosition;
		std::wstring windowtext;
		std::map <cint,std::vector<std::function<void()>>> EventHandler;

	protected:
		bool ifin;
		PatternManagerBase* pPatternManager;
		virtual void initevents() = 0;

		void addevents(cuieventenum cee, const std::function<void()>& func)
		{
			EventHandler[cee].push_back(func);
		}
	};

	class cuibutton : public cibboncontrolbase//这是一个简单的button
	{
	public:
		cuibutton(PatternManagerBase* pPatternManager, const CRect& _Position, const std::wstring& _text, bool Enable = true);
		~cuibutton() = default;
		void Onclick(const std::function<void()>& func);
	private:
		void initevents() override;
		
	};


	





	class cuistatic :public cibboncontrolbase
	{
		cuistatic(PatternManagerBase* pPatternManager, const CRect& _Position, const std::wstring& _text, bool Enable = true);
	private:
		void initevents() override;
	};
}
