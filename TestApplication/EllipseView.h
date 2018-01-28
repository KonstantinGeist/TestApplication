// Copyright (C) The Company

#pragma once

#include "ConvexShapeView.h"

namespace view
{

	class CEllipseView : public CConvexShapeView
	{
	public:
		virtual std::string GetTypeName() const override;
		virtual void Render(CDC* pDC) override;
	};

}
