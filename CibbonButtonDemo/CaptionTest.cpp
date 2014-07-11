#include "CaptionTest.h"

using namespace cibbonui;
CaptionTest::CaptionTest(HINSTANCE hInst) :MainWindow(hInst, L"Demo"),CaptionPattern(MainWindow.gethwnd()),
closePattern(MainWindow.gethwnd()),  
Caption(&CaptionPattern,D2D1::RectF(0,0,MainWindow.getPosition().right,40),L"Demo"),
closebutton(&closePattern, D2D1::RectF( MainWindow.getPosition().right-20 , 0, MainWindow.getPosition().right, 20), L"Demo")
{
	MainWindow.registerobserver(&closebutton);
	MainWindow.registerobserver(&Caption);
	MainWindow.run();
}


CaptionTest::~CaptionTest()
{
}

