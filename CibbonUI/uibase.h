#pragma once

#include "stdafx.h"
namespace cibbonui
{
	using cint = __int32;
	using cdword = DWORD;
	using CPointf = D2D1_POINT_2F;
	using CRect = D2D1_RECT_F;
	const bool already = true;
	const bool notyet = false;

	const int maxstyles = 3;
	const int maxcolorinstyle = 20;

	const float Captionheight = 26.f;
	const float closebuttonwidth = 34.f;
	const int glowoffset = 0;
	const int glowsize = 3;
	const int shadowsharpness = 4;
	const int shadowdarkness = 230;

	const int Tooltiptime = 300;

	inline cdword PreMultiply(COLORREF cl, unsigned char nAlpha)
	{
		// It's strange that the byte order of RGB in 32b BMP is reverse to in COLORREF
		return (GetRValue(cl) * (cdword)nAlpha / 255) << 16 |
			(GetGValue(cl) * (cdword)nAlpha / 255) << 8 |
			(GetBValue(cl) * (cdword)nAlpha / 255);
	}
	enum cuiStyle
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
		mousehover,
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

	struct cuievent
	{
		cuieventenum eventname;
		CPointf eventposition;
		//cuievent(CPointf point)
		virtual ~cuievent() = default;
	};
	struct cuibuttonevent : public cuievent
	{


		//待添加？
	};

	enum daycolors
	{
		defaultbackgroundcolor = 0xeeeef2,
		defaultcontentcolor = 0x1e1e1e,
		defaultdisabledcontentcolor = 0xc3c3c6,
		defaultmoveinbackgroundcolor = 0xfcfcfc,
		defaultmoveincontentcolor = 0x007bcb,
		defaultclickbackgroundcolor = 0x007acd,
		defaultclickcontentcolor = 0xfffdfd,
		defaulttitlecolor = 0x526fa5,
		defaultshadowcolor = D2D1::ColorF::Black
	};

	enum nightcolors
	{
		nightbackgroundcolor = defaultcontentcolor,
		nightcontentcolor = defaultbackgroundcolor,
		nightdisabledcontentcolor = 0x4e4e50,
		nightmoveinbackgroundcolor = 0x3f3f41,
		nightmoveincontentcolor = 0xffffff,
		nightclickbackgroundcolor = 0x007acc,
		nightclickcontentcolor = nightmoveincontentcolor,
		nighttitlecolor = 0x99999b,
		nightshadowcolor = 0xd07c00
	};

	enum layoutenum{
		rightAlignment,//以最右控件对齐,为标题栏三个按钮设计
		leftAlignmant,
		averageAlignment,
		oneandtwoAlignment
		//... so on , in the future...
	};

	using win32 = std::tuple<HWND, UINT, WPARAM, LPARAM>;



	enum colorsnum
	{
		backgroundcolornum,
		contentcolornum,
		disabledcontentcolornum,
		moveinbackgroundcolornum,
		moveincontentcolornum,
		clickbackgroundcolornum,
		clickcontentcolornum,
		titlecolornum,
		shadowcolornum
	};

	class SkinManager
	{
	public:
		static void setstyle(cuiStyle _style)
		{
			SkinManager::style = _style;
		}
		static cint getStyleColor(cint _colornum)
		{
			return SkinContainer[SkinManager::style][_colornum];
		}
		SkinManager() = default;
	private:
		static cuiStyle style;
		static int SkinContainer[maxstyles][maxcolorinstyle];
	};

	 
	
	
}