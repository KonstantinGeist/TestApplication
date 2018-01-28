// Copyright (C) The Company

#include "stdafx.h"
#include "Ellipse.h"

namespace domain_model
{

	std::string CEllipse::GetTypeName() const
	{
		return "Ellipse";
	}

	bool CEllipse::ContainsPoint(const CPoint& p) const
	{
		const CRect rc(GetRect());

		// Determine radii.
		double a = (rc.right - rc.left) / 2;
		double b = (rc.bottom - rc.top) / 2;

		// Determine x, y.
		double x = p.x - (rc.left + rc.right) / 2;
		double y = p.y - (rc.top + rc.bottom) / 2;

		// Apply ellipse formula.
		return ((x * x) / (a * a) + (y * y) / (b * b) <= 1);
	}

}
