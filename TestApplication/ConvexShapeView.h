// Copyright (C) The Company

#pragma once

#include "ConcreteShapeViewBase.h"

namespace view
{

	class CConvexShapeView : public CConcreteShapeViewBase
	{
	public:
		virtual void SetRect(const CRect& rect) override;
		virtual CRect GetRect() const override;

	private:
		CRect m_rect;
	};

}
