// Copyright (C) The Company

#include "stdafx.h"
#include "Shape.h"

namespace domain_model
{
	// **************************
	//         CShape
	// **************************

	CShape::CShape()
		: m_listener(nullptr)
	{
	}

	CRect CShape::GetRect() const
	{
		return m_rect;
	}

	void CShape::SetRect(const CRect& rect)
	{
		m_rect = rect;

		if (m_listener)
			m_listener->OnRectChanged(rect);
	}

	bool CShape::ContainsPoint(const CPoint& p) const
	{
		return GetRect().PtInRect(p);
	}

	void CShape::SetListener(IShapeListener* listener)
	{
		m_listener = listener;
	}

}
