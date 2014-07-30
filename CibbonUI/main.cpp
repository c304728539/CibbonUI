#include "CUIBase.h"
#include <iostream>

int main()
{
	cibbonui::cuiRect rect{ 10.0f, 10.f, 20.f, 20.f };
	rect.getMargin(2.0, 3.0);
	std::cout << rect.left << " " << rect.right << " " << rect.top << " " << rect.bottom << std::endl;
	getchar();
	cibbonui::Color cl;
	cl.r = 255;
	cl.g = 255;
	cl.b = 255;
	cl.a = 255;
	std::cout << cl.color << std::endl;
	getchar();
	return 0;
}