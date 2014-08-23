#pragma once
#ifdef UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
#include <d2d1.h>
#include <dwrite.h>
#include <algorithm>
#include <windowsx.h>
#include <dwmapi.h>
#include <exception>
#include <memory>
#include <map>
#include <unordered_map>
#include <d2d1helper.h>
#include <process.h>
#include <functional>
#include <numeric>
#pragma comment(lib,"Dwmapi.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"Comctl32.lib")

namespace std
{
	template<>
	struct hash<pair<int,int>>
	{	
		size_t operator() (const std::pair<int, int>& p) const
		{
			return hash<int>()(p.first) + hash<int>()(p.second);
		}
		// hash functor for std::pair<int,int>
	};
}


namespace cibbonui
{
	using cint32_t = __int32;
	using cint = cint32_t;
	using ucint = unsigned __int32;
	
	struct cuiPoint
	{
		float x;
		float y;
		cuiPoint(float _x, float _y);
		cuiPoint();
		cuiPoint(const cuiPoint&);
		cuiPoint(cuiPoint&&);

		operator D2D1_POINT_2F() const;
		operator D2D1_POINT_2U() const;
		operator POINT() const;

		cuiPoint Move(float offx, float offy);
	};
	//operator overloads for cuiPoint

	bool operator == (const cuiPoint& cp1, const cuiPoint& cp2);
	bool operator != (const cuiPoint& cp1, const cuiPoint& cp2);

	struct cuiRect
	{
		float left;
		float top;
		float right;
		float bottom;
		//Basic constructors
		cuiRect(float x1, float y1, float x2, float y2);
		cuiRect();
		cuiRect(const cuiPoint& leftcorner, const cuiPoint& rightcorner);
		cuiRect(const cuiRect& cr);
		cuiRect(const RECT& rc);
		//Move semantics
		cuiRect(cuiRect&& cr);
		//type casting
		operator D2D1_RECT_F()const;
		operator D2D1_RECT_U()const;
		operator RECT() const;
		//helper functions
		cuiRect getMargin(float x, float y);
		float GetWidth();
		float GetHeight();
		float getSize();
		void MoveDown(float yoff);
		
		
	};
	//operator overloads for cuiRect
	bool operator == (const cuiRect& cr1, const cuiRect& cr2);
	bool operator != (const cuiRect& cr1, const cuiRect& cr2);
	bool operator > (const cuiRect& cr1, const cuiRect& cr2);
	bool operator < (const cuiRect& cr1, const cuiRect& cr2);
	
	/*union Color
	{
		struct 
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};
		ucint color;
	};
*/

	/*Helper functions*/
	template<typename P, typename R>
	inline bool PtinRect(const P& Point, const R& Rect)
	{
		return Point.x >= Rect.left && Point.x < Rect.right && Point.y >= Rect.top && Point.y < Rect.bottom;
	}

	template<typename R1,typename R2>
	inline bool RinRect(R1& rect1, R2 rect2)
	{
		//return (std::max)(rect1, rect2) == rect2;
		auto temp = rect1;
		return (rect1 = (std::min)(rect1, rect2)) == temp;
	}

	inline cuiRect GetMoveDown(const cuiRect& rc ,float yoff)
	{
		return cuiRect(rc.left, rc.top + yoff, rc.right, rc.bottom + yoff);
	}
	inline cuiRect GetMoveDirect(const cuiRect& rc, float yoff)
	{
		return cuiRect(rc.left + yoff, rc.top, rc.right + yoff, rc.bottom);
	}

	template<typename T>
	inline const std::wstring& GetText(T* p)
	{
		return p->GetControlText();
	}
	struct Color
	{
		union
		{
			struct
			{
				unsigned char r;
				unsigned char g;
				unsigned char b;
				unsigned char a;
			};
			ucint value;
		};

		Color()
			:r(0), g(0), b(0), a(255)
		{
		}

		Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255)
			:r(_r), g(_g), b(_b), a(_a)
		{
		}

		cint Compare(Color color)const
		{
			return value - color.value;
		}

		
		bool operator==(Color color)const { return Compare(color) == 0; }
		bool operator!=(Color color)const { return Compare(color) != 0; }
		bool operator<(Color color)const { return Compare(color)<0; }
		bool operator<=(Color color)const { return Compare(color) <= 0; }
		bool operator>(Color color)const { return Compare(color)>0; }
		bool operator>=(Color color)const { return Compare(color) >= 0; }
	};

}