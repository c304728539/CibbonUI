
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
		//����ӵ��¼�
		buttonclick,

	};

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