// Copyright (C) The Company

#pragma once

#include "ConvexShapeView.h"

namespace view
{

	class CRectangleView : public CConvexShapeView
	{
		virtual std::string GetTypeName() const override;
		virtual void Render(class CDC* pDC) override;
	};

}
