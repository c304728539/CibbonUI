#include "stdafx.h"
#include "cuibutton.h"

using namespace D2D1;
namespace cibbonui{


	ButtonPattern::ButtonPattern(HWND hWnd) : PatternManagerBase(hWnd)
	{

	}
	void ButtonPattern::drawdown(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->FillRect(pControl->getPosition(), ColorF::SkyBlue);
		drawtexthelper(pControl, Alignmentcenter, ColorF::Black);
		pRendermanager->enddraw();
	}
	void ButtonPattern::drawusual(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->FillRect(pControl->getPosition(), ColorF::White);
		drawtexthelper(pControl, Alignmentcenter, ColorF::Black);
		pRendermanager->enddraw();
	}
	void ButtonPattern::drawfocus(cibboncontrolbase* pControl)
	{

	}
	void ButtonPattern::drawmove(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->FillRect(pControl->getPosition(), ColorF::LightBlue);
		drawtexthelper(pControl, Alignmentcenter, ColorF::Black);
		pRendermanager->enddraw();
	}
	inline void ButtonPattern::drawup(cibboncontrolbase* pControl)
	{
		//Sleep(2000);
		return drawusual(pControl);
	}

	void ButtonPattern::initdraw(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		//pRendermanager->clearall();
		pRendermanager->FillRect(pControl->getPosition(), ColorF::White);
		drawtexthelper(pControl, Alignmentcenter, ColorF::Black);
		pRendermanager->enddraw();
	}

	closebuttonPattern::closebuttonPattern(HWND hWnd) :
		ButtonPattern(hWnd)
	{

	}

	void closebuttonPattern::drawusual(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->FillRect(pControl->getPosition(), ColorF::White);
		drawhelper(pControl);
		pRendermanager->enddraw();
	}

	void closebuttonPattern::drawhelper(cibboncontrolbase* pControl)
	{
		auto x = pControl->getPosition();
		const float gama = 1.f / 3.f;     
		float sdistance = x.right - x.left;
		float vdistance = x.bottom - x.top;
		pRendermanager->drawline(Point2F(x.left + sdistance * gama, x.top + vdistance *gama), Point2F(x.left + sdistance *(1 - gama), x.top + vdistance*(1 - gama)));
		pRendermanager->drawline(Point2F(x.left + sdistance * gama, x.top + vdistance * (1 - gama)), Point2F(x.left + sdistance *(1 - gama), x.top + vdistance* gama));
	}

	void closebuttonPattern::drawmove(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->FillRect(pControl->getPosition(), ColorF::Gray);
		drawhelper(pControl);
		pRendermanager->enddraw();
	}

	void closebuttonPattern::drawdown(cibboncontrolbase* pControl)
	{
		pRendermanager->begindraw();
		pRendermanager->FillRect(pControl->getPosition(), ColorF::SkyBlue);
		drawhelper(pControl);
		pRendermanager->enddraw();
	}
	void closebuttonPattern::initdraw(cibboncontrolbase* pControl)
	{
		return drawusual(pControl);
	}
	void closebuttonPattern::drawfocus(cibboncontrolbase* pControl)
	{
		return;
	}
	void closebuttonPattern::drawup(cibboncontrolbase* pControl)
	{
		return drawusual(pControl);
	}

}