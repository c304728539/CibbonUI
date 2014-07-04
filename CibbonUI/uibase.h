
namespace cibbonui
{
	using cint = __int32;
	using cdword = DWORD;
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
		buttonclick,

	};

	struct cuievent
	{
		//事件的必要信息
		HWND hWnd;
		cuieventenum Event;
		WPARAM wParam;
		LPARAM lParam;
	};

	using win32 =  std::tuple<HWND, UINT, WPARAM, LPARAM>;


}