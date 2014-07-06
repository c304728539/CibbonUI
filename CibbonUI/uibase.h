#include "stdafx.h"
namespace cibbonui
{
	using cint = __int32;
	using cdword = DWORD;
	using CPointf = D2D1_POINT_2F;
	using CRect = D2D1_RECT_F;
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

	struct cuieventbase
	{
		cuieventenum eventname;
		virtual ~cuieventbase() = default;
	};
	struct cuibuttonevent : public cuieventbase
	{
		CPointf eventposition;
		
		//待添加？
	};


	using win32 =  std::tuple<HWND, UINT, WPARAM, LPARAM>;


}