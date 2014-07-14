#include "stdafx.h"
namespace cibbonui
{
	using cint = __int32;
	using cdword = DWORD;
	using CPointf = D2D1_POINT_2F;
	using CRect = D2D1_RECT_F;
	const bool already = true;
	const bool notyet = false;

	const int Captionheight = 26;
	const int closebuttonwidth = 34;
	const int glowoffset = 2;
	const int shadowsharpness = 5;
	const int shadowdarkness = 150;

	const int defaultbackgroundcolor = 0xeeeef2;
	const int defaultcontentcolor = 0x1e1e1e;
	const int defaultdisabledcontentcolor = 0xc3c3c6;
	const int defaultmoveinbackgroundcolor = 0xfcfcfc;
	const int defaultmoveincontentcolor = 0x007bcb;
	const int defaultclickbackgroundcolor = 0x007acd;
	const int defaultclickcontentcolor = 0xfffdfd;
	//using eeeef2181818181818eeedf21c1c1c
	inline cdword PreMultiply(COLORREF cl, unsigned char nAlpha)
	{
		// It's strange that the byte order of RGB in 32b BMP is reverse to in COLORREF
		return (GetRValue(cl) * (cdword)nAlpha / 255) << 16 |
			(GetGValue(cl) * (cdword)nAlpha / 255) << 8 |
			(GetBValue(cl) * (cdword)nAlpha / 255);
	}
	enum cstyle
	{
		daystyle,
		nightstyle
		//...
	};


	enum cuieventenum
	{
		//待添加的事件
		controlinit,
		mousemove,
		mousemovein,
		mousemoveout,
		lbuttondown,
		lbuttonup,
		rbuttondown,
		rbuttonup,
		lbuttondoubleclick,
		rightbuttonclick

	};

	const auto Alignmentcenter = DWRITE_TEXT_ALIGNMENT_CENTER;
	const auto Alignmentleft = DWRITE_TEXT_ALIGNMENT_LEADING;
	const auto Alignmentright = DWRITE_TEXT_ALIGNMENT_TRAILING;

	//struct cuievent
	//{
	//	//事件的必要信息
	//	HWND hWnd;
	//	cuieventenum Event;
	//	WPARAM wParam;
	//	LPARAM lParam;
	//};

	struct cuievent
	{
		cuieventenum eventname;
		POINT eventposition;
		//cuievent(CPointf point)
		virtual ~cuievent() = default;
	};
	struct cuibuttonevent : public cuievent
	{
		
		
		//待添加？
	};
	enum layoutenum{
		rightAlignment,//以最右控件对齐,为标题栏三个按钮设计
		leftAlignmant,
		averageAlignment,
		oneandtwoAlignment
		//... so on , in the future...
	};

	using win32 =  std::tuple<HWND, UINT, WPARAM, LPARAM>;


}