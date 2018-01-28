// Copyright (C) The Company

#include "stdafx.h"
#include "ConvexShapeView.h"

namespace view
{

	void CConvexShapeView::SetRect(const CRect& rect)
	{
		m_rect = rect;
	}

	CRect CConvexShapeView::GetRect() const
	{
		return m_rect;
	}

}
