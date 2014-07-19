
#pragma once
#include "cuirendermanager.h"

namespace cibbonui{

#define WINPAR HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam
	class glowwindow;
	class cuiTooltip;
	class cibboncontrolbase;
	class PatternManagerBase;
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
		using winfunc = std::function<bool CALLBACK(WINPAR)>;
		cuiwindowbase() = default;
		cuiwindowbase(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle, cint _width = 640, cint _height = 480, cdword dwExStyle = 0, std::wstring classname = L"cuistdwindow", HWND Parent = NULL);
		virtual ~cuiwindowbase() = default;

		HWND gethwnd() const
		{
			return m_hWnd;
		}
		cuirect getclientPosition() const
		{
			RECT rect;
			GetClientRect(m_hWnd, &rect);
			return cuirect(rect);
		}
		RECT getwindowPosition() const
		{
			RECT rect;
			GetWindowRect(m_hWnd, &rect);
			return rect;
		}
		void setPosition(RECT _rect, UINT uFlags = SWP_SHOWWINDOW)//全局矩形
		{
			SetWindowPos(m_hWnd, 0, _rect.left, _rect.top, _rect.right - _rect.left, _rect.bottom - _rect.top, uFlags);
		}
		HINSTANCE gethInst()const
		{
			return hInst;
		}

		void setWindowText(const std::wstring& _text)
		{
			title = _text;
		}
		void addevents(UINT Message, winfunc Func);
	protected:
		virtual void initevents() = 0;
		//具体实现应该形如addevents(WM_SIZE,_Func)
		void init();
		HWND m_hWnd;
		HWND ParenthWnd;
		std::wstring windowclassname;
		std::wstring title;
	private:

		static LRESULT CALLBACK WndProc
			(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		std::map <UINT, std::vector<winfunc>> windowmessage;
		HINSTANCE hInst;


		cdword windowstyle;
		cdword extendstyle;
		cint width;
		cint height;

	};

	class  cuistdwindow : public cuiwindowbase
	{
	public:
		cuistdwindow();
		cuistdwindow(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP, cdword dwExStyle = 0, cint _width = 640, cint _height = 480);
		~cuistdwindow();
		void run();
		void runmessageloop();
	protected:
		void initevents() override;
		void addhelper(UINT Message, cuieventenum cuienum, bool bif);
	private:
		bool iftrack;

	};

	class glowwindow : public cuiwindowbase
	{
	public:
		enum ShadowStatus
		{
			shadowenabled = 1,	// Shadow is enabled, if not, the following one is always false
			shadowvisible = 1 << 1,	// Shadow window is visible
			parentvisible = 1 << 2,	// Parent window is visible, if not, the above one is always false
		};
		glowwindow(cuiwindowbase* _pOwner, cint size = 3);
		~glowwindow() = default;
	private:
		void update();
		void show();
		void init();
		void initevents();
		void MakeShadow(UINT32 *pShadBits, HWND hParent, RECT *rcParent, cint Color = D2D1::ColorF::Black);

		HWND thishwnd;
		bool ifupdate;
		cint m_size;
		cint status;
		cuiwindowbase* pOwner;
		LPARAM m_WndSize;
	};

	class cuiTooltip 
	{
	public:
		cuiTooltip(RECT _rect, const std::wstring& text);
		static void setpOwner(cuiwindowbase* p)
		{
			cuiTooltip::pOwner = p;
		}
	private:
		static cuiwindowbase* pOwner;
		
	};

	class cibboncontrolbase :public observer, public subject
	{
	public:
		cibboncontrolbase();
		cibboncontrolbase(PatternManagerBase* pPatternManager, const cuirect& _Position, const std::wstring& _text, bool Enable = true);
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
		void setPosition(const cuirect& _pos)
		{
			Position = _pos;
		}
		const cuirect& getPosition() const
		{
			return Position;
		}
		PatternManagerBase* getPatternManager() const
		{
			return pPatternManager;
		}

		void RegisterTooltip(const std::wstring& _text)
		{
			cuiTooltip x(rPosition, _text);
			
		}
	private:
		bool iffocus;
		bool ifenabled;
		cuiTooltip* pTooltip;
		cuirect Position;
		RECT rPosition;
		std::wstring windowtext;
		std::map <cint, std::vector<std::function<void(cuievent* pe)>>> EventHandler;
		std::wstring Tooltiptext;
	protected:
		bool ifin;
		PatternManagerBase* pPatternManager;
		virtual void initevents() = 0;
		void addevents(cuieventenum cee, const std::function<void(cuievent* pe)>& func)
		{
			EventHandler[cee].push_back(func);
		}
	};




	class cuistatic :public cibboncontrolbase
	{
		cuistatic(PatternManagerBase* pPatternManager, const cuirect& _Position, const std::wstring& _text, bool Enable = true);
	private:
		void initevents() override;
	};
}
