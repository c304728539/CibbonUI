
#pragma once
#include "cuirendermanager.h"

namespace cibbonui{

#define WINPAR HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam
	class glowwindow;
	class cuiTooltip;
	class cibboncontrolbase;
	class PatternManagerBase;
	class SizeBoxPattern;
	class glowwindow;

	class observer
	{
	public:
		observer() = default;
		virtual ~observer() = default;
		virtual void HandleNotify(cuievent) = 0;//Core
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
			for_each(Observers.begin(), Observers.end(), [pce](observer* po)->void { po->HandleNotify(*pce); });
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
		void setPosition(RECT _rect, UINT uFlags = SWP_SHOWWINDOW)//ȫ�־���
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
		//����ʵ��Ӧ������addevents(WM_SIZE,_Func)
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
		cibboncontrolbase(PatternManagerBase* pPatternManager, cuiwindowbase* pOwner,const cuirect& _Position, const std::wstring& _text, bool Enable = true);
		virtual ~cibboncontrolbase();
		void HandleNotify(cuievent) override;

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
		const std::shared_ptr<PatternManagerBase>& getPatternManager() const
		{
			return pPatternManager;
		}

		void RegisterTooltip(const std::wstring& _text)
		{
			cuiTooltip x(rPosition, _text);
			
		}

		
	private:
		
		cuiTooltip* pTooltip;
		
		std::wstring Tooltiptext;
	protected:
		bool ifin;
		std::shared_ptr<PatternManagerBase> pPatternManager;
		virtual void initevents() = 0;
		cuirect Position;
		RECT rPosition;
		bool iffocus;
		bool ifenabled;
		std::wstring windowtext;
		cuiwindowbase* pOwner;
		std::map <cint, std::vector<std::function<void(cuievent* pe)>>> EventHandler;
		void addevents(cuieventenum cee, const std::function<void(cuievent* pe)>& func)
		{
			EventHandler[cee].push_back(func);
		}
		void setpOwner(cuiwindowbase* _pw)
		{
			pOwner = _pw;
		}
		void setPattern(const std::shared_ptr<PatternManagerBase>& p)
		{
			pPatternManager = p;
		}
	};

