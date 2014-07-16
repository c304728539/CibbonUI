#include "stdafx.h"
#include "cuibutton.h"

using namespace D2D1;
namespace cibbonui{


	ButtonPattern::ButtonPattern(HWND hWnd) : PatternManagerBase(hWnd)
	{

	}

	void ButtonPattern::drawhelper(cibboncontrolbase* pControl, cint Color)
	{
		return drawtexthelper(pControl, Alignmentcenter, Color);//正常的按钮 只有文字
	}

	void ButtonPattern::drawdown(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->FillRect(pControl->getPosition(), SkinManager::getStyleColor(clickbackgroundcolornum));
		drawhelper(pControl, SkinManager::getStyleColor(clickcontentcolornum));
		pRendermanager->enddraw();
	}
	void ButtonPattern::drawusual(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->FillRect(pControl->getPosition(), SkinManager::getStyleColor(backgroundcolornum));
		drawhelper(pControl,  SkinManager::getStyleColor(contentcolornum));
		pRendermanager->enddraw();
	}
	void ButtonPattern::drawfocus(cibboncontrolbase* pControl)
	{

	}
	void ButtonPattern::drawmove(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->FillRect(pControl->getPosition(), SkinManager::getStyleColor(moveinbackgroundcolornum));
		drawhelper(pControl,SkinManager::getStyleColor(moveincontentcolornum));
		pRendermanager->enddraw();
	}
	inline void ButtonPattern::drawup(cibboncontrolbase* pControl)
	{
		return drawusual(pControl);
	}

	void ButtonPattern::initdraw(cibboncontrolbase* pControl)
	{
		return drawusual(pControl);
	}

	SizeBoxPattern::SizeBoxPattern(HWND hWnd) :ButtonPattern(hWnd)
	{

	}

	void SizeBoxPattern::drawhelper(cibboncontrolbase* pControl, cint Color)
	{
		return drawtexthelper(pControl, Alignmentcenter, Color,L"Webdings");
	}
	//closebuttonPattern::closebuttonPattern(HWND hWnd) :
	//	ButtonPattern(hWnd)
	//{

	//}

	//

	//void closebuttonPattern::drawhelper(cibboncontrolbase* pControl,cint Color)
	//{
	//	auto x = pControl->getPosition();
	//	pRendermanager->drawline(Point2F(x.left + 13, 9), Point2F(x.left + 22, 17), 1.5f, Color);
	//	pRendermanager->drawline(Point2F(x.left + 13, 17), Point2F(x.left + 22, 9), 1.5f, Color);
	//}

	//void closebuttonPattern::drawmove(cibboncontrolbase* pControl)
	//{
	//	pRendermanager->begindraw();
	//	pRendermanager->FillRect(pControl->getPosition(), defaultmoveinbackgroundcolor);
	//	drawhelper(pControl,defaultmoveincontentcolor);
	//	pRendermanager->enddraw();
	//}

	//void closebuttonPattern::drawdown(cibboncontrolbase* pControl)
	//{
	//	pRendermanager->begindraw();
	//	pRendermanager->FillRect(pControl->getPosition(), defaultclickbackgroundcolor);
	//	drawhelper(pControl,defaultclickcontentcolor);
	//	pRendermanager->enddraw();
	//}
	//void closebuttonPattern::initdraw(cibboncontrolbase* pControl)
	//{
	//	return drawusual(pControl);
	//}
	//void closebuttonPattern::drawfocus(cibboncontrolbase* pControl)
	//{
	//	return;
	//}
	//void closebuttonPattern::drawup(cibboncontrolbase* pControl)
	//{
	//	return drawusual(pControl);
	//}


	//minimizebuttonPattern::minimizebuttonPattern(HWND hWnd) :
	//	ButtonPattern(hWnd)
	//{

	//}

	//void minimizebuttonPattern::drawusual(cibboncontrolbase* pControl)
	//{
	//	pRendermanager->begindraw();
	//	pRendermanager->FillRect(pControl->getPosition(), defaultcontentcolor);
	//	
	//	drawhelper(pControl, defaultbackgroundcolor);
	//	pRendermanager->enddraw();
	//}

	//void minimizebuttonPattern::drawhelper(cibboncontrolbase* pControl, cint Color)
	//{
	//	auto x = pControl->getPosition();
	//	wchar_t xy = 0x0030;
	//	std::wstring s{ xy };
	//	pRendermanager->drawtext(s, 15, pControl->getPosition(), L"Webdings",Alignmentcenter,defaultbackgroundcolor);
	//	//pRendermanager->drawline(Point2F(x.left + 13, 15), Point2F(x.left + 23, 15), 3.0f, Color);
	//	//pRendermanager->drawline(Point2F(x.left + 12, 17), Point2F(x.left + 22, 9), 2.0f, Color);
	//}

	//void minimizebuttonPattern::drawmove(cibboncontrolbase* pControl)
	//{
	//	pRendermanager->begindraw();
	//	pRendermanager->FillRect(pControl->getPosition(), defaultmoveinbackgroundcolor);
	//	drawhelper(pControl, defaultmoveincontentcolor);
	//	pRendermanager->enddraw();
	//}

	//void minimizebuttonPattern::drawdown(cibboncontrolbase* pControl)
	//{
	//	pRendermanager->begindraw();
	//	pRendermanager->FillRect(pControl->getPosition(), defaultclickbackgroundcolor);
	//	drawhelper(pControl, defaultclickcontentcolor);
	//	pRendermanager->enddraw();
	//}
	//void minimizebuttonPattern::initdraw(cibboncontrolbase* pControl)
	//{
	//	return drawusual(pControl);
	//}
	//void minimizebuttonPattern::drawfocus(cibboncontrolbase* pControl)
	//{
	//	return;
	//}
	//void minimizebuttonPattern::drawup(cibboncontrolbase* pControl)
	//{
	//	return drawusual(pControl);
	//}

}