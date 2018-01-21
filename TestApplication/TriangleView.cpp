// Copyright (C) The Company

#include "stdafx.h"

#include "TriangleView.h"

const std::string CTriangleView::GetTypeName() const
{
	return "Triangle";
}

inline int sign(const CPoint& p1, const CPoint& p2, const CPoint& p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool CTriangleView::HitTest(const CPoint& p) const
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

void CTriangleView::Render(CDC* pDC, ShapeViewLayer layer)
{
	if (layer == ShapeViewLayer::Default)
	{
		CBrush brush(RGB(255, 255, 0));
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		CPen penBlack;
		penBlack.CreatePen(PS_SOLID, 3, RGB(255, 66, 0));
		CPen* pOldPen = pDC->SelectObject(&penBlack);

		POINT p[3];
		const CRect rect(GetRect());
		p[0].x = rect.left + (rect.right - rect.left) / 2;
		p[0].y = rect.top;
		p[1].x = rect.right;
		p[1].y = rect.bottom;
		p[2].x = rect.left;
		p[2].y = rect.bottom;
		pDC->Polygon(&p[0], 3);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	CConvexShapeView::Render(pDC, layer);
}
