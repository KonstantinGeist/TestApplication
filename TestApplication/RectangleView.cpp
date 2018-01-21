#include "stdafx.h"

#include "RectangleView.h"

const std::string CRectangleView::GetTypeName() const
{
	return "Rectangle";
}

void CRectangleView::Render(CDC* pDC, ShapeViewLayer layer)
{
	if (layer == ShapeViewLayer::Default)
	{
		CBrush brush(RGB(255, 255, 0));
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		CPen penBlack;
		penBlack.CreatePen(PS_SOLID, 3, RGB(255, 66, 0));
		CPen* pOldPen = pDC->SelectObject(&penBlack);

		pDC->Rectangle(GetRect());

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	CConvexShapeView::Render(pDC, layer);
}
