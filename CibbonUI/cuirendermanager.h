#pragma once

#include "uibase.h"

namespace cibbonui{
	
	class cuirendermanager
	{
	public:
		
		static std::shared_ptr<cuirendermanager> getManager(HWND hWnd);
		~cuirendermanager();

		void clearall(cint color = D2D1::ColorF::White);
		void begindraw();
		void enddraw();
		void drawrect(const CRect& rect,float linewidth = 1.0f, int color = D2D1::ColorF::Black);
		void drawline(const CPointf& ltop, const CPointf& rbottom, float linewidth = 1.0f, int color = D2D1::ColorF::Black);
		void FillRect(const D2D1_RECT_F& rect, int color = D2D1::ColorF::White);
		void drawtext(std::wstring text, float fontsize, const CRect& _rect, DWRITE_TEXT_ALIGNMENT Alig = Alignmentcenter, cint color = D2D1::ColorF::Black);
	private:

		cuirendermanager() = default;
		cuirendermanager(HWND hWnd);
		static std::shared_ptr<cuirendermanager> pManager;
		HWND m_hWnd;
		bool ifbegin;
		CRect windowrect;
		std::map<int, ID2D1SolidColorBrush*> brushmap;
		unsigned int beginnum;
		CRITICAL_SECTION g_cs ;
		float DPIX;
		float DPIY;

		ID2D1Factory* pD2DFactory;
		ID2D1HwndRenderTarget* pRT;
		IDWriteFactory* pDwFactory;
		
		
		void CreateDeviceIndependentResources();
		void CreateDeviceResources();
		ID2D1SolidColorBrush* getBrush(int color);
		IDWriteTextFormat* getFormat(float fontsize, std::wstring fontname = L"Microsoft YaHei");
	};


	class PatternManagerBase
	{
		friend class cibboncontrolbase;
	public:
		PatternManagerBase(HWND hWnd);
		virtual void drawusual(cibboncontrolbase* pControl) = 0;
		virtual void drawfocus(cibboncontrolbase* pControl) = 0;
		virtual void drawmove(cibboncontrolbase* pControl) = 0;
		virtual void initdraw(cibboncontrolbase* pControl) = 0;
		virtual ~PatternManagerBase() = default;
		
		std::shared_ptr<cuirendermanager> pRendermanager;
	protected:
		void drawtexthelper(cibboncontrolbase* pControl, DWRITE_TEXT_ALIGNMENT Alig, cint Color = D2D1::ColorF::Black);
	
	};


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