	/******************************************************************
	*                                                                 *
	*                                                                 *
	*                   Button                                        *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/

	template<typename T>
	class cuibutton : public cibboncontrolbase//����һ���򵥵�button
	{
	public:
		cuibutton(cuiwindowbase* pwindow, cuirect _Position, const std::wstring& _text, bool Enable = true);
		~cuibutton() = default;
		void Onclick(const std::function<void(cuievent* pe)>& func);
	private:
		void initevents() override;

	};

	template<typename T>
	cuibutton<T>::cuibutton(cuiwindowbase* pwindow,cuirect _Position, const std::wstring& _text, bool Enable)
		:cibboncontrolbase(new T(pwindow->gethwnd()),pwindow, _Position, _text, Enable)
	{
		initevents();
	}
	//�����¼�
	template<typename T>
	void cuibutton<T>::initevents()
	{
		addevents(controlinit, [this](cuievent* pe)->void{
			std::static_pointer_cast<ButtonPattern>(pPatternManager)->initdraw(this);
		});
		addevents(lbuttondown, [this](cuievent* pe)->void{
			std::static_pointer_cast<ButtonPattern>(pPatternManager)->drawdown(this);
		});
		addevents(lbuttonup, [this](cuievent* pe)->void{
			std::static_pointer_cast<ButtonPattern>(pPatternManager)->drawup(this);
		});
		addevents(mousemovein, [this](cuievent* pe)->void{
			std::static_pointer_cast<ButtonPattern>(pPatternManager)->drawmove(this);
		});
		addevents(mousemoveout, [this](cuievent* pe)->void{
			std::static_pointer_cast<ButtonPattern>(pPatternManager)->drawusual(this);
		});//��Ӷ���
	}
	template<typename T>
	void cuibutton<T>::Onclick(const std::function<void(cuievent* pe)>& func)
	{
		return addevents(lbuttonup, func);
	}



	class ButtonPattern :public PatternManagerBase
	{
	public:
		ButtonPattern(HWND hWnd);
		virtual void drawusual(cibboncontrolbase* pControl) override;
		virtual void drawmove(cibboncontrolbase* pControl) override;
		virtual void drawdown(cibboncontrolbase* pControl) override;
		virtual void drawup(cibboncontrolbase* pControl);
		virtual void initdraw(cibboncontrolbase* pControl) override;

		virtual ~ButtonPattern() = default;
	protected:
		virtual void drawhelper(cibboncontrolbase* pControl, cint Color);
	};

	class SizeBoxPattern :public ButtonPattern
	{
	public:
		SizeBoxPattern(HWND hWnd);
	protected:
		virtual void drawhelper(cibboncontrolbase* pControl, cint Color);
	};

	/******************************************************************
	*                                                                 *
	*                                                                 *
	*                  cuiwindow                                      *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/



	class  cuistdwindow : public cuiwindowbase
	{
	public:
		cuistdwindow() = default;
		cuistdwindow(HINSTANCE _hInst, std::wstring _title, cdword _windowstyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP, cdword dwExStyle = 0, cint _width = 640, cint _height = 480);
		~cuistdwindow();
		void run();
		void runmessageloop();
	protected:
		void initevents() override;
		void addhelper(UINT Message, cuieventenum cuienum, bool bif);
	private:
		bool iftrack;
		cuibutton<SizeBoxPattern> closebutton;
		cuibutton<SizeBoxPattern> minibutton;
		glowwindow gw;
	};


	/******************************************************************
	*                                                                 *
	*                                                                 *
	*                  Fixed Tabs                                     *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/
	template<typename T> class cuiFixedTabs;
	class cuiFixedTab :public cibboncontrolbase
	{
		template<typename T> friend class cuiFixedTabs;
	public:
		
		cuiFixedTab(cuiwindowbase* pwindow, const std::wstring& _text, bool Enable = true);
		void addcontrols(cibboncontrolbase* pControl)
		{
			registerobserver(pControl);//ʹControl��Ϊ�Լ��Ĺ۲���
			//ע�⣡��Ҫ�򴰿�ע�ᣡ
			pOwner->registerobserver(pControl);
		}
		//void registerhelper()
		/*void HandleNotify(cuievent* pe) override
		{
			bool x(ifinrect(pe->eventposition, Position));
			if (pe->eventname == lbuttondown)
			{
				
				if (x == ifselected) return;
				else ifselected = x;
			}
			if (pe->eventname == mousemove)
			{
				if (x == ifin) return;
				pe->eventname = x ? mousemovein : mousemoveout;
				ifin = x;
			}
			else if (pe->eventname == mousemoveout)
				ifin = false;
			else
				if (!x) return;
			auto it = EventHandler.find(pe->eventname);
			if (it != EventHandler.end())
			{
				for (auto vit : it->second)
					vit(pe);
			}
		}*/
	private:
		cuiwindowbase* pWindow;
		bool ifselected;
		void initevents() override
		{
			addevents(controlinit, [this](cuievent* pe)->void
			{
				pPatternManager->initdraw(this);
			});
			addevents(lbuttondown, [this](cuievent* pe)->void
			{
				pPatternManager->drawdown(this);
			});
			addevents(mousemovein, [this](cuievent* pe)->void
			{
				pPatternManager->drawmove(this);
			});
		}
	};

	



	template<typename T>
	class cuiFixedTabs : public cibboncontrolbase
	{
	public:

		cuiFixedTabs(cuiwindowbase* pwindow, cuirect _Position, const std::wstring& _text, bool Enable = true);
		~cuiFixedTabs() = default;
		void AddTab(const std::initializer_list<cuiFixedTab*>& il);
	private:
		void initevents();
		float endx;

	};

	template<typename T>
	cuiFixedTabs<T>::cuiFixedTabs(cuiwindowbase* pwindow, cuirect _Position, const std::wstring& _text, bool Enable)
		:cibboncontrolbase(new T(pwindow->gethwnd()),pwindow, _Position, _text, Enable),
		endx(_Position.left+0.5f)
	{
		initevents();
	}

	template<typename T>
	void cuiFixedTabs<T>::AddTab(const std::initializer_list<cuiFixedTab*>& il)
	{
		for (auto tab : il)
		{
			registerobserver(tab);//ÿ��Tab����cuiFixedTab�Ĺ۲���
			auto temp = endx;
			tab->setPosition(cuirect(
				temp,
				Position.top,
				endx += (tab->windowtext.size() *5+ 10),
				Position.top + 21
				));
			tab->setPattern(getPatternManager());
			endx += 1.5f;
			//pOwner->registerobserver(tab);
		}
	}
	template<typename T>
	void cuiFixedTabs<T>::initevents()
	{
		addevents(controlinit, [this](cuievent* pe)->void
		{
			static_pointer_cast<TabPattern>(pPatternManager)->drawclear(this);
			notifyobservers(pe);
		});
		addevents(lbuttondown, [this](cuievent* pe)->void
		{
			notifyobservers(pe);
		});
		addevents(mousemovein, [this](cuievent* pe)->void
		{
			pe->eventname = mousemove;
			notifyobservers(pe);
		});
	}



	//template<typename T>
	//void cuiFixedTabs<T>::HandleNotify(cuievent* pceb)//cuiFixedTabs��һ����תվ
	//{

	//}


	class TabPattern : public PatternManagerBase
	{
	public:
		TabPattern(HWND hWnd);
		void drawusual(cibboncontrolbase* pControl);
		void drawmove(cibboncontrolbase* pControl);
		void drawdown(cibboncontrolbase* pControl);
		void initdraw(cibboncontrolbase* pControl);
		void drawclear(cibboncontrolbase* pControl);
		virtual ~TabPattern() = default;


	};










	class cuistatic :public cibboncontrolbase
	{
		cuistatic(PatternManagerBase* pPatternManager, const cuirect& _Position, const std::wstring& _text, bool Enable = true);
	private:
		void initevents() override;
	};
}
