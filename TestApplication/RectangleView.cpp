// Copyright (C) The Company

#include "stdafx.h"
#include "RectangleView.h"

namespace view
{

	std::string CRectangleView::GetTypeName() const
	{
		return "Rectangle";
	}

	void CRectangleView::Render(CDC* pDC)
	{
		CBrush brush(RGB(255, 255, 0));
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		CPen pen;
		pen.CreatePen(PS_SOLID, 3, RGB(255, 66, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);

		pDC->Rectangle(GetRect());

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

}
