#pragma once

#include "cibbonwindowbase.h"

namespace cibbonui{
	template<typename T>
	class cuibutton : public cibboncontrolbase//这是一个简单的button
	{
	public:
		cuibutton(HWND hWnd, const cuirect& _Position, const std::wstring& _text, bool Enable = true);
		~cuibutton() = default;
		void Onclick(const std::function<void(cuievent* pe)>& func);
	private:
		void initevents() override;

	};

	template<typename T>
	cuibutton<T>::cuibutton(HWND hWnd, const cuirect& _Position, const std::wstring& _text, bool Enable)
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
		addevents(lbuttonup, [this](cuievent* pe)->void{
			static_cast<ButtonPattern*>(pPatternManager)->drawup(this);  });
		addevents(mousemovein, [this](cuievent* pe)->void{
			static_cast<ButtonPattern*>(pPatternManager)->drawmove(this);
		});
		addevents(mousemoveout, [this](cuievent* pe)->void{
			static_cast<ButtonPattern*>(pPatternManager)->drawusual(this);
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
		virtual void drawfocus(cibboncontrolbase* pControl) override;
		virtual void drawmove(cibboncontrolbase* pControl) override;
		virtual void drawdown(cibboncontrolbase* pControl);
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


	/*class closebuttonPattern :public ButtonPattern
	{
	public:
		closebuttonPattern(HWND hWnd);
		virtual ~closebuttonPattern() = default;
	protected
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
*/

}