#include "stdafx.h"
namespace cibbonui
{
	using cint = __int32;
	using cdword = DWORD;
	using CPointf = D2D1_POINT_2F;
	using CRect = D2D1_RECT_F;
	const bool already = true;
	const bool notyet = false;
	//using 
	
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