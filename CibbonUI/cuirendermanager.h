#pragma once

#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodec.h>
#include <dwrite.h>
namespace cibbonui{
	class cuirendermanager
	{

	public:
		
		static std::shared_ptr<cuirendermanager> getManager(HWND hWnd);
		~cuirendermanager();

		void begindraw();
		void enddraw();
		void drawrect(const CPointf& ltop, const CPointf& rbottom, float linewidth = 1.0f, int color = D2D1::ColorF::Black);
		void drawline(const CPointf& ltop, const CPointf& rbottom, float linewidth = 1.0f, int color = D2D1::ColorF::Black);
		void FillRect(const D2D1_RECT_F& rect, int color = D2D1::ColorF::White);
		void drawtext(wstring text, cint fontsize, const CRect& _rect, decltype(Alignmentcenter) Alig = Alignmentcenter, cint color = D2D1::ColorF::Black);
	private:

		cuirendermanager() = default;
		cuirendermanager(HWND hWnd);
		static std::shared_ptr<cuirendermanager> pManager;
		HWND m_hWnd;
		bool ifbegin;
		CRect windowrect;
		std::map<int, ID2D1SolidColorBrush*> brushmap;

		ID2D1Factory* pD2DFactory;
		ID2D1HwndRenderTarget* pRT;
		IDWriteFactory* pDwFactory;
		
		
		void CreateDeviceIndependentResources();
		void CreateDeviceResources();
		ID2D1SolidColorBrush* getBrush(int color);
		IDWriteTextFormat* getFormat(int fontsize, wstring fontname = L"Microsoft YaHei");
	};


	class LayoutManagerBase
	{
	protected:
		LayoutManagerBase(HWND hWnd);
		virtual void drawusual() = 0;
		virtual void drawfocus() = 0;
		virtual void drawmove() = 0;
		virtual ~LayoutManagerBase() = default;
	private:
		std::shared_ptr<cuirendermanager> pRendermanager;
	};

}