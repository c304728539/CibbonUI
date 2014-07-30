#pragma once
#include <d2d1.h>

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
		//Move semantics
		cuiRect(cuiRect&& cr);
		//type casting
		operator D2D1_RECT_F()const;
		operator D2D1_RECT_U()const;
		operator RECT() const;
		//helper functions
		cuiRect getMargin(float x, float y);
		float getSize();
		
	};
	//operator overloads for cuiRect
	bool operator == (const cuiRect& cr1, const cuiRect& cr2);
	bool operator != (const cuiRect& cr1, const cuiRect& cr2);
	bool operator > (const cuiRect& cr1, const cuiRect& cr2);
	bool operator < (const cuiRect& cr1, const cuiRect& cr2);
	
	union Color
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


	/*Helper functions*/
	template<typename P, typename R>
	inline bool PtinRect(const P& Point, const R& Rect)
	{
		return Point.x >= Rect.left && Point.x < Rect.right && Point.y >= Rect.top && Point.y < Rect.bottom;
	}

}