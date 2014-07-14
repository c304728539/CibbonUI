
#pragma once
#include "cuirendermanager.h"

namespace cibbonui{

#define WINPAR HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam
	class glowwindow;
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
		friend class glowwindow;
	public:
		using winfunc = std::function<bool CALLBACK(WINPAR)>;
		cuiwindowbase();
		cuiwindowbase(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle, cdword dwExStyle, cint _width = 640, cint _height = 480, cstyle _style = cstyle::daystyle);
		virtual ~cuiwindowbase();
		void run();
		HWND gethwnd() const
		{
			return m_hWnd;
		}
		CRect getPosition() const
		{
			RECT rect;
			GetClientRect(m_hWnd, &rect);
			return D2D1::RectF(static_cast<float>(rect.left), static_cast<float>(rect.top), static_cast<float>(rect.right), static_cast<float>(rect.bottom));
		}
		void addevents(UINT Message, winfunc Func);
	protected:
		virtual void initevents() = 0;
		//具体实现应该形如addevents(WM_SIZE,_Func)
		virtual void init();
		void runmessageloop();
		std::wstring title;
	private:

		static LRESULT CALLBACK WndProc
			(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		std::map <UINT, std::vector<winfunc>> windowmessage;
		HINSTANCE hInst;
		HWND m_hWnd;
		
		cdword windowstyle;
		cdword extendstyle;
		cint width;
		cint height;
		cstyle realstyle;

	};

	class  cuistdwindow : public cuiwindowbase
	{
	public:
		cuistdwindow();
		cuistdwindow(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP, cdword dwExStyle = 0 , cint _width = 640, cint _height = 480, cstyle _style = cstyle::daystyle);
		~cuistdwindow();
	protected:
		void initevents() override;
		void addhelper(UINT Message, cuieventenum cuienum,bool bif);
	private:
		int framewidth;
		float ratio;

	};

	class glowwindow
	{
	public:
		enum ShadowStatus
		{
			shadowenabled = 1,	// Shadow is enabled, if not, the following one is always false
			shadowvisible = 1 << 1,	// Shadow window is visible
			parentvisible = 1 << 2,	// Parent window is visible, if not, the above one is always false
		};
		glowwindow(cuiwindowbase* _pOwner,cint size = 6);
		~glowwindow() = default;
	private:
		void update();
		void show();
		void init();
		void initevents();
		void MakeShadow(UINT32 *pShadBits, HWND hParent, RECT *rcParent);

		HWND thishwnd;
		bool ifupdate;
		cint m_size;
		cint status;
		cuiwindowbase* pOwner;
		LPARAM m_WndSize;
	};

	class cibboncontrolbase :public observer, public subject
	{
	public:
		cibboncontrolbase();
		cibboncontrolbase(PatternManagerBase* pPatternManager, const CRect& _Position, const std::wstring& _text, bool Enable = true);
		virtual ~cibboncontrolbase();
		void HandleNotify(cuievent*) override;

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
		std::map <cint, std::vector<std::function<void()>>> EventHandler;

	protected:
		bool ifin;
		PatternManagerBase* pPatternManager;
		virtual void initevents() = 0;
		void addevents(cuieventenum cee, const std::function<void()>& func)
		{
			EventHandler[cee].push_back(func);
		}
	};




	class cuistatic :public cibboncontrolbase
	{
		cuistatic(PatternManagerBase* pPatternManager, const CRect& _Position, const std::wstring& _text, bool Enable = true);
	private:
		void initevents() override;
	};
}
