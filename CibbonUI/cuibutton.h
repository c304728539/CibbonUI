#pragma once

#include "cibbonwindowbase.h"

namespace cibbonui{
	template<typename T>
	class cuibutton : public cibboncontrolbase//这是一个简单的button
	{
	public:
		cuibutton(HWND hWnd, const CRect& _Position, const std::wstring& _text, bool Enable = true);
		~cuibutton() = default;
		void Onclick(const std::function<void(cuievent* pe)>& func);
	private:
		void initevents() override;

	};

	template<typename T>
	cuibutton<T>::cuibutton(HWND hWnd, const CRect& _Position, const std::wstring& _text, bool Enable)
		:cibboncontrolbase(new T(hWnd), _Position, _text, Enable)
	{
		initevents();
	}
	//绘制事件
	template<typename T>
	void cuibutton<T>::initevents()
	{
		addevents(controlinit, [this](cuievent* pe)->void{
			static_cast<ButtonPattern*>(pPatternManager)->initdraw(this);
		});
		addevents(lbuttondown, [this](cuievent* pe)->void{
			static_cast<ButtonPattern*>(pPatternManager)->drawdown(this);
		});
		/*auto Func = static_cast<unsigned int(__stdcall*)(void*)>([](void* p)-> unsigned int{
			auto x = static_cast<cuibutton*>(p);
			static_cast<ButtonPattern*>(x->getPatternManager())->drawup(x);
			return 0;
		});*/
		addevents(lbuttonup, [this](cuievent* pe)->void{
			static_cast<ButtonPattern*>(pPatternManager)->drawup(this);  });//添加动画
			//Func = static_cast<unsigned int(__stdcall*)(void*)>([](void* p)-> unsigned int{
			//	auto x = static_cast<cuibutton*>(p);
			//	static_cast<ButtonPattern*>(x->getPatternManager())->drawmove(x);
			//	return 0;
			//});
			//addevents(mousemovein, [this, Func]()->void{
			//	//cuirendermanager::getManager()->begindraw();
			//	_beginthreadex(0, 0,
			//		Func,
			//		this, 0, 0);  
			//	//cuirendermanager::getManager()->enddraw(); 
			//});
			addevents(mousemovein, [this](cuievent* pe)->void{
				static_cast<ButtonPattern*>(pPatternManager)->drawmove(this);
			});
			/*auto Func = static_cast<unsigned int(__stdcall*)(void*)>([](void* p)-> unsigned int{
				auto x = static_cast<cuibutton*>(p);
				static_cast<ButtonPattern*>(x->getPatternManager())->drawusual(x);
				return 0;
			});*/
			addevents(mousemoveout, [this](cuievent* pe)->void{
				static_cast<ButtonPattern*>(pPatternManager)->drawusual(this); });//添加动画
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
		virtual void drawfocus(cibboncontrolbase* pControl) override;
		virtual void drawmove(cibboncontrolbase* pControl) override;
		virtual void drawdown(cibboncontrolbase* pControl);
		virtual void drawup(cibboncontrolbase* pControl);
		virtual void initdraw(cibboncontrolbase* pControl) override;
		virtual ~ButtonPattern() = default;
	};

	class closebuttonPattern :public ButtonPattern
	{
	public:
		closebuttonPattern(HWND hWnd);
		virtual void drawusual(cibboncontrolbase* pControl) override;
		virtual void drawfocus(cibboncontrolbase* pControl) override;
		virtual void drawmove(cibboncontrolbase* pControl) override;
		virtual void drawdown(cibboncontrolbase* pControl);
		virtual void drawup(cibboncontrolbase* pControl);
		virtual void initdraw(cibboncontrolbase* pControl) override;
		virtual ~closebuttonPattern() = default;
	private:
		void drawhelper(cibboncontrolbase* pControl, cint Color);
	};
	class minimizebuttonPattern :public ButtonPattern
	{
	public:
		minimizebuttonPattern(HWND hWnd);
		virtual void drawusual(cibboncontrolbase* pControl) override;
		virtual void drawfocus(cibboncontrolbase* pControl) override;
		virtual void drawmove(cibboncontrolbase* pControl) override;
		virtual void drawdown(cibboncontrolbase* pControl);
		virtual void drawup(cibboncontrolbase* pControl);
		virtual void initdraw(cibboncontrolbase* pControl) override;
		virtual ~minimizebuttonPattern() = default;
	private:
		void drawhelper(cibboncontrolbase* pControl, cint Color);
	};


}