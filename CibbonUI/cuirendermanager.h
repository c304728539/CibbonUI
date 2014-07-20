#pragma once

#include "uibase.h"

namespace cibbonui{

	template<typename P, typename R>
	inline bool ifinrect(P Point, R Rect)
	{
		return Point.x > Rect.left && Point.x < Rect.right && Point.y > Rect.top && Point.y < Rect.bottom;
	}
	struct cuirect
	{
		float left;
		float top;
		float right;
		float bottom;
		operator D2D1_RECT_F() const
		{
			return D2D1::RectF(left, top, right, bottom);
		}
		operator D2D1_RECT_U() const
		{
			return D2D1::RectU(static_cast<UINT32>(left), static_cast<UINT32>(top), static_cast<UINT32>(right), static_cast<UINT32>(bottom));
		}
		operator RECT() const
		{
			RECT rc = { static_cast<LONG>(left), static_cast<LONG>(top), static_cast<LONG>(right), static_cast<LONG>(bottom) };
			return rc;
		}
		cuirect(float x1, float y1, float x2, float y2) :left(x1), top(y1), right(x2), bottom(y2)
		{

		}
		cuirect() :left(0.f), top(0.f), right(0.f), bottom(0.f)
		{

		}
		cuirect(const cuirect& r) :left(r.left), top(r.top), right(r.right), bottom(r.bottom)
		{

		}cuirect(cuirect&& r) :left(r.left), top(r.top), right(r.right), bottom(r.bottom)
		{

		}
		cuirect(const RECT& r) :left(static_cast<float>(r.left)), top(static_cast<float>(r.top)), right(static_cast<float>(r.right)), bottom(static_cast<float>(r.bottom))
		{

		}
		
		cuirect operator / (int rat)
		{
			return operator/(static_cast<float>(rat));
		}
		cuirect operator /(float rat)
		{
			return cuirect(left / rat, top / rat, right / rat, bottom / rat);
		}
		friend bool operator ==(const cuirect& r1, const cuirect r2)
		{
			return r1.left == r2.left && r1.top == r2.top && r1.right == r2.right && r1.bottom == r2.bottom;
		}

		friend bool operator !=(const cuirect& r1, const cuirect& r2)
		{
			return !(r1 == r2);
		}
		

	};



	class FactoryManager
	{
	public:
		static const std::unique_ptr<ID2D1Factory, std::function<void(ID2D1Factory*)>>& getpD2DFactory();
		static const std::unique_ptr<IDWriteFactory, std::function<void(IDWriteFactory*)>>& getpDwFactory();

	private:
		static std::unique_ptr<ID2D1Factory, std::function<void(ID2D1Factory*)>> pD2DFactory;
		static std::unique_ptr<IDWriteFactory, std::function<void(IDWriteFactory*)>> pDwFactory;
	};

	
	
	class cuirendermanager
	{
	public:
		
		static std::shared_ptr<cuirendermanager> getManager(HWND hWnd);
		~cuirendermanager();

		
		void clearall(cint color = D2D1::ColorF::White);
		void resize(cint x, cint y);
		void begindraw();
		void enddraw();
		void drawrect(const cuirect& rect,float linewidth = 1.0f, int color = D2D1::ColorF::Black);
		void drawline(const CPointf& ltop, const CPointf& rbottom, float linewidth = 1.0f, int color = D2D1::ColorF::Black);
		void FillRect(const cuirect& rect, int color = D2D1::ColorF::White);
		void FillRect(const cuirect& rect, cint color1, cint color2);
		void drawtext(std::wstring text, float fontsize, const cuirect& _rect, std::wstring fontname = L"Microsoft YaHei", DWRITE_TEXT_ALIGNMENT Alig = Alignmentcenter, cint color = D2D1::ColorF::Black);
		void setxytransform(cint xoffset, cint yoffset);
	private:

		cuirendermanager() = default;
		cuirendermanager(HWND hWnd);
		static std::map<HWND ,std::shared_ptr<cuirendermanager>> RenderManagers;
		HWND m_hWnd;
		bool ifbegin;
		cuirect windowrect;
		std::map<int, ID2D1SolidColorBrush*> brushmap;

		unsigned int beginnum;
		float DPIX;
		float DPIY;

		ID2D1HwndRenderTarget* pRT;
		
		void CreateDeviceResources();
		ID2D1SolidColorBrush* getBrush(int color);
		ID2D1LinearGradientBrush* getBrush(cint color1, cint color2,cuirect _rect);
		IDWriteTextFormat* getFormat(float fontsize, std::wstring fontname = L"Microsoft YaHei");
	};


	class PatternManagerBase
	{
		friend class cibboncontrolbase;
	public:
		PatternManagerBase(HWND hWnd);
		virtual void drawusual(cibboncontrolbase* pControl) = 0;
		virtual void drawdown(cibboncontrolbase* pControl) = 0;
		virtual void drawmove(cibboncontrolbase* pControl) = 0;
		virtual void initdraw(cibboncontrolbase* pControl) = 0;
		virtual ~PatternManagerBase() = default;
		
		std::shared_ptr<cuirendermanager> pRendermanager;
	protected:
		void drawtexthelper(cibboncontrolbase* pControl, DWRITE_TEXT_ALIGNMENT Alig, cint Color = D2D1::ColorF::Black,std::wstring fontname = L"Microsoft YaHei");
	
	};


	template<layoutenum L>
	class LayoutManager
	{
	public:
		LayoutManager();
		LayoutManager(const std::initializer_list<cibboncontrolbase*>& il);
		~LayoutManager();

	private:
		std::vector<cibboncontrolbase*> controlvt;
	};
	/*程序架构的简单分析：
	
	  1.可以创建一个App类，将控件与Window放入其中。制定控件的位置；
	  2.将控件全部放入Layout中。Layout负责计算控件的位置。

	  控件间交互 目前来看，只能是使用观察者机制。
	
	
	*/

}