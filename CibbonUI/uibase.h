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
		//����ӵ��¼�
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
	//	//�¼��ı�Ҫ��Ϣ
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
		
		
		//����ӣ�
	};
	enum layoutenum{
		rightAlignment,//�����ҿؼ�����,Ϊ������������ť���
		leftAlignmant,
		averageAlignment,
		oneandtwoAlignment
		//... so on , in the future...
	};

	using win32 =  std::tuple<HWND, UINT, WPARAM, LPARAM>;


}