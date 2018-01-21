// Copyright (C) The Company

#include "stdafx.h"

#include "EllipseView.h"

const std::string CEllipseView::GetTypeName() const
{
	return "Ellipse";
}

bool CEllipseView::HitTest(const CPoint& p) const
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

void CEllipseView::Render(CDC* pDC, ShapeViewLayer layer)
{
	if (layer == ShapeViewLayer::Default)
	{
		CBrush brush(RGB(255, 255, 0));
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		CPen pen;
		pen.CreatePen(PS_SOLID, 3, RGB(255, 66, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);

		pDC->Ellipse(GetRect());

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	CConvexShapeView::Render(pDC, layer);
}
