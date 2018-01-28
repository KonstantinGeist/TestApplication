// Copyright (C) The Company

#include "stdafx.h"
#include "EllipseView.h"

namespace view
{

	std::string CEllipseView::GetTypeName() const
	{
		return "Ellipse";
	}

	void CEllipseView::Render(class CDC* pDC)
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

}
