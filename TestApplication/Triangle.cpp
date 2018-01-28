// Copyright (C) The Company

#include "stdafx.h"
#include "Triangle.h"

namespace domain_model
{

	std::string CTriangle::GetTypeName() const
	{
		return "Triangle";
	}

	inline int sign(const CPoint& p1, const CPoint& p2, const CPoint& p3)
	{
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}

	bool CTriangle::ContainsPoint(const CPoint& p) const
	{
		const CRect rect(GetRect());
		const CPoint v1(rect.left, rect.bottom);
		const CPoint v2(rect.left + (rect.right - rect.left) / 2, rect.top);
		const CPoint v3(rect.right, rect.bottom);

		bool b1 = sign(p, v1, v2) < 0;
		bool b2 = sign(p, v2, v3) < 0;
		bool b3 = sign(p, v3, v1) < 0;

		return ((b1 == b2) && (b2 == b3));
	}

}
