// Copyright (C) The Company

#include "stdafx.h"
#include "TriangleView.h"

namespace view
{

	std::string CTriangleView::GetTypeName() const
	{
		return "Triangle";
	}

	void CTriangleView::Render(class CDC* pDC)
	{
		CBrush brush(RGB(255, 255, 0));
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		CPen pen;
		pen.CreatePen(PS_SOLID, 3, RGB(255, 66, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);

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

}
