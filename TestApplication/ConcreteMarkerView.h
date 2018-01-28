// Copyright (C) The Company

#pragma once

#include "IMarkerView.h"

namespace view
{

	// A simple implementation of IMarkerView.
	class CConcreteMarkerView final : public IMarkerView
	{
	public:
		virtual CRect GetRect() const override;
		virtual void SetRect(const CRect& rect) override;

	private:
		CRect m_rect;
	};

}
