
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
		cuirect getPosition() const
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
		void HandleHelper(cuievent ce)
		{
			auto it = EventHandler.find(ce.eventname);
			if (it != EventHandler.end())
			{
				for (auto vit : it->second)
					vit(&ce);
			}
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
	class cuibutton : public cibboncontrolbase//这是一个简单的button
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
	//绘制事件
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
		});//添加动画
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
		friend class TabPattern;
	public:
		
		cuiFixedTab(cuiwindowbase* pwindow, const std::wstring& _text, bool Enable = true);
		void addcontrols(cibboncontrolbase* pControl)
		{
			registerobserver(pControl);//使Control成为自己的观察者
			//注意！还要向窗口注册！
			pOwner->registerobserver(pControl);
		}
		//void registerhelper()
		void HandleNotify(cuievent ce) override
		{
			if (ce.eventname == controlinit) goto notify;

			bool x(ifinrect(ce.eventposition, EventPosition));
			if (ce.eventname == mousemove)
			{
				if (x == ifin) return;
				ce.eventname = (x ? mousemovein : mousemoveout);
				ifin = x;
			}
			else if (ce.eventname == mousemoveout)
				ifin = false;

			/*else if (!iffocus&&pceb->eventname!=lbuttondown)
			return;*/
		notify:
			HandleHelper(ce);
		

		}
	private:
		cuiwindowbase* pWindow;
		bool ifselected;
		cuirect EventPosition;
		void initevents() override;

	
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
		void HandleNotify(cuievent ce) override;
		float endx;
		bool iffirst;
		int SelectedTabindex;

	};

	template<typename T>
	cuiFixedTabs<T>::cuiFixedTabs(cuiwindowbase* pwindow, cuirect _Position, const std::wstring& _text, bool Enable)
		:cibboncontrolbase(new T(pwindow->gethwnd()),pwindow, _Position.movedown(Captionheight), _text, Enable),
		endx(_Position.left), iffirst(true), SelectedTabindex(0)
	{
		initevents();
	}

	template<typename T>
	void cuiFixedTabs<T>::AddTab(const std::initializer_list<cuiFixedTab*>& il)
	{
		for (auto tab : il)
		{
			registerobserver(tab);//每个Tab又是cuiFixedTab的观察者
			auto temp = endx;
			tab->setPosition(cuirect(
				temp,
				Position.top,
				endx += (tab->windowtext.size() *5+ 20),
				Position.top + 21
				));
			tab->setPattern(getPatternManager());
			endx += 1.f;
			//tab->EventPosition = tab->Position;
			if (iffirst)
			{
				cuievent ce;
				ce.eventname = tabselected;
				tab->HandleNotify(ce);
				iffirst = false;
			}
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
			notifyobservers(pe);
		});
		addevents(mousemoveout, [this](cuievent* pe)->void
		{
			notifyobservers(pe);
		});
		addevents(mousemove, [this](cuievent* pe)->void
		{
			notifyobservers(pe);
		});
	}//cuirect(Position.left,Position.top,endx,Position.top+21)
	template<typename T>
	void cuiFixedTabs<T>::HandleNotify(cuievent ce)
	{
		if (ce.eventname == controlinit) goto notify;
		bool x(ifinrect(ce.eventposition, cuirect(Position.left, Position.top, endx, Position.top + 21)));
		if (ce.eventname == mousemove)
		{
			if (!x&&!ifin) return;//如果原先没有在矩形中，现在依旧没有在，直接返回
			/*if (x&&ifin)
			{
				for (int i = 0; i < Observers.size(); i++)
				{
					if (i == SelectedTabindex) continue;
					Observers[i]->HandleNotify(ce);
				}
				Observers[SelectedTabindex]->HandleNotify(ce);
				return;
			}*/
			if (ifin && !x)//如果原先在，现在不在，说明鼠标依旧离开了Tabs区域，则发送mousemoveout消息
			{
				ce.eventname = mousemoveout;
				ifin = false;
				notifyobservers(&ce);//先通知鼠标离开
				ce.eventname = controlinit;
				notifyobservers(&ce);
				ifin = x;
				return;
			}
			ifin = x;
		}
		else if (ce.eventname == mousemoveout)//如果鼠标不在窗口中，发送mousemoveout消息
			ifin = false;
		else
			if (!x) return;
		//至此mousemove消息处理完毕
		//接着是lbuttondown消息的处理
		if (ce.eventname == lbuttondown)
		{
			if (ifinrect(ce.eventposition, static_cast<cuiFixedTab*>(Observers[SelectedTabindex])->EventPosition))//如果在现在选中的Tab内，返回
				return;
			for (int i = 0; i != Observers.size(); i++)
			{
				if (ifinrect(ce.eventposition, static_cast<cuiFixedTab*>(Observers[i])->getPosition()))//如果在该矩形内，说明该选项卡已被选中
					//现在说明选项卡的选项已经发生变化
				{
					ce.eventname = tabunselected;
					Observers[SelectedTabindex]->HandleNotify(ce);
					SelectedTabindex = i;
					ce.eventname = tabselected;
					Observers[SelectedTabindex]->HandleNotify(ce);
					//现在，我们跳出循环，在这之前，将选中Tab优先级设置为最低
					std::swap(Observers[SelectedTabindex], Observers[Observers.size()-1]);
					break;
				}
				/*std::swap(Observers[SelectedTabindex], Observers[2]);
				SelectedTabindex = 2;*/
			}
			ce.eventname = controlinit;
			notifyobservers(&ce);
			return;
		}
		
	notify:
		
		HandleHelper(ce);
	}

	//template<typename T>
	//void cuiFixedTabs<T>::HandleNotify(cuievent* pceb)//cuiFixedTabs是一个中转站
	//{

	//}


	class TabPattern : public PatternManagerBase
	{
	public:
		TabPattern(HWND hWnd);
		void drawusual(cibboncontrolbase* pControl);
		void drawmove(cibboncontrolbase* pControl);
		void drawdown(cibboncontrolbase* pControl);
		void drawselected(cuiFixedTab* pTab);
		void initdraw(cibboncontrolbase* pControl);
		void drawclear(cibboncontrolbase* pControl);
		void clearBack(cibboncontrolbase* pTab);
		virtual ~TabPattern() = default;


	};










	class cuistatic :public cibboncontrolbase
	{
		cuistatic(PatternManagerBase* pPatternManager, const cuirect& _Position, const std::wstring& _text, bool Enable = true);
	private:
		void initevents() override;
	};
}
