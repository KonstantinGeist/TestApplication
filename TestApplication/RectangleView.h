// Copyright (C) The Company

#pragma once

#include "ConvexShapeView.h"

class CRectangleView final: public CConvexShapeView
{
public:
	virtual const std::string GetTypeName() const override;
	virtual void Render(CDC* pDC, ShapeViewLayer layer)  override;
};
