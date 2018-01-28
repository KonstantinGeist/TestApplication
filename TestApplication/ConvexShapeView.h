// Copyright (C) The Company

#pragma once

#include "IShapeView.h"

namespace view
{

	class CConvexShapeView : public IShapeView
	{
	public:
		virtual void SetRect(const CRect& rect) override;
		virtual CRect GetRect() const override;

	private:
		CRect m_rect;
	};

}
