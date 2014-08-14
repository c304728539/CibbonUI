#include "CUIBase.h"

namespace cibbonui{

	cuiPoint::cuiPoint(float _x, float _y)
		:x(_x), y(_y)
	{

	}

	cuiPoint::cuiPoint()
		: x(0.f), y(0.f)
	{

	}

	cuiPoint::cuiPoint(const cuiPoint& cp)
		: x(cp.x), y(cp.y)
	{

	}

	cuiPoint::cuiPoint(cuiPoint&& cp)
		: x(cp.x), y(cp.y)
	{

	}

	cuiPoint::operator D2D1_POINT_2F() const
	{
		return D2D1::Point2F(x, y);
	}

	cuiPoint::operator D2D1_POINT_2U() const
	{
		return D2D1::Point2U(static_cast<UINT>(x), static_cast<UINT>(y));
	}

	cuiPoint::operator POINT() const
	{
		return POINT{ static_cast<LONG>(x), static_cast<LONG>(y) };
	}

	cuiPoint cuiPoint::Move(float offx, float offy)
	{
		return cuiPoint{ x + offx, y + offy };
	}

	bool operator == (const cuiPoint& cp1, const cuiPoint& cp2)
	{
		return cp1.x == cp2.x && cp1.y == cp2.y;
	}

	bool operator != (const cuiPoint& cp1, const cuiPoint& cp2)
	{
		return !(cp1 == cp2);
	}

	cuiRect::cuiRect()
		:left(0.f), top(0.f), right(0.f), bottom(0.f)
	{

	}

	cuiRect::cuiRect(float x1, float y1, float x2, float y2)
		: left(x1), top(y1), right(x2), bottom(y2)
	{

	}

	cuiRect::cuiRect(const cuiPoint& p1, const cuiPoint& p2)
		: left(p1.x), top(p1.y), right(p2.x), bottom(p2.y)
	{

	}

	cuiRect::cuiRect(const cuiRect& cr)
		: left(cr.left), top(cr.top), right(cr.right), bottom(cr.bottom)
	{

	}

	cuiRect::cuiRect(const RECT& rc)
		:left((float)rc.left),
		top((float)rc.top),
		right((float)rc.right),
		bottom((float)rc.bottom)
	{
	}

	cuiRect::cuiRect(cuiRect&& cr)
		: left(cr.left), top(cr.top), right(cr.right), bottom(cr.bottom)
	{

	}

	cuiRect::operator D2D1_RECT_F() const
	{
		return D2D1::RectF( left, top, right, bottom );
	}

	cuiRect::operator D2D1_RECT_U() const
	{
		return D2D1::RectU(static_cast<UINT>(left), static_cast<UINT>(top), static_cast<UINT>(right), static_cast<UINT>(bottom));
	}

	cuiRect::operator RECT() const
	{
		return RECT{ static_cast<LONG>(left), static_cast<LONG>(top), static_cast<LONG>(right), static_cast<LONG>(bottom) };
	}

	cuiRect cuiRect::getMargin(float x, float y)
	{
		return cuiRect(left + x, top + y, right - x, bottom - y);
	}

	float cuiRect::getSize()
	{
		return GetWidth() * GetHeight();
	}

	float cuiRect::GetWidth()
	{
		return right - left;
	}

	float cuiRect::GetHeight()
	{
		return bottom - top;
	}

	void cuiRect::MoveDown(float yoff)
	{
		top += yoff;
		bottom += yoff;
	}

	bool operator ==(const cuiRect& r1, const cuiRect& r2)
	{
		return r1.left == r2.left && r1.top == r2.top && r1.right == r2.right && r1.bottom == r2.bottom;
	}

	bool operator !=(const cuiRect& r1, const cuiRect& r2)
	{
		return !(r1 == r2);
	}

	bool operator > (const cuiRect& cr1, const cuiRect& cr2)
	{
		return cr1.left < cr2.left && cr1.right > cr2.right && cr1.top < cr2.top && cr1.bottom > cr2.bottom;
	}

	bool operator < (const cuiRect& cr1, const cuiRect& cr2)
	{
		return !(cr1 > cr2);
	}
}