
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
		//����ӵ��¼�
		buttonclick,

	};

	const auto Alignmentcenter = DWRITE_TEXT_ALIGNMENT_CENTER;
	const auto Alignmentleft = DWRITE_TEXT_ALIGNMENT_LEADING;
	const auto Alignmentright = DWRITE_TEXT_ALIGNMENT_TRAILING;

	struct cuievent
	{
		//�¼��ı�Ҫ��Ϣ
		HWND hWnd;
		cuieventenum Event;
		WPARAM wParam;
		LPARAM lParam;
	};

	using win32 =  std::tuple<HWND, UINT, WPARAM, LPARAM>;


}