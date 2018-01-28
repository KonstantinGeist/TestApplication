// Copyright (C) The Company

#include "stdafx.h"
#include "ConcreteMarker.h"

namespace pres_model
{

	CConcreteMarker::CConcreteMarker(EMarkerKind kind, const CRect& rect)
		: m_kind(kind), m_rect(rect), m_listener(nullptr)
	{
	}

	EMarkerKind CConcreteMarker::GetKind() const
	{
		return m_kind;
	}

	CRect CConcreteMarker::GetRect() const
	{
		return m_rect;
	}

	void CConcreteMarker::SetRect(const CRect& rect)
	{
		m_rect = rect;

		if (m_listener)
			m_listener->OnRectChanged(rect);
	}

	void CConcreteMarker::SetListener(IMarkerListener* listener)
	{
		m_listener = listener;
	}

}
