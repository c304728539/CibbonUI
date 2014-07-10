
#pragma once

#include "..\\CibbonUI\\cibbonwindows.h"

class CaptionTest
{

public:
	CaptionTest(HINSTANCE hInst);
	~CaptionTest();

private:
	
	cibbonui::cuistdwindow MainWindow;
	cibbonui::ButtonPattern CaptionPattern;
	cibbonui::ButtonPattern closePattern;
	cibbonui::cuibutton Caption;
	cibbonui::cuibutton closebutton;
	
};

