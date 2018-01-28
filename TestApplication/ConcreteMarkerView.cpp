// Copyright (C) The Company

#include "stdafx.h"
#include "ConcreteMarkerView.h"

namespace view
{

	CRect CConcreteMarkerView::GetRect() const
	{
		return m_rect;
	}

	void CConcreteMarkerView::SetRect(const CRect& rect)
	{
		m_rect = rect;
	}

}